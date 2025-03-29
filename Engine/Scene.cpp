#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshLoader.h"
#include "Scene.h"

Scene::Scene(Camera* camera, Lighting* lightSource, std::vector<Mesh*>& objectsInScene, std::vector<MeshLoader*>& meshLoaders,
	Shader* objectShader, Shader* boundingBoxShader) 
	: mCamera{ camera }, mLightSource{lightSource}, mObjectsInScene{objectsInScene}, mMeshLoaders {meshLoaders}, 
	mObjectShader{ objectShader }, mBoundingBoxShader{ boundingBoxShader }
{
	loadDefaultScene();
}

Scene::~Scene()
{
	for (int i = mObjectsInScene.size() - 1; i >= 0; i--)
	{
		//without this line mObjectsInScene[i] would still contain pointer to deleted memory
		//which could lead to undefined behaviour if accessed
		delete mObjectsInScene[i];
		mObjectsInScene[i] = nullptr;
	}
	mObjectsInScene.clear();
}

void Scene::loadDefaultScene()
{
	//BLENDER: rotate around X for 270 (-90) degrees,	EXPORT: forward: -X, up: Z
	//forward: -X, up: Z - file: joint.blend
	//MeshLoader lightBulbLoader("lightBulb.txt");
	//MeshLoader cubeLoader("cubeFlat.txt");
	MeshLoader dragonLoader("dragonSmooth.txt");
	MeshLoader templeLoader("templeFlat.txt");
	MeshLoader frogLoader("frogSmooth.txt");
	MeshLoader teddyLoader("teddyFlat.txt");
	MeshLoader sphereLoader("sphere.txt");
	MeshLoader coneLoader("cone.txt");
	//MeshLoader jointLoader("joint4.txt");

	//mMeshLoaders.push_back(&lightBulbLoader);
	//mMeshLoaders.push_back(&cubeLoader);
	
	mMeshLoaders.push_back(&dragonLoader);
	mMeshLoaders.push_back(&templeLoader);
	mMeshLoaders.push_back(&frogLoader);
	mMeshLoaders.push_back(&teddyLoader);
	mMeshLoaders.push_back(&sphereLoader);
	mMeshLoaders.push_back(&coneLoader);
	
	//mMeshLoaders.push_back(&jointLoader);

	for (int i = 0; i < mMeshLoaders.size(); i++)
	{
		std::cout << "MESH LOADER (SCENE) " << i << " VERTICES: " << mMeshLoaders[i]->vertices.size() << std::endl;
	}

	float id = 0;

	Mesh* temple = new Mesh(mObjectShader, mBoundingBoxShader, &templeLoader, glm::vec3(-5.0f, 4.0f, 0.0f), id++);
	Mesh* dragon = new Mesh(mObjectShader, mBoundingBoxShader, &dragonLoader, glm::vec3(5.0f, 4.0f, 0.0f), id++);
	//Mesh* cube = new Mesh(mObjectShader, mBoundingBoxShader, &cubeLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);
	Mesh* sphere = new Mesh(mObjectShader, mBoundingBoxShader, &sphereLoader, glm::vec3(-35.0f, 0.0f, 0.0f), id++);
	//Mesh* cone = new Mesh(&coneLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);
	//Mesh* joint = new Mesh(mObjectShader, mBoundingBoxShader, &jointLoader, glm::vec3(0.0f, 0.0f, 0.0f), id++);

	//mObjectsInScene.push_back(lightBulb);
	mObjectsInScene.push_back(dragon);
	//mObjectsInScene.push_back(cube);
	mObjectsInScene.push_back(sphere);
	mObjectsInScene.push_back(temple);
	//mObjectsInScene.push_back(joint);

}

void Scene::renderObjects()
{
	//std::cout << "SCENE OBJECTS: " << mObjectsInScene.size() << std::endl;
	//clear screen
	glClearColor(0.247059f, 0.247059f, 0.247059f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < mObjectsInScene.size(); i++)
	{
		mObjectsInScene[i]->Render(*mCamera, *mLightSource);
	}

	for (int i = 0; i < mMeshLoaders.size(); i++)
	{
		std::cout << "MESH LOADER (SCENE) " << i << " VERTICES: " << mMeshLoaders[i]->vertices.size() << std::endl;

	}
}

void Scene::addObject(Mesh* object)
{
	mObjectsInScene.push_back(object);
}

void Scene::addObjects(const std::vector<Mesh*>& objects)
{
	mObjectsInScene.insert(std::end(mObjectsInScene), std::begin(objects), std::end(objects));
}

void Scene::removeObject()
{

}