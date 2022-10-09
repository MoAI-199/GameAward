#include "SceneTitle.h"
#include "Drawer.h"
#include "Image.h"
#include "Define.h"
#include "Controller.h"
#include "System.h"
#include "SaveData.h"
#include "Transition.h"
#include "Music.h"
#include "Application.h"

SCENE( SceneTitle );

const int START_IMAGE_WIDTH = 880;
const int START_IMAGE_HEIGHT = 80;

const int DATA_IMAGE_WIDTH = 410;
const int DATA_IMAGE_HEIGHT = 320;

SceneTitle::SceneTitle( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_transition = TransitionPtr( new Transition( ) );
	_controller = ControllerPtr( new Controller( ) );
	//_start = drawer->createImage( "Resource/Image/Title/start.png" );
	_save_data_frame = drawer->createImage( "Resource/Image/UI/strata_window_db.png" );
	_save_data_text = drawer->createImage( "Resource/Image/Title/save_data.png" );
	_number = drawer->createImage( "Resource/Image/image_number.png" );
	_window = drawer->createImage( "Resource/Image/UI/strata_window_db.png" );
	_background = drawer->createImage( "Resource/Image/background/title2_BG.png" );
	_back = drawer->createImage( "Resource/Image/UI/back.png" );
	_back_frame = drawer->createImage( "Resource/Image/UI/back_frame.png" );
	_b_botton = drawer->createImage( "Resource/Image/UI/b_botton.png" );
	_new = drawer->createImage( "Resource/Image/Title/NEW_bold.png" );
	_please_push_botton = drawer->createImage( "Resource/Image/UI/press_button.png" );
	_button_switch[ 0 ] = drawer->createImage( "Resource/Image/UI/a_button_false.png" );
	_button_switch[ 1 ] = drawer->createImage( "Resource/Image/UI/a_button_true.png" );

	_switch_image_num = 0;
	_switch_count = 0;
	_select_data = 0;
	_mode = MODE::NORMAL;
	_anime_image_num = 0;
	_anime_wait_time = 0;
	_next_loop_wait_time = 0;
	_title_text_wait_time = 0;
	_title_text_image_num = 0;
	_selected_data = false;
	_extend = true;
	_first = false;
	setupAnime( );
	setupTextAnime( );
}

SceneTitle::~SceneTitle( ) {
}

void SceneTitle::initialize( ) {
	SaveDataPtr save_data = Task::get< SaveData >( );
	ApplicationPtr app = Application::getInstance( );
	save_data->reset( );
	for ( int i = 0; i < 3; i++ ) {
		save_data->setDataNum( i );
		save_data->load( );
		_degree_of_progress[ i ] = save_data->getDegreeOfProgress( );
		save_data->reset( );
	}
	_selected_data = false;

	_transition->setIrisIn( app->getScreenWidth( ) / 2,
							app->getScreenHeight( ) / 2,
							0,
							0,
							DEFAULT_IRIS_SPEED,
							0,
							0 );

	MusicPtr music = Task::get< Music >( );
	music->playBGM( "demon_soul_loop_cyber_13", "mp3", 50 );
}

void SceneTitle::update( ) {
	if ( _first ) {
		draw( );
	}
	_first = true;
	_transition->update( );
	switch ( _mode ) {
	case MODE::NORMAL:
		next( );
		break;
	case MODE::LOAD:
		chooseLoad( );
		selectLoad( );
		backNormal( );
		break;
	}
	_controller->updateTilted( );
}

void SceneTitle::finalize( ) {
	MusicPtr music = Task::get< Music >( );
	music->stopBGM( );
}

void SceneTitle::draw( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	//drawer->drawBox( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color( 1, 1, 1 ), true );
	_background->draw( 0, 0 );
	drawAnime( );
	if ( _mode == MODE::LOAD ) {
		drawLoad( );
	} else {
		drawTitle( );
		drawNormal( );
	}
}

