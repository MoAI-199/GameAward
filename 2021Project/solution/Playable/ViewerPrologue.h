#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include <vector>
#include <array>

PTR( Image );
PTR( Player );
class ViewerPrologue {
public:
	ViewerPrologue( PlayerPtr plaeyer );
	virtual ~ViewerPrologue( );
public:
	void draw( );
private:
	void drawText( );
	void drawPlayer( );
	void drawBackGround( ) const;
	void drawFloor( );
	void animePlayerWalkRight( Vector2 pos );
	void setupPlayerWalk( );
private:
	PlayerPtr _player;
	ImagePtr _prologue;
	ImagePtr _background;
	std::array< ImagePtr, 22 > _player_walk_right;
	std::array< ImagePtr, 22 > _player_walk_left;
	int _scroll;
	int _player_walk_right_wait_time;
	int _player_walk_right_num;
};

