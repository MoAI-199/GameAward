#include "SceneTitle.h"
#include "Drawer.h"
#include "Application.h"
#include "Device.h"
#include "SceneManager.h"

SCENE( SceneTitle );

SceneTitle::SceneTitle( ) :
	SCREEN_HEIGHT( Application::getInstance( )->getScreenHeight( ) ),
	SCREEN_WIDTH( Application::getInstance( )->getScreenWidth( ) ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_taitoru = drawer->createImage( "resource/taitoru.png" );
}

SceneTitle::~SceneTitle( ) {

}

void SceneTitle::update( ) {
	drawTaitoru( );
	changeScene( );
}

void SceneTitle::changeScene( ) {
	DevicePtr device = Task::get< Device >();
	SceneManagerPtr manager = Task::get< SceneManager >( );
	if ( device->getPush( ) == BUTTON_B ) {
		loadScene( "SceneMain" );
	}
}

void SceneTitle::drawTaitoru( ) {
	_taitoru->drawExtend(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}
