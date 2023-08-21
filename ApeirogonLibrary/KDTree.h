#pragma once

class APEIROGON_API KDNode
{
public:
	KDNode() : mLocation(), mGameObjectID(0), mActorType(0), mLeftNode(nullptr), mRightNode(nullptr) {}
	~KDNode() { Clear(); }

	KDNode(const KDNode& inNode)
	{
		this->mLocation			= inNode.mLocation;
		this->mGameObjectID		= inNode.mGameObjectID;
		this->mActorType		= inNode.mActorType;
		this->mLeftNode			= inNode.mLeftNode;
		this->mRightNode		= inNode.mRightNode;
	}

	KDNode& operator=(const KDNode& inNode)
	{
		this->mLocation			= inNode.mLocation;
		this->mGameObjectID		= inNode.mGameObjectID;
		this->mActorType		= inNode.mActorType;
		this->mLeftNode			= inNode.mLeftNode;
		this->mRightNode		= inNode.mRightNode;
		return *this;
	}

public:
	bool IsValid()
	{
		return mGameObjectID != 0;
	}

	void Clear() 
	{ 
		mLocation = FVector(0.0f, 0.0f, 0.0f); 
		mGameObjectID = 0; 
		mActorType = 0;
		mLeftNode = nullptr; 
		mRightNode = nullptr; 
	}

	void SetNode(const Location& inLocation, const int64& inObjectID, const uint8& iActorType) 
	{ 
		this->SetLocation(inLocation); 
		this->SetObjectID(inObjectID);  
		this->SetActorType(iActorType); 
		this->SetLeftNode(nullptr);
		this->SetRightNode(nullptr);
	}
	
	void SetLocation(const Location& inLocation)	{ mLocation = inLocation; }
	void SetObjectID(const int64& inObjectID)		{ mGameObjectID = inObjectID; }
	void SetActorType(const uint8& inActorType)		{ mActorType = inActorType; }
	void SetLeftNode(KDNode* inLeftNode)			{ mLeftNode = inLeftNode; }
	void SetRightNode(KDNode* inRightNode)			{ mRightNode = inRightNode; }

public:
	const Location& GetLocation()	const	{ return mLocation; }
	const int64&	GetObjectID()	const	{ return mGameObjectID; }
	const uint8&	GetActorType()	const	{ return mActorType; }
	KDNode*			GetLeftNode()	const	{ return mLeftNode; }
	KDNode*			GetRightNode()	const	{ return mRightNode; }

private:
	Location	mLocation;
	int64		mGameObjectID;
	uint8		mActorType;

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
	KDTree(const uint16& inPoolSize) : mRootNode(new KDNode)
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
			mNodesPool.push_back(new KDNode);
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

		delete mRootNode;
		mRootNode = nullptr;
	}

	KDTree(const KDTree&) = delete;
	KDTree(KDTree&&) noexcept = delete;

	KDTree& operator=(const KDTree&) = delete;
	KDTree& operator=(KDTree&&) noexcept = delete;

public:
	void InsertNode(const Location& inLocation, const int64& inGameObjectID, const uint8& inActorType)
	{
		KDNode* newNode = NextAvailableNode();
		//KDNode* newNode = new KDNode;
		newNode->SetNode(inLocation, inGameObjectID, inActorType);

		if (mRootNode == nullptr)
		{
			mRootNode = newNode;
			return;
		}

		InsertKDTree(mRootNode, newNode, INIT_DEPTH);
	}

	void DeleteTree()
	{
		mAvailableNodeNumber = std::queue<uint16>();

		for (uint16 index = 0; index < static_cast<uint16>(mNodesPool.size()); ++index)
		{
			mNodesPool.at(index)->Clear();
			mAvailableNodeNumber.push(index);
		}

		mRootNode->SetNode(FVector(0.0f, 0.0f, 0.0f), 0, 0);

		//DeleteNodePreOrder(mRootNode);
		//mRootNode = nullptr;
	}

	//bool SearchNodes(const FVector& inLocation, const CollisionComponent& inBoxCollisionComponent, const uint8& inActorType, std::vector<ActorPtr>& outResultActor, const uint32& inMaxResult = INFINITE)
	//{
	//	if (inActorType == 0)
	//	{
	//		return false;
	//	}

	//	return outResultActor.size();
	//}

	//bool SearchNodes(const FVector& inLocation, const CapsuleCollision& inCapsuleCollisionComponent, const uint8& inActorType, std::vector<ActorPtr>& outResultActor, const uint32& inMaxResult = INFINITE)
	//{
	//	return outResultActor.size();
	//}

	//bool SearchNodes(const FVector& inLocation, const SphereCollision& inSphereCollisionComponent, const uint8& inActorType, std::vector<ActorPtr>& outResultActor, const uint32& inMaxResult = INFINITE)
	//{
	//	return outResultActor.size();
	//}

	bool FindNodes(const FVector& inFindLocation, const float& inRadius, const uint8& inActorType, std::vector<int64>& outGameObjectIDs)
	{
		if (inActorType == 0)
		{
			return false;
		}

		SearchNodePreOrder(inFindLocation, inRadius, inActorType, mRootNode, outGameObjectIDs);
		return outGameObjectIDs.size();
	}

	void DebugKDTree()
	{
		DebugPrintPreOrder(mRootNode);
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

		const Location& parentLocation	= inParentNode->GetLocation();
		const Location& nodeLocation	= inNode->GetLocation();

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
		if (inNode == nullptr)
		{
			return;
		}

		DeleteNodePreOrder(inNode->GetLeftNode());
		DeleteNodePreOrder(inNode->GetRightNode());

		delete inNode;
		inNode = nullptr;
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

		printf("[NODE-ID::%lld] Location[%+-3.2f\t%+-3.2f\t%+-3.2f]\n", inNode->GetObjectID(), inNode->GetLocation().GetX(), inNode->GetLocation().GetY() , inNode->GetLocation().GetZ());
		DebugPrintPreOrder(inNode->GetLeftNode());
		DebugPrintPreOrder(inNode->GetRightNode());
	}

	void SearchNodePreOrder(const FVector& inFindLocation, const float& inRadius, const uint8& inActorType, KDNode* inNode, std::vector<int64>& outGameObjectIDs)
	{
		if (inNode == nullptr)
		{
			return;
		}

		const Location& nodeLocation = inNode->GetLocation();
		const float distance = FVector::Distance(inFindLocation, nodeLocation);
		if (distance <= inRadius && inActorType == inNode->GetActorType())
		{
			outGameObjectIDs.push_back(inNode->GetObjectID());
		}

		SearchNodePreOrder(inFindLocation, inRadius, inActorType, inNode->GetLeftNode(), outGameObjectIDs);
		SearchNodePreOrder(inFindLocation, inRadius, inActorType, inNode->GetRightNode(), outGameObjectIDs);
	}

private:
	KDNode*					mRootNode;
	std::vector<KDNode*>	mNodesPool;
	std::queue<uint16>		mAvailableNodeNumber;
};

