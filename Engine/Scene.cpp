#include "MeshLoader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Lighting.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Object.h"
#include "SphereCollider.h"
#include "RigidBody.h"
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

	auto templeTransform = std::make_unique<Transform>(glm::vec3(-5.0f, 4.0f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	auto dragonTransform = std::make_unique<Transform>(glm::vec3(5.0f, 50.0f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	auto frogTransform = std::make_unique<Transform>(glm::vec3(-35.0f, 0.0f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));

	auto templeRenderer = std::make_unique<MeshRenderer>(nullptr, std::move(std::make_unique<Mesh>(mBoundingBoxShader, &templeLoader)), std::move(std::make_unique<Material>(mObjectShader)));
	auto dragonRenderer = std::make_unique<MeshRenderer>(nullptr, std::move(std::make_unique<Mesh>(mBoundingBoxShader, &dragonLoader)), std::move(std::make_unique<Material>(mObjectShader)));
	auto frogRenderer = std::make_unique<MeshRenderer>(nullptr, std::move(std::make_unique<Mesh>(mBoundingBoxShader, &frogLoader)), std::move(std::make_unique<Material>(mObjectShader)));

	mObjectsInScene.push_back(std::make_unique<Object>(std::move(templeTransform), std::move(templeRenderer)));
	mObjectsInScene.push_back(std::make_unique<Object>(std::move(dragonTransform), std::move(dragonRenderer)));
	
	auto dragonCollider = std::make_unique<SphereCollider>(1.0f);
	mObjectsInScene.back()->addComponent(std::move(dragonCollider));
	auto dragonRigidBody = std::make_unique<RigidBody>(1.0f, 0.8f);
	mObjectsInScene.back()->addComponent(std::move(dragonRigidBody));
	
	mObjectsInScene.push_back(std::make_unique<Object>(std::move(frogTransform), std::move(frogRenderer)));
	

	MeshLoader cubeLoader("cubeFlat.txt");
	MeshLoader jointLoader("joint4.txt");

	auto cube = std::make_unique<Mesh>(mBoundingBoxShader, &cubeLoader);
	auto joint = std::make_unique<Mesh>(mBoundingBoxShader, &jointLoader);

	auto cubeTransform = std::make_unique<Transform>(glm::vec3(3.2f, 1.7f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	auto jointTransform = std::make_unique<Transform>(glm::vec3(-30.0f, 0.0f, 0.0f), 
		glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
	
	auto jointRenderer = std::make_unique<MeshRenderer>(nullptr, std::move(joint), std::make_unique<Material>(mObjectShader));
	auto cubeRenderer = std::make_unique<MeshRenderer>(nullptr, std::move(cube), 
		std::make_unique<Material>(mObjectShader));
	
	mObjectsInScene.push_back(std::make_unique<Object>(
		std::move(cubeTransform), std::move(cubeRenderer)
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