#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshLoader.h"
#include "Scene.h"
#include "Lighting.h"

Scene::Scene(Camera* camera, Lighting* lightSource, std::vector<std::unique_ptr<Mesh>>& objectsInScene, std::vector<std::unique_ptr<MeshLoader>>& meshLoaders, Shader* objectShader, Shader* boundingBoxShader) 
	: mCamera{ camera }, mLightSource{ lightSource }, mObjectsInScene{ objectsInScene }, 
	mMeshLoaders { meshLoaders }, mObjectShader{ objectShader }, mBoundingBoxShader{ boundingBoxShader }
{
	loadDefaultScene();
}

void Scene::loadDefaultScene()
{
	//BLENDER: rotate around X for 270 (-90) degrees,	EXPORT: forward: -X, up: Z
	//forward: -X, up: Z - file: joint.blend
	//MeshLoader lightBulbLoader("lightBulb.txt");
	//MeshLoader cubeLoader("cubeFlat.txt");
	mMeshLoaders.push_back(std::move(std::make_unique<MeshLoader>("templeFlat.txt")));
	mMeshLoaders.push_back(std::move(std::make_unique<MeshLoader>("dragonSmooth.txt")));
	mMeshLoaders.push_back(std::move(std::make_unique<MeshLoader>("frogSmooth.txt")));
	mMeshLoaders.push_back(std::move(std::make_unique<MeshLoader>("teddyFlat.txt")));
	mMeshLoaders.push_back(std::move(std::make_unique<MeshLoader>("sphere.txt")));
	mMeshLoaders.push_back(std::move(std::make_unique<MeshLoader>("cone.txt")));

	/*for (int i = 0; i < mMeshLoaders.size(); i++)
	{
		std::cout << "MESH LOADER (SCENE) " << i << " VERTICES: " << mMeshLoaders[i]->vertices.size() << std::endl;
	}*/

	mObjectsInScene.push_back(std::move(std::make_unique<Mesh>(mObjectShader, mBoundingBoxShader, 
		mMeshLoaders[0].get(), glm::vec3(-5.0f, 4.0f, 0.0f))));
	mObjectsInScene.push_back(std::move(std::make_unique<Mesh>(mObjectShader, mBoundingBoxShader, 
		mMeshLoaders[1].get(), glm::vec3(5.0f, 4.0f, 0.0f))));
	mObjectsInScene.push_back(std::move(std::make_unique<Mesh>(mObjectShader, mBoundingBoxShader,
		mMeshLoaders[2].get(), glm::vec3(-35.0f, 0.0f, 0.0f))));
	//Mesh* temple = new Mesh(mObjectShader, mBoundingBoxShader, &templeLoader, glm::vec3(-5.0f, 4.0f, 0.0f), id++);
	//Mesh* dragon = new Mesh(mObjectShader, mBoundingBoxShader, &dragonLoader, glm::vec3(5.0f, 4.0f, 0.0f), id++);
	//Mesh* cube = new Mesh(mObjectShader, mBoundingBoxShader, &cubeLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);
	//Mesh* sphere = new Mesh(mObjectShader, mBoundingBoxShader, &sphereLoader, glm::vec3(-35.0f, 0.0f, 0.0f), id++);
	//Mesh* cone = new Mesh(&coneLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);
	//Mesh* joint = new Mesh(mObjectShader, mBoundingBoxShader, &jointLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);

	//mObjectsInScene.push_back(lightBulb);
	//mObjectsInScene.push_back(dragon);
	//mObjectsInScene.push_back(cube);
	//mObjectsInScene.push_back(sphere);
	//mObjectsInScene.push_back(temple);
	//mObjectsInScene.push_back(joint);

}

void Scene::renderScene()
{
	//clear screen
	glClearColor(0.247059f, 0.247059f, 0.247059f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderLight();
	renderObjects();
}

void Scene::renderLight()
{
	mLightSource->Draw(*mCamera);
}

void Scene::renderObjects()
{
	//std::cout << "SCENE OBJECTS: " << mObjectsInScene.size() << std::endl;
	for (int i = 0; i < mObjectsInScene.size(); i++)
	{
		if (mObjectsInScene[i]) mObjectsInScene[i]->Render(*mCamera, *mLightSource);
	}

	/*for (int i = 0; i < mMeshLoaders.size(); i++)
	{
		std::cout << "MESH LOADER (SCENE) " << i << " VERTICES: " << mMeshLoaders[i]->vertices.size() << std::endl;

	}*/
}

void Scene::addObject(std::unique_ptr<Mesh> object)
{
	mObjectsInScene.push_back(std::move(object));
}

void Scene::addObjects(const std::vector<Mesh*>& objects)
{
	mObjectsInScene.insert(std::end(mObjectsInScene), std::begin(objects), std::end(objects));
}

void Scene::removeObject()
{

}