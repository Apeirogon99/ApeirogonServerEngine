#include "pch.h"
#include "PriorityQueue.h"

bool CompareNode::operator()(const TaskNodePtr& node1, const TaskNodePtr& node2)
{
	return node1->GetPriority() > node2->GetPriority();
}
