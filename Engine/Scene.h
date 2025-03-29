#pragma once
#include <iostream>
#include <vector>

//Forward declaration
class Shader;
class Camera;
class Lighting;
class Mesh;
class MeshLoader;

class Scene
{
private:
	std::vector<Mesh*>& mObjectsInScene;
	std::vector<MeshLoader*>& mMeshLoaders; //THIS HAS TO BE MADE REFERENCE IF IT WILL BE USED (LOOK AT THE LINE ABOVE)
	Lighting* mLightSource;
	Camera* mCamera;
	Shader* mObjectShader;
	Shader* mBoundingBoxShader;

public:
	Scene(Camera* camera, Lighting* lightSource, std::vector<Mesh*>& objectsInScene, std::vector<MeshLoader*>& meshLoaders,
		Shader* objectShader, Shader* boundingBoxShader);
	~Scene();
	void loadDefaultScene();
	//Renders all objects in scene
	void renderObjects();
	//Adds object to scene
	void addObject(Mesh* object);
	//Adds multiple objects to the scene
	void addObjects(const std::vector<Mesh*>& objects);
	void removeObject();
};