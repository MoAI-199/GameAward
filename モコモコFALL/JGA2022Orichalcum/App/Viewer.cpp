#include "Viewer.h"
#include "CloudsController.h"
#include "SheepsController.h"
#include "Drawer.h"
#include "define.h"
#include "GimmickController.h"
#include "ColorBall.h"
#include "Exception.h"
#include "PropellerManager.h"
#include "Image.h"
#include "FundamentalGameSystem.h"

static const int NUMBER_HORIZONTAL_SIZE = 60;
static const int NUMBER_VERTICAL_SIZE = 92;
static const int UI_HORIZONTAL_SIZE = 300;
static const int UI_VERTICAL_SIZE = 50;

Viewer::Viewer( SheepsControllerPtr sheeps_controller,
				CloudsControllerPtr clouds_controller,
				GimmickControllerPtr gimmick_controller,
				PropellerManagerPtr propeller_manager,
				FundamentalGameSystemPtr game_system ) :
	_sheeps_controller( sheeps_controller ),
	_clouds_controller( clouds_controller ),
	_gimmick_controller( gimmick_controller ),
	_propeller_manager( propeller_manager ),
	_game_system( game_system ) {
	loadImages( );
	_heart_extend = 0.0;
	_heart_reverse = false;
}

Viewer::~Viewer( ) {

}

void Viewer::draw( ) {
	drawBgMain( );
	drawGimmicks( );
	drawClouds( );
	drawSheeps( );
	drawPropeller( );

	Vector2 draw_pos = Vector2( SCREEN_WIDTH - UI_HORIZONTAL_SIZE + NUMBER_HORIZONTAL_SIZE - 20, 0 );
	int level = _game_system->getLevel( );
	drawUserInterface( draw_pos, level, USER_INTERFACE::LEVEL );
	int moko_rate = _game_system->getMokoRate( );
	if ( moko_rate < 0 ) {
		moko_rate = 0;
	}
	draw_pos.y += UI_VERTICAL_SIZE * 2;
	drawUserInterface( draw_pos, moko_rate, USER_INTERFACE::MOKO_RATE );
}

void Viewer::loadImages( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_img_bg_main = drawer->createImage( "resource/bg_main.png" );
	_img_cloud = drawer->createImage( "resource/cloud.png" );
	_img_sheep_none = drawer->createImage( "resource/sheep_none.png" );
	_img_sheep_red = drawer->createImage( "resource/sheep_red.png" );
	_img_sheep_bule = drawer->createImage( "resource/sheep_bule.png" );
	_img_sheep_yellow = drawer->createImage( "resource/sheep_yellow.png" );
	_img_sheep_green = drawer->createImage( "resource/sheep_green.png" );
	_img_sheep_purple = drawer->createImage( "resource/sheep_purple.png" );
	_img_sheep_orange = drawer->createImage( "resource/sheep_orange.png" );
	_img_sheep_secret = drawer->createImage( "resource/sheep_secret.png" );
	_img_propeller = drawer->createImage( "resource/propeller.png" );
	_img_heart = drawer->createImage( "resource/heart.png" );
	_img_board = drawer->createImage( "resource/board.png" );
	_img_ui[ 0 ] = drawer->createImage( "resource/level.png" );
	_img_ui[ 1 ] = drawer->createImage( "resource/rate.png" );
	for ( int i = 0; i < 10; i++ ) {
		string file_path = "resource/number_";
		file_path += to_string( i );
		file_path += ".png";
		_img_number[ i ] = drawer->createImage( file_path.c_str( ) );
	}
}

void Viewer::drawBgMain( ) {
	_img_bg_main->draw( 0, 0 );
}

void Viewer::drawSheeps( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	int player_num = _sheeps_controller->getSheepNum( );
	for ( int i = 0; i < player_num; i++ ) {
		Vector2 pos = _sheeps_controller->getPos( i );
		double size = _sheeps_controller->getSize( i );
		double scale = _sheeps_controller->getScale( i );
		double angle = _sheeps_controller->getAngle( i );
		double view_size = _sheeps_controller->getViewSize( i );
		ImagePtr img = specifySheep( i, view_size, angle, _sheeps_controller->getColorType( i ) );
		img->drawRectRotation( pos, scale, angle, 50, 50, 0, 0, 100, 100 );
		drawer->drawCircle( pos, size, Color( 1.0, 0, 0 ), false );
	}
}

