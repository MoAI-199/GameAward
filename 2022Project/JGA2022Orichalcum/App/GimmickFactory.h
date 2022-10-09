#pragma once
#include "smart_ptr.h"
#include "Gimmick.h"
#include "mathematics.h"
#include "ColorBall.h"

PTR( GimmickManager );
PTR( Gimmick );
PTR( Coin );
PTR( Carrot );
PTR( ColorBall );
PTR( Hole );

class GimmickFactory {
public:
	GimmickFactory( GimmickManagerPtr manager );
	virtual ~GimmickFactory( );
public:
	void deploy( Vector2 pos, Gimmick::TYPE type );
	void deploy( Vector2 pos, Gimmick::TYPE type, ColorBall::COLOR_TYPE color_type );
private:
	GimmickPtr createCoin( Vector2 pos ) const;
	GimmickPtr createColor( Vector2 pos, ColorBall::COLOR_TYPE type ) const;
	GimmickPtr createHole( Vector2 pos ) const;
	GimmickPtr createCarrot( Vector2 pos ) const;

private:
	GimmickManagerPtr _manager;
};

