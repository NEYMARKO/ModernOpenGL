#include "MeshLoader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Lighting.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Object.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "RigidBody.h"
#include <string>
#include "Scene.h"
Scene::Scene(Camera* camera, Lighting* lightSource, std::vector<std::unique_ptr<Object>>& objectsInScene, /*std::vector<std::unique_ptr<MeshLoader>>& meshLoaders,*/ Shader* objectShader, Shader* boundingBoxShader) 
	: mCamera{ camera }, mLightSource{ lightSource }, mObjectsInScene{ objectsInScene }, 
	/*mMeshLoaders { meshLoaders },*/ mObjectShader{ objectShader }, mBoundingBoxShader{ boundingBoxShader }
{
	loadDefaultScene();
}

void Scene::loadDefaultScene()
{
	//BLENDER: rotate around X for 270 (-90) degrees,	EXPORT: forward: -X, up: Z
	//forward: -X, up: Z - file: joint.blend

	MeshLoader templeLoader("templeFlat.txt");
	MeshLoader dragonLoader("dragonSmooth.txt");
	MeshLoader frogLoader("frogSmooth.txt");
	MeshLoader floorLoader("planeFlat.txt");

	auto templeTransform = Transform(glm::vec3(-5.0f, 4.0f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	auto dragonTransform = Transform(glm::vec3(5.0f, 50.0f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	auto frogTransform = Transform(glm::vec3(5.0f, 35.0f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	auto floorTransform = Transform(glm::vec3(0.0f, -4.0f, 0.0f),
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));

	auto templeMesh = ResourceManager<Mesh>::addResource("temple", std::make_unique<Mesh>(mBoundingBoxShader, &templeLoader));
	auto dragonMesh = ResourceManager<Mesh>::addResource("dragom", std::make_unique<Mesh>(mBoundingBoxShader, &dragonLoader));
	auto frogMesh = ResourceManager<Mesh>::addResource("frog", std::make_unique<Mesh>(mBoundingBoxShader, &frogLoader));
	auto floorMesh = ResourceManager<Mesh>::addResource("floor", std::make_unique<Mesh>(mBoundingBoxShader, &floorLoader));

	auto templeMaterial = ResourceManager<Material>::addResource("temple", std::make_unique<Material>(mObjectShader));
	auto dragonMaterial = ResourceManager<Material>::addResource("dragon", std::make_unique<Material>(mObjectShader));
	auto frogMaterial = ResourceManager<Material>::addResource("frog", std::make_unique<Material>(mObjectShader));
	auto floorMaterial = ResourceManager<Material>::addResource("floor", std::make_unique<Material>(mObjectShader));

	auto templeRenderer = MeshRenderer(templeMesh, templeMaterial);
	auto dragonRenderer = MeshRenderer(dragonMesh, dragonMaterial);
	auto frogRenderer = MeshRenderer(frogMesh, frogMaterial);
	auto floorRenderer = MeshRenderer(floorMesh, floorMaterial);

	mObjectsInScene.push_back(std::make_unique<Object>(std::move(templeTransform), std::move(templeRenderer), "temple"));
	mObjectsInScene.push_back(std::make_unique<Object>(std::move(dragonTransform), std::move(dragonRenderer), "dragon"));
	
	auto dragonCollider = std::make_unique<SphereCollider>(1.0f);
	mObjectsInScene.back()->addComponent(std::move(dragonCollider));
	auto dragonRigidBody = std::make_unique<RigidBody>(1.0f, 0.8f);
	mObjectsInScene.back()->addComponent(std::move(dragonRigidBody));
	
	mObjectsInScene.push_back(std::make_unique<Object>(std::move(frogTransform), std::move(frogRenderer), "frog"));
	
	auto frogCollider = std::make_unique<SphereCollider>(1.0f);
	mObjectsInScene.back()->addComponent(std::move(frogCollider));
	auto frogRigidBody = std::make_unique<RigidBody>(0.3f, 0.8f);
	mObjectsInScene.back()->addComponent(std::move(frogRigidBody));

	mObjectsInScene.push_back(std::make_unique<Object>(std::move(floorTransform), std::move(floorRenderer), "floor"));

	Transform* floorTransformPtr = mObjectsInScene.back()->getComponent<Transform>();
	auto floorCollider = std::make_unique<PlaneCollider>(floorTransformPtr->glmToBulletVec3(glm::vec3(0.0f, 1.0f, 0.0f)), 0.0f);
	mObjectsInScene.back()->addComponent(std::move(floorCollider));
	auto floorRigidBody = std::make_unique<RigidBody>(0.0f, 0.8f);
	mObjectsInScene.back()->addComponent(std::move(floorRigidBody));

	MeshLoader cubeLoader("cubeFlat.txt");
	MeshLoader jointLoader("joint4.txt");

	auto cubeTransform = Transform(glm::vec3(3.2f, 1.7f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	auto jointTransform = Transform(glm::vec3(-30.0f, 0.0f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));

	auto cubeMesh = ResourceManager<Mesh>::addResource("cube", std::make_unique<Mesh>(mBoundingBoxShader, &cubeLoader));
	auto jointMesh = ResourceManager<Mesh>::addResource("joint", std::make_unique<Mesh>(mBoundingBoxShader, &jointLoader));

	auto cubeMaterial = ResourceManager<Material>::addResource("cube", std::make_unique<Material>(mObjectShader));
	auto jointMaterial = ResourceManager<Material>::addResource("joint", std::make_unique<Material>(mObjectShader));

	auto jointRenderer = std::make_unique<MeshRenderer>(jointMesh, jointMaterial);
	auto cubeRenderer = MeshRenderer(cubeMesh, cubeMaterial);
	
	mObjectsInScene.push_back(std::make_unique<Object>(
		std::move(cubeTransform), std::move(cubeRenderer), "cube"
	));

	m_ikChain = std::make_unique<KinematicChain>(14, 45.0f,
		glm::vec3(0.0f, 0.0f, 0.0f), mObjectsInScene.back()->getComponent<Transform>());
	
	m_ikChain.get()->setMeshRenderer(std::move(jointRenderer));

}

void Scene::renderScene()
{
	//clear screen
	glClearColor(0.247059f, 0.247059f, 0.247059f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderLight();
	renderObjects();
	
	renderIKChain();

	elapsedTime += 0.005f;

	m_ikChain.get()->moveTarget(elapsedTime);
}

void Scene::renderLight()
{
	mLightSource->Draw(*mCamera);
}

void Scene::renderObjects()
{
	//need to use constant reference to avoid copying elements of mObjectsInScene
	//into obj
	for (const std::unique_ptr<Object>& obj : mObjectsInScene)
	{
		if (Object* object = obj.get())
			if (MeshRenderer* mr = object->getComponent<MeshRenderer>())
				mr->draw(*mCamera, *mLightSource);
	}
}

void Scene::renderIKChain()
{
	m_ikChain.get()->simulate(10);
	std::vector<Transform*>& jointTransforms = m_ikChain.get()->getJointsTransforms();
	for (Transform* transform : jointTransforms)
	{
		m_ikChain.get()->getMeshRenderer()->draw(*mCamera, *mLightSource, transform);
	}
}
void Scene::addObject(std::unique_ptr<Object> object)
{
	mObjectsInScene.push_back(std::move(object));
}

//void Scene::addObjects(const std::vector<Mesh*>& objects)
//{
//	mObjectsInScene.insert(std::end(mObjectsInScene), std::begin(objects), std::end(objects));
//}

void Scene::removeObject()
{

}