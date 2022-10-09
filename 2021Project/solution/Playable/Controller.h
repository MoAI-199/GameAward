#pragma once
#include "smart_ptr.h"

PTR( Controller );

class Controller {
public:
	Controller( );
	virtual ~Controller( );
public:
	void update( );

	bool getPushA( ) const;
	bool getPushB( ) const;
	bool getPushX( ) const;
	bool getPushY( ) const;
	bool getHoldB( ) const;

	bool getStickUp( ) const;
	bool getStickDown( ) const;
	bool getStickLeft( ) const;
	bool getStickRight( ) const;
	bool isTilted( );

	void updateTilted( );
	void setTilted( bool tilted );
private:
	bool _tilted;

};