void Viewer::drawClouds( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	int size = _clouds_controller->getMaxSize( );
	for ( int i = 0; i < size; i++ ) {
		int idx = i;
		if ( !_clouds_controller->getValid( idx ) ) {
			continue;
		}
		Vector2 cloud_pos = _clouds_controller->getPos( idx );
		double radius = CLOUD_RADIUS_SIZE * _clouds_controller->getScale( idx );

		Vector2 img_size( 120, 120 );
		//drawer->drawCircle( cloud_pos, radius, Color( 1.0, 1.0, 1.0 ), true );
		//drawer->drawCircle( cloud_pos, radius + 2, Color( 0.0, 0.0, 0.0 ), false );

		_img_cloud->draw(cloud_pos - ( img_size.multiply( 0.5 ) ) );

		//“–‚½‚è”»’è
		drawer->drawCircle( cloud_pos, radius, Color( 1.0, 0.0, 0.0 ), false );
	}
	/*
	
	for ( int i = 0; i < size; i++ ) {
		int idx = i;
		if ( !_clouds_controller->getValid( idx ) ) {
			continue;
		}
		Vector2 cloud_pos = _clouds_controller->getPos( idx );
		double radius = CLOUD_RADIUS_SIZE * _clouds_controller->getScale( idx );

		drawer->drawCircle( cloud_pos, radius + 1, Color( 1.0, 1.0, 1.0 ), true );
	}
	*/
}

void Viewer::drawGimmicks( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	int gimmick_num = _gimmick_controller->getGimmickNum( );
	double animation_speed = 0.01;
	if ( _heart_reverse ) {
		_heart_extend -= animation_speed;
	} else {
		_heart_extend += animation_speed;
	}
	if ( _heart_extend > 1.0 || ( _heart_extend < 0.5 && _heart_reverse ) ) {
		_heart_reverse = !_heart_reverse;
	}
	for ( int i = 0; i < gimmick_num; i++ ) {
		int idx = i;
		Vector2 pos = _gimmick_controller->getPos( idx );
		double size = _gimmick_controller->getSize( idx );
		Gimmick::TYPE type = _gimmick_controller->getType( idx );

		switch ( type ) {
		case Gimmick::TYPE::COIN:
			drawCoin( pos, size );
			break;
		case Gimmick::TYPE::HOLE:
			drawer->drawCircle( pos, size, Color( 1.0, 1.0, 1.0 ), true );
			break;
		case Gimmick::TYPE::COLOR_BALL:
			//ColorBall::COLOR_TYPE color_type = _gimmick_controller->getColorType( idx );
			drawer->drawCircle( pos, size, specifyColorBall( _gimmick_controller->getColorType( idx ) ), true );
			break;
		case Gimmick::TYPE::CARROT:
			drawCarrot( pos, size );
			drawer->drawCircle( pos, size, Color( 1.0, 1.0, 1.0 ), true );
			break;
		}
	}
}

void Viewer::drawCoin( Vector2 pos, double size ) {
	_img_heart->drawRectRotation( pos, _heart_extend, 0.0, 50, 50, 0, 0, 100, 100 );
}

void Viewer::drawCarrot( Vector2 pos, double size ) {
	//_img_heart->drawRectRotation( pos, _heart_extend, 0.0, 50, 50, 0, 0, 100, 100 );
}

void Viewer::drawPropeller( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	PropellerManager::Status status = _propeller_manager->getStatus( );


	double angle = atan2( ( status.shaft_pos.y + status.shaft_size / 2 ) - ( status.wing1[ 7 ].pos.y + status.wing1[ 7 ].size ),
						  ( status.shaft_pos.x + status.shaft_size / 2 ) - ( status.wing1[ 7 ].pos.x + status.wing1[ 7 ].size ) );

	if ( angle < 0 ) {
		angle = angle + 2 * PI;
	}
	angle = angle * 360 / ( 2 * PI ) - 90;
	angle = angle * PI / 180;

	Vector2 draw_pos = Vector2( status.shaft_pos.x, status.shaft_pos.y );

	_img_propeller->drawRectRotation( draw_pos, 0.5, angle, 350, 378, 0, 0, 700, 700 );

}

