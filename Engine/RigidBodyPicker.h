#pragma once
#include <btBulletDynamicsCommon.h>
struct RigidBodyPicker
{
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	btRigidBody* m_pickedBody;
	btVector3 m_pickPos;
	btVector3 m_oldPickingPos;
	btVector3 m_hitPos;
	btScalar m_oldPickingDist;
	btTypedConstraint* m_pickedConstraint;
	int m_savedState;

	void pickBody(const btVector3& rayFromWorld, const btVector3& rayToWorld)
	{
		if (m_pickedBody)
		{
			if (!(m_pickedBody->isStaticObject() || m_pickedBody->isKinematicObject()))
			{
				m_savedState = m_pickedBody->getActivationState();
				m_pickedBody->setActivationState(DISABLE_DEACTIVATION);
				btVector3 localPivot = m_pickedBody->getCenterOfMassTransform().inverse() * m_pickPos;
				btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*m_pickedBody, localPivot);
				m_dynamicsWorld->addConstraint(p2p, true);
				m_pickedConstraint = p2p;
				btScalar mousePickClamping = 30.0f;
				p2p->m_setting.m_impulseClamp = mousePickClamping;
				p2p->m_setting.m_tau = 0.001f;
			}
			m_oldPickingPos = rayToWorld;
			m_hitPos = m_pickPos;
			m_oldPickingDist = (m_pickPos - rayFromWorld).length();
		}
		
	}
	void movePickedBody(const btVector3& rayFromWorld, const btVector3& rayToWorld)
	{
		if (m_pickedBody && m_pickedConstraint)
		{
			btPoint2PointConstraint* pickCon = static_cast<btPoint2PointConstraint*>(m_pickedConstraint);
			if (pickCon)
			{
				btVector3 newPivotB;

				btVector3 dir = rayToWorld - rayFromWorld;
				dir.normalize();
				dir *= m_oldPickingDist;

				newPivotB = rayFromWorld + dir;
				pickCon->setPivotB(newPivotB);
			}
		}
	}

	void removePickingConstraint()
	{
		if (m_pickedConstraint)
		{
			m_pickedBody->forceActivationState(m_savedState);
			m_pickedBody->activate();
			m_dynamicsWorld->removeConstraint(m_pickedConstraint);
			delete m_pickedConstraint;
			m_pickedConstraint = 0;
			m_pickedBody = 0;
		}
	}
};