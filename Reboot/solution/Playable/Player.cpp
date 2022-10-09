#include "Player.h"
#include "Controller.h"
#include "mathematics.h"
#include "MapData.h"
#include "Application.h"
#include "Define.h"
#include "Music.h"
#include "SaveData.h"

static const double GRAVITY = 9.8;
static const int MOVE_SPEED = 5;

double y = 0;

Player::Player( Vector2 pos, ControllerPtr controller ) :
	_pos( pos ),
	_controller( controller ) {
	_vec = Vector2( 0, 0 );
	_before_pos = Vector2( );
	_size = Vector2( PLAYER_SIZE_X, PLAYER_SIZE_Y );
	_gravity_count = 20;
	_jump_power = 0.0;
	_jump_count = 0;
	_soaked_count = 0;
	_stick_used_count = 0;
	_before_stand_pos_y = pos.y;
	_maximum_pos_y = pos.y;
	_hp = PLAYER_HP;
	_move_speed = MOVE_SPEED;
	_inherited = false;
	_standing = true;
	_jumping = false;
	_delete = false;
	_die_adjusted = false;
	_moving = false;
	_collapse = false;
	_dead_lie = false;
	_drawn_die_effect = false;
	_reason = DEAD_REASON::NONE;
	_state = STATE::ALIVE;
	_dir = DIR::RIGHT;
	_ending_move_stop_count = 120;
	_suicide_count = 0;
	_after_suicide_existence_count = 0;
	_explosive = false;
	_cannot_explosive = false;
}

Player::~Player( ) {
}

void Player::update( ) {
	_moving = false;
	updatePos( );
	if ( _standing ) {
		_before_stand_pos_y = _pos.y;
		_maximum_pos_y = _pos.y + PLAYER_SIZE_Y;
		_gravity_count = 30;
	}
	gravity( );
	if ( _state == STATE::ALIVE ) {
		jump( );
		move( );
		suicide( );
	} else {
		acidDamage( );
		if ( _reason == DEAD_REASON::SUICIDE ) {
			_after_suicide_existence_count++;
		}
	}
	if ( !_explosive && !_moving && _standing ) {
		if ( _controller->getHoldB( ) ) {
			_cannot_explosive = true;
		} else {
			_cannot_explosive = false;
		}
	} else {
		_cannot_explosive = false;
	}
	_explosive = true;
}

void Player::endingUpdate( ) {
	updatePos( );
	if ( _standing ) {
		_before_stand_pos_y = _pos.y;
		_maximum_pos_y = _pos.y + PLAYER_SIZE_Y;
		_gravity_count = 30;
	}
	if ( _state == STATE::ALIVE ) {
		endingMove( );
	}
	if ( _pos.y > 1600 ) {
		setDead( true );
	} else {
		gravity( );
	}
}

void Player::updatePos( ) {
	ApplicationPtr app = Application::getInstance( );
	if ( _reason != DEAD_REASON::NEEDLE ) {
		_pos += _vec;
	}
	_vec = Vector2( 0, 0 );
}

void Player::gravity( ) {
	if ( _reason == DEAD_REASON::NEEDLE || _reason == DEAD_REASON::STICKY_NET ) {
		return;
	}
	ApplicationPtr app = Application::getInstance( );
	_vec.y += GRAVITY * pow( _gravity_count / 32, 2 );
	_gravity_count++;
}

void Player::move( ) {
	MusicPtr music = Task::get< Music >( );
	if ( _controller->getStickLeft( ) ) {
		_vec.x -= _move_speed;
		_stick_used_count++;
		_dir = DIR::LEFT;
		_moving = true;
	}
	if ( _controller->getStickRight( ) ) {
		_vec.x += _move_speed;
		_stick_used_count++;
		_dir = DIR::RIGHT;
		_moving = true;
	}
#if _DEBUG
	if ( _controller->getStickUp( ) ) {
		_vec.y -= _move_speed;
	}
	if ( _controller->getStickDown( ) ) {
		_vec.y += _move_speed;
	}
#endif
}

