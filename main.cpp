#include "btBulletDynamicsCommon.h"
#include <stdio.h>
#include "Ogre\Ogre.h"

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OISInputManager.h>

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
// Это относительно простой компонент внешне, а на самом деле он
// обеспечивает симуляцию физики с компонентами, которые обрабатывают 
// основные задачи, такие как определить, как Bullet управляет распределением памяти,
// обеспечивает алгоритмы решения различных столкновений (Box-Box, Sphere-Box, и так далее),
// и как управлять данными, получаемыми из широкой фазы определения столкновений под 
// названием Коллекторы (Manifolds). Для этого проекта, мы будем делать всё просто и использовать 
// для столкновений Bullet объект конфигурации по умолчанию, btDefaultCollisionConfiguration. 
	btDefaultCollisionConfiguration* collisionConfiguration;
// Диспетчер столкновений, как следует из названия, отправляет столкновения в наше
// приложение. Для видео игры, она практически гарантирует, что мы хотим,
// быть в курсе взаимодействия объектов столкновения в какой-то момент, и это цель
// Диспетчера столкновений.
// Один из встроенных в определения класса Диспетчер столкновений, которые приходят с пули
// основной btCollisionDispatcher, единственное требование состоит в том, что он 
// должен быть подан с объектом конфигурации столкновений в своем конструкторе 
// (что заставляет нас создавать этот объект)
	btCollisionDispatcher* dispatcher;
// Объект должен рассказать нашему миру какую технику объект должен
// использовать для обнаружения столкновений широкий фазы, мы будем использовать btDbvtBroadphase. 
	btBroadphaseInterface* overlappingPairCache;
// Задача решателя (constraint solver) в том, чтобы наши объекты реагировали 
// на конкретные ограничения
	btSequentialImpulseConstraintSolver* solver;
// Объект первичного контроля за симуляцией физики Bullet экземпляр btDynamicsWorld.
// Все физические объекты будут управляться по правилам определенными у этого класса.
// Есть несколько типов btDynamicsWorld которые могут быть использованы,
// в зависимости от того, как мы хотите, настроить моделирование физических процессов,
// но тот, который мы будет использовать это btDiscreteDynamicsWorld.
// Этот мир перемещает объекты в дискретными шагагами (отсюда название) в пространстве,
// с течением время.
// Этот класс не определяет, как обнаружить столкновения, или как объекты
// ответитят на столкновения. Это определяет только, как они движутся в ответ
// на активизацию моделирование с течением времени.
	btDiscreteDynamicsWorld* dynamicsWorld;
// </Переменные Bullet Physics>

// <Переменные OIS>
	OIS::InputManager *m_InputManager;
	OIS::Keyboard     *m_Keyboard;
// </Переменные OIS>

// <Тест>
	Ogre::Entity* sphere001;
	Ogre::SceneNode *node001;
	btRigidBody* fallRigidBody;
	btCollisionShape* groundShape;

	float f_c_X;
	float f_c_Y;
	float f_c_Z;
// </Тест>

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
//		sceneManager->setSkyBox(true, "Examples/SpaceSkyBox");

		f_c_X = 0; f_c_Y = 0; f_c_Z = 0;
		camera->pitch(Ogre::Degree(f_c_X));
		camera->yaw(Ogre::Degree(f_c_Y));
		camera->roll(Ogre::Degree(f_c_Z));
	}
// </Инициализация графики>

// <Инициализация физики>
	void physics_init()
	{
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new	btCollisionDispatcher(collisionConfiguration);
		overlappingPairCache = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver;
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
		dynamicsWorld->setGravity(btVector3(0,-10,0));
	}
// </Инициализация физики>

//<Инициализация ввода>
	void input_init()
	{
		size_t hWnd = 0;
		window->getCustomAttribute("WINDOW", &hWnd);
		m_InputManager = OIS::InputManager::createInputSystem(hWnd);
		m_Keyboard = static_cast<OIS::Keyboard*>(m_InputManager->createInputObject(OIS::OISKeyboard, false));
	}
//</Инициализация ввода>

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
		dynamicsWorld->stepSimulation(1 / 60.f, 10);
	}
// </Просчёт физики>

//<Обработка ввода>
	void input_execution()
	{
		m_Keyboard->capture();
		if (m_Keyboard->isKeyDown(OIS::KC_UP))
		{
			f_c_X += 10;
			camera->pitch(Ogre::Degree(f_c_X));
		}
		if (m_Keyboard->isKeyDown(OIS::KC_DOWN))
		{
			f_c_X -= 10;
			camera->pitch(Ogre::Degree(f_c_X));
		}
	}
//<Обработка ввода>

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
		sphere001 = sceneManager->createEntity("Sphere.mesh");
		node001 = sceneManager->createSceneNode("node001");
		sceneManager->getRootSceneNode()->addChild(node001);
		node001->attachObject(sphere001);
		//
		Ogre::Light* light1 = sceneManager->createLight("Light1"); 
		light1->setType(Ogre::Light::LT_POINT); 
		light1->setPosition(0,50,0); 
		light1->setDiffuseColour(1.0f,1.0f,1.0f); 
		//

		// Bullet
		btCollisionShape* fallShape = new btSphereShape(1);
		btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		fallShape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
		fallRigidBody = new btRigidBody(fallRigidBodyCI);
		dynamicsWorld->addRigidBody(fallRigidBody);
		//
		groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
		dynamicsWorld->addRigidBody(groundRigidBody);
	}

// </Тестовый бкок>



// <Инициализация>
	void init()
	{
		graphics_init();
		physics_init();
		input_init();
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
		input_execution();
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
