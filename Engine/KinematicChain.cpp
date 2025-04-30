#include "Joint.h"
#include "Mesh.h"
#include "Gizmos.h"
#include "KinematicChain.h"

#define DISTANCE_BETWEEN_JOINTS 0.1f
#define ERROR_MARGIN 0.1f
KinematicChain::KinematicChain(int numberOfJoints, float angleConstraint, 
	const glm::vec3& chainStartPos, Mesh* meshContainer, Mesh* target, Gizmos* gizmos)
	:
	mChainOrigin{ chainStartPos }, mTarget{ target }
{
	mChain.push_back(std::make_unique<Joint>(mID, angleConstraint, meshContainer));
	mChain[0]->SetPosition(mChainOrigin);
	mChain[0]->SetTempPosition(mChainOrigin);
	/*std::string name = "j" + std::to_string(mID);
	gizmos->AddRay(name, this->mChain[0]->GetPosition(), this->mChain[0]->GetForwardVector(), 10);*/
	for (int i = 1; i < numberOfJoints; i++)
	{
		mChain.push_back(std::make_unique<Joint>(mID, angleConstraint, meshContainer));
		//creating offset between joints
		//this->chain[i]->SetPosition(this->chainStartPos + glm::vec3(DISTANCE_BETWEEN_JOINTS, 0.0f, 0.0f) * this->chain[i]->GetSegmentLength() * (float) i);
		mChain[i]->SetPosition(mChainOrigin + (glm::vec3(-DISTANCE_BETWEEN_JOINTS, 0.0f, 0.0f) * 
			mChain[i]->GetSegmentLength()  * (float)i));
		
		mChain[i]->SetParent(mChain[i - 1].get());
		mChain[i - 1]->SetChild(mChain[i].get());
		
		//std::string name = "j" + std::to_string(mID);
		//std::cout << "ID: " << id << " NAME: " << name << std::endl;
		/*gizmos->AddRay(name, this->mChain[i]->GetPosition(), this->mChain[i]->GetForwardVector(), 10);
		gizmos->AddPoint(this->mChain[i]->GetPosition());*/
	}
	//gizmos->SetupPointsBuffer();
}

void KinematicChain::BackwardsPass()
{
	//Joint* currentJoint = mChain[mChain.size() - 1].get();

	//currentJoint->SetTempPosition(mTarget->objectPos - (currentJoint->GetForwardVector() * currentJoint->GetSegmentLength()));
	//currentJoint = currentJoint->GetParent();
	//while (currentJoint->GetParent())
	//{
	//	currentJoint->SetTempPosition(
	//		currentJoint->GetChild()->GetTempPosition() + (
	//			glm::normalize(currentJoint->GetPosition() - currentJoint->GetChild()->GetTempPosition()) * (currentJoint->GetSegmentLength() + DISTANCE_BETWEEN_JOINTS)
	//			)
	//	);
	//	//no need to rotate every step, it is enough to rotate once in main loop before rendering
	//	//currentJoint->RotateTowardsTarget(this->target->objectPos);
	//	currentJoint = currentJoint->GetParent();
	//}
}

void KinematicChain::ForwardPass()
{
	for (int i = 0; i < mChain.size(); i++)
	{
		Joint* joint = mChain[i].get();
		if (!joint->GetParent())
		{
			continue;
		}
		else
		{
			joint->SetPosition(
				joint->GetParent()->GetPosition() + (
					glm::normalize(joint->GetTempPosition() - joint->GetParent()->GetPosition()) * (joint->GetSegmentLength() + DISTANCE_BETWEEN_JOINTS)
					)
			);
		}
	}
}

void KinematicChain::FabrikAlgorithm(const int numberOfIterations)
{
	////Check if target is out of reach
	//if (targetOutOfReach())
	//{
	//	//Position of start of the chain should never be changed
	//	for (int i = 1; i < mChain.size(); i++)
	//	{
	//		mChain[i]->SetPosition(glm::normalize(mTarget->objectPos - mChainOrigin)
	//			* (mChain[i]->GetSegmentLength() + DISTANCE_BETWEEN_JOINTS) * (float)i);
	//	}
	//	return;
	//}

	////end of chain has reached the target
	//else if (ErrorTooSmall())
	//{
	//	return;
	//}
	////Shouldn't put i < numberOfIterations || ErrorTooSmall() BECAUSE IT WILL RUN INFINITELY SINCE ErrorTooSmall will always return true
	//for (int i = 0; (i < numberOfIterations && !ErrorTooSmall()); i++)
	//{
	//	BackwardsPass();

	//	ForwardPass();

	//	//TODO: See if joint surpasses constraint
	//}
}

bool KinematicChain::targetOutOfReach()
{
	/*float segmentLength = mChain[0]->GetSegmentLength();
	return glm::distance(mChainOrigin, mTarget->objectPos) > (mChain.size() * (segmentLength + DISTANCE_BETWEEN_JOINTS) - DISTANCE_BETWEEN_JOINTS);*/
	return true;
}

glm::vec3 KinematicChain::CalculateNewJointPosition(Joint* joint, const float direction)
{
	/*return glm::normalize(joint->GetParent()->GetPosition() - mTarget->objectPos) * 
		direction * joint->GetSegmentLength() + joint->GetPosition();*/
	return glm::vec3(0.0f,0.0f,0.0f);
}


bool KinematicChain::ErrorTooSmall()
{
	////return (glm::distance(this->chain[this->chain.size() - 1]->GetPosition(), this->target->GetPosition()) <= ERROR_MARGIN);
	//return (glm::distance(this->mChain[this->mChain.size() - 1]->GetJointEnd(), this->mTarget->GetPosition()) < ERROR_MARGIN);
	return true;
}


//void KinematicChain::simulate()
//{
//	glm::vec3 jointTargetPos;
//	for (int i = 0; i < mChain.size(); i++)
//	{
//		Joint* joint = mChain[i].get();
//		jointTargetPos = (joint->GetChild() == nullptr ? mTarget->GetPosition() : joint->GetChild()->GetPosition());
//
//		joint->GetMeshContainer()->Translate(joint->GetPosition());
//		joint->RotateTowardsTarget(jointTargetPos);
//
//		joint->SetPosition(joint->GetTempPosition());
//		joint->RotateTowardsTarget(mTarget->objectPos);
//		std::string name = "j" + std::to_string(mID);
//		//gizmos->UpdateLine(name, joint->GetPosition(), joint->GetForwardVector(), 4);
//		//gizmos->RenderBoundingBox(joint->GetMeshContainer()->boundingBox);
//		joint->GetMeshContainer()->Translate(joint->GetPosition());
//		joint->GetMeshContainer()->Render();
//	}
//}

KinematicChain::~KinematicChain()
{
	std::cout << "Kinematic chain destructor called" << std::endl;
}