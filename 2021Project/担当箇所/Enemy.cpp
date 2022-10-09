#include "Enemy.h"
#include "Define.h"
#include "mathematics.h"
#include "Define.h"

Enemy::Enemy( Vector2 pos, MOVE_PATTERN move, Vector2 vec ) {
	_pos = pos;
	_size = ENEMY_SIZE;
	_vec_move = vec;
	_vec = Vector2( 0, 0 );
	_status = STATUS::HARMFUL;
	_move_pattern = move;
	_angle = 0.0;
	_time = 0;
	_delete = false;
	_dir = DIR::RIGHT;
	_change_time = 0;
	_change = false;
}

Enemy::~Enemy( ) {
}

void Enemy::update( ) {
	move( );
	modeChange();
	if ( _status == STATUS::HARMFUL ) {
		movePattern( );
	} else {
		_vec.y = ENEMY_RIGING.y ;
	}
	if ( _vec.x <= 0 ) {
		_dir = DIR::LEFT;
	}
	else {
		_dir = DIR::RIGHT;
	}
	if ( _pos.y <  ENEMY_DELETE_POS) {
		_delete = true;
	}
}

void Enemy::move( ) {
	_pos += _vec;
}

//type•Êenemy‚Ì“®‚«
void Enemy::movePattern( ) {
	_angle += 0.025;
	switch ( _move_pattern ) {
	case Enemy::MOVE_PATTERN::CLOCK_WISE:
		_vec.x = cos( _angle ) * _vec_move.x;
		_vec.y = sin( _angle ) * _vec_move.y;
		break;
	case Enemy::MOVE_PATTERN::VERTICAL:
		_vec.x = 0.0;
		_vec.y = sin( _angle ) * _vec_move.y;
		break;
	case Enemy::MOVE_PATTERN::HORIZONTAL:
		_vec.x = cos( _angle ) * _vec_move.x;
		_vec.y = 0.0;
		break;
	case Enemy::MOVE_PATTERN::COUNTER_CLOCK_WISE:
		_vec.x = -cos( _angle ) * _vec_move.x;
		_vec.y = sin( _angle ) * _vec_move.y;
		break;
	}
}

void Enemy::capturePlayer( PlayerPtr player ) {
	if ( _delete ) {
		player->setDelete( );
	}
}

void Enemy::setChange( bool change ) {
	_change = change;
}

void Enemy::setStatus( STATUS status ) {
	_status = status;
}

bool Enemy::isDeleted( )  const{
	return _delete;
}

bool Enemy::isChange( ) const {
	return _change;
}

void Enemy::modeChange( ) {
	if ( _change ) {
		_change_time++;
	}
	if ( _change_time > 30 ) {
		_change = false;
	}
}

Enemy::STATUS Enemy::getStatus( ) const {
	return _status;
}

Enemy::DIR Enemy::getDir( ) const{
	return _dir;
}

Vector2 Enemy::getPos( ) const {
	return _pos;
}

Vector2 Enemy::getSize( ) const {
	return _size;
}

Vector2 Enemy::getVec( ) const {
	return _vec;
}
