#pragma once
#include <iostream>
#include <vector>
#include <memory>

//Forward declaration
class Shader;
class Camera;
class Lighting;
class Mesh;
class MeshLoader;

class Scene
{
private:
	//No need for cleanup if using smart pointers
	std::vector<std::unique_ptr<Mesh>>& mObjectsInScene;
	std::vector<std::unique_ptr<MeshLoader>>& mMeshLoaders; 
	Lighting* mLightSource;
	Camera* mCamera;
	Shader* mObjectShader;
	Shader* mBoundingBoxShader;

public:
	Scene(Camera* camera, Lighting* lightSource, std::vector<std::unique_ptr<Mesh>>& objectsInScene, std::vector<std::unique_ptr<MeshLoader>>& meshLoaders,
		Shader* objectShader, Shader* boundingBoxShader);
	void loadDefaultScene();

	void renderScene();
	void renderLight();
	//Renders all objects in scene
	void renderObjects();
	//Adds object to scene
	void addObject(std::unique_ptr<Mesh> object);
	//Adds multiple objects to the scene
	void addObjects(const std::vector<Mesh*>& objects);
	void removeObject();
};