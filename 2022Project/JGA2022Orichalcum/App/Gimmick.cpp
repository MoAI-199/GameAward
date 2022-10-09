#include "Gimmick.h"
#include "define.h"
#include <cmath>

static const double SPEED = 5.0;

static const double AREA = 100.0;

Gimmick::Gimmick( ) {
}

Gimmick::~Gimmick( ) {
}

void Gimmick::update( ) {
	if ( _status.type == TYPE::HOLE ) {
		return;
	}
	Vector2 diff;
	diff.x = SCREEN_WIDTH / 2 - ( _status.pos.x + _status.size );
	diff.y = SCREEN_HEIGHT / 2 - ( _status.pos.y + _status.size );

	if ( !_status.mode ) {
		double radian = atan2( diff.y, diff.x );
		_status.vec.x = cos( radian ) * 4;
		_status.vec.y = sin( radian ) * 4;
	}

	

	diff.x = abs( _status.pos.x - SCREEN_WIDTH / 2 );
	diff.y = abs( _status.pos.y - SCREEN_HEIGHT / 2 );

	if ( diff.x < AREA && diff.y < AREA ) {
		_status.mode = true;
	}

}

void Gimmick::setPos( Vector2 pos ) {
	_status.pos = pos;
}

void Gimmick::setSize( double size ) {
	_status.size = size;
}

void Gimmick::setType( TYPE type ) {
	_status.type = type;
}

Vector2 Gimmick::getPos( ) const {
	return _status.pos;
}

Vector2 Gimmick::getVec( ) const {
	return _status.vec;
}

double Gimmick::getSize( ) const {
	return _status.size;
}

Gimmick::TYPE Gimmick::getType( ) const {
	return _status.type;
}

