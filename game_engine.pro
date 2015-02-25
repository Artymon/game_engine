TEMPLATE = app
CONFIG += console

CONFIG -= qt
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += "/usr/include/bullet"
INCLUDEPATH += "/usr/include/OGRE"
#INCLUDEPATH += "/usr/include/boost"
LIBS += -lboost_system
LIBS += -lBulletDynamics -lBulletCollision -lLinearMath
LIBS += -lOgreMain -lOgrePaging -lGL -lglut -lGLU
#LIBS += -lBulletMultiThreaded -lBulletWorldImporter -lBulletXmlWorldImporter -ltalloc

SOURCES += main.cpp