void Player::jump( ) {
	MusicPtr music = Task::get< Music >( );
	double height = 25;
	double distortion = 2;

	if ( _controller->getPushA( ) && _standing ) {
		music->jumpSE( );
		_jumping = true;
		_standing = false;
		_jump_count = 0;
	}

	if ( _jump_count >= 90 || _standing ) {
		_jumping = false;
		_jump_count = 0;
	}

	if ( _jumping ) {
		double rad = cos( _jump_count * ( PI / 180 ) );
		y = height * pow( rad, distortion );
		if ( _jump_count >= 90 ) {
			y = 0;
		}
		_jump_count++;
		_vec.y -= y;
		if ( _vec.y < 0 && _maximum_pos_y > _pos.y + PLAYER_SIZE_Y ) {
			_maximum_pos_y = _pos.y + PLAYER_SIZE_Y;
		}
	}
}

void Player::acidDamage( ) {
	if ( _collapse ) {
		_hp -= 3;
		_vec.y = 1;
		/*
		if ( _hp < ( PLAYER_HP * 4 / 5 ) ) {
			Vector2 size;
			if ( _dead_lie ) {
				double width = PLAYER_LIE_SIZE_X - ( double )( ( PLAYER_HP * 4 / 5 - _hp ) / 2 );
				if ( width < PLAYER_LIE_SIZE_X / 2 ) {
					width = PLAYER_LIE_SIZE_X / 2;
				}
				_size = Vector2( width, _size.y );
			} else {
				double height = PLAYER_SIZE_Y - ( double )( ( PLAYER_HP * 4 / 5 - _hp ) / 2 );
				if ( height < PLAYER_SIZE_Y / 2 ) {
					height = PLAYER_SIZE_Y / 2;
				}
				_size = Vector2( _size.x, height );
			}
		}
		*/
	}
	if ( _hp < 0 ) {
		_delete = true;
	}
}

void Player::suicide( ) {
	if ( _explosive ) {
		if ( !_moving || _standing ) {
			if ( _controller->getHoldB( ) ) {
				_suicide_count++;
			} else {
				_suicide_count = 0;
			}
		}
	}
	if ( _moving || !_standing ) {
		_suicide_count = 0;
	}
	if ( _suicide_count > SUICIDE_MAX_COUNT ) {
		setDead( false );
		setDeadReason( DEAD_REASON::SUICIDE );
		_size = Vector2( 75, 65 );
	}
}

void Player::endingMove( ) {
	MusicPtr music = Task::get< Music >( );
	SaveDataPtr save_data = Task::get< SaveData >( );
	_moving = true;
	if ( _pos.x >= 2050 ) {
		if ( save_data->isComplete( ) ) {
			if ( _ending_move_stop_count == 0 && _move_speed != 0 ) {
				_ending_move_stop_count = 120;
			}
			_move_speed = 0;
			_moving = false;
			if ( _ending_move_stop_count < 0 ) {
				if ( _state == STATE::ALIVE ) {
					_move_speed = 50;
				}
				setDead( false );
				setDeadReason( DEAD_REASON::SUICIDE );
				_size = Vector2( 75, 65 );
			} else {
				_ending_move_stop_count--;
			}
		}
	} else if ( _pos.x >= 1900 ) {
		if ( _ending_move_stop_count <= 0 ) {
			_move_speed = 2;
		} else {
			_ending_move_stop_count--;
			_move_speed = 0;
			_moving = false;
		}
	} else if ( _pos.x == 500 ) {
		if ( _ending_move_stop_count < 0 ) {
			_move_speed = 8;
			_ending_move_stop_count = 120;
		} else {
			if ( _ending_move_stop_count == 120 ) {
				music->exclamationSE( );
			}
			_ending_move_stop_count -= 5;
			_move_speed = 0;
			_moving = false;
		}
	}
	_vec.x += _move_speed;
	_stick_used_count++;
	_dir = DIR::RIGHT;
}

