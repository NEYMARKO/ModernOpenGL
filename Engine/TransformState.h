#pragma once
#include <glm/glm.hpp>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
//#undef GLFW_INCLUDE_NONE

#include "State.h"
#include "TransformPlane.h"
//#include "OpenGLIncludes.h"

class Camera;
class Transform;

enum TransformAxis
{
	NONE,
	X,
	Y,
	Z
};

//struct TransformPlane
//{
//	glm::vec3 normal;
//	float D;
//	glm::vec3 intersectionPoint;
//
//	void calculatePlaneParameters(Camera* camera, Object* object);
//	void updateIntersectionPoint(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld);
//	//glm::vec3 projectAxisOntoPlane(const glm::vec3& localAxis);
//};

class TransformState : public State
{
protected:
	const glm::vec3 m_worldForward{0.0f, 0.0f, -1.0f};
	const glm::vec3 m_worldRight{1.0f, 0.0f, 0.0f};
	const glm::vec3 m_worldUp{0.0f, 1.0f, 0.0f};
	glm::vec3 m_projectedAxis;
	glm::vec3 m_infiniteAxis;
	bool m_trackingMouse{ true };
	Camera* m_camera;
	Transform* m_selectedTransform;
	TransformPlane m_transformPlane;
	TransformAxis m_transformAxis{NONE};
	bool m_freeMode = false;
	glm::vec4 mouseWorldPos;
	glm::vec3 mouseWorldDirection;
	void toggleFreeMode() { m_freeMode = !m_freeMode; };
	void update();
public:
	TransformState(StateMachine* stateMachine, Camera* camera, Transform* objectTransform);

	void enter();
	virtual void onMouseMove(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld) override;
	virtual void onMouseClick(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld,
		int button, int action) override;
	virtual void onKeyboardPress(int keyCode, int action) override;
};