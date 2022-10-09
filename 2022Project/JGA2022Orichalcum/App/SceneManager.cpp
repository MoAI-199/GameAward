#include "SceneManager.h"


SceneManager::SceneManager( ) {
	_next_scene = NEXT_SCENE::NONE;
}

SceneManager::~SceneManager( ) {
}

void SceneManager::setNextScene( NEXT_SCENE next ) {
	_next_scene = next;
}

SceneManager::NEXT_SCENE SceneManager::getNextScene( ) const {
	return _next_scene;
}
