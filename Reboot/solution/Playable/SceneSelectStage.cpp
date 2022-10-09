#include "SceneSelectStage.h"
#include "SaveData.h"
#include "Controller.h"
#include "Drawer.h"
#include "StageManager.h"
#include "Define.h"
#include "Image.h"
#include "Score.h"
#include "Transition.h"
#include "Music.h"
#include "System.h"

SCENE( SceneSelectStage );

const int STAGE_UNLOOK_MOVE_TIME = 150;
const int STRATA_UNLOOK_MOVE_TIME = 220;

SceneSelectStage::SceneSelectStage( ) {
	_player_pos_x = 0;
	_player_pos_y = 0;
	_now_select_stage_num = 0;
	_strata_anime_count = 100;
	_before_select_stage_icon_anime_count = 0;
	_now_select_stage_icon_anime_count = 6;
	_move = 133;


	_wait_time_close = 0;
	wait_time_open = 0;
	_stage_ui_pos[ 0 ] = Vector2( 450, 500 );
	_stage_ui_pos[ 1 ] = Vector2( 850, 800 );
	_stage_ui_pos[ 2 ] = Vector2( 1250, 400 );
	_stage_ui_pos[ 3 ] = Vector2( 1600, 700 );
	_stage_ui_pos[ 4 ] = Vector2( 500, 400 );
	_stage_ui_pos[ 5 ] = Vector2( 1000, 400 );
	_stage_ui_pos[ 6 ] = Vector2( 1300, 800 );
	_stage_ui_pos[ 7 ] = Vector2( 1650, 800 );
	_stage_ui_pos[ 8 ] = Vector2( 500, 600 );
	_stage_ui_pos[ 9 ] = Vector2( 900, 300 );
	_stage_ui_pos[ 10 ] = Vector2( 900, 800 );
	_stage_ui_pos[ 11 ] = Vector2( 1400, 600 );
	_stage_ui_pos[ 12 ] = Vector2( 600, 500 );
	_stage_ui_pos[ 13 ] = Vector2( 900, 800 );
	_stage_ui_pos[ 14 ] = Vector2( 1400, 800 );
	_stage_ui_pos[ 15 ] = Vector2( 1650, 400 );

	if ( _stage_ui_pos.size( ) > 16 ) {
		for ( int i = 16; i < _stage_ui_pos.size( ); i++ ) {
			_stage_ui_pos[ i ] = Vector2( 500 + ( __int64 )400 * ( i % NUMBER_OF_STAGES_PER_STRATA ), 600 );
		}
	}

	ApplicationPtr app = Application::getInstance( );
	for ( int i = 0; i < _stage_ui_pos.size( ); i++ ) {
		_stage_ui_pos[ i ].y += ( __int64 )( ( STAGE_NUM - 1 - i ) / NUMBER_OF_STAGES_PER_STRATA ) * app->getScreenHeight( );
	}

	DrawerPtr drawer = Task::get< Drawer >( );
	_number = drawer->createImage( "Resource/Image/image_number.png" );
	_background = drawer->createImage( "Resource/Image/background/select_BG.png" );

	_player_img = drawer->createImage( "Resource/Image/player/idle_right/idle_000.png" );
	_star = drawer->createImage( "Resource/Image/UI/silver_star.png" );
	_doughnut_star = drawer->createImage( "Resource/Image/UI/doughnut_star.png" );
	_strata_text = drawer->createImage( "Resource/Image/UI/strata.png" );
	_strata_window = drawer->createImage( "Resource/Image/UI/strata_window_db.png" );
	_dark_strata_window = drawer->createImage( "Resource/Image/UI/dark_strata_window.png" );
	_icon_base = drawer->createImage( "Resource/Image/UI/icon_base_short_lb.png" );
	_back = drawer->createImage( "Resource/Image/UI/back.png" );
	_back_frame = drawer->createImage( "Resource/Image/UI/back_frame.png" );
	_b_botton = drawer->createImage( "Resource/Image/UI/b_botton.png" );
	_icon_door[ 0 ] = drawer->createImage( "Resource/image/Respawn/fill/lab_door_01.png" );
	_icon_door[ 1 ] = drawer->createImage( "Resource/image/Respawn/fill/lab_door_02.png" );
	_icon_door[ 2 ] = drawer->createImage( "Resource/image/Respawn/fill/lab_door_03.png" );
	_icon_door[ 3 ] = drawer->createImage( "Resource/image/Respawn/fill/lab_door_04.png" );
	_icon_door[ 4 ] = drawer->createImage( "Resource/image/Respawn/fill/lab_door_05.png" );
	_icon_door[ 5 ] = drawer->createImage( "Resource/image/Respawn/fill/lab_door_06.png" );
	_icon_door[ 6 ] = drawer->createImage( "Resource/image/Respawn/fill/lab_door_07.png" );
	_icon_door[ 7 ] = drawer->createImage( "Resource/image/Respawn/fill/lab_door_08.png" );
	_spark[ 0 ] = drawer->createImage( "Resource/Image/UI/rectangle/rectangle_spark_effect_01.png" );
	_spark[ 1 ] = drawer->createImage( "Resource/Image/UI/rectangle/rectangle_spark_effect_02.png" );
	_spark[ 2 ] = drawer->createImage( "Resource/Image/UI/rectangle/rectangle_spark_effect_03.png" );
	_spark[ 3 ] = drawer->createImage( "Resource/Image/UI/rectangle/rectangle_spark_effect_04.png" );
	_spark[ 4 ] = drawer->createImage( "Resource/Image/UI/rectangle/rectangle_spark_effect_05.png" );
	_spark[ 5 ] = drawer->createImage( "Resource/Image/UI/rectangle/rectangle_spark_effect_06.png" );
	_window_anime[ 0 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime00.png" );
	_window_anime[ 1 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime01.png" );
	_window_anime[ 2 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime02.png" );
	_window_anime[ 3 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime03.png" );
	_window_anime[ 4 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime04.png" );
	_window_anime[ 5 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime05.png" );
	_window_anime[ 6 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime06.png" );
	_window_anime[ 7 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime07.png" );
	_window_anime[ 8 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime08.png" );
	_window_anime[ 9 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime09.png" );
	_window_anime[ 10 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime10.png" );
	_window_anime[ 11 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime11.png" );
	_window_anime[ 12 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime12.png" );
	_window_anime[ 13 ] = drawer->createImage( "Resource/Image/UI/window_anime/strata_window_db_anime13.png" );
	_energy_field[ 0 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00000.png" );
	_energy_field[ 1 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00001.png" );
	_energy_field[ 2 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00002.png" );
	_energy_field[ 3 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00003.png" );
	_energy_field[ 4 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00004.png" );
	_energy_field[ 5 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00005.png" );
	_energy_field[ 6 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00006.png" );
	_energy_field[ 7 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00007.png" );
	_energy_field[ 8 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00008.png" );
	_energy_field[ 9 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00009.png" );
	_energy_field[ 10 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00010.png" );
	_energy_field[ 11 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00011.png" );
	_energy_field[ 12 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00012.png" );
	_energy_field[ 13 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00013.png" );
	_energy_field[ 14 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00014.png" );
	_energy_field[ 15 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00015.png" );
	_energy_field[ 16 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00016.png" );
	_energy_field[ 17 ] = drawer->createImage( "Resource/Image/UI/energy field/energy field_00017.png" );
	_transition = TransitionPtr( new Transition( ) );
	_camera_y = 0;
	_wait_select_count = 0;
	_selected = false;
	_backed = false;
	_draw_effect = false;
	_wait_criteria_time = 30;
	_unlook_image_num = 0;
	_unlook_image_wait_time = 0;
	_window_anime_image_num = 0;
	setupWalk( );
}

SceneSelectStage::~SceneSelectStage( ) {

}

void SceneSelectStage::initialize( ) {
	SaveDataPtr save_data = Task::get< SaveData >( );
	SystemPtr system = Task::get< System >( );
	ApplicationPtr app = Application::getInstance( );
	_now_select_stage_num = save_data->getLastPlayStage( );
	_wait_criteria_time = 30;
	if ( !save_data->isSecondCleared( save_data->getLastClearStage( ) ) &&
		 !system->isRetired( ) && !system->isLoaded( ) &&
		 save_data->getDegreeOfProgress( ) != -1 && save_data->getDegreeOfProgress( ) != STAGE_NUM - 1 ) {
		//first_clear
		_wait_criteria_time = STAGE_UNLOOK_MOVE_TIME;
		if ( save_data->getLastClearStage( ) % NUMBER_OF_STAGES_PER_STRATA == NUMBER_OF_STAGES_PER_STRATA - 1 ) {
			_wait_criteria_time = STRATA_UNLOOK_MOVE_TIME;
		}
	}
	if ( save_data->getDegreeOfProgress( ) == -1 ) {
		_now_select_stage_num = 0;
		_move = -20;
	}

	if ( system->isLoaded( ) ) {
		_move = -20;
	}

	_before_select_strata = ( STAGE_NUM - 1 - _now_select_stage_num ) / NUMBER_OF_STAGES_PER_STRATA;
	_before_select_stage = _now_select_stage_num;
	_camera_y = ( ( STAGE_NUM - 1 - _now_select_stage_num ) / NUMBER_OF_STAGES_PER_STRATA ) * app->getScreenHeight( );
	_controller = ControllerPtr( new Controller( ) );
	_wait_select_count = 0;
	_selected = false;
	_backed = false;

	_player_pos_x = ( _stage_ui_pos[ _now_select_stage_num ].x - PLAYER_SIZE_X / 2 - 15 );
	_player_pos_y = ( _stage_ui_pos[ _now_select_stage_num ].y - PLAYER_SIZE_Y - _camera_y );

	system->resetRetired( );
	system->resetLoaded( );

	_transition->setIrisIn( _stage_ui_pos[ _now_select_stage_num ].x,
							_stage_ui_pos[ _now_select_stage_num ].y,
							0,
							_camera_y,
							5,
							0,
							0 );

	MusicPtr music = Task::get< Music >( );
	music->playBGM( "demon_soul_loop_cyber_40", "mp3", 50 );
}

void SceneSelectStage::update( ) {
	MusicPtr music = Task::get< Music >( );
	_wait_select_count++;
	if ( _wait_select_count > _wait_criteria_time ) {
		if ( !_transition->isTransition( ) ) {
			select( );
		}
		_wait_select_count = _wait_criteria_time;
		backTitle( );
		inToTheSelectStage( );
	} else {
		unlookEffectMove( );
	}


	moveCamera( );
	if ( _wait_select_count >= 2 ) {
		draw( );
	}
#if _DEBUG
	DrawerPtr drawer = Task::get< Drawer >( );
	drawer->drawString( 0, 0, "now_select_stage_num:%d", _now_select_stage_num );
#endif
	_controller->update( );
	_transition->update( );
	moveAnimePlayer( );
}

void SceneSelectStage::finalize( ) {
	MusicPtr music = Task::get< Music >( );
	music->stopBGM( );
}

void SceneSelectStage::select( ) {
	ApplicationPtr app = Application::getInstance( );
	SaveDataPtr save_data = Task::get< SaveData >( );
	MusicPtr music = Task::get< Music >( );
	if ( !_controller->isTilted( ) ) {
		if ( _controller->getStickLeft( ) ) {
			if ( _now_select_stage_num % NUMBER_OF_STAGES_PER_STRATA != 0 ) {
				music->cursorSE( );
				_now_select_stage_num--;
			}
		}
		if ( _controller->getStickRight( ) ) {
			if ( save_data->getDegreeOfProgress( ) + 1 > _now_select_stage_num ) {
				if ( _now_select_stage_num % NUMBER_OF_STAGES_PER_STRATA != NUMBER_OF_STAGES_PER_STRATA - 1 ) {
					music->cursorSE( );
					_now_select_stage_num++;
				}
			}
		}
		if ( _controller->getStickDown( ) ) {
			if ( _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA > 0 ) {
				music->cursorSE( );
				_now_select_stage_num = ( _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA - 1 ) * NUMBER_OF_STAGES_PER_STRATA;
			}
		}
		if ( _controller->getStickUp( ) ) {
			if ( ( save_data->getDegreeOfProgress( ) + 1 ) / NUMBER_OF_STAGES_PER_STRATA > _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA ) {
				if ( _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA < STAGE_NUM / NUMBER_OF_STAGES_PER_STRATA - 1 ) {
					music->cursorSE( );
					_now_select_stage_num = ( _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA + 1 ) * NUMBER_OF_STAGES_PER_STRATA;
				}
			}
		}
	}
}

void SceneSelectStage::inToTheSelectStage( ) {
	MusicPtr music = Task::get< Music >( );
	ApplicationPtr app = Application::getInstance( );
	if ( _controller->getPushA( ) && _camera_y % app->getScreenHeight( ) == 0 ) {
		StageManagerPtr manager = Task::get< StageManager >( );
		manager->setStageNum( _now_select_stage_num );
		if ( !_transition->isTransition( ) ) {
			music->decisionSE( );
			_transition->setIrisOut( _stage_ui_pos[ _now_select_stage_num ].x,
									 _stage_ui_pos[ _now_select_stage_num ].y - 60,
									 0,
									 _camera_y,
									 DEFAULT_IRIS_SPEED,
									 80,
									 40 );
			_player_pos_x = ( _stage_ui_pos[ _now_select_stage_num ].x - PLAYER_SIZE_X / 2 - 15 );
			_player_pos_y = ( _stage_ui_pos[ _now_select_stage_num ].y - PLAYER_SIZE_Y - _camera_y );
			music->stopBGM( );
			_selected = true;
		}
	}
	if ( !_transition->isScaling( ) && _selected ) {
		loadScene( "SceneMain" );
	}
}

void SceneSelectStage::backTitle( ) {
	MusicPtr music = Task::get< Music >( );
	ApplicationPtr app = Application::getInstance( );
	if ( _controller->getPushB( ) && !_transition->isTransition( ) ) {
		music->cancelSE( );
		_transition->setFadeOut( 0.05, Transition::COLOR::BLACK );
		_backed = true;
	}

	if ( !_transition->isScaling( ) && _backed ) {
		loadScene( "SceneTitle" );
	}
}

void SceneSelectStage::draw( ) {
	drawBackGround( );
	drawStageUI( );
	drawStrataUI( );
	drawScoreUI( );
	drawBack( );
	unlookStageEffect( );
	walkPlayerIn( );
	walkPlayerOut( );
}

void SceneSelectStage::drawBackGround( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	ApplicationPtr app = Application::getInstance( );
	int now_strata_hight = _camera_y / app->getScreenHeight( );
	int adjust_brightness = 5;
	double ratio = ( double )( ( __int64 )( STAGE_NUM / NUMBER_OF_STAGES_PER_STRATA ) - now_strata_hight / 2 + adjust_brightness - now_strata_hight % 2 * 0.5 ) / 10;
	drawer->setBlend( scope, Drawer::BLEND::ADD, ratio );
	_background->draw( 0, 0 - _camera_y + app->getScreenHeight( ) * ( STAGE_NUM / NUMBER_OF_STAGES_PER_STRATA - 5 ) );

}


void SceneSelectStage::drawStageUI( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	const Vector2 DOOR_SIZE = Vector2( 207, 200 );
	const Vector2 BASE_SIZE = Vector2( 256, 128 );
	int door_adjust_y = 3;
	SaveDataPtr save_data = Task::get< SaveData >( );
	/*for ( int i = 1; i < NUMBER_OF_STAGES_PER_STRATA; i++ ) {
		drawer->drawLine( _stage_ui_pos[ ( __int64 )( _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA ) * NUMBER_OF_STAGES_PER_STRATA + i - 1 ].x,
						  _stage_ui_pos[ ( __int64 )( _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA ) * NUMBER_OF_STAGES_PER_STRATA + i - 1 ].y - _camera_y,
						  _stage_ui_pos[ ( __int64 )( _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA ) * NUMBER_OF_STAGES_PER_STRATA + i ].x,
						  _stage_ui_pos[ ( __int64 )( _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA ) * NUMBER_OF_STAGES_PER_STRATA + i ].y - _camera_y,
						  Color( 1, 1, 1 ) );
	}*/

	for ( int i = 0; i < STAGE_NUM; i++ ) {
		//int stage_num = ( __int64 )( _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA ) * NUMBER_OF_STAGES_PER_STRATA + i;
		int stage_num = i;
		_icon_base->draw( _stage_ui_pos[ stage_num ].x - BASE_SIZE.x / 2,
						  _stage_ui_pos[ stage_num ].y - 35 - _camera_y );

		if ( stage_num == _now_select_stage_num ) {
			if ( _before_select_stage != stage_num ) {
				if ( _now_select_stage_icon_anime_count > 0 ) {
					_wait_time_close++;
					_wait_time_close %= SELECT_ICON_DOOR_WAIT_TIME;
					if ( _wait_time_close == 0 ) {
						_now_select_stage_icon_anime_count--;
					}
				} else {
					_now_select_stage_icon_anime_count = 7;
				}
				_icon_door[ _now_select_stage_icon_anime_count ]->draw( _stage_ui_pos[ stage_num ].x - DOOR_SIZE.x / 2,
																		_stage_ui_pos[ stage_num ].y - DOOR_SIZE.y - door_adjust_y - _camera_y );
			} else {
				_icon_door[ 0 ]->draw( _stage_ui_pos[ stage_num ].x - DOOR_SIZE.x / 2,
									   _stage_ui_pos[ stage_num ].y - DOOR_SIZE.y - door_adjust_y - _camera_y );
			}
		} else if ( stage_num <= save_data->getDegreeOfProgress( ) + 1 ) {
			if ( _before_select_stage == stage_num ) {
				if ( _before_select_stage_icon_anime_count < 6 ) {
					wait_time_open++;
					wait_time_open %= SELECT_ICON_DOOR_WAIT_TIME;
					if ( wait_time_open == 0 ) {
						_before_select_stage_icon_anime_count++;
					}
				} else {
					_before_select_stage_icon_anime_count = 0;
				}
				_icon_door[ _before_select_stage_icon_anime_count ]->draw( _stage_ui_pos[ stage_num ].x - DOOR_SIZE.x / 2,
																		   _stage_ui_pos[ stage_num ].y - DOOR_SIZE.y - door_adjust_y - _camera_y );
			} else {
				int image_num = 6;
				if ( _wait_criteria_time >= STAGE_UNLOOK_MOVE_TIME && !_draw_effect &&
					 stage_num == save_data->getDegreeOfProgress( ) + 1 ) {
					image_num = 7;
				}
				_icon_door[ image_num ]->draw( _stage_ui_pos[ stage_num ].x - DOOR_SIZE.x / 2,
											   _stage_ui_pos[ stage_num ].y - DOOR_SIZE.y - door_adjust_y - _camera_y );
			}
		} else {
			_icon_door[ 7 ]->draw( _stage_ui_pos[ stage_num ].x - DOOR_SIZE.x / 2,
								   _stage_ui_pos[ stage_num ].y - DOOR_SIZE.y - door_adjust_y - _camera_y );
		}
	}
	if ( _now_select_stage_icon_anime_count == 0 || _before_select_stage_icon_anime_count == 6 ) {
		_before_select_stage = _now_select_stage_num;
	}
	if ( !_transition->isStop( ) && _move <= -20 ) {
		_player_img->draw( _stage_ui_pos[ _now_select_stage_num ].x - PLAYER_SIZE_X / 2 - 15,
						   _stage_ui_pos[ _now_select_stage_num ].y - PLAYER_SIZE_Y - _camera_y );
	}
}

void SceneSelectStage::drawStrataUI( ) {
	ApplicationPtr app = Application::getInstance( );
	DrawerPtr drawer = Task::get< Drawer >( );
	SaveDataPtr save_data = Task::get< SaveData >( );
	Vector2 pos = Vector2( 0, ( app->getScreenHeight( ) - ( ( STAGE_NUM / NUMBER_OF_STAGES_PER_STRATA - 1 ) * 100 ) - 200 ) / 2 );
	int speed = 7;
	for ( int i = STAGE_NUM / NUMBER_OF_STAGES_PER_STRATA - 1; i >= 0; i-- ) {
		Vector2 size = Vector2( 170, 100 );
		if ( _before_select_strata != _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA ) {
			_before_select_strata = i;
			_strata_anime_count = 0;
		}
		if ( i == _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA ) {
			size += Vector2( ( __int64 )speed * 2 * _strata_anime_count, ( __int64 )speed * _strata_anime_count );
			if ( size.x > 350 || size.y > 200 ) {
				size = Vector2( 350, 200 );
			} else {
				_strata_anime_count++;
			}
		}

		if ( ( save_data->getLastClearStage( ) / NUMBER_OF_STAGES_PER_STRATA == i - 1 &&
			 _wait_criteria_time == STRATA_UNLOOK_MOVE_TIME ) ) {
			if ( !_draw_effect ) {
				_dark_strata_window->drawExtend( pos.x, pos.y, pos.x + size.x, pos.y + size.y );
			} else if ( _window_anime_image_num < _window_anime.size( ) ) {
				_window_anime[ _window_anime_image_num ]->drawExtend( pos.x, pos.y, pos.x + size.x, pos.y + size.y );
				_window_anime_image_num++;
			} else {
				_strata_window->drawExtend( pos.x, pos.y, pos.x + size.x, pos.y + size.y );
			}
		} else if ( i > ( save_data->getDegreeOfProgress( ) + 1 ) / NUMBER_OF_STAGES_PER_STRATA ) {
			_dark_strata_window->drawExtend( pos.x, pos.y, pos.x + size.x, pos.y + size.y );
		} else {
			_strata_window->drawExtend( pos.x, pos.y, pos.x + size.x, pos.y + size.y );
		}

		if ( i == STAGE_NUM / NUMBER_OF_STAGES_PER_STRATA - 1 ) {
			drawExtendNumber( pos.x, pos.y, size.y, 1 );
			_strata_text->drawRectExtend( pos.x + size.y * 3 / 4, pos.y, pos.x + size.y + size.y * 3 / 4, pos.y + size.y, 128, 0, 128, 128 );
		} else {
			_strata_text->drawRectExtend( pos.x, pos.y, pos.x + size.y, pos.y + size.y, 0, 0, 128, 128 );
			drawExtendNumber( pos.x + size.y * 3 / 4, pos.y, size.y, STAGE_NUM / NUMBER_OF_STAGES_PER_STRATA - 1 - i );
		}
		pos.y += size.y;
	}
}

void SceneSelectStage::drawScoreUI( ) const {
	ApplicationPtr app = Application::getInstance( );
	DrawerPtr drawer = Task::get< Drawer >( );
	SaveDataPtr save_data = Task::get< SaveData >( );
	int radius = 64;
	drawNumber( ( __int64 )app->getScreenWidth( ) - 700, 0, _now_select_stage_num / NUMBER_OF_STAGES_PER_STRATA + 1 );
	_number->drawRect( ( __int64 )app->getScreenWidth( ) - 580, -20, NUMBER_IMAGE_SIZE * 3, NUMBER_IMAGE_SIZE * 3, NUMBER_IMAGE_SIZE, NUMBER_IMAGE_SIZE );
	drawNumber( ( __int64 )app->getScreenWidth( ) - 500, 0, _now_select_stage_num % NUMBER_OF_STAGES_PER_STRATA + 1 );
	ScorePtr score = Task::get< Score >( );
	int score_num = score->getBestScore( _now_select_stage_num );
	for ( int i = 0; i < 3; i++ ) {
		int pos_x = app->getScreenWidth( ) - NUMBER_IMAGE_SIZE * ( 3 - i );
		if ( i < score_num ) {
			_star->draw( pos_x, 0 );
		} else {
			_doughnut_star->draw( pos_x, 0 );
		}
	}
#if _DEBUG
	drawNumber( ( __int64 )app->getScreenWidth( ) - 300, 0, save_data->getDeadPlayerNum( _now_select_stage_num ) );
#endif
}

void SceneSelectStage::drawBack( ) const {
	ApplicationPtr app = Application::getInstance( );
	int back_extend = 140;
	int botton_extend = 50;
	_back_frame->drawExtend( -100, app->getScreenHeight( ) - 130,
							 200, app->getScreenHeight( ) );
	_back->drawExtend( 20, app->getScreenHeight( ) - 130,
					   20 + back_extend, app->getScreenHeight( ) - 130 + back_extend );
	_b_botton->drawExtend( 120, app->getScreenHeight( ) - 110,
						   120 + botton_extend, app->getScreenHeight( ) - 110 + botton_extend );
}

void SceneSelectStage::drawNumber( double pos_x, double pos_y, int number ) const {
	const int SIDE = 4;

	std::vector< int > number_per_digit;

	do {
		number_per_digit.push_back( number % 10 );
		number /= 10;
	} while ( number > 0 );

	Vector2 draw_pos = Vector2( pos_x, pos_y );
	for ( int i = ( int )number_per_digit.size( ) - 1; i >= 0; i-- ) {
		int col = number_per_digit[ i ] / SIDE;
		int row = number_per_digit[ i ] % SIDE;
		_number->drawRect( draw_pos, row * NUMBER_IMAGE_SIZE, col * NUMBER_IMAGE_SIZE, NUMBER_IMAGE_SIZE, NUMBER_IMAGE_SIZE );
		draw_pos += Vector2( NUMBER_IMAGE_SIZE, 0 );
	}
}

void SceneSelectStage::drawExtendNumber( double pos1_x, double pos1_y, double size, int number ) const {
	const int SIDE = 4;

	std::vector< int > number_per_digit;

	do {
		number_per_digit.push_back( number % 10 );
		number /= 10;
	} while ( number > 0 );

	Vector2 draw_pos1 = Vector2( pos1_x, pos1_y );
	Vector2 draw_pos2 = Vector2( pos1_x + size, pos1_y + size );
	for ( int i = ( int )number_per_digit.size( ) - 1; i >= 0; i-- ) {
		int col = number_per_digit[ i ] / SIDE;
		int row = number_per_digit[ i ] % SIDE;
		_number->drawRectExtend( draw_pos1, draw_pos2, row * NUMBER_IMAGE_SIZE, col * NUMBER_IMAGE_SIZE, NUMBER_IMAGE_SIZE, NUMBER_IMAGE_SIZE );
		draw_pos1 += Vector2( size, 0 );
	}
}

void SceneSelectStage::moveCamera( ) {
	ApplicationPtr app = Application::getInstance( );
	const int SPEED = 70;
	if ( _camera_y != ( ( STAGE_NUM - 1 - _now_select_stage_num ) / NUMBER_OF_STAGES_PER_STRATA ) * app->getScreenHeight( ) ) {
		if ( _camera_y > ( ( STAGE_NUM - 1 - _now_select_stage_num ) / NUMBER_OF_STAGES_PER_STRATA ) * app->getScreenHeight( ) ) {
			_camera_y -= SPEED;
			if ( _camera_y < ( ( STAGE_NUM - 1 - _now_select_stage_num ) / NUMBER_OF_STAGES_PER_STRATA ) * app->getScreenHeight( ) ) {
				_camera_y = ( ( STAGE_NUM - 1 - _now_select_stage_num ) / NUMBER_OF_STAGES_PER_STRATA ) * app->getScreenHeight( );
			}
		} else {
			_camera_y += SPEED;
			if ( _camera_y > ( ( STAGE_NUM - 1 - _now_select_stage_num ) / NUMBER_OF_STAGES_PER_STRATA ) * app->getScreenHeight( ) ) {
				_camera_y = ( ( STAGE_NUM - 1 - _now_select_stage_num ) / NUMBER_OF_STAGES_PER_STRATA ) * app->getScreenHeight( );
			}
		}
	}
}

void SceneSelectStage::moveAnimePlayer( ) {
	if ( _transition->isStop( ) ) {
		int move_vec = 3;
		_player_pos_x += move_vec;
	}
}

void SceneSelectStage::walkPlayerOut( ) {
	SaveDataPtr save_data = Task::get< SaveData >( );
	if ( _move > -20 && save_data->getDegreeOfProgress( ) != -1 ) {
		_player_wait_time++;
		_player_wait_time %= 5;
		if ( _player_wait_time == 0 ) {
			_player_image_num++;
		}
		if ( _player_image_num == 21 ) {
			_player_image_num = 0;
		}
		_move -= 3;
		_player_walk_right[ _player_image_num ]->drawRect( Vector2( ( double )_player_pos_x - 20, ( double )_player_pos_y ),
														   _move, 0,
														   133, 150 );
	}
}

void SceneSelectStage::walkPlayerIn( ) {
	if ( _transition->isStop( ) ) {
		_player_wait_time++;
		_player_wait_time %= 5;
		if ( _player_wait_time == 0 ) {
			_player_image_num++;
		}
		if ( _player_image_num == 21 ) {
			_player_image_num = 0;
		}
		_player_walk_right[ _player_image_num ]->drawRect( Vector2( _player_pos_x, _player_pos_y ), 0, 0,
														   _stage_ui_pos[ _now_select_stage_num ].x + 80 - _player_pos_x, 150 );
	}
}

void SceneSelectStage::unlookEffectMove( ) {
	SaveDataPtr save_data = Task::get< SaveData >( );
	MusicPtr music = Task::get< Music >( );
	if ( _wait_criteria_time >= STAGE_UNLOOK_MOVE_TIME && _wait_select_count == _wait_criteria_time - 10 ) {
		_now_select_stage_num = save_data->getLastClearStage( ) + 1;
		if ( _now_select_stage_num >= STAGE_NUM ) {
			_now_select_stage_num = STAGE_NUM - 1;
		}
		if ( save_data->getDegreeOfProgress( ) == -1 ) {
			_now_select_stage_num = 0;
		}
	}
}

void SceneSelectStage::unlookStageEffect( ) {
	ApplicationPtr app = Application::getInstance( );
	SaveDataPtr save_data = Task::get< SaveData >( );
	MusicPtr music = Task::get< Music >( );
	if ( _wait_criteria_time >= STAGE_UNLOOK_MOVE_TIME && !_draw_effect ) {
		if ( save_data->getLastClearStage( ) % NUMBER_OF_STAGES_PER_STRATA == NUMBER_OF_STAGES_PER_STRATA - 1 ) {
			Vector2 pos = Vector2( 0, ( app->getScreenHeight( ) - ( ( STAGE_NUM / NUMBER_OF_STAGES_PER_STRATA - 1 ) * 100 ) - 200 ) / 2
								   + ( ( __int64 )STAGE_NUM / NUMBER_OF_STAGES_PER_STRATA - ( save_data->getLastClearStage( ) + 1 ) / NUMBER_OF_STAGES_PER_STRATA - 1 ) * 100 );

			if ( _wait_select_count > 95 && _wait_select_count < 155 ) {
				_spark[ _unlook_image_num ]->drawExtend( pos - Vector2( 50, 40 ), pos + Vector2( 250, 160 ) );

				//DrawerPtr drawer = Task::get< Drawer >( );
				//drawer->drawBox( pos.x, pos.y, pos.x + 170, pos.y + 100, Color( 0, 0, 0 ), false );
				_unlook_image_wait_time++;
				if ( _unlook_image_wait_time > 5 ) {
					_unlook_image_num++;
					_unlook_image_wait_time = 0;
				}

				if ( _unlook_image_num >= _spark.size( ) ) {
					_unlook_image_num = 0;
				}
				music->electroSE( );
			} else if ( _wait_select_count >= 170 ) {
				_draw_effect = true;
				music->onSE( );
			}
		} else {
			if ( _wait_select_count > 60 ) {
				Vector2 pos = Vector2( _stage_ui_pos[ ( __int64 )_now_select_stage_num + 1 ].x,
									   _stage_ui_pos[ ( __int64 )_now_select_stage_num + 1 ].y - _camera_y );

				_energy_field[ _unlook_image_num ]->drawExtend( pos - Vector2( 120, 220 ), pos + Vector2( 120, 20 ) );

				_unlook_image_wait_time++;
				if ( _unlook_image_wait_time > 2 ) {
					_unlook_image_num++;
					_unlook_image_wait_time = 0;
				}

				if ( _unlook_image_num >= _energy_field.size( ) ) {
					_draw_effect = true;
					music->openSE( );
				}
			}
		}
	}
}

void SceneSelectStage::setupWalk( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	//right
	_player_walk_right[ 0 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_000.png" );
	_player_walk_right[ 1 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_001.png" );
	_player_walk_right[ 2 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_002.png" );
	_player_walk_right[ 3 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_003.png" );
	_player_walk_right[ 4 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_004.png" );
	_player_walk_right[ 5 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_005.png" );
	_player_walk_right[ 6 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_006.png" );
	_player_walk_right[ 7 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_007.png" );
	_player_walk_right[ 8 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_008.png" );
	_player_walk_right[ 9 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_009.png" );
	_player_walk_right[ 10 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_010.png" );
	_player_walk_right[ 11 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_011.png" );
	_player_walk_right[ 12 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_012.png" );
	_player_walk_right[ 13 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_013.png" );
	_player_walk_right[ 14 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_014.png" );
	_player_walk_right[ 15 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_015.png" );
	_player_walk_right[ 16 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_016.png" );
	_player_walk_right[ 17 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_017.png" );
	_player_walk_right[ 18 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_018.png" );
	_player_walk_right[ 19 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_019.png" );
	_player_walk_right[ 20 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_020.png" );
	_player_walk_right[ 21 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_021.png" );
}