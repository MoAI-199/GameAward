#pragma once
#include "Task.h"
#include "smart_ptr.h"

PTR( SceneManager );

class SceneManager : public Task {
public:
	SceneManager( );
	~SceneManager( );
public:
	enum class NEXT_SCENE {
		NONE,
		TITLE,
		MAIN,
		RESULT,
	};
public:
	void setNextScene( NEXT_SCENE next );
	NEXT_SCENE getNextScene( ) const;
private:
	NEXT_SCENE _next_scene;
};

