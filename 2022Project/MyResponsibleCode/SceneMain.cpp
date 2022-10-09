#include "SceneMain.h"
#include "SheepsManager.h"
#include "SheepFactory.h"
#include "SheepsController.h"
#include "SheepEvent.h"
#include "CloudsManager.h"
#include "CloudsController.h"
#include "CloudEvent.h"
#include "CloudFactory.h"
#include "CloudsController.h"
#include "GimmickManager.h"
#include "GimmickController.h"
#include "GimmickFactory.h"
#include "GimmickEvent.h"
#include "DeviceController.h"
#include "PropellerManager.h"
#include "Collision.h"
#include "Viewer.h"
#include "SceneManager.h"
#include "FundamentalGameSystem.h"
#include "define.h"
#include "Tool.h"

SCENE( SceneMain );

SceneMain::SceneMain( ) {
	_device_controller = DeviceControllerPtr( new DeviceController );

	_sheeps_manager = SheepsManagerPtr( new SheepsManager );
	_sheep_factory = SheepFactoryPtr( new SheepFactory( _sheeps_manager ) );
	_sheeps_controller = SheepsControllerPtr( new SheepsController( _sheeps_manager ) );
	_clouds_manager = CloudsManagerPtr( new CloudsManager );
	_cloud_factory = CloudFactoryPtr( new CloudFactory( _clouds_manager ) );
	_clouds_controller = CloudsControllerPtr( new CloudsController( _clouds_manager ) );
	_gimmicks_manager = GimmickManagerPtr( new GimmickManager );
	_game_system = FundamentalGameSystemPtr( new FundamentalGameSystem( _sheeps_manager,
		_clouds_manager,
		_sheep_factory ) );
	_propeller_manager = PropellerManagerPtr( new PropellerManager( _device_controller ) );
	_collision = CollisionPtr( new Collision( _sheeps_manager,
		_clouds_manager,
		_gimmicks_manager,
		_game_system,
		_propeller_manager ) );
	_gimmick_controller = GimmickControllerPtr( new GimmickController( _gimmicks_manager ) );
	_gimmick_factory = GimmickFactoryPtr( new GimmickFactory( _gimmicks_manager ) );
	_gimmick_event = GimmickEventPtr( new GimmickEvent( _collision,
		_sheeps_manager,
		_gimmicks_manager,
		_sheep_factory,
		_gimmick_factory,
		_clouds_manager,
		_game_system ) );
	_sheep_event = SheepEventPtr( new SheepEvent( _collision,
		_sheeps_manager,
		_clouds_manager,
		_sheep_factory,
		_game_system,
		_propeller_manager ) );
	_cloud_event = CloudEventPtr( new CloudEvent( _cloud_factory,
		_clouds_manager,
		_device_controller,
		_game_system ) );
	_viewer = ViewerPtr( new Viewer( _sheeps_controller,
		_clouds_controller,
		_gimmick_controller,
		_propeller_manager,
		_game_system ) );


#ifdef debug
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
	_gimmick_factory->deploy( Vector2( SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 ), Gimmick::TYPE::CARROT );
#endif 
}

SceneMain::~SceneMain( ) {

}

void SceneMain::update( ) {
	_viewer->draw( );
	_collision->update( );
	_gimmick_event->update( );
	_sheep_event->update( );
	_cloud_event->update( );
	_sheeps_manager->update( );
	_clouds_manager->update( );
	_propeller_manager->update( );
	_gimmicks_manager->update( );
	_game_system->update( );


	#ifdef debug


	//debug
	//time_count++;
	if ( time_count == 1000 ) {
		//_gimmick_factory->deploy( Vector2( 1520 / 2, 780 / 2 ), Gimmick::TYPE::COIN );
		time_count = 0;
	}
	#endif // debug
	setNextScene( );
	changeScene( );

}

void SceneMain::changeScene( ) {
	SceneManagerPtr manager = Task::get< SceneManager >( );
	SceneManager::NEXT_SCENE scene = manager->getNextScene( );
	switch ( scene ) {
	case SceneManager::NEXT_SCENE::NONE:
	case SceneManager::NEXT_SCENE::MAIN:
		break;
	case SceneManager::NEXT_SCENE::TITLE:
		loadScene( "SceneTitle" );
		break;
	case SceneManager::NEXT_SCENE::RESULT:
		loadScene( "SceneResult" );
		break;
	}
}

//set‚¶‚á‚È‚­‚ËH
void SceneMain::setNextScene( ) {
	int sheep_num = _sheeps_controller->getSheepNum( );
	if ( sheep_num == 0 ) {
		SceneManagerPtr manager = Task::get< SceneManager >( );
		manager->setNextScene( SceneManager::NEXT_SCENE::RESULT );
	}
}
