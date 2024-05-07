#include "KinematicChain.h"

#define DISTANCE_BETWEEN_JOINTS 0.1f
#define ERROR_MARGIN 0.1f
KinematicChain::KinematicChain(int numberOfJoints, float angleConstraint, const glm::vec3& chainStartPos, Mesh* meshContainer, Mesh* target, Gizmos* gizmos)
{
	//first element doesn't have parent
	this->chainStartPos = chainStartPos;
	this->target = target;
	int id = 0;
	this->chain.push_back(new Joint(angleConstraint, meshContainer, id));
	this->chain[0]->SetPosition(this->chainStartPos);
	this->chain[0]->SetTempPosition(this->chainStartPos);
	for (int i = 1; i < numberOfJoints; i++)
	{
		id++;
		this->chain.push_back(new Joint(angleConstraint, meshContainer, id));
		//creating offset between joints
		//this->chain[i]->SetPosition(this->chainStartPos + glm::vec3(DISTANCE_BETWEEN_JOINTS, 0.0f, 0.0f) * this->chain[i]->GetSegmentLength() * (float) i);
		this->chain[i]->SetPosition(this->chainStartPos + (glm::vec3(-DISTANCE_BETWEEN_JOINTS, 0.0f, 0.0f) * this->chain[i]->GetSegmentLength()  * (float)i));
		this->chain[i]->SetParent(this->chain[i - 1]);
		this->chain[i - 1]->SetChild(this->chain[i]);
		std::string name = "j" + id;
		gizmos->AddRay(name, this->chain[i]->GetPosition(), this->chain[i]->GetForwardVector(), 10);
	}
}

void KinematicChain::FabrikAlgorithm(const int numberOfIterations)
{
	//Check if target is out of reach
	if (glm::distance(this->chainStartPos, this->target->objectPos) > (this->chain.size() * this->chain[0]->GetSegmentLength()))
	{
		float segmentLength = this->chain[0]->GetSegmentLength();
		//Position of start of the chain should never be changed
		for (int i = 1; i < this->chain.size(); i++)
		{
			this->chain[i]->SetPosition(glm::normalize(this->target->objectPos - this->chainStartPos) * (segmentLength + DISTANCE_BETWEEN_JOINTS) * (float) i);
		}
		return;
	}

	//end of chain has reached the target
	else if (ErrorTooSmall())
	{
		return;
	}
	//Shouldn't put i < numberOfIterations || ErrorTooSmall() BECAUSE IT WILL RUN INDEFINITELY SINCE ErrorTooSmall will always return true
	for (int i = 0; (i < numberOfIterations && !ErrorTooSmall()); i++)
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
	currentJoint->SetTempPosition(this->target->objectPos);
	currentJoint = currentJoint->GetParent();
	while (currentJoint->GetParent())
	{
		currentJoint->SetTempPosition(
			currentJoint->GetChild()->GetTempPosition() + glm::normalize(currentJoint->GetPosition() - currentJoint->GetChild()->GetTempPosition()) * currentJoint->GetSegmentLength()
		);
		//currentJoint->RotateTowardsTarget(this->target->objectPos);
		currentJoint = currentJoint->GetParent();
	}
}

void KinematicChain::BackwardsPass()
{
	for (Joint* joint : this->chain)
	{
		if (!joint->GetParent())
		{
			//PrintClass::PrintVec3(joint->GetMeshContainer()->GetPosition());
			continue;
		}
		else
		{
			joint->SetPosition(
				joint->GetParent()->GetPosition() + glm::normalize(joint->GetTempPosition() - joint->GetParent()->GetPosition()) * joint->GetSegmentLength()
			);
			//joint->RotateTowardsTarget(this->target->objectPos);
		}
	}
}
glm::vec3 KinematicChain::CalculateNewJointPosition(Joint* joint, const float direction)
{
	return glm::normalize(joint->GetParent()->GetPosition() - this->target->objectPos) * direction * joint->GetSegmentLength() + joint->GetPosition();
}


bool KinematicChain::ErrorTooSmall()
{
	//return (glm::distance(this->chain[this->chain.size() - 1]->GetPosition(), this->target->GetPosition()) <= ERROR_MARGIN);
	return (glm::distance(this->chain[this->chain.size() - 1]->GetJointEnd(), this->target->GetPosition()) < ERROR_MARGIN);
}

std::vector<Joint*>* KinematicChain::GetAllJoints()
{
	return &this->chain;
}
Mesh* KinematicChain::GetTarget()
{
	return this->target;
}
KinematicChain::~KinematicChain()
{
	for (Joint* joint : this->chain)
	{
		delete joint;
	}
}
