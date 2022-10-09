#include "Introduction.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "Device.h"
#include "Mouse.h"
#include "Log.h"
#include "Application.h"
#include "Exception.h"

void Introduction::procedure( ) {
	Task::create< Drawer >( );
	Task::create< Keyboard >( );
	Task::create< Device >( );
	Task::create< Mouse >( );
	Task::create< Log >( );
	ApplicationPtr app = Application::getInstance( );
	app->setFPS( 30 );

	LogPtr log = Task::get< Log >( );
	log->toggleShow( );
#if _DEBUG
	struct stat buf;
	if ( stat( "caution.txt", &buf ) == 0 ) {
		EXCEPTION( true, "ERROR[$(ProjectDir)] change to $(SolutionDir)" );
	}
#endif
	REGIST( SceneEditor );
	start( "SceneEditor" );
}