Vector2 Player::getPos( ) const {
	return _pos;
}

Vector2 Player::getVec( ) const {
	return _vec;
}


Player::STATE Player::getState( ) const {
	return _state;
}

Player::DIR Player::getDir( ) const {
	return _dir;
}

bool Player::isInherited( ) const {
	return _inherited;
}

int Player::getStickUsedCount( ) const {
	return _stick_used_count;
}

bool Player::isDeleted( ) const {
	return _delete;
}

bool Player::isStanding( ) const {
	return _standing;
}

bool Player::isJumping( ) const {
	return _jumping;
}

double Player::getBeforeStandPosY( ) const {
	return _before_stand_pos_y;
}

double Player::getMaximumPosY( ) const {
	return _maximum_pos_y;
}

int Player::getNowSoakedCount( ) const {
	return _soaked_count;
}

int Player::getSuicideCount( ) const {
	return _suicide_count;
}

Player::DEAD_REASON Player::getReason( ) const {
	return _reason;
}

int Player::getAfterSuicideExistenceCount( ) const {
	return _after_suicide_existence_count;
}


bool Player::isCollapse( ) const {
	return _collapse;
}

int Player::getHP( ) const {
	return _hp;
}

bool Player::isDeadLie( ) const {
	return _dead_lie;
}

Vector2 Player::getSize( ) const {
	return _size;
}

bool Player::isDieAdjusted( ) const {
	return _die_adjusted;
}

bool Player::isDrawnDieEffdect( ) const {
	return _drawn_die_effect;
}

bool Player::isExplosive( ) const {
	return _explosive;
}

bool Player::isCannotExplosive( ) const {
	return _cannot_explosive;
}

bool Player::isMoving( ) const {
	return _moving;
}


void Player::addSoakedCount( ) {
	_soaked_count++;
}

void Player::resetSoakedCount( ) {
	_soaked_count = 0;
}

Vector2& Player::setPos( ) {
	return _pos;
}

void Player::setDieAdjusted( ) {
	_die_adjusted = true;
}

void Player::setDrawnDieEffect( ) {
	_drawn_die_effect = true;
}

void Player::setDeadReason( DEAD_REASON reason ) {
	_reason = reason;
}



void Player::setCollapse( ) {
	_collapse = true;
}

void Player::setDelete( ) {
	_delete = true;
}

void Player::setMoving( bool moving ) {
	_moving = moving;
}

void Player::setExplosive( bool explosive ) {
	_explosive = explosive;
}

Vector2& Player::setVec( ) {
	return _vec;
}

void Player::speedDeceleration( ) {
	if ( _move_speed > PLAYER_MINIMAM_SPEED ) {
		_move_speed--;
	}
}

double& Player::setGravityCount( ) {
	return _gravity_count;
}

void Player::setInherited( ) {
	_inherited = true;
}

void Player::setStanding( bool standing ) {
	_standing = standing;
}


void Player::setDead( bool lie ) {
	if ( _state != STATE::DEAD ) {
		_dead_lie = lie;
		if ( lie ) {
			if ( _dir == DIR::LEFT ) {
				_pos += Vector2( ( __int64 )PLAYER_SIZE_X - PLAYER_LIE_SIZE_X, 0 );
			}
			_pos += Vector2( 0, ( __int64 )PLAYER_SIZE_Y - PLAYER_LIE_SIZE_Y );
			_size = Vector2( PLAYER_LIE_SIZE_X, PLAYER_LIE_SIZE_Y );
		} else {
			_size = Vector2( PLAYER_LIE_SIZE_Y, PLAYER_LIE_SIZE_X );
		}
		MusicPtr music = Task::get< Music >( );
		music->deadSE( );
	}
	_state = STATE::DEAD;
}

