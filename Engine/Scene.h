#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include "KinematicChain.h"
#include "RigidBodyRegistry.h"
#include "ResourceManager.h"

//Forward declaration
class Shader;
class Camera;
class Lighting;
class Mesh;
class MeshLoader;
class Object;

class Scene
{
private:
	//No need for cleanup if using smart pointers
	std::vector<std::unique_ptr<Object>>& mObjectsInScene;
	std::unique_ptr<KinematicChain> m_ikChain;
	Lighting* mLightSource;
	Camera* mCamera;
	Shader* mObjectShader;
	Shader* mBoundingBoxShader;

	float elapsedTime = 0.0f;

	RigidBodyRegistry m_rigidBodyRegistry;

public:
	Scene(Camera* camera, Lighting* lightSource, std::vector<std::unique_ptr<Object>>& objectsInScene, Shader* objectShader);
	void loadDefaultScene();

	void renderScene();
	void renderLight();
	//Renders all objects in scene
	void renderObjects();
	void renderIKChain();
	//Adds object to scene
	void addObject(std::unique_ptr<Object> object);
	//Adds multiple objects to the scene
	//void addObjects(const std::vector<Mesh*>& objects);
	void removeObject();
};