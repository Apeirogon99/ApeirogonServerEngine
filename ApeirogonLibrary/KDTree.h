#pragma once

class KDNode
{
public:
	KDNode(const uint32& inNodeNumber) : mActorRef(), mNodeNumber(inNodeNumber), mLeftNode(nullptr), mRightNode(nullptr) {}
	~KDNode() { Clear(); }

	KDNode(const KDNode& inNode)
	{
		this->mActorRef		= inNode.mActorRef;
		this->mLeftNode		= inNode.mLeftNode;
		this->mRightNode	= inNode.mRightNode;
	}

	KDNode& operator=(const KDNode& inNode)
	{
		this->mActorRef		= inNode.mActorRef;
		this->mLeftNode		= inNode.mLeftNode;
		this->mRightNode	= inNode.mRightNode;
		return *this;
	}

public:
	bool IsValid()
	{
		return mActorRef.lock() != nullptr;
	}

	void Clear() 
	{ 
		mActorRef.reset();
		mLeftNode = nullptr; 
		mRightNode = nullptr; 
	}

	void SetNode(ActorRef inActor)
	{ 
		this->SetActorRef(inActor);
		this->SetLeftNode(nullptr);
		this->SetRightNode(nullptr);
	}
	
	void SetActorRef(ActorRef inActor)			{ mActorRef = inActor; };
	void SetLeftNode(KDNode* inLeftNode)		{ mLeftNode = inLeftNode; }
	void SetRightNode(KDNode* inRightNode)		{ mRightNode = inRightNode; }

public:
	ActorRef		GetActorRef()	const { return mActorRef; }
	const uint32&	GetNodeNumber() const { return mNodeNumber; }
	KDNode*			GetLeftNode()	const { return mLeftNode; }
	KDNode*			GetRightNode()	const { return mRightNode; }

private:
	ActorRef	mActorRef;
	uint32		mNodeNumber;

	KDNode*		mLeftNode;
	KDNode*		mRightNode;
};

class KDTree
{

	enum
	{
		INIT_DEPTH = 0,
		INCREASE_DEPTH = 1,
		MAX_AXIS = 3,
	};

public:
	KDTree(const uint16& inPoolSize)
	{
		if (inPoolSize > UINT16_MAX)
		{
			wprintf(L"[KD-Tree] [Init] It's too big to make\n");
			return;
		}

		uint32 newPoolsize = inPoolSize;
		if (!((inPoolSize & (inPoolSize - 1)) == 0))
		{
			newPoolsize = static_cast<uint32>(pow(2, floor(log2(inPoolSize)) + 1));
		}

		for (uint16 index = 0; index < newPoolsize; ++index)
		{
			mNodesPool.push_back(new KDNode(index));
			mAvailableNodeNumber.push(index);
		}

	}
	~KDTree() 
	{
		for (KDNode* node : mNodesPool)
		{
			node->Clear();
			delete node;
			node = nullptr;
		}

		mNodesPool.clear();
		mUseNode.clear();
	}

	KDTree(const KDTree&) = delete;
	KDTree(KDTree&&) noexcept = delete;

	KDTree& operator=(const KDTree&) = delete;
	KDTree& operator=(KDTree&&) noexcept = delete;

public:
	void InsertNode(ActorRef inActor)
	{
		ActorPtr actor = inActor.lock();
		if (nullptr == actor)
		{
			return;
		}

		if (nullptr == actor->GetDefaultCollisionComponent())
		{
			return;
		}

		KDNode* nextNode = NextAvailableNode();
		if (nextNode == nullptr)
		{
			return;
		}
		nextNode->Clear();
		nextNode->SetNode(actor);

		if (mUseNode.size() == 0)
		{
			mUseNode.push_back(nextNode);
			return;
		}

		InsertKDTree(mUseNode[0], nextNode, INIT_DEPTH);

		mUseNode.push_back(nextNode);
	}

	void DeleteTree()
	{

		if (mUseNode.size() == 0)
		{
			return;
		}

		for (KDNode* node : mUseNode)
		{
			mAvailableNodeNumber.push(node->GetNodeNumber());
		}

		DeleteNodePreOrder(mUseNode[0]);

		mUseNode.clear();
	}

	bool SearchNodes(LineTrace& inLineTrace, const uint8& inActorType, std::vector<FVector>& outIntersection, const size_t& inMaxResult = INFINITE)
	{
		if (inActorType == 0)
		{
			return false;
		}

		if (mUseNode.size() == 0)
		{
			return false;
		}

		const Location& location = inLineTrace.GetCenterLocation();
		const float radius = inLineTrace.GetDistance();

		std::vector<int64> findNodeIDs;
		SearchNodePreOrder(location, radius, inActorType, mUseNode[0], findNodeIDs);

		for (size_t index = 0; index < findNodeIDs.size(); ++index)
		{
			const int64& gameObjectID = findNodeIDs[index];
			ActorPtr actor = mNodesPool[gameObjectID]->GetActorRef().lock();
			if (nullptr == actor)
			{
				continue;
			}

			CollisionComponent* collisionComponent = actor->GetDefaultCollisionComponent();
			if (nullptr == collisionComponent)
			{
				continue;
			}

			if (false == inLineTrace.CollisionTrace(collisionComponent))
			{
				continue;
			}

			outIntersection.emplace_back(inLineTrace.GetImpactPoint());

			if (outIntersection.size() == inMaxResult)
			{
				break;
			}

		}

		return outIntersection.size();
	}

