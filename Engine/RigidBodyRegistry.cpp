#include "RigidBody.h"
#include "RigidBodyRegistry.h"

std::vector<RigidBodyQueueItem> RigidBodyRegistry::m_queue;
std::unordered_map<int, int> RigidBodyRegistry::m_worldRequests;

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

RigidBody* RigidBodyRegistry::getFromQueue(int worldID)
{
	if (m_queue.empty() || !pendingForWorld(worldID)) return nullptr;
	for (RigidBodyQueueItem queueItem : m_queue)
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

std::vector<RigidBody*> RigidBodyRegistry::getAllForWorld(int worldID)
{
	std::vector<RigidBody*> rigidBodies;
	for (RigidBodyQueueItem queueItem : m_queue)
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