#pragma once
#include "mathematics.h"
#include "smart_ptr.h"

PTR( CloudsManager );

class CloudsController {
public:
	CloudsController( CloudsManagerPtr manager );
	virtual ~CloudsController( );
public:
	Vector2 getPos( int idx ) const;
	double getScale( int idx ) const;
	int getMaxSize( ) const;
	bool getValid( int idx ) const;
private:
	CloudsManagerPtr _manager;
};

