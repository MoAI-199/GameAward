#include "Introduction.h"
#include "Drawer.h"
#include "Keyboard.h"
#include "Device.h"
#include "Log.h"
#include "Application.h"
#include "Sound.h"
#include "StageManager.h"
#include "SaveData.h"
#include "Exception.h"
#include "Score.h"
#include "System.h"

void Introduction::procedure( ) {
	Task::create< Drawer >( );
	Task::create< Keyboard >( );
	Task::create< Device >( );
	Task::create< Log >( );
	Task::create< Sound >( );
	Task::create< StageManager >( );
	Task::create< SaveData >( );
	Task::create< Score >( );
	Task::create< System >( );
	ApplicationPtr app = Application::getInstance( );
	app->setFPS( 60 );

	LogPtr log = Task::get< Log >( );
#if _DEBUG
	log->toggleShow( );
	struct stat buf;
	if ( stat( "caution.txt", &buf ) == 0 ) {
		EXCEPTION( true, "ERROR[$(ProjectDir)] change to $(SolutionDir)" );
	}
#endif
	REGIST( ScenePrologue );
	REGIST( SceneTitle );
	REGIST( SceneSelectStage );
	REGIST( SceneMain );

	start( "SceneTitle" );
}
