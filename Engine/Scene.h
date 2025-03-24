#pragma once
#include <iostream>
#include <vector>

//Forward declaration
class Camera;
class Lighting;
class Mesh;

class Scene
{
private:
	std::vector<Mesh*> mObjectsInScene{};
	Lighting* mLightSource;
	Camera* mCamera;

public:
	//Renders all objects in scene
	void renderObjects();
	//Adds object to scene
	void addObject(Mesh* object);
	//Adds multiple objects to the scene
	void addObjects(const std::vector<Mesh*>& objects);
	void removeObject();
	Scene();
	~Scene();
};