#include "Camera.h"
#include "Keyboard.h"

const int SPEED = 10;

Camera::Camera( ) { 
	_pos = Vector2( 0, 0 );
}

Camera::~Camera( ) {

}

void Camera::update( ) {
	scroll( );
}

Vector2 Camera::getPos( ) const {
	return _pos;
}

void Camera::scroll( ) { 
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( keyboard->isHoldKey( "W" ) ) {
		_pos -= Vector2( 0, SPEED );
	}
	if ( keyboard->isHoldKey( "S" ) ) {
		_pos += Vector2( 0, SPEED );
	}
	if ( keyboard->isHoldKey( "A" ) ) {
		_pos -= Vector2( SPEED, 0 );
	}
	if ( keyboard->isHoldKey( "D" ) ) {
		_pos += Vector2( SPEED, 0 );
	}
}