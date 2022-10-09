#include "Camera.h"
#include "Keyboard.h"
#include "MapData.h"
#include "Player.h"
#include "Define.h"
#include "StageManager.h"

const int SPEED = 5;

Camera::Camera( MapDataPtr data ) :
	_data( data ) {
	_pos = Vector2( 0, 0 );
	_dead_pos = Vector2( 0, 0 );
	_return_vec = Vector2( 0, 0 );
	chaseAlivePlayer( );
}

Camera::~Camera( ) {

}

void Camera::update( ) {
#ifdef _DEBUG
	debugScroll( );
#endif
	StageManagerPtr stage_manager = Task::get< StageManager >( );
	if ( _data->getStageName( stage_manager->getStageNum( ) ) == "ending_stage" ) {
		endingMove( );
	} else {
		chaseAlivePlayer( );
		returnCameraToRespawn( );
	}
}

Vector2 Camera::getPos( ) const {
	return _pos;
}

void Camera::debugScroll( ) {
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
	restrictPos( _pos );
}

void Camera::restrictPos( Vector2& pos ) {
	ApplicationPtr app = Application::getInstance( );
	if ( pos.x < 0 ) {
		pos = Vector2( 0, pos.y );
	}
	if ( _pos.x + app->getScreenWidth( ) > _data->getWidth( ) ) {
		pos = Vector2( ( __int64 )_data->getWidth( ) - app->getScreenWidth( ), pos.y );
	}

	if ( pos.y < 0 ) {
		pos = Vector2( pos.x, 0 );
	}
	if ( pos.y + app->getScreenHeight( ) > _data->getHeight( ) ) {
		pos = Vector2( pos.x, ( __int64 )_data->getHeight( ) - app->getScreenHeight( ) );
	}
}

void Camera::chaseAlivePlayer( ) {
	ApplicationPtr app = Application::getInstance( );
	std::vector< PlayerPtr > players = _data->getPlayers( );
	for ( int i = 0; i < players.size( ); i++ ) {
		if ( players[ i ]->getState( ) == Player::STATE::ALIVE ) {
			_pos = players[ i ]->getPos( ) - Vector2( app->getScreenWidth( ) / 2, app->getScreenHeight( ) / 2 )
				+ Vector2( players[ i ]->getSize( ).x / 2, players[ i ]->getSize( ).y / 2 );
		}
	}
	restrictPos( _pos );
}

void Camera::returnCameraToRespawn( ) {
	int wait = 30;
	ApplicationPtr app = Application::getInstance( );
	if ( _data->getRespawnCount( ) != 0 ) {
		if ( _data->getRespawnCount( ) == 1 ) {
			_dead_pos = _pos;
			Vector2 respawn_pos = Vector2( );
			std::vector< GimmickPtr > gimmicks = _data->getGimmicks( );
			for ( int i = 0; i < gimmicks.size( ); i++ ) {
				if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::RESPAWN ) {
					respawn_pos = gimmicks[ i ]->getPos( ) - Vector2( app->getScreenWidth( ) / 2, app->getScreenHeight( ) / 2 );
					restrictPos( respawn_pos );
				}
			}
			_return_vec = respawn_pos - _dead_pos;
		} else if ( _data->getRespawnCount( ) > wait ) {
			Vector2 vec = Vector2( _return_vec.x * ( ( __int64 )_data->getRespawnCount( ) - wait ) / ( ( __int64 )RESPAWN_TIME - wait ),
								   _return_vec.y * ( ( __int64 )_data->getRespawnCount( ) - wait ) / ( ( __int64 )RESPAWN_TIME - wait ) );
			_pos = _dead_pos + vec;
			restrictPos( _pos );
		}
	}

}

void Camera::endingMove( ) {
	std::vector< PlayerPtr > players = _data->getPlayers( );
	for ( int i = 0; i < players.size( ); i++ ) {
		if ( players[ i ]->getState( ) == Player::STATE::ALIVE ) {
			if ( _pos.x < 0 ) {
				chaseAlivePlayer( );
			} else if ( players[ i ]->getPos( ).x >= 1900 ) {
				chaseAlivePlayer( );
				_pos.x += 1900;
			}
		}
	}
	restrictPos( _pos );
}
