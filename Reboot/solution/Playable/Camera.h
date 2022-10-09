#pragma once
#include "smart_ptr.h"
#include "mathematics.h"

PTR( MapData );

class Camera {
public:
	Camera( MapDataPtr data );
	virtual ~Camera( );
public:
	void update( );
	Vector2 getPos( ) const;
private:
	void debugScroll( );
	void restrictPos( Vector2& pos );
	void chaseAlivePlayer( );
	void returnCameraToRespawn( );
	void endingMove( );
private:
	MapDataPtr _data;
	Vector2 _pos;
	Vector2 _dead_pos;
	Vector2 _return_vec;
};

