#pragma once

#include <vector>
#include <unordered_map>

class RigidBody;

struct RigidBodyQueueItem
{
	RigidBody* rbComponent;
	int worldID;

	//overriding the equality operator to compare RigidBodyQueueItem objects
	//in std::remove function 
	bool operator==(const RigidBodyQueueItem& other) const 
	{
		return rbComponent == other.rbComponent && worldID == other.worldID;
	}
};

class RigidBodyRegistry
{
private:
	static std::vector<RigidBodyQueueItem> m_queue;
	//form: {worldID: requestCount}
	static std::unordered_map<int, int> m_worldRequests;
public:
	RigidBodyRegistry() = default;
	//adds the rigidbody to the queue
	static void queueRigidBody(RigidBody* rbComponent, int worldID);
	//returns true if there are rigidbodies in queue that belong to
	//the physics world with the given ID
	static bool pendingForWorld(int worldID);
	//returns RigidBody* from the queue that belongs to the physics world with the given ID
	static RigidBody* getFromQueue(int worldID);
	static std::vector<RigidBody*> getAllForWorld(int worldID);
};
