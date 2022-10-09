#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include "Gimmick.h"
#include "ColorBall.h"

PTR( GimmickManager );

class GimmickController {
public:
	GimmickController( GimmickManagerPtr manager );
	virtual ~GimmickController( );
public:
	int getGimmickNum( );
	Vector2 getPos( int idx ) const;
	double getSize( int idx ) const;
	Gimmick::TYPE getType( int idx ) const;
	ColorBall::COLOR_TYPE getColorType( int idx ) const;
private:
	GimmickManagerPtr _manager;
};

