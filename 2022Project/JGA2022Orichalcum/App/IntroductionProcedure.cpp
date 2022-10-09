#include "Introduction.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "Device.h"
#include "Log.h"
#include "SceneManager.h"
#include "SceneResult.h"


void Introduction::procedure( ) {
	Task::create< Drawer >( );
	Task::create< Keyboard >( );
	Task::create< Device >( );
	Task::create< Log >( );
	Task::create< SceneManager >( );
	LogPtr log = Task::get< Log >();
	log->toggleShow( );
	
	REGIST( SceneTitle );
	REGIST( SceneMain );
	REGIST( SceneResult );
	start( "SceneTitle" );
}

