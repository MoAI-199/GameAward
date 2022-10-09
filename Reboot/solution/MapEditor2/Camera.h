#pragma once
#include "smart_ptr.h"
#include "mathematics.h"

class Camera {
public:
	Camera( );
	virtual ~Camera( );
public:
	void update( );
	Vector2 getPos( ) const;
private:
	void scroll( );
private:
	Vector2 _pos;
};


