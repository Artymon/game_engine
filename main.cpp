#include "btBulletDynamicsCommon.h"
#include <stdio.h>
#include "Ogre\Ogre.h"

//<Тест>
#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OISInputManager.h>
//</Тест>

class game_engine
{

private:

// <Переменные Ogre3D>
	Ogre::Root * root;
	Ogre::RenderWindow * window;
	Ogre::SceneManager * sceneManager;
	Ogre::Camera * camera;
	Ogre::Viewport * viewport;
// </Переменные Ogre3D>

// <Переменные Bullet Physics>
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
// </Переменные Bullet Physics>



// <Тестовые переменные>
//	OIS::InputManager m_InputManager;
//	OIS::Keyboard     *mKeyboard;

	Ogre::Entity* sphere001;
	Ogre::SceneNode *node001;
	btRigidBody* fallRigidBody;
// </Тестовые переменные>


//<Тест>
//	void input_init()
//	{
//		size_t hWnd = 0;
//		window->getCustomAttribute("WINDOW", &hWnd);
//		m_InputManager = OIS::InputManager::createInputSystem(hWnd);
//		m_Keyboard = static_cast<OIS::Keyboard*>(m_InputManager->createInputObject(OIS::OISKeyboard, false));
//	}
//</Тест>

// <Инициализация графики>
	void graphics_init()
	{
		root = new Ogre::Root("plugins.cfg");
		root->showConfigDialog();
		window = root->initialise(true, "Test");
		sceneManager = root->createSceneManager(Ogre::ST_GENERIC);
		camera = sceneManager->createCamera("Camera");
		camera->setPosition(Ogre::Vector3(0,0,50));
		camera->lookAt(Ogre::Vector3(0,0,50));
		camera->setNearClipDistance(5);
		viewport = window->addViewport(camera);
		viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
		camera->setAspectRatio(Ogre::Real(viewport->getActualWidth())/Ogre::Real(viewport->getActualHeight()));
	}
// </Инициализация графики>
int i;
// <Инициализация физики>
	void physics_init()
	{
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new	btCollisionDispatcher(collisionConfiguration);
		overlappingPairCache = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver;
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
		dynamicsWorld->setGravity(btVector3(0,-10,0));
		i=0;
	}
// </Инициализация физики>

// <Рендеринг кадра>
	void graphics_execution()
	{
		Ogre::WindowEventUtilities::messagePump();
		root->renderOneFrame();
	}
// </Рендеринг кадра>

// <Просчёт физики>
	void physics_execution()
	{
		for (; i<100; i++)
		dynamicsWorld->stepSimulation(1 / 60.f, 10);
		// <Тест>
		
		// </Тест>
	}
// </Просчёт физики>

public:



// <Тестовый блок>

	void test_load_resource()
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("resource/sphere.zip","Zip");
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}

	void test_create_objects()
	{
		// Ogre
		sphere001 = sceneManager->createEntity("sphere.mesh");
		node001 = sceneManager->createSceneNode("node001");
		sceneManager->getRootSceneNode()->addChild(node001);
		node001->attachObject(sphere001);
		// Bullet
		btCollisionShape* fallShape = new btSphereShape(1);
		btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		fallShape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
		fallRigidBody = new btRigidBody(fallRigidBodyCI);
		dynamicsWorld->addRigidBody(fallRigidBody);
	}

// </Тестовый бкок>



// <Инициализация>
	void init()
	{
		graphics_init();
		physics_init();
	}
// </Инициализация>

// <Выполнение>
	void execution()
	{
		// <Тест>
		btTransform trans;
		fallRigidBody->getMotionState()->getWorldTransform(trans);
		btQuaternion rot = trans.getRotation();
		node001->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
		btVector3 pos = trans.getOrigin();
		node001->setPosition(pos.x(), pos.y(), pos.z());
		// </Тест>
		graphics_execution();
		physics_execution();
	}
// </Выполнение>

};

int main (void)
{
	game_engine game_one;
	game_one.init();
	game_one.test_load_resource();
	game_one.test_create_objects();
	while(true)
	{
		game_one.execution();
	}
	return 0;
}
