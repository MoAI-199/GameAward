#include "SceneMain.h"
#include "Scene.h"
#include "Player.h"
#include "Viewer.h"
#include "MapData.h"
#include "Collider.h"
#include "Log.h"
#include "Camera.h"
#include "Enemy.h"
#include "Music.h"
#include "Device.h"
#include "Controller.h"
#include "System.h"
#include "Menu.h"
#include "StageManager.h"
#include "Transition.h"
#include "SaveData.h"
#include "Drawer.h"

SCENE( SceneMain );

SceneMain::SceneMain( ) {
	StageManagerPtr stage_manager = Task::get< StageManager >( );
	SystemPtr system = Task::get< System >( );
	_controller = ControllerPtr( new Controller( ) );
	_transition = TransitionPtr( new Transition( ) );
	_menu = MenuPtr( new Menu( _controller ) );
	_map_data = MapDataPtr( new MapData( stage_manager->getStageNum( ), _controller, _menu ) );
	_collider = ColliderPtr( new Collider( _map_data, _menu ) );
	_camera = CameraPtr( new Camera( _map_data ) );
	_viewer = ViewerPtr( new Viewer( _map_data, _camera, _menu, _collider, _transition, _controller ) );
}

SceneMain::~SceneMain( ) {
}

void SceneMain::initialize( ) {
	DrawerPtr draw = Task::get<Drawer>( );
	SystemPtr system = Task::get< System >( );
	system->resetClear( );
	system->resetRetired( );

	draw->drawBox( 0, 0, 1920, 1080, Color( 0, 0, 0 ), true );
	startIris( );
	StageManagerPtr stage_manager = Task::get< StageManager >( );
	_first_time = true;
	_skip_ending = false;
	_fade_clear = false;
	MusicPtr music = Task::get< Music >( );
	music->playBGM( "dragon_march", "mp3", 40 );
	if ( _map_data->getStageName( stage_manager->getStageNum( ) ) == "ending_stage" ) {
		music->stopBGM( );
	}
}

void SceneMain::update( ) {
	LogPtr log = Task::get< Log >( );
	SaveDataPtr save_data = Task::get< SaveData >( );
	StageManagerPtr stage_manager = Task::get< StageManager >( );
	SystemPtr system = Task::get< System >( );
	MusicPtr music = Task::get< Music >( );
	if ( !system->isClear( ) ) {
		if ( !_transition->isTransition( ) || _map_data->getStageName( stage_manager->getStageNum( ) ) == "ending_stage" ) {
			_map_data->update( );
			_menu->update( );
		}
	} else {
		if ( _first_time ) {
			music->stopBGM( );
			save_data->setAll( stage_manager->getStageNum( ), _map_data->getDeadPlayerNum( ) );
			save_data->save( );
			_first_time = false;
		}
	}

	_collider->update( );
	_camera->update( );
	endingUpdate( );
	_viewer->draw( );
	_controller->update( );

	log->status( "camera_pos.x:%.lf", _camera->getPos( ).x );
	std::vector< PlayerPtr > players = _map_data->getPlayers( );
	log->status( "camera_pos.y:%.lf", _camera->getPos( ).y );
	for ( int i = 0; i < players.size( ); i++ ) {
		if ( players[ i ]->getState( ) == Player::STATE::ALIVE ) {
			log->status( "vec_X:%.lf", players[ i ]->getVec( ).x );
			log->status( "vec_Y:%.lf", players[ i ]->getVec( ).y );
		}
	}
	if ( system->isClear( ) ) {
		if ( _controller->getPushA( ) ) {
			if ( _viewer->getViewClearCount( ) >= 95 ) {
				if ( !_transition->isTransition( ) ) {
					music->decisionSE( );
					_transition->setFadeOut( 0.05, Transition::COLOR::BLACK );
					_fade_clear = true;
				}
			} else {
				_viewer->skipClear( );
			}
		}
		if ( !_transition->isScaling( ) && _fade_clear ) {
			if ( _map_data->getStageName( stage_manager->getStageNum( ) + 1 ) == "ending_stage" ) {
				stage_manager->setStageNum( stage_manager->getStageNum( ) + 1 );
				loadScene( "SceneMain" );
			} else {
				loadScene( "SceneSelectStage" );
			}
		}

		if ( _viewer->getViewClearCount( ) == CLEAR_BASE_COUNT[ 0 ] ) {
			//clear BGM or SE
			MusicPtr music = Task::get< Music >( );
			music->clearSE( );
		}
	}
	if ( _menu->isRetire( ) ) {
		save_data->setLastPlayStage( stage_manager->getStageNum( ) );
		music->stopBGM( );
		system->setRetired( );
		if ( !_transition->isScaling( ) ) {
			loadScene( "SceneSelectStage" );
		}
	}
}

void SceneMain::finalize( ) {
	MusicPtr music = Task::get< Music >( );
	music->stopBGM( );
	music->stopClearSE( );
}

void SceneMain::endingUpdate( ) {
	MusicPtr music = Task::get< Music >( );
	StageManagerPtr stage_manager = Task::get< StageManager >( );
	SystemPtr system = Task::get< System >( );
	SaveDataPtr save_data = Task::get< SaveData >( );

	std::vector< PlayerPtr > players = _map_data->getPlayers( );
	int wait_time = 20;
	if ( save_data->isComplete( ) ) {
		wait_time = 120;
	}
	if ( _map_data->getStageName( stage_manager->getStageNum( ) ) == "ending_stage" ) {
		std::vector< PlayerPtr > players = _map_data->getPlayers( );
		for ( int i = 0; i < players.size( ); i++ ) {
			if ( players[ i ]->getState( ) == Player::STATE::DEAD && _viewer->getEndingWaitCount( ) > wait_time ) {
				if ( !_transition->isTransition( ) ) {
					_transition->setFadeOut( DEFAULT_FADE_SPEED, Transition::COLOR::BLACK );
					system->setEndingMovieEnded( );
				}
			} else if ( !_transition->isTransition( ) && players[ i ]->getPos( ).x >= 1900 && _first_time ) {
				_transition->setFadeIn( 0.005, Transition::COLOR::WHITE );
				music->birdSE( );
				_first_time = false;
			}
		}

		if ( save_data->isSecondCleared( STAGE_NUM - 1 ) && !system->isEndingMovieEnded( ) &&
			 !save_data->isComplete( ) ) {
			if ( !_transition->isTransition( ) && _controller->getPushA( ) ) {
				_transition->setFadeOut( 0.05, Transition::COLOR::BLACK );
				music->decisionSE( );
				_skip_ending = true;
			}
			if ( _skip_ending && !_transition->isScaling( ) ) {
				loadScene( "SceneSelectStage" );
			}
		}

		if ( system->isEndingMovieEnded( ) && _controller->getPushA( ) ) {
			music->stopBirdSE( );
			music->decisionSE( );
			loadScene( "SceneTitle" );
			system->resetEndingMovieEnded( );
		}
	}
}

void SceneMain::startIris( ) {
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	int start_pos_x = 0;
	int start_pos_y = 0;

	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::RESPAWN ) {
			start_pos_x = gimmicks[ i ]->getPos( ).x + gimmicks[ i ]->getSize( ).x;
			start_pos_y = gimmicks[ i ]->getPos( ).y + gimmicks[ i ]->getSize( ).y;
		}
	}
	_transition->setIrisIn( start_pos_x, start_pos_y, _camera->getPos( ).x, _camera->getPos( ).y, 10, 0, 0 );

}