void SceneTitle::drawTitle( ) {
	ApplicationPtr app = Application::getInstance( );
	int pos_x = app->getScreenWidth( ) / 2 - 600;
	int pos_y = app->getScreenHeight( ) / 2 - 200;
	_title_text_delay_time++;
	if ( _title_text_delay_time > 120 ) {
		_title_text_wait_time++;
		_title_text_wait_time %= 8;
		if ( _title_text_wait_time == 0 ) {
			_title_text_image_num++;
			if ( _title_text_image_num >= 7 ) {
				_title_text_delay_time = 0;
			}
			_title_text_image_num %= 7;
		}
	}
	_title_text_anime[ _title_text_image_num ]->draw( pos_x, pos_y );
}

void SceneTitle::drawNormal( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );
	int add_pos_x = 40;
	int add_pos_y = 200;
	int width = app->getScreenWidth( );
	int height = app->getScreenHeight( );
	int button_size = 120;


	_please_push_botton->draw( width / 2 - START_IMAGE_WIDTH / 2 - 50 + add_pos_x, height / 2 + START_IMAGE_HEIGHT + add_pos_y );
	_switch_count++;
	_switch_count %= 30;
	if ( _switch_count == 0 ) {
		_switch_image_num++;
	}
	if ( _switch_image_num > 1 ) {
		_switch_image_num = 0;
	}
	_button_switch[ _switch_image_num ]->draw( width / 2 - button_size / 2 - 80 + add_pos_x, height / 2 + START_IMAGE_HEIGHT - 15 + add_pos_y );
}

void SceneTitle::drawLoad( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	int frame_extend = 50;
	Vector2 text_extend = Vector2( 160, 40 );
	int back_adjust = 140;
	int botton_adjust = 50;
	int pos_y = 500;
	int number_pos_y = pos_y + 65;
	int extend_number_adjust_x = 50;
	int number_adjust_x = 10;
	int new_adjust_x = 90;
	int new_pos_y = pos_y + 90;

	ApplicationPtr app = Application::getInstance( );
	int interval = ( app->getScreenWidth( ) - DATA_IMAGE_WIDTH * 3 ) / 4;

	for ( int i = 0; i < 3; i++ ) {
		int pos_x = interval + DATA_IMAGE_WIDTH * i + interval * i;
		if ( _select_data == i ) {
			_save_data_frame->drawExtend( pos_x - frame_extend, pos_y - frame_extend,
										  pos_x + DATA_IMAGE_WIDTH + frame_extend, pos_y + DATA_IMAGE_HEIGHT + frame_extend );
			_save_data_text->drawExtend( pos_x + 10 - frame_extend * 3 / 4, pos_y - text_extend.y / 2,
										 pos_x + 10 - frame_extend * 3 / 4 + 256 + text_extend.x, pos_y + 64 + text_extend.y / 2 );
		} else {
			_save_data_frame->draw( pos_x, pos_y );
			_save_data_text->draw( pos_x + 10, pos_y + 10 );
		}

		if ( _degree_of_progress[ i ] != -1 ) {
			if ( _select_data == i ) {
				drawExtendNumber( ( __int64 )pos_x + number_adjust_x + extend_number_adjust_x - frame_extend, ( __int64 )number_pos_y + 65 - frame_extend, ( __int64 )NUMBER_IMAGE_SIZE + frame_extend, _degree_of_progress[ i ] / NUMBER_OF_STAGES_PER_STRATA + 1 );
				_number->drawRectExtend( pos_x + number_adjust_x + 105 + extend_number_adjust_x - frame_extend / 2, number_pos_y + 65 - 10 - frame_extend,
										 pos_x + number_adjust_x + 105 + extend_number_adjust_x + NUMBER_IMAGE_SIZE + frame_extend / 2, number_pos_y + 65 - 10 + NUMBER_IMAGE_SIZE,
										 NUMBER_IMAGE_SIZE * 3, NUMBER_IMAGE_SIZE * 3, NUMBER_IMAGE_SIZE, NUMBER_IMAGE_SIZE );
				drawExtendNumber( ( __int64 )pos_x + number_adjust_x + 160 + extend_number_adjust_x, ( __int64 )number_pos_y + 65 - frame_extend, ( __int64 )NUMBER_IMAGE_SIZE + frame_extend, _degree_of_progress[ i ] % NUMBER_OF_STAGES_PER_STRATA + 1 );
			} else {
				drawNumber( ( __int64 )pos_x + number_adjust_x, number_pos_y, _degree_of_progress[ i ] / NUMBER_OF_STAGES_PER_STRATA + 1 );
				_number->drawRect( pos_x + number_adjust_x + 95, number_pos_y - 10, NUMBER_IMAGE_SIZE * 3, NUMBER_IMAGE_SIZE * 3, NUMBER_IMAGE_SIZE, NUMBER_IMAGE_SIZE );
				//drawer->drawLine( pos_x + 140, 420, pos_x + 180, 420, Color( 1.0, 1.0, 1.0 ) );
				drawNumber( ( __int64 )pos_x + number_adjust_x + 160, number_pos_y, _degree_of_progress[ i ] % NUMBER_OF_STAGES_PER_STRATA + 1 );
			}
		} else {
			//new
			if ( _select_data == i ) {
				_new->drawExtend( pos_x + new_adjust_x - frame_extend / 4, new_pos_y + frame_extend / 2,
								  pos_x + new_adjust_x + 140 * 2 - frame_extend / 4, new_pos_y + 53 * 2 + frame_extend / 2 );
			} else {
				_new->draw( pos_x + new_adjust_x, new_pos_y );
			}
		}
	}
	int height = app->getScreenHeight( );

	_back_frame->drawExtend( -100, height - 130, 200, height );
	_back->drawExtend( 20, height - 130, 20 + back_adjust, height - 130 + back_adjust );
	_b_botton->drawExtend( 120, height - 110, 120 + botton_adjust, height - 110 + botton_adjust );
}

