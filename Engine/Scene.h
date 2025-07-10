#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include "KinematicChain.h"
#include "RigidBodyRegistry.h"
#include "ResourceManager.h"
#include "Object.h"
#include "SceneEntity.h"

//Forward declaration
class Shader;
class Camera;
class Lighting;
class Mesh;
class MeshLoader;
//class Object;
//struct SceneEntity;
class Ragdoll;

class Scene
{
private:
	//No need for cleanup if using smart pointers
	//std::vector<std::unique_ptr<Object>>& m_objectsInScene;
	std::vector<std::unique_ptr<SceneEntity>>& m_objectsInScene;
	std::unique_ptr<KinematicChain> m_ikChain;
	Ragdoll* m_ragdoll;
	Lighting* mLightSource;
	Camera* mCamera;
	Shader* mObjectShader;
	Shader* mBoundingBoxShader;

	float elapsedTime = 0.0f;

	RigidBodyRegistry m_rigidBodyRegistry;

public:
	/*Scene(Camera* camera, Lighting* lightSource, std::vector<std::unique_ptr<Object>>& objectsInScene, Shader* objectShader);*/
	Scene(Camera* camera, Lighting* lightSource, std::vector<std::unique_ptr<SceneEntity>>& objectsInScene, Shader* objectShader);
	void loadDefaultScene();

	void renderScene();
	void renderLight();
	//Renders all objects in scene
	void renderObjects();
	void renderIKChain();
	void addRagdoll(Ragdoll* ragdoll);
	void renderRagdoll();
	//Adds object to scene
	void addObject(std::unique_ptr<Object> object);
	//Adds multiple objects to the scene
	//void addObjects(const std::vector<Mesh*>& objects);
	void removeObject();
};