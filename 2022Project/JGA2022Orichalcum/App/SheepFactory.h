#pragma once
#include "smart_ptr.h"
#include "mathematics.h"

PTR( SheepsManager );
PTR( Sheep );

class SheepFactory {
public:
	SheepFactory( SheepsManagerPtr manager );
	virtual ~SheepFactory( );
public:
	void deploy( Vector2 pos, Vector2 vec );
private:
	SheepPtr createSheep( Vector2 pos, Vector2 vec );
private:
	SheepsManagerPtr _manager;
};

