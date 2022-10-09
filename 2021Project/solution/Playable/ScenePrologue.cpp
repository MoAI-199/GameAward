#include "ScenePrologue.h"
#include "Drawer.h"
#include "Define.h"
#include "System.h"
#include "Controller.h"
#include "MapData.h"
#include "Menu.h"
#include "StageManager.h"
#include "Transition.h"
#include "Collider.h"
#include "Camera.h"
#include "ViewerPrologue.h"
#include "Music.h"
#include "mathematics.h"

SCENE( ScenePrologue );

static const int SCROLL_SPEED = 2;

ScenePrologue::ScenePrologue( ) {
	ApplicationPtr app = Application::getInstance( );
	_controller = ControllerPtr( new Controller( ) );
	_transition = TransitionPtr( new Transition( ) );
	_player = PlayerPtr( new Player( Vector2( 0, app->getScreenHeight( ) / 2 ), _controller ) );
	_viewer_prologue = ViewerProloguePtr( new ViewerPrologue( _player ) );
}

ScenePrologue::~ScenePrologue( ) {
}

void ScenePrologue::initialize( ) {
}

void ScenePrologue::update( ) {
	playerMove( );
	_viewer_prologue->draw( );
}

void ScenePrologue::playerMove( ) {
	ApplicationPtr app = Application::getInstance( );
	if ( _player->getPos( ).x <= app->getScreenWidth( ) / 2 ) {
		_player->setPos( ).x += 3;
	}
}




