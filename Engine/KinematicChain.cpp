#include "Joint.h"
//#include "Mesh.h"
//#include "Gizmos.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "KinematicChain.h"

#define DISTANCE_BETWEEN_JOINTS 0.1f
#define ERROR_MARGIN 0.1f

KinematicChain::KinematicChain(int numberOfJoints, float angleConstraint, 
	const glm::vec3& chainStartPos/*, Mesh* meshContainer, Mesh* target*/,
	Transform* targetTransform/*,
	Gizmos* gizmos*/)
	:
	m_chainOrigin{ chainStartPos }, m_targetTransform{ targetTransform }
{
	m_chain.push_back(std::make_unique<Joint>(m_id, angleConstraint, m_hardcodedLength/*, meshContainer*/));
	m_chain[0]->SetPosition(m_chainOrigin);
	m_chain[0]->SetTempPosition(m_chainOrigin);
	/*std::string name = "j" + std::to_string(m_id);
	gizmos->AddRay(name, this->mChain[0]->GetPosition(), this->mChain[0]->GetForwardVector(), 10);*/
	for (int i = 1; i < numberOfJoints; i++)
	{
		m_chain.push_back(std::make_unique<Joint>(m_id, angleConstraint, m_hardcodedLength));
		//creating offset between joints
		//this->chain[i]->SetPosition(this->chainStartPos + glm::vec3(DISTANCE_BETWEEN_JOINTS, 0.0f, 0.0f) * this->chain[i]->GetSegmentLength() * (float) i);
		m_chain[i]->SetPosition(m_chainOrigin + (glm::vec3(-DISTANCE_BETWEEN_JOINTS, 0.0f, 0.0f) *
			m_chain[i]->GetSegmentLength()  * (float)i));
		
		m_chain[i]->SetParent(m_chain[i - 1].get());
		m_chain[i - 1]->SetChild(m_chain[i].get());
		
		//std::string name = "j" + std::to_string(m_id);
		//std::cout << "ID: " << id << " NAME: " << name << std::endl;
		/*gizmos->AddRay(name, this->mChain[i]->GetPosition(), this->mChain[i]->GetForwardVector(), 10);
		gizmos->AddPoint(this->mChain[i]->GetPosition());*/
	}
	//gizmos->SetupPointsBuffer();
}

void KinematicChain::setMeshRenderer(std::unique_ptr<MeshRenderer> meshRenderer)
{
	m_meshRenderer = std::move(meshRenderer);
}

void KinematicChain::BackwardsPass()
{
	Joint* currentJoint = m_chain.back().get();

	currentJoint->SetTempPosition(m_targetTransform->getPosition() - (currentJoint->getForwardVector() * currentJoint->GetSegmentLength()));
	currentJoint = currentJoint->GetParent();
	while (currentJoint->GetParent())
	{
		currentJoint->SetTempPosition(
			currentJoint->GetChild()->getTempPosition() + (
				glm::normalize(currentJoint->getPosition() - currentJoint->GetChild()->getTempPosition()) * (currentJoint->GetSegmentLength() + DISTANCE_BETWEEN_JOINTS)
				)
		);
		//no need to rotate every step, it is enough to rotate once in main loop before rendering
		//currentJoint->RotateTowardsTarget(this->target->objectPos);
		currentJoint = currentJoint->GetParent();
	}
}

void KinematicChain::ForwardPass()
{
	for (int i = 0; i < m_chain.size(); i++)
	{
		Joint* joint = m_chain[i].get();
		if (!joint->GetParent())
		{
			continue;
		}
		else
		{
			joint->SetPosition(
				joint->GetParent()->getPosition() + (
					glm::normalize(joint->getTempPosition() - joint->GetParent()->getPosition()) * (joint->GetSegmentLength() + DISTANCE_BETWEEN_JOINTS)
					)
			);
		}
	}
}

void KinematicChain::FabrikAlgorithm(const int numberOfIterations)
{
	//Check if target is out of reach
	if (targetOutOfReach())
	{
		//Position of start of the chain should never be changed
		for (int i = 1; i < m_chain.size(); i++)
		{
			m_chain[i]->SetPosition(glm::normalize(m_targetTransform->getPosition() - m_chainOrigin)
				* (m_chain[i]->GetSegmentLength() + DISTANCE_BETWEEN_JOINTS) * (float)i);
		}
		return;
	}

	//end of chain has reached the target
	else if (ErrorTooSmall())
	{
		return;
	}
	//Shouldn't put i < numberOfIterations || ErrorTooSmall() BECAUSE IT WILL RUN INFINITELY SINCE ErrorTooSmall will always return true
	for (int i = 0; (i < numberOfIterations && !ErrorTooSmall()); i++)
	{
		BackwardsPass();

		ForwardPass();

		//TODO: See if joint surpasses constraint
	}
}

void KinematicChain::moveTarget(float elapsedTime)
{

	float sphereRadius = 10.0f;
	float theta = elapsedTime * glm::radians(360.0f);

	//angle around x-axis
	//should be in range [0,pi]
	float phi = elapsedTime * glm::radians(180.0f);

	float x = sphereRadius * glm::cos(theta) * glm::sin(phi);
	float y = sphereRadius * glm::cos(phi);
	float z = sphereRadius * glm::sin(theta) * glm::sin(phi);

	m_targetTransform->setPosition(glm::vec3(x, y, z));
}

bool KinematicChain::targetOutOfReach()
{
	float segmentLength = m_chain.front().get()->GetSegmentLength();
	return glm::distance(m_chainOrigin, m_targetTransform->getPosition()) > (m_chain.size() * (segmentLength + DISTANCE_BETWEEN_JOINTS) - DISTANCE_BETWEEN_JOINTS);
}

glm::vec3 KinematicChain::CalculateNewJointPosition(Joint* joint, const float direction)
{
	return glm::normalize(joint->GetParent()->getPosition() - m_targetTransform->getPosition()) * 
		direction * joint->GetSegmentLength() + joint->getPosition();
}


bool KinematicChain::ErrorTooSmall()
{
	//return (glm::distance(this->chain[this->chain.size() - 1]->GetPosition(), this->target->GetPosition()) <= ERROR_MARGIN);
	return (glm::distance(m_chain.back()->getJointEnd(), m_targetTransform->getPosition()) < ERROR_MARGIN);
	//return true;
}


void KinematicChain::simulate(const int steps)
{
	FabrikAlgorithm(steps);

	glm::vec3 jointTargetPos;

	for (int i = 0; i < m_chain.size(); i++)
	{
		Joint* joint = m_chain[i].get();
		jointTargetPos = (joint->GetChild() == nullptr ? m_targetTransform->getPosition()
			: joint->GetChild()->getPosition());

		joint->RotateTowardsTarget(jointTargetPos);
	}
}

glm::vec3 KinematicChain::getTargetPos()
{
	return m_targetTransform->getPosition();
}

std::vector<Transform*>& KinematicChain::getJointsTransforms()
{
	m_jointsTransforms.clear();
	for (int i = 0; i < m_chain.size(); i++)
	{
		m_jointsTransforms.push_back(m_chain[i]->getTransform());
	}
	return m_jointsTransforms;
}

MeshRenderer* KinematicChain::getMeshRenderer()
{
	return m_meshRenderer.get();
}

KinematicChain::~KinematicChain()
{
	std::cout << "Kinematic chain destructor called" << std::endl;
}