	bool SearchNodes(BoxTrace& inBoxTrace, const uint8& inActorType, std::vector<int64>& outGameObjectIDs, const size_t& inMaxResult = INFINITE)
	{
		if (inActorType == 0)
		{
			return false;
		}

		if (mUseNode.size() == 0)
		{
			return false;
		}
		
		const Location& boxLocation = inBoxTrace.GetCenterLocation();
		const float radius = std::sqrtf(std::powf(inBoxTrace.GetBoxCollision().GetBoxExtent().GetX(), 2) + std::powf(inBoxTrace.GetBoxCollision().GetBoxExtent().GetY(), 2));	//외접원 반지름
		
		std::vector<int64> findNodeIDs;
		SearchNodePreOrder(boxLocation, radius, inActorType, mUseNode[0], findNodeIDs);

		for (size_t index = 0; index < findNodeIDs.size(); ++index)
		{
			const int64& gameObjectID = findNodeIDs[index];
			ActorPtr actor = mNodesPool[gameObjectID]->GetActorRef().lock();
			if (nullptr == actor)
			{
				continue;
			}

			CollisionComponent* collisionComponent = actor->GetDefaultCollisionComponent();
			if (nullptr == collisionComponent)
			{
				continue;
			}

			if (false == inBoxTrace.CollisionTrace(collisionComponent))
			{
				continue;
			}

			outGameObjectIDs.emplace_back(actor->GetGameObjectID());

			if (outGameObjectIDs.size() == inMaxResult)
			{
				break;
			}

		}
		
		return outGameObjectIDs.size();
	}

	bool SearchNodes(SphereTrace& inSphereTrace, const uint8& inActorType, std::vector<int64>& outGameObjectIDs, const size_t& inMaxResult = INFINITE)
	{
		if (inActorType == 0)
		{
			return false;
		}

		if (mUseNode.size() == 0)
		{
			return false;
		}

		const Location& sphereLocation = inSphereTrace.GetCenterLocation();
		const float radius = inSphereTrace.GetSphereCollision().GetRadius();

		std::vector<int64> findNodeIDs;
		SearchNodePreOrder(sphereLocation, radius, inActorType, mUseNode[0], findNodeIDs);

		for (size_t index = 0; index < findNodeIDs.size(); ++index)
		{
			const int64& gameObjectID = findNodeIDs[index];
			ActorPtr actor = mNodesPool[gameObjectID]->GetActorRef().lock();
			if (nullptr == actor)
			{
				continue;
			}

			CollisionComponent* collisionComponent = actor->GetDefaultCollisionComponent();
			if (nullptr == collisionComponent)
			{
				continue;
			}

			if (false == inSphereTrace.CollisionTrace(collisionComponent))
			{
				continue;
			}

			outGameObjectIDs.emplace_back(actor->GetGameObjectID());

			if (outGameObjectIDs.size() == inMaxResult)
			{
				break;
			}

		}

		return outGameObjectIDs.size();
	}

	//bool SearchNodes(const FVector& inLocation, const SphereCollision& inSphereCollisionComponent, const uint8& inActorType, std::vector<ActorPtr>& outResultActor, const uint32& inMaxResult = INFINITE)
	//{
	//	return outResultActor.size();
	//}

	bool SearchNodes(const FVector& inFindLocation, const float& inRadius, const uint8& inActorType, std::vector<int64>& outGameObjectIDs, const size_t& inMaxResult = INFINITE)
	{
		if (inActorType == 0)
		{
			return false;
		}

		if (mUseNode.size() == 0)
		{
			return false;
		}

		std::vector<int64> findNodeIDs;
		SearchNodePreOrder(inFindLocation, inRadius, inActorType, mUseNode[0], findNodeIDs);

		for (size_t index = 0; index < findNodeIDs.size(); ++index)
		{
			const int64& gameObjectID = findNodeIDs[index];
			ActorPtr actor = mNodesPool[gameObjectID]->GetActorRef().lock();
			if (nullptr == actor)
			{
				continue;
			}

			outGameObjectIDs.emplace_back(actor->GetGameObjectID());

			if (outGameObjectIDs.size() == inMaxResult)
			{
				break;
			}

		}

		return outGameObjectIDs.size();
	}

