#include "Fan.h"
#include "Define.h"

Fan::Fan( Vector2 pos, Vector2 size, DIR dir, double vec, bool start_state, bool auto_switching, int on_time, int off_time ) :
	Gimmick( pos, size, TYPE::FAN, dir ) {
	_vec = vec;
	_on = start_state;
	_count = 0;
	_on_time = on_time;
	_off_time = off_time;
	_auto_switching = auto_switching;
}

Fan::~Fan( ) {

}

void Fan::update( ) {
	if ( !_auto_switching ) {
		return;
	}
	_count++;
	if ( _on ) {
		if ( _count >= _on_time ) {
			_count = 0;
			_on = false;
		}
	} else {
		if ( _count >= _off_time  ) {
			_count = 0;
			_on = true;
		}
	}
}

Vector2 Fan::getVec( ) const {
	Vector2 vec = Vector2( );
	switch ( getDir( ) ) {
	case DIR::LEFT:
		vec = Vector2( -_vec, 0 );
		break;
	case DIR::RIGHT:
		vec = Vector2( _vec, 0 );
		break;
	case DIR::UP:
		vec = Vector2( 0, -_vec );
		break;
	case DIR::DOWN:
		vec = Vector2( 0, _vec );
		break;
	}

	return vec;
}

bool Fan::isOn( ) const {
	return _on;
}

int Fan::getCount( ) const{
	return _count;
}

int Fan::getOffTime( ) const{
	return _off_time;
}

int Fan::getOnTime( ) const{
	return _on_time;
}
