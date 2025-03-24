#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"

Scene::Scene()
{

}
void Scene::renderObjects()
{
	for (int i = 0; i < mObjectsInScene.size(); i++)
	{
		//mObjectsInScene[i]->Render(mCamera, mLightSource);
	}
}
void Scene::addObject(Mesh* object)
{
	mObjectsInScene.push_back(object);
}

void Scene::addObjects(const std::vector<Mesh*>& objects)
{
	mObjectsInScene.insert(std::end(mObjectsInScene), std::begin(objects), std::end(objects);
}

void Scene::removeObject()
{

}