#pragma once
#include "Sheep.h"
#include "smart_ptr.h"
#include "mathematics.h"

PTR( SheepsManager );

class SheepsController {
public:
	SheepsController( SheepsManagerPtr manager );
	virtual ~SheepsController( );
public:
	int getSheepNum( ) const;
	int getLeadPlayer( ) const;
	double getSize( int idx ) const;
	double getAngle( int idx ) const;
	double getViewSize( int idx ) const;
	double getScale( int idx ) const;
	Vector2 getPos( int idx ) const;
	Sheep::COLOR_TYPE getColorType( int idx ) const;
private:
	SheepsManagerPtr _manager;
};

