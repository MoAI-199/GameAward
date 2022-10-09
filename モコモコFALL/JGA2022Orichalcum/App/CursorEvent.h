#pragma once
#include "smart_ptr.h"

PTR( Cursor );
PTR( PlayersManager );
PTR( CloudsManager );

class CursorEvent {
public:
	CursorEvent( CursorPtr cursor,
				 CloudsManagerPtr clouds_manager,
				 PlayersManagerPtr players_manager );
	~CursorEvent( );
public:
	void update( );
private:
	void looping( );
	bool isLoop( );
private:
	CursorPtr _cursor;
	CloudsManagerPtr _clouds_manager;
	PlayersManagerPtr _players_manger;

};
