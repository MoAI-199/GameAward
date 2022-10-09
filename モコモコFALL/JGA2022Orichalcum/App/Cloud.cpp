#include "Cloud.h"
#include "define.h"

Cloud::Cloud( Vector2 base_pos,  double scale ) {
	_status.pos = base_pos;
	_status.scale = scale;
	_status.base_line = _status.pos;
	_status.valid = true;
}

Cloud::~Cloud( ) {
}

void Cloud::update() {
	_status.pos += _status.vec;
}

Vector2 Cloud::getPos( ) const{
	return _status.pos;
}

double Cloud::getScale( ) const{
	return _status.scale;
}

bool Cloud::getValid( ) const {
	return _status.valid;
}

Vector2 Cloud::geBaseLdine() const {
	return _status.base_line;
}

void Cloud::setPos( Vector2 pos ) {
	_status.pos = pos;
}

void Cloud::setVec(Vector2 vec) {
	_status.vec = vec;
}

void Cloud::setValid( bool valid ) {
	_status.valid = valid;
}


