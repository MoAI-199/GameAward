#pragma once
#include "mathematics.h"


class BlackCloud {
public:
	BlackCloud( );
	virtual ~BlackCloud( );
public:
	struct Status {
		Vector2 pos = Vector2( );
		double scale = 0.0;
	};
public:
	void setStatus( Status status );
public:
	Vector2 getPos( ) const;
	double getScale( ) const;
private:
	Status _status;
};

