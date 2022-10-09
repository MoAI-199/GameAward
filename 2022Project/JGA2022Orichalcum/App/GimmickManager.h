#pragma once
#include <vector>
#include "smart_ptr.h"
#include "Gimmick.h"
#include "ColorBall.h"
#include "mathematics.h"

PTR( Gimmick );

class GimmickManager {
public:
	GimmickManager( );
	virtual ~GimmickManager( );
public:
	void update( );
	void clearAll( );
	void addGimmick( GimmickPtr gimmick );
	void deleteGimmick( int idx );
	void setPos( int idx, Vector2 pos );
	Vector2 getVec( int idx );
	int getMaxNum( );
	Vector2 getPos( int idx ) const;
	double getSize( int idx ) const;
	Gimmick::TYPE getType( int idx ) const;
	ColorBall::COLOR_TYPE getColorType( int idx ) const;
private:
	std::vector< GimmickPtr > _list;
};

