#include "Gimmick.h"


Gimmick::Gimmick( Vector2 pos, Vector2 size, TYPE type, DIR dir ) {
	_pos = pos;
	_size = size;
	_type = type;
	_dir = dir;
	_delete = false;
}

Gimmick::~Gimmick( ) {

}

Vector2 Gimmick::getPos( ) const{
	return _pos;
}

Vector2 Gimmick::getSize( ) const {
	return _size;
}

Gimmick::TYPE Gimmick::getType( ) const {
	return _type;
}

Gimmick::DIR Gimmick::getDir( ) const {
	return _dir;
}

bool Gimmick::isDeleted( ) const {
	return _delete;
}

void Gimmick::setDelete( ) {
	_delete = true;
}