	void DebugKDTree()
	{

		if (mUseNode.size() == 0)
		{
			return;
		}

		printf("DEBUG KD-TREE\n");
		printf("{\n");
		DebugPrintPreOrder(mUseNode[0]);
		printf("}\n");
	}

public:
	const int32 GetKDTreeSize() const
	{
		return static_cast<int32>(mNodesPool.size());
	}

protected:
	KDNode* NextAvailableNode()
	{
		if (0 == mAvailableNodeNumber.size())
		{
			wprintf(L"[KD-Tree] [NextAvailableNode] No nodes left\n");
			return nullptr;
		}

		const uint16 nextNumber = mAvailableNodeNumber.front();
		mAvailableNodeNumber.pop();

		return mNodesPool.at(nextNumber);
	}

	void InsertKDTree(KDNode* inParentNode, KDNode* inNode, const uint32& inDepth)
	{

		const uint32 axis = inDepth % MAX_AXIS;

		const Location& parentLocation	= inParentNode->GetActorRef().lock()->GetLocation();
		const Location& nodeLocation	= inNode->GetActorRef().lock()->GetLocation();

		bool result = CompareLocation(parentLocation, nodeLocation, axis);

		if (result)
		{
			if (inParentNode->GetLeftNode() == nullptr)
			{
				inParentNode->SetLeftNode(inNode);
			}
			else
			{
				InsertKDTree(inParentNode->GetLeftNode(), inNode, inDepth + INCREASE_DEPTH);
			}
		}
		else
		{
			if (inParentNode->GetRightNode() == nullptr)
			{
				inParentNode->SetRightNode(inNode);
			}
			else
			{
				InsertKDTree(inParentNode->GetRightNode(), inNode, inDepth + INCREASE_DEPTH);
			}
		}
	}

	void DeleteNodePreOrder(KDNode* inNode)
	{
		if (nullptr == inNode)
		{
			return;
		}

		if (false == inNode->IsValid())
		{
			return;
		}

		DeleteNodePreOrder(inNode->GetLeftNode());
		DeleteNodePreOrder(inNode->GetRightNode());

		inNode->Clear();
	}

	bool CompareLocation(const Location& inParentLocation, const Location& inNodeLocation, const uint32& inAxis)
	{
		if (inAxis == 0)
		{
			const float& parentX	= inParentLocation.GetX();
			const float& nodeX		= inNodeLocation.GetX();
			return nodeX < parentX;
		}
		else if (inAxis == 1)
		{
			const float& parentY	= inParentLocation.GetY();
			const float& nodeY		= inNodeLocation.GetY();
			return nodeY < parentY;
		}
		else
		{
			const float& parentZ	= inParentLocation.GetZ();
			const float& nodeZ		= inNodeLocation.GetZ();
			return nodeZ < parentZ;
		}

	}

	void DebugPrintPreOrder(KDNode* inNode)
	{

		if (nullptr == inNode)
		{
			return;
		}

		if (false == inNode->IsValid())
		{
			return;
		}

		ActorPtr		actor		= inNode->GetActorRef().lock();
		const uint32&	nodeID		= inNode->GetNodeNumber();
		const Location& location	= actor->GetLocation();
		const WCHAR*	name		= actor->GetGameObjectName();

		wprintf(L"\t[NODE-ID::%04u] [Location::(%+09.3f  %+09.3f  %+09.3f)] [Name::%ws]\n", nodeID, location.GetX(), location.GetY(), location.GetZ(), name);
		DebugPrintPreOrder(inNode->GetLeftNode());
		DebugPrintPreOrder(inNode->GetRightNode());
	}

	void SearchNodePreOrder(const FVector& inFindLocation, const float& inRadius, const uint8& inActorType, KDNode* inNode, std::vector<int64>& outGameObjectIDs)
	{
		if (nullptr == inNode)
		{
			return;
		}

		if (false == inNode->IsValid())
		{
			return;
		}

		ActorPtr			actor				= inNode->GetActorRef().lock();
		CollisionComponent* collision			= actor->GetDefaultCollisionComponent();
		const uint8&		type				= actor->GetActorType();
		const Location&		location			= actor->GetLocation();
		const Location&		collisionLocation	= location + collision->GetLocalLocation();
		const float&		collisionRadius		= collision->GetLocalRadius();

		const float distance = FVector::Distance(inFindLocation, collisionLocation);
		if (distance <= inRadius + collisionRadius && inActorType == type)
		{
			outGameObjectIDs.push_back(inNode->GetNodeNumber());
		}

		SearchNodePreOrder(inFindLocation, inRadius, inActorType, inNode->GetLeftNode(), outGameObjectIDs);
		SearchNodePreOrder(inFindLocation, inRadius, inActorType, inNode->GetRightNode(), outGameObjectIDs);
	}

private:
	std::vector<KDNode*>	mNodesPool;
	std::vector<KDNode*>	mUseNode;
	std::queue<uint16>		mAvailableNodeNumber;
};

