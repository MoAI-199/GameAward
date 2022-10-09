#include "CursorEvent.h"
#include "Cursor.h"
#include "CloudsManager.h"
#include "PlayersManager.h"
#include "define.h"

CursorEvent::CursorEvent( CursorPtr cursor,
						  CloudsManagerPtr clouds_manager,
						  PlayersManagerPtr players_manager ) {

}

CursorEvent::~CursorEvent( ) {
}

void CursorEvent::update( ) {
	if ( isLoop( ) ) {
		looping( );
	}
}

void CursorEvent::looping( ) {
	_cursor->setPos( SCREEN_WIDTH / 2 );
} 

bool CursorEvent::isLoop( ) {
	Vector2 cloud_tip_pos;
	Vector2 player_pos;

	if (  cloud_tip_pos.x - player_pos.x <  ) {

	}

	return false;
}

