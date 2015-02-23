#include "btBulletDynamicsCommon.h"
#include <stdio.h>
#include "Ogre\Ogre.h"

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

	}
// </Просчёт физики>

public:

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
		graphics_execution();
		physics_execution();
	}
// </Выполнение>

};

int main (void)
{
	game_engine game_one;
	game_one.init();
	while(true)
	{
		game_one.execution();
	}
	return 0;
}
