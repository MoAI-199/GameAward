#include "ViewerPrologue.h"
#include "Drawer.h"
#include "Define.h"
#include "Image.h"
#include "MapData.h"
#include "Camera.h"

ViewerPrologue::ViewerPrologue( PlayerPtr player ) :
	_player( player ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_prologue = drawer->createImage( "Resource/image/prologue/prologue_text.png" );
	//_background = drawer->createImage( "Resource/image/" );
	setupPlayerWalk( );

	_scroll = 0;
	_player_walk_right_wait_time = 0;
	_player_walk_right_num = 0;
}

ViewerPrologue::~ViewerPrologue( ) {
}

void ViewerPrologue::draw( ) {
	drawText( );
	drawPlayer( );
}

void ViewerPrologue::drawText( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );

	_prologue->drawRect( 0, 0, 0, _scroll, app->getScreenWidth( ), app->getScreenHeight( ) + _scroll );
	_scroll += 2;
}

void ViewerPrologue::drawPlayer( ) {
	animePlayerWalkRight( _player->getPos( ) );
}

void ViewerPrologue::drawBackGround( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	drawer->setBlend( scope, Drawer::BLEND::ADD, BG_BRIGHTNESS );
	_background->draw( 0, 0 );
}

void ViewerPrologue::drawFloor( ) {
}

void ViewerPrologue::animePlayerWalkRight( Vector2 pos ) {
	_player_walk_right_wait_time++;
	_player_walk_right_wait_time %= 2;
	if ( _player_walk_right_wait_time == 0 ) {
		_player_walk_right_num++;
	}
	if ( _player_walk_right_num == 22 ) {
		_player_walk_right_num = 0;
	}

	_player_walk_right[ _player_walk_right_num ]->draw( pos );
}

void ViewerPrologue::setupPlayerWalk( ) {
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
	//left
	_player_walk_left[ 0 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_000.png" );
	_player_walk_left[ 1 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_001.png" );
	_player_walk_left[ 2 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_002.png" );
	_player_walk_left[ 3 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_003.png" );
	_player_walk_left[ 4 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_004.png" );
	_player_walk_left[ 5 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_005.png" );
	_player_walk_left[ 6 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_006.png" );
	_player_walk_left[ 7 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_007.png" );
	_player_walk_left[ 8 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_008.png" );
	_player_walk_left[ 9 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_009.png" );
	_player_walk_left[ 10 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_010.png" );
	_player_walk_left[ 11 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_011.png" );
	_player_walk_left[ 12 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_012.png" );
	_player_walk_left[ 13 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_013.png" );
	_player_walk_left[ 14 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_014.png" );
	_player_walk_left[ 15 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_015.png" );
	_player_walk_left[ 16 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_016.png" );
	_player_walk_left[ 17 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_017.png" );
	_player_walk_left[ 18 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_018.png" );
	_player_walk_left[ 19 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_019.png" );
	_player_walk_left[ 20 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_020.png" );
	_player_walk_left[ 21 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_021.png" );

}
