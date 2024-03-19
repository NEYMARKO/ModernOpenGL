#include "KinematicChain.h"

KinematicChain::KinematicChain(int numberOfJoints, float angleConstraint, Mesh* meshContainer, Mesh* target)
{
	//first element doesn't have parent
	this->chain.push_back(new Joint(angleConstraint, meshContainer));
	for (int i = 1; i < numberOfJoints; i++)
	{
		this->chain.push_back(new Joint(angleConstraint, meshContainer));
		this->chain[i]->SetParent(this->chain[i - 1]);
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
	Joint* currentJoint = this->chain[this->chain.size()];

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

KinematicChain::~KinematicChain()
{
	for (Joint* joint : this->chain)
	{
		delete joint;
	}
}