#include "RigidBody.h"
#include "RigidBodyRegistry.h"

std::vector<RigidBodyQueueItem> RigidBodyRegistry::m_queue;
std::unordered_map<int, int> RigidBodyRegistry::m_worldRequests;
std::unordered_map<int, int> RigidBodyRegistry::m_deleteRequests;
std::vector<RigidBodyQueueItem> RigidBodyRegistry::m_deleteQueue;

void RigidBodyRegistry::queueRigidBody(RigidBody* rbComponent, int worldID)
{
	m_queue.push_back(RigidBodyQueueItem{rbComponent, worldID});
	if (!m_worldRequests[worldID]) m_worldRequests[worldID] = 1;
	else m_worldRequests[worldID]++;
}

bool RigidBodyRegistry::pendingForWorld(int worldID)
{
	if (!m_worldRequests[worldID]) return false;
	return m_worldRequests[worldID] > 0;
}

bool RigidBodyRegistry::pendingForDeletion(int worldID)
{
	if (!m_deleteRequests[worldID]) return false;
	return m_deleteRequests[worldID] > 0;
}

void RigidBodyRegistry::addToDeleteQueue(RigidBody* rbComponent, int worldID)
{
	m_deleteQueue.push_back(RigidBodyQueueItem{rbComponent, worldID});
	if (!m_deleteRequests[worldID]) m_deleteRequests[worldID] = 1;
	else m_deleteRequests[worldID]++;
}

std::vector<RigidBody*> RigidBodyRegistry::getDiscardedRigidBodies(int worldID)
{
	std::vector<RigidBody*> v;
	for (const auto& rbQueueItem : m_deleteQueue)
	{
		if (rbQueueItem.worldID == worldID)
		{
			v.push_back(rbQueueItem.rbComponent);
			m_deleteQueue.erase(std::remove(m_deleteQueue.begin(), m_deleteQueue.end(), rbQueueItem), m_deleteQueue.end());
		}
	}
	m_deleteRequests[worldID] = 0;
	return v;
}

RigidBody* RigidBodyRegistry::getFromQueue(int worldID)
{
	if (m_queue.empty() || !pendingForWorld(worldID)) return nullptr;
	for (const RigidBodyQueueItem& queueItem : m_queue)
	{
		if (queueItem.worldID == worldID)
		{
			RigidBody* rb = queueItem.rbComponent;
			m_worldRequests[worldID]--;

			//no more requests for this world, remove it from the map
			if (m_worldRequests[worldID] == 0)
				m_worldRequests.erase(worldID);

			//remove item from queue
			m_queue.erase(std::remove(m_queue.begin(), m_queue.end(), queueItem), m_queue.end());
			return rb;
		}
	}
}

RigidBody* RigidBodyRegistry::getDiscardedFromQueue(int worldID)
{
	if (m_deleteQueue.empty() || !pendingForDeletion(worldID)) return nullptr;
	for (const RigidBodyQueueItem& queueItem : m_deleteQueue)
	{
		if (queueItem.worldID == worldID)
		{
			RigidBody* rb = queueItem.rbComponent;
			m_deleteRequests[worldID]--;

			//no more requests for this world, remove it from the map
			if (m_deleteRequests[worldID] == 0)
				m_deleteRequests.erase(worldID);

			//remove item from queue
			m_deleteQueue.erase(std::remove(m_deleteQueue.begin(), m_deleteQueue.end(), queueItem), m_deleteQueue.end());
			return rb;
		}
	}
}

std::vector<RigidBody*> RigidBodyRegistry::getAllForWorld(int worldID)
{
	std::vector<RigidBody*> rigidBodies;
	for (const RigidBodyQueueItem& queueItem : m_queue)
	{
		if (queueItem.worldID == worldID)
		{
			RigidBody* rb = queueItem.rbComponent;
			//remove item from queue
			m_queue.erase(std::remove(m_queue.begin(), m_queue.end(), queueItem), m_queue.end());
			rigidBodies.push_back(rb);
		}
	}

	m_worldRequests[worldID] = 0;
	return rigidBodies;
}