void Viewer::drawUserInterface( Vector2 pos, int num, USER_INTERFACE ui ) {
	vector< int > numbers;
	if ( num > 999 ) {
		num = 999;
	}
	while ( num != 0 ) {
		numbers.push_back( num % 10 );
		num /= 10;
	}
	int size = ( int )numbers.size( );
	if ( size < 3 ) {
		int diff = 3 - size;
		for ( int i = 0; i < diff; i++ ) {
			numbers.push_back( 0 );
		}
		size = ( int )numbers.size( );
	}
	Vector2 draw_pos = pos;
	draw_pos.x -= ( size * NUMBER_HORIZONTAL_SIZE ) / 2;
	draw_pos.x += UI_HORIZONTAL_SIZE / 2;
	Vector2 left_top = Vector2( draw_pos.x - NUMBER_HORIZONTAL_SIZE / 4 - UI_HORIZONTAL_SIZE - UI_HORIZONTAL_SIZE / 6, draw_pos.y );
	Vector2 right_bottom = Vector2( ( draw_pos.x + size * NUMBER_HORIZONTAL_SIZE ) + NUMBER_HORIZONTAL_SIZE / 2,
									draw_pos.y + NUMBER_VERTICAL_SIZE );

	_img_board->drawExtend( left_top, right_bottom );

	Vector2 ui_pos = left_top;
	ui_pos.x += UI_HORIZONTAL_SIZE / 6;
	ui_pos.y += UI_VERTICAL_SIZE / 2;
	switch ( ui ) {
	case Viewer::USER_INTERFACE::LEVEL:
		_img_ui[ 0 ]->draw( ui_pos );
		break;
	case Viewer::USER_INTERFACE::MOKO_RATE:
		_img_ui[ 1 ]->draw( ui_pos );
		break;
	}

	for ( int i = 0; i < size; i++ ) {
		int idx = size - 1 - i;
		int num = numbers[ idx ];
		_img_number[ num ]->draw( draw_pos );
		draw_pos.x += NUMBER_HORIZONTAL_SIZE;
	}
}

//Œã‚Å–ß‚è’l‚ð‰æ‘œ‚É·‚µ‘Ö‚¦
Color Viewer::specifyColorBall( ColorBall::COLOR_TYPE type ) {
	Color color;
	switch ( type ) {
	case ColorBall::COLOR_TYPE::NONE:
		EXCEPTION( true, "ColorType Error" );
		break;
	case ColorBall::COLOR_TYPE::RED:
		color = Color( 1.0, 0.0, 0.0 );
		break;
	case ColorBall::COLOR_TYPE::BULE:
		color = Color( 0.0, 0.0, 1.0 );
		break;
	case ColorBall::COLOR_TYPE::GREEN:
		color = Color( 0.0, 1.0, 0.0 );
		break;
	case ColorBall::COLOR_TYPE::YELLO:
		color = Color( 1.0, 1.0, 0.0 );
		break;
	case ColorBall::COLOR_TYPE::ORANGE:
		color = Color( 1.0, 0.6, 0.2 );
		break;
	case ColorBall::COLOR_TYPE::PURPLE:
		color = Color( 0.75, 0.0, 1.0 );
		break;
	}
	return color;
}


ImagePtr Viewer::specifySheep( int sheep_num, double size, double angle, Sheep::COLOR_TYPE type ) {
	ImagePtr img;
	Vector2 sheeps_pos = _sheeps_controller->getPos( sheep_num );
	switch ( type ) {
	case Sheep::COLOR_TYPE::NONE:
		return _img_sheep_none;
		break;
	case Sheep::COLOR_TYPE::RED:
		return _img_sheep_red;
		break;
	case Sheep::COLOR_TYPE::BULE:
		return _img_sheep_bule;
		break;
	case Sheep::COLOR_TYPE::GREEN:
		return _img_sheep_green;
		break;
	case Sheep::COLOR_TYPE::YELLOW:
		return _img_sheep_yellow;
		break;
	case Sheep::COLOR_TYPE::ORANGE:
		return _img_sheep_orange;
		break;
	case Sheep::COLOR_TYPE::PURPLE:
		return _img_sheep_purple;
		break;
	case Sheep::COLOR_TYPE::SECRET:
		return _img_sheep_secret;
	}
	return img;
}

ImagePtr Viewer::changeSheepSize( ImagePtr sheep, Vector2 pos, double size, double angle ) {
	sheep->drawRectRotation( pos, size, angle, 50, 50, 0, 0, 100, 100 );
	return sheep;
}