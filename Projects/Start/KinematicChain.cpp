#include "KinematicChain.h"

KinematicChain::KinematicChain(int numberOfJoints, float angleConstraint, const glm::vec3& chainStartPos, Mesh* meshContainer, Mesh* target)
{
	//first element doesn't have parent
	this->chainStartPos = chainStartPos;
	this->target = target;
	this->meshContainer = meshContainer;
	this->chain.push_back(new Joint(angleConstraint, this->meshContainer));
	this->chain[0]->SetPosition(this->chainStartPos);
	for (int i = 1; i < numberOfJoints; i++)
	{
		this->chain.push_back(new Joint(angleConstraint, meshContainer));
		//creating offset between joints
		this->chain[i]->SetPosition(this->chainStartPos + glm::vec3(1.2f, 0.0f, 0.0f) * this->chain[i]->GetSegmentLength() * (float) i);
		this->chain[i]->SetParent(this->chain[i - 1]);
		
		glm::vec3 pos = this->chain[i]->GetPosition();

		std::cout << "Created joint " << i << " at position " << pos.x << ", " << pos.y << " " << pos.z << std::endl;
	}

	this->target = target;
}

void KinematicChain::FabrikAlgorithm(const int numberOfIterations)
{
	for (int i = 0; i < numberOfIterations; i++)
	{
		//Calculate new position of every joint
		ForwardPass();

		BackwardsPass();
		//See if joint surpasses constraint

		//
	}
}

void KinematicChain::ForwardPass()
{
	Joint* currentJoint = this->chain[this->chain.size() - 1];

	while (currentJoint->GetParent())
	{
		currentJoint->SetTempPosition(CalculateNewJointPosition(currentJoint, 1));
		currentJoint = currentJoint->GetParent();
	}
}

void KinematicChain::BackwardsPass()
{
	glm::vec3 startingPosition = this->chain[0]->GetPosition();
	glm::vec3 newPosition = CalculateNewJointPosition(this->chain[1], -1);
	this->chain[1]->SetPosition(newPosition);
	for (int i = 2; i < this->chain.size(); i++)
	{
		this->chain[i]->SetPosition(CalculateNewJointPosition(this->chain[i], -1));
	}
}
glm::vec3 KinematicChain::CalculateNewJointPosition(Joint* joint, const float direction)
{
	return glm::normalize(joint->GetPosition() - this->target->objectPos) * direction * joint->GetSegmentLength();
}

std::vector<Joint*>* KinematicChain::GetAllJoints()
{
	return &this->chain;
}

KinematicChain::~KinematicChain()
{
	for (Joint* joint : this->chain)
	{
		delete joint;
	}
}