void SceneTitle::drawNumber( double pos_x, double pos_y, int number ) const {
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

void SceneTitle::drawExtendNumber( double pos1_x, double pos1_y, double size, int number ) const {
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

void SceneTitle::next( ) {
	MusicPtr music = Task::get< Music >( );
	if ( _controller->getPushA( ) && !_transition->isTransition( ) ) {
		music->decisionSE( );
		_mode = MODE::LOAD;
	}
}

void SceneTitle::chooseLoad( ) {
	MusicPtr music = Task::get< Music >( );
	if ( !_transition->isTransition( ) ) {
		if ( !_controller->isTilted( ) ) {
			if ( _controller->getStickLeft( ) && _select_data > 0 ) {
				music->cursorSE( );
				_select_data--;
			}
			if ( _controller->getStickRight( ) && _select_data < 2 ) {
				music->cursorSE( );
				_select_data++;
			}
			//–ß‚é‚ð‘I‚Ô
			/*if ( _controller->getStickUp( ) && _select_data == -1 ) {
				_select_data = 0;
			}
			if ( _controller->getStickDown( ) ) {
				_select_data = -1;
			}*/
		}
	}
}

void SceneTitle::selectLoad( ) {
	MusicPtr music = Task::get< Music >( );
	ApplicationPtr app = Application::getInstance( );
	SystemPtr system = Task::get< System >( );
	int interval = ( app->getScreenWidth( ) - DATA_IMAGE_WIDTH * 3 ) / 4;

	if ( _controller->getPushA( ) && !_transition->isTransition( ) ) {
		music->decisionSE( );
		if ( _select_data >= 0 ) {
			SaveDataPtr save_data = Task::get< SaveData >( );
			MusicPtr music = Task::get< Music >( );
			save_data->setDataNum( _select_data );
			save_data->load( );
			_transition->setIrisOut( interval * ( _select_data + 1 ) + DATA_IMAGE_WIDTH * _select_data + DATA_IMAGE_WIDTH / 2,
									 500 + DATA_IMAGE_HEIGHT / 2,
									 0,
									 0,
									 DEFAULT_IRIS_SPEED,
									 0,
									 0 );

			_selected_data = true;
		} else {
			_select_data = 0;
			_mode = MODE::NORMAL;
		}
	}
	if ( !_transition->isScaling( ) && _selected_data ) {
		system->setLoaded( );
		loadScene( "SceneSelectStage" );
	}
}

void SceneTitle::backNormal( ) {
	MusicPtr music = Task::get< Music >( );
	if ( _controller->getPushB( ) && !_selected_data ) {
		music->cancelSE( );
		_mode = MODE::NORMAL;
	}
}

void SceneTitle::drawAnime( ) {
	_anime_wait_time++;
	_anime_wait_time %= 4;
	if ( _anime_wait_time == 0 ) {
		if ( _anime_image_num != 23 ) {
			_anime_image_num++;

		}
	}
	if ( _anime_image_num == 23 ) {
		_next_loop_wait_time++;
		_next_loop_wait_time %= 40;
		if ( _next_loop_wait_time == 0 ) {
			_anime_image_num = 0;
		}
	}
	_title_palyer_anime[ _anime_image_num ]->draw( 0, 0 );
}

void SceneTitle::setupAnime( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_title_palyer_anime[ 0 ] = drawer->createImage( "Resource/image/title/anime/anime00.png" );
	_title_palyer_anime[ 1 ] = drawer->createImage( "Resource/image/title/anime/anime01.png" );
	_title_palyer_anime[ 2 ] = drawer->createImage( "Resource/image/title/anime/anime02.png" );
	_title_palyer_anime[ 3 ] = drawer->createImage( "Resource/image/title/anime/anime03.png" );
	_title_palyer_anime[ 4 ] = drawer->createImage( "Resource/image/title/anime/anime04.png" );
	_title_palyer_anime[ 5 ] = drawer->createImage( "Resource/image/title/anime/anime05.png" );
	_title_palyer_anime[ 6 ] = drawer->createImage( "Resource/image/title/anime/anime06.png" );
	_title_palyer_anime[ 7 ] = drawer->createImage( "Resource/image/title/anime/anime07.png" );
	_title_palyer_anime[ 8 ] = drawer->createImage( "Resource/image/title/anime/anime08.png" );
	_title_palyer_anime[ 9 ] = drawer->createImage( "Resource/image/title/anime/anime09.png" );
	_title_palyer_anime[ 10 ] = drawer->createImage( "Resource/image/title/anime/anime10.png" );
	_title_palyer_anime[ 11 ] = drawer->createImage( "Resource/image/title/anime/anime11.png" );
	_title_palyer_anime[ 12 ] = drawer->createImage( "Resource/image/title/anime/anime12.png" );
	_title_palyer_anime[ 13 ] = drawer->createImage( "Resource/image/title/anime/anime13.png" );
	_title_palyer_anime[ 14 ] = drawer->createImage( "Resource/image/title/anime/anime14.png" );
	_title_palyer_anime[ 15 ] = drawer->createImage( "Resource/image/title/anime/anime15.png" );
	_title_palyer_anime[ 16 ] = drawer->createImage( "Resource/image/title/anime/anime16.png" );
	_title_palyer_anime[ 17 ] = drawer->createImage( "Resource/image/title/anime/anime17.png" );
	_title_palyer_anime[ 18 ] = drawer->createImage( "Resource/image/title/anime/anime18.png" );
	_title_palyer_anime[ 19 ] = drawer->createImage( "Resource/image/title/anime/anime19.png" );
	_title_palyer_anime[ 20 ] = drawer->createImage( "Resource/image/title/anime/anime20.png" );
	_title_palyer_anime[ 21 ] = drawer->createImage( "Resource/image/title/anime/anime21.png" );
	_title_palyer_anime[ 22 ] = drawer->createImage( "Resource/image/title/anime/anime22.png" );
	_title_palyer_anime[ 23 ] = drawer->createImage( "Resource/image/title/anime/anime23.png" );

}

void SceneTitle::setupTextAnime( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_title_text_anime[ 0 ] = drawer->createImage( "Resource/image/title/title/reboot_anime_8.png" );
	_title_text_anime[ 1 ] = drawer->createImage( "Resource/image/title/title/reboot_anime_7.png" );
	_title_text_anime[ 2 ] = drawer->createImage( "Resource/image/title/title/reboot_anime_6.png" );
	_title_text_anime[ 3 ] = drawer->createImage( "Resource/image/title/title/reboot_anime_5.png" );
	_title_text_anime[ 4 ] = drawer->createImage( "Resource/image/title/title/reboot_anime_4.png" );
	_title_text_anime[ 5 ] = drawer->createImage( "Resource/image/title/title/reboot_anime_3.png" );
	_title_text_anime[ 6 ] = drawer->createImage( "Resource/image/title/title/reboot_anime_2.png" );
	_title_text_anime[ 7 ] = drawer->createImage( "Resource/image/title/title/reboot_anime_1.png" );
}



