#pragma once
#include <glm/glm.hpp>
#include "State.h"
//#include "OpenGLIncludes.h"

class Camera;
class Object;

struct TransformPlane
{
	glm::vec3 normal;
	float D;
	glm::vec3 intersectionPoint;

	void calculatePlaneParameters(Camera* camera, Object* object);
	void updateIntersectionPoint(const glm::vec3& mouseStartWorld, const glm::vec3& mouseDirectionWorld);
	//glm::vec3 projectAxisOntoPlane(const glm::vec3& localAxis);
};

class TransformState : public State
{
protected:
	TransformPlane m_transformPlane;
	Camera* m_camera;
	Object* m_selectedObject;
	bool m_freeMode = false;
	glm::vec4 mouseWorldPos;
	glm::vec3 mouseWorldDirection;
	void toggleFreeMode() { m_freeMode = !m_freeMode; };
	void update();
public:
	TransformState(StateMachine* stateMachine, Camera* camera, Object* object) :
		State{ stateMachine }, m_camera{ camera }, m_selectedObject{ object }
	{ }

	void enter();
	void onKeyboardPress(const int keyCode, const int action);
	void onMouseMove(const double mouseX, const double mouseY);
	void exit();
};