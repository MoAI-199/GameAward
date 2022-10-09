#include "Collider.h"
#include "MapData.h"
#include "Player.h"
#include "Log.h"
#include "Define.h"
#include "Enemy.h"
#include "Fan.h"
#include "System.h"
#include "Menu.h"

Collider::Collider( MapDataPtr map_data, MenuPtr menu ) :
	_map_data( map_data ),
	_menu( menu ) {
	_dir = COLLIDE_DIR::NONE;
	_collide_up = false;
	_collide_down = false;
	_collide_left = false;
	_collide_right = false;
	//wind
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::FAN ) {
			setupWind( std::dynamic_pointer_cast< Fan >( gimmicks[ i ] ) );
		}
	}
}

Collider::~Collider( ) {

}

void Collider::update( ) {
	if ( _menu->isMenu( ) ) {
		return;
	}
	collidePlayers( );
}

Collider::Wind* Collider::getWind( FanPtr fan ) {
	return _winds[ fan ];
}

void Collider::collidePlayers( ) {
	std::vector< PlayerPtr > players = _map_data->getPlayers( );

	//wind
	collideWithWinds( players );

	//ground
	collidePlayerWithGrounds( players );

	//other_player
	collidePlayerWithOtherPlayers( players );

	//gimmick
	collidePlayerWithGimmicks( players );

	//enemy
	collidePlayerWithEnemies( players );


	//out of map
	for ( int i = 0; i < players.size( ); i++ ) {
		PlayerPtr src_player = players[ i ];
		if ( src_player->getPos( ).y > _map_data->getHeight( ) ) {
			src_player->setDead( true );
			src_player->setDelete( );
		}
	}
}

void Collider::collidePlayerWithGrounds( std::vector< PlayerPtr > players ) {
	std::vector< MapData::Ground* > grounds = _map_data->getGrounds( );
	for ( int i = 0; i < grounds.size( ); i++ ) {
		MapData::Ground* ground = grounds[ i ];
		for ( int j = 0; j < players.size( ); j++ ) {
			PlayerPtr src_player = players[ j ];
			if ( src_player->getReason( ) == Player::DEAD_REASON::ENEMY ) {
				continue;
			}
			collidePlayerWithGround( src_player, ground );
		}
	}
}

void Collider::collidePlayerWithGround( PlayerPtr player, const MapData::Ground* ground ) {
	LogPtr log = Task::get< Log >( );

	_dir = COLLIDE_DIR::NONE;

	_collide_up = false;
	_collide_down = false;
	_collide_left = false;
	_collide_right = false;

	double player_left_sx = player->getPos( ).x;
	double player_right_sx = player->getPos( ).x + player->getSize( ).x;
	double player_top_sy = player->getPos( ).y;
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	//上下
	{
		if ( collidePlayerWithGroundFromUp( player, ground ) ) {
			//log->status( "hit_up" );
			_dir = COLLIDE_DIR::UP;
			_collide_up = true;
		}
		if ( collidePlayerWithGroundFromDown( player, ground ) ) {
			//log->status( "hit_down" );
			_dir = COLLIDE_DIR::DOWN;
			_collide_down = true;
		}

		if ( _collide_up && _collide_down ) {
			//vecが貫通しているとき
			//log->status( "hit_up_down" );
			double p_bottom_to_g_top = ground->pos.y - player_bottom_sy;
			double p_bottom_to_g_bottom = ground->pos.y + ground->actual_size.y - player_bottom_sy;
			if ( p_bottom_to_g_top < p_bottom_to_g_bottom ) {
				_dir = COLLIDE_DIR::UP;
				_collide_down = false;
			} else {
				_dir = COLLIDE_DIR::DOWN;
				_collide_up = false;
			}
		}

	}

	//左右
	{
		if ( collidePlayerWithGroundFromLeft( player, ground ) ) {
			//log->status( "hit_left" );
			_dir = COLLIDE_DIR::LEFT;
			_collide_left = true;
		}
		if ( collidePlayerWithGroundFromRight( player, ground ) ) {
			//log->status( "hit_right" );
			_dir = COLLIDE_DIR::RIGHT;
			_collide_right = true;
		}

		if ( _collide_left && _collide_right ) {
			//vecが貫通しているとき
			//log->status( "hit_left_right" );
			double p_left_to_g_left = ground->pos.x - player_left_sx;
			double p_left_to_g_right = ( ground->pos.x + ground->actual_size.x ) - player_left_sx;
			if ( p_left_to_g_left < p_left_to_g_right ) {
				_dir = COLLIDE_DIR::LEFT;
				_collide_right = false;
			} else {
				_dir = COLLIDE_DIR::RIGHT;
				_collide_left = false;
			}
		}
	}

	//角に当たったとき
	if ( ( _collide_up || _collide_down ) && ( _collide_left || _collide_right ) ) {
		collideCornerWithGround( player, ground );
	}

	double adjust_vec_x = player->getVec( ).x;
	double adjust_vec_y = player->getVec( ).y;
	switch ( _dir ) {
	case Collider::COLLIDE_DIR::LEFT:
		adjust_vec_x = ground->pos.x - player_right_sx;
		player->setMoving( false );
		if ( adjust_vec_x < 0.0 ) {
			adjust_vec_x = 0.0;
		}
		break;
	case Collider::COLLIDE_DIR::RIGHT:
		adjust_vec_x = ( ground->pos.x + ground->actual_size.x ) - player_left_sx;
		player->setMoving( false );
		if ( adjust_vec_x > 0.0 ) {
			adjust_vec_x = 0.0;
		}
		break;
	case Collider::COLLIDE_DIR::UP:
		adjust_vec_y = ground->pos.y - player_bottom_sy;
		if ( adjust_vec_y < 0.0 ) {
			adjust_vec_y = 0.0;
		}
		player->setStanding( true );
		//バウンド
		//player->setGravityCount( ) = 40 - abs( ( player->getPos( ).y + player->getSize( ).y - player->getMaximumPosY( ) ) / 20 );
		//落下死
		if ( player->getPos( ).y > player->getMaximumPosY( ) + DEAD_DOWNWARD_HEIGHT ) {
			player->setDead( true );
			collidePlayerWhenDie( player );
		}
		break;
	case Collider::COLLIDE_DIR::DOWN:
		adjust_vec_y = ( ground->pos.y + ground->actual_size.y ) - player_top_sy;
		if ( adjust_vec_y > 0.0 ) {
			adjust_vec_y = 0.0;
		}
		break;
	}

	if ( adjust_vec_y > 0 ) {
		player->setStanding( false );
	}

	player->setVec( ) = Vector2( adjust_vec_x, adjust_vec_y );
}

void Collider::collideCornerWithGround( PlayerPtr player, const MapData::Ground* ground ) {
	double player_top_sy = player->getPos( ).y;
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	if ( _collide_up && _collide_left ) {
		if ( player_bottom_sy > ground->pos.y ) {
			_dir = COLLIDE_DIR::LEFT;
		} else {
			_dir = COLLIDE_DIR::UP;
		}
	}
	if ( _collide_up && _collide_right ) {
		if ( player_bottom_sy > ground->pos.y ) {
			_dir = COLLIDE_DIR::RIGHT;
		} else {
			_dir = COLLIDE_DIR::UP;
		}

	}
	if ( _collide_down && _collide_left ) {
		if ( player_top_sy < ground->pos.y + ground->actual_size.y ) {
			_dir = COLLIDE_DIR::LEFT;
		} else {
			_dir = COLLIDE_DIR::DOWN;
		}
	}
	if ( _collide_down && _collide_right ) {
		if ( player_top_sy < ground->pos.y + ground->actual_size.y ) {
			_dir = COLLIDE_DIR::RIGHT;
		} else {
			_dir = COLLIDE_DIR::DOWN;
		}
	}
}

bool Collider::collidePlayerWithGroundFromLeft( PlayerPtr player, const MapData::Ground* ground ) {
	bool collide_from_left = false;

	double player_left_sx = player->getPos( ).x;

	double player_moved_right_sx = player->getPos( ).x + player->getSize( ).x + player->getVec( ).x;
	double player_top_sy = player->getPos( ).y + player->getVec( ).y;
	if ( player->getVec( ).y > 0 ) {
		player_top_sy = player->getPos( ).y;
	}
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y + player->getVec( ).y;
	if ( player->getVec( ).y < 0 ) {
		player_bottom_sy = player->getPos( ).y + player->getSize( ).y;
	}

	if ( player_top_sy < ground->pos.y && player_bottom_sy > ground->pos.y + ground->actual_size.y ) {
		//player_height > ground_height
		if ( player_moved_right_sx >= ground->pos.x && player_left_sx < ground->pos.x ) {
			collide_from_left = true;
		} else {
			collide_from_left = false;
		}
	} else {
		bool top_y_line_collide = false;
		bool bottom_y_line_collide = false;

		//player_top_line with ground_left_line
		Vector2 player_left_top_pos = Vector2( player_left_sx, player_top_sy );
		Vector2 player_right_top_pos = Vector2( player_moved_right_sx, player_top_sy );

		Vector2 ground_left_top_pos = ground->pos;
		Vector2 ground_left_bottom_pos = Vector2( ground->pos.x, ground->pos.y + ground->actual_size.y );

		top_y_line_collide = isCrossingOtherLineSegment( player_left_top_pos, player_right_top_pos,
														 ground_left_top_pos, ground_left_bottom_pos, false );


		//player_bottom_line with ground_left_line
		Vector2 player_left_bottom_pos = Vector2( player_left_sx, player_bottom_sy );
		Vector2 player_right_bottom_pos = Vector2( player_moved_right_sx, player_bottom_sy );

		bottom_y_line_collide = isCrossingOtherLineSegment( player_left_bottom_pos, player_right_bottom_pos,
															ground_left_top_pos, ground_left_bottom_pos, false );



		collide_from_left = top_y_line_collide || bottom_y_line_collide;
	}
	return collide_from_left;
}

bool Collider::collidePlayerWithGroundFromRight( PlayerPtr player, const MapData::Ground* ground ) {
	bool collide_from_right = false;

	double player_right_sx = player->getPos( ).x + player->getSize( ).x;

	double player_moved_left_sx = player->getPos( ).x + player->getVec( ).x;
	double player_top_sy = player->getPos( ).y + player->getVec( ).y;
	if ( player->getVec( ).y > 0 ) {
		player_top_sy = player->getPos( ).y;
	}
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y + player->getVec( ).y;
	if ( player->getVec( ).y < 0 ) {
		player_bottom_sy = player->getPos( ).y + player->getSize( ).y;
	}

	if ( player_top_sy < ground->pos.y && player_bottom_sy > ground->pos.y + ground->actual_size.y ) {
		//player_height > ground_height
		if ( player_moved_left_sx <= ground->pos.x + ground->actual_size.x && player_right_sx > ground->pos.x + ground->actual_size.x ) {
			collide_from_right = true;
		} else {
			collide_from_right = false;
		}
	} else {
		bool top_y_line_collide = false;
		bool bottom_y_line_collide = false;

		//player_top_line with ground_right_line
		Vector2 player_right_top_pos = Vector2( player_right_sx, player_top_sy );
		Vector2 player_left_top_pos = Vector2( player_moved_left_sx, player_top_sy );

		Vector2 ground_right_top_pos = Vector2( ground->pos.x + ground->actual_size.x, ground->pos.y );
		Vector2 ground_right_bottom_pos = Vector2( ground->pos.x + ground->actual_size.x, ground->pos.y + ground->actual_size.y );

		top_y_line_collide = isCrossingOtherLineSegment( player_right_top_pos, player_left_top_pos,
														 ground_right_top_pos, ground_right_bottom_pos, false );


		//player_bottom_line with ground_right_line
		Vector2 player_right_bottom_pos = Vector2( player_right_sx, player_bottom_sy );
		Vector2 player_left_bottom_pos = Vector2( player_moved_left_sx, player_bottom_sy );

		bottom_y_line_collide = isCrossingOtherLineSegment( player_right_bottom_pos, player_left_bottom_pos,
															ground_right_top_pos, ground_right_bottom_pos, false );

		collide_from_right = top_y_line_collide || bottom_y_line_collide;
	}
	return collide_from_right;
}

bool Collider::collidePlayerWithGroundFromUp( PlayerPtr player, const MapData::Ground* ground ) {
	bool collide_from_up = false;

	double player_top_sy = player->getPos( ).y;

	double player_left_sx = player->getPos( ).x + player->getVec( ).x;
	if ( player->getVec( ).x > 0 ) {
		player_left_sx = player->getPos( ).x;
	}
	double player_right_sx = player->getPos( ).x + player->getSize( ).x + player->getVec( ).x;
	if ( player->getVec( ).x < 0 ) {
		player_right_sx = player->getPos( ).x + player->getSize( ).x;
	}

	double player_moved_bottom_sy = player->getPos( ).y + player->getSize( ).y + player->getVec( ).y;

	if ( player_left_sx < ground->pos.x && player_right_sx > ground->pos.x + ground->actual_size.x ) {
		//player_width > ground_width
		if ( player_moved_bottom_sy >= ground->pos.y && player_top_sy < ground->pos.y ) {
			collide_from_up = true;
		} else {
			collide_from_up = false;
		}
	} else {
		bool left_x_line_collide = false;
		bool right_x_line_collide = false;

		//player_left_line with ground_top_line
		Vector2 player_left_top_pos = Vector2( player_left_sx, player_top_sy );
		Vector2 player_left_bottom_pos = Vector2( player_left_sx, player_moved_bottom_sy );

		Vector2 ground_left_top_pos = ground->pos;
		Vector2 ground_right_top_pos = Vector2( ground->pos.x + ground->actual_size.x, ground->pos.y );

		left_x_line_collide = isCrossingOtherLineSegment( player_left_top_pos, player_left_bottom_pos,
														  ground_left_top_pos, ground_right_top_pos, false );


		//player_right_line with ground_top_line
		Vector2 player_right_top_pos = Vector2( player_right_sx, player_top_sy );
		Vector2 player_right_bottom_pos = Vector2( player_right_sx, player_moved_bottom_sy );

		right_x_line_collide = isCrossingOtherLineSegment( player_right_top_pos, player_right_bottom_pos,
														   ground_left_top_pos, ground_right_top_pos, false );

		collide_from_up = left_x_line_collide || right_x_line_collide;
	}
	return collide_from_up;
}

bool Collider::collidePlayerWithGroundFromDown( PlayerPtr player, const MapData::Ground* ground ) {
	bool collide_from_down = false;

	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	double player_left_sx = player->getPos( ).x + player->getVec( ).x;
	if ( player->getVec( ).x > 0 ) {
		player_left_sx = player->getPos( ).x;
	}
	double player_right_sx = player->getPos( ).x + player->getSize( ).x + player->getVec( ).x;
	if ( player->getVec( ).x < 0 ) {
		player_right_sx = player->getPos( ).x + player->getSize( ).x;
	}
	double player_moved_top_sy = player->getPos( ).y + player->getVec( ).y;

	if ( player_left_sx < ground->pos.x && player_right_sx > ground->pos.x + ground->actual_size.x ) {
		if ( player_moved_top_sy <= ground->pos.y + ground->actual_size.y && player_bottom_sy > ground->pos.y + ground->actual_size.y ) {
			collide_from_down = true;
		} else {
			collide_from_down = false;
		}
	} else {
		bool left_x_line_collide = false;
		bool right_x_line_collide = false;

		//player_left_line with ground_bottom_line
		Vector2 player_left_bottom_pos = Vector2( player_left_sx, player_bottom_sy );
		Vector2 player_left_top_pos = Vector2( player_left_sx, player_moved_top_sy );

		Vector2 ground_left_bottom_pos = Vector2( ground->pos.x, ground->pos.y + ground->actual_size.y );
		Vector2 ground_right_bottom_pos = Vector2( ground->pos.x + ground->actual_size.x, ground->pos.y + ground->actual_size.y );

		left_x_line_collide = isCrossingOtherLineSegment( player_left_bottom_pos, player_left_top_pos,
														  ground_left_bottom_pos, ground_right_bottom_pos, false );


		//player_right_line with ground_bottom_line
		Vector2 player_right_bottom_pos = Vector2( player_right_sx, player_bottom_sy );
		Vector2 player_right_top_pos = Vector2( player_right_sx, player_moved_top_sy );

		right_x_line_collide = isCrossingOtherLineSegment( player_right_bottom_pos, player_right_top_pos,
														   ground_left_bottom_pos, ground_right_bottom_pos, false );

		collide_from_down = left_x_line_collide || right_x_line_collide;
	}
	return collide_from_down;
}

void Collider::collidePlayerWithOtherPlayers( std::vector<PlayerPtr> players ) {
	for ( int i = 0; i < players.size( ); i++ ) {
		PlayerPtr src_player = players[ i ];
		for ( int k = 0; k < players.size( ); k++ ) {
			//for ( int k = players.size( ) - 1; k >= 0; k-- ) {
			if ( i == k ) {
				continue;
			}
			PlayerPtr dst_player = players[ k ];
			if ( src_player->getReason( ) == Player::DEAD_REASON::ENEMY ) {
				continue;
			}
			collidePlayerWithOtherPlayer( src_player, dst_player );
		}
	}
}

void Collider::collidePlayerWithOtherPlayer( PlayerPtr src_player, PlayerPtr dst_player ) {
	LogPtr log = Task::get< Log >( );

	_dir = COLLIDE_DIR::NONE;
	_collide_up = false;
	_collide_down = false;
	_collide_left = false;
	_collide_right = false;

	double src_player_left_sx = src_player->getPos( ).x;
	double src_player_right_sx = src_player->getPos( ).x + src_player->getSize( ).x;
	double src_player_top_sy = src_player->getPos( ).y;
	double src_player_bottom_sy = src_player->getPos( ).y + src_player->getSize( ).y;

	double dst_player_left_sx = dst_player->getPos( ).x;
	double dst_player_right_sx = dst_player->getPos( ).x + dst_player->getSize( ).x;
	double dst_player_top_sy = dst_player->getPos( ).y;
	double dst_player_bottom_sy = dst_player->getPos( ).y + dst_player->getSize( ).y;

	//上下
	{
		if ( collidePlayerWithOtherPlayerFromUp( src_player, dst_player ) ) {
			//log->status( "p_hit_up" );
			_dir = COLLIDE_DIR::UP;
			_collide_up = true;
		}
		if ( collidePlayerWithOtherPlayerFromDown( src_player, dst_player ) ) {
			//log->status( "p_hit_down" );
			_dir = COLLIDE_DIR::DOWN;
			_collide_down = true;
		}

		if ( _collide_up && _collide_down ) {
			//vecが貫通しているとき
			//log->status( "p_hit_up_down" );
			double sp_bottom_to_dp_top = dst_player_top_sy - src_player_bottom_sy;
			double sp_bottom_to_dp_bottom = dst_player_bottom_sy - src_player_bottom_sy;
			if ( sp_bottom_to_dp_top < sp_bottom_to_dp_bottom ) {
				_dir = COLLIDE_DIR::UP;
				_collide_down = false;
			} else {
				_dir = COLLIDE_DIR::DOWN;
				_collide_up = false;
			}
		}
	}

	//左右
	{
		if ( collidePlayerWithOtherPlayerFromLeft( src_player, dst_player ) ) {
			//log->status( "p_hit_left" );
			_dir = COLLIDE_DIR::LEFT;
			_collide_left = true;
		}
		if ( collidePlayerWithOtherPlayerFromRight( src_player, dst_player ) ) {
			//log->status( "p_hit_right" );
			_dir = COLLIDE_DIR::RIGHT;
			_collide_right = true;
		}

		if ( _collide_left && _collide_right ) {
			//vecが貫通しているとき
			//log->status( "p_hit_left_right" );
			double sp_left_to_dp_left = dst_player_left_sx - src_player_left_sx;
			double sp_left_to_dp_right = dst_player_right_sx - src_player_left_sx;
			if ( sp_left_to_dp_left < sp_left_to_dp_right ) {
				_dir = COLLIDE_DIR::LEFT;
				_collide_right = false;
			} else {
				_dir = COLLIDE_DIR::RIGHT;
				_collide_left = false;
			}
		}
	}

	//角に当たったとき
	if ( ( _collide_up || _collide_down ) && ( _collide_left || _collide_right ) ) {
		collideCornerWithOtherPlayer( src_player, dst_player );
	}

	double adjust_vec_x = src_player->getVec( ).x;
	double adjust_vec_y = src_player->getVec( ).y;
	switch ( _dir ) {
	case Collider::COLLIDE_DIR::LEFT:
		adjust_vec_x = dst_player_left_sx - src_player_right_sx - 1;
		//
		//
		src_player->setMoving( false );
		if ( adjust_vec_x < 0.0 ) {
			adjust_vec_x = 0.0;
		}
		break;
	case Collider::COLLIDE_DIR::RIGHT:
		adjust_vec_x = dst_player_right_sx - src_player_left_sx + 1;
		//
		//
		src_player->setMoving( false );
		if ( adjust_vec_x > 0.0 ) {
			adjust_vec_x = 0.0;
		}
		break;
	case Collider::COLLIDE_DIR::UP:
		adjust_vec_y = dst_player_top_sy - src_player_bottom_sy - 1;
		if ( adjust_vec_y < 0.0 ) {
			adjust_vec_y = 0.0;
		}
		src_player->setStanding( true );
		//バウンド
		//src_player->setGravityCount( ) = 40 - abs( ( src_player->getPos( ).y + src_player->getSize( ).y - src_player->getMaximumPosY( ) ) / 20 );
		//落下死
		if ( src_player->getPos( ).y > src_player->getMaximumPosY( ) + DEAD_DOWNWARD_HEIGHT ) {
			src_player->setDead( true );
			collidePlayerWhenDie( src_player );
		}

		if ( dst_player->getReason( ) == Player::DEAD_REASON::ACID && src_player->getReason( ) != Player::DEAD_REASON::ACID ) {
			dst_player->setCollapse( );
		}
		break;
	case Collider::COLLIDE_DIR::DOWN:
		adjust_vec_y = dst_player_bottom_sy - src_player_top_sy + 1;
		if ( adjust_vec_y > 0.0 ) {
			adjust_vec_y = 0.0;
		}
		break;
	}

	if ( adjust_vec_y > 0 ) {
		//src_player->setStanding( false );
	}

	src_player->setVec( ) = Vector2( adjust_vec_x, adjust_vec_y );
}

void Collider::collideCornerWithOtherPlayer( PlayerPtr src_player, PlayerPtr dst_player ) {
	double src_player_top_sy = src_player->getPos( ).y;
	double src_player_bottom_sy = src_player->getPos( ).y + src_player->getSize( ).y;

	double dst_player_top_sy = dst_player->getPos( ).y;
	double dst_player_bottom_sy = dst_player->getPos( ).y + dst_player->getSize( ).y;

	if ( _collide_up && _collide_left ) {
		if ( src_player_bottom_sy > dst_player_top_sy ) {
			_dir = COLLIDE_DIR::LEFT;
		} else {
			_dir = COLLIDE_DIR::UP;
		}
	}
	if ( _collide_up && _collide_right ) {
		if ( src_player_bottom_sy > dst_player_top_sy ) {
			_dir = COLLIDE_DIR::RIGHT;
		} else {
			_dir = COLLIDE_DIR::UP;
		}

	}
	if ( _collide_down && _collide_left ) {
		if ( src_player_top_sy < dst_player_bottom_sy ) {
			_dir = COLLIDE_DIR::LEFT;
		} else {
			_dir = COLLIDE_DIR::DOWN;
		}
	}
	if ( _collide_down && _collide_right ) {
		if ( src_player_top_sy < dst_player_bottom_sy ) {
			_dir = COLLIDE_DIR::RIGHT;
		} else {
			_dir = COLLIDE_DIR::DOWN;
		}
	}
}

bool Collider::collidePlayerWithOtherPlayerFromLeft( PlayerPtr src_player, PlayerPtr dst_player ) {
	bool collide_from_left = false;
	double src_player_left_sx = src_player->getPos( ).x;

	double src_player_moved_right_sx = src_player->getPos( ).x + src_player->getSize( ).x + src_player->getVec( ).x;
	double src_player_top_sy = src_player->getPos( ).y + src_player->getVec( ).y;
	if ( src_player->getVec( ).y > 0 ) {
		src_player_top_sy = src_player->getPos( ).y;
	}
	double src_player_bottom_sy = src_player->getPos( ).y + src_player->getSize( ).y + src_player->getVec( ).y;
	if ( src_player->getVec( ).y < 0 ) {
		src_player_bottom_sy = src_player->getPos( ).y + src_player->getSize( ).y;
	}

	double dst_player_left_sx = dst_player->getPos( ).x;
	double dst_player_top_sy = dst_player->getPos( ).y;
	double dst_player_moved_bottom_sy = dst_player->getPos( ).y + dst_player->getSize( ).y + dst_player->getVec( ).y;

	if ( src_player_top_sy < dst_player_top_sy && src_player_bottom_sy > dst_player_moved_bottom_sy ) {
		//src_player_height > _dst_player_height
		if ( src_player_moved_right_sx >= dst_player_left_sx && src_player_left_sx < dst_player_left_sx ) {
			collide_from_left = true;
		} else {
			collide_from_left = false;
		}
	} else {
		bool top_y_line_collide = false;
		bool bottom_y_line_collide = false;

		//src_player_top_line with dst_player_left_line
		Vector2 src_player_left_top_pos = Vector2( src_player_left_sx, src_player_top_sy );
		Vector2 src_player_right_top_pos = Vector2( src_player_moved_right_sx, src_player_top_sy );

		Vector2 dst_player_left_top_pos = Vector2( dst_player_left_sx, dst_player_top_sy );
		Vector2 dst_player_left_bottom_pos = Vector2( dst_player_left_sx, dst_player_moved_bottom_sy );

		top_y_line_collide = isCrossingOtherLineSegment( src_player_left_top_pos, src_player_right_top_pos,
														 dst_player_left_top_pos, dst_player_left_bottom_pos, true );


		//src_player_bottom_line with dst_player_left_line
		Vector2 src_player_left_bottom_pos = Vector2( src_player_left_sx, src_player_bottom_sy );
		Vector2 src_player_right_bottom_pos = Vector2( src_player_moved_right_sx, src_player_bottom_sy );

		bottom_y_line_collide = isCrossingOtherLineSegment( src_player_left_bottom_pos, src_player_right_bottom_pos,
															dst_player_left_top_pos, dst_player_left_bottom_pos, true );

		collide_from_left = top_y_line_collide || bottom_y_line_collide;
	}
	return collide_from_left;
}

bool Collider::collidePlayerWithOtherPlayerFromRight( PlayerPtr src_player, PlayerPtr dst_player ) {
	bool collide_from_right = false;
	double src_player_right_sx = src_player->getPos( ).x + src_player->getSize( ).x;;

	double src_player_moved_left_sx = src_player->getPos( ).x + src_player->getVec( ).x;
	double src_player_top_sy = src_player->getPos( ).y + src_player->getVec( ).y;
	if ( src_player->getVec( ).y > 0 ) {
		src_player_top_sy = src_player->getPos( ).y;
	}
	double src_player_bottom_sy = src_player->getPos( ).y + src_player->getSize( ).y + src_player->getVec( ).y;
	if ( src_player->getVec( ).y < 0 ) {
		src_player_bottom_sy = src_player->getPos( ).y + src_player->getSize( ).y;
	}

	double dst_player_right_sx = dst_player->getPos( ).x + dst_player->getSize( ).x;
	double dst_player_top_sy = dst_player->getPos( ).y;
	double dst_player_moved_bottom_sy = dst_player->getPos( ).y + dst_player->getSize( ).y + dst_player->getVec( ).y;

	if ( src_player_top_sy < dst_player_top_sy && src_player_bottom_sy > dst_player_moved_bottom_sy ) {
		//src_player_height > dst_player_height
		if ( src_player_moved_left_sx <= dst_player_right_sx && src_player_right_sx > dst_player_right_sx ) {
			collide_from_right = true;
		} else {
			collide_from_right = false;
		}
	} else {
		bool top_y_line_collide = false;
		bool bottom_y_line_collide = false;

		//src_player_top_line with dst_player_right_line
		Vector2 src_player_right_top_pos = Vector2( src_player_right_sx, src_player_top_sy );
		Vector2 src_player_left_top_pos = Vector2( src_player_moved_left_sx, src_player_top_sy );

		Vector2 dst_player_right_top_pos = Vector2( dst_player_right_sx, dst_player_top_sy );
		Vector2 dst_player_right_bottom_pos = Vector2( dst_player_right_sx, dst_player_moved_bottom_sy );

		top_y_line_collide = isCrossingOtherLineSegment( src_player_right_top_pos, src_player_left_top_pos,
														 dst_player_right_top_pos, dst_player_right_bottom_pos, true );


		//src_player_bottom_line with dst_player_right_line
		Vector2 src_player_right_bottom_pos = Vector2( src_player_right_sx, src_player_bottom_sy );
		Vector2 src_player_left_bottom_pos = Vector2( src_player_moved_left_sx, src_player_bottom_sy );

		bottom_y_line_collide = isCrossingOtherLineSegment( src_player_right_bottom_pos, src_player_left_bottom_pos,
															dst_player_right_top_pos, dst_player_right_bottom_pos, true );

		collide_from_right = top_y_line_collide || bottom_y_line_collide;
	}
	return collide_from_right;
}

bool Collider::collidePlayerWithOtherPlayerFromUp( PlayerPtr src_player, PlayerPtr dst_player ) {
	bool collide_from_up = false;

	double src_player_top_sy = src_player->getPos( ).y;

	double src_player_left_sx = src_player->getPos( ).x + src_player->getVec( ).x;
	if ( src_player->getVec( ).x > 0 ) {
		src_player_left_sx = src_player->getPos( ).x;
	}
	double src_player_right_sx = src_player->getPos( ).x + src_player->getSize( ).x + src_player->getVec( ).x;
	if ( src_player->getVec( ).x < 0 ) {
		src_player_right_sx = src_player->getPos( ).x + src_player->getSize( ).x;
	}
	double src_player_moved_bottom_sy = src_player->getPos( ).y + src_player->getSize( ).y + src_player->getVec( ).y;

	double dst_player_left_sx = dst_player->getPos( ).x;
	double dst_player_right_sx = dst_player->getPos( ).x + dst_player->getSize( ).x;
	double dst_player_top_sy = dst_player->getPos( ).y;

	if ( src_player_left_sx < dst_player_left_sx && src_player_right_sx > dst_player_right_sx ) {
		//src_player_width > dst_player_width
		if ( src_player_moved_bottom_sy >= dst_player_top_sy && src_player_top_sy < dst_player_top_sy ) {
			collide_from_up = true;
		} else {
			collide_from_up = false;
		}
	} else {
		bool left_x_line_collide = false;
		bool right_x_line_collide = false;

		//src_player_left_line with dst_player_top_line
		Vector2 src_player_left_top_pos = Vector2( src_player_left_sx, src_player_top_sy );
		Vector2 src_player_left_bottom_pos = Vector2( src_player_left_sx, src_player_moved_bottom_sy );

		Vector2 dst_player_left_top_pos = Vector2( dst_player_left_sx, dst_player_top_sy );
		Vector2 dst_player_right_top_pos = Vector2( dst_player_right_sx, dst_player_top_sy );

		left_x_line_collide = isCrossingOtherLineSegment( src_player_left_top_pos, src_player_left_bottom_pos,
														  dst_player_left_top_pos, dst_player_right_top_pos, true );


		//src_player_right_line with dst_player_top_line
		Vector2 src_player_right_top_pos = Vector2( src_player_right_sx, src_player_top_sy );
		Vector2 src_player_right_bottom_pos = Vector2( src_player_right_sx, src_player_moved_bottom_sy );

		right_x_line_collide = isCrossingOtherLineSegment( src_player_right_top_pos, src_player_right_bottom_pos,
														   dst_player_left_top_pos, dst_player_right_top_pos, true );

		collide_from_up = left_x_line_collide || right_x_line_collide;
	}

	return collide_from_up;
}

bool Collider::collidePlayerWithOtherPlayerFromDown( PlayerPtr src_player, PlayerPtr dst_player ) {
	bool collide_from_down = false;

	double src_player_bottom_sy = src_player->getPos( ).y + src_player->getSize( ).y;

	double src_player_left_sx = src_player->getPos( ).x + src_player->getVec( ).x;
	if ( src_player->getVec( ).x > 0 ) {
		src_player_left_sx = src_player->getPos( ).x;
	}
	double src_player_right_sx = src_player->getPos( ).x + src_player->getSize( ).x + src_player->getVec( ).x;
	if ( src_player->getVec( ).x < 0 ) {
		src_player_right_sx = src_player->getPos( ).x + src_player->getSize( ).x;
	}
	double src_player_moved_top_sy = src_player->getPos( ).y + src_player->getVec( ).y;

	double dst_player_left_sx = dst_player->getPos( ).x;
	double dst_player_right_sx = dst_player->getPos( ).x + dst_player->getSize( ).x;
	double dst_player_bottom_sy = dst_player->getPos( ).y + dst_player->getSize( ).y;

	if ( src_player_left_sx < dst_player_left_sx && src_player_right_sx > dst_player_right_sx ) {
		//src_player_width > dst_player_width
		if ( src_player_moved_top_sy <= dst_player_bottom_sy && src_player_bottom_sy > dst_player_bottom_sy ) {
			collide_from_down = true;
		} else {
			collide_from_down = false;
		}
	} else {
		bool left_x_line_collide = false;
		bool right_x_line_collide = false;

		//src_player_left_line with dst_player_bottom_line
		Vector2 src_player_left_bottom_pos = Vector2( src_player_left_sx, src_player_bottom_sy );
		Vector2 src_player_left_top_pos = Vector2( src_player_left_sx, src_player_moved_top_sy );

		Vector2 dst_player_left_bottom_pos = Vector2( dst_player_left_sx, dst_player_bottom_sy );
		Vector2 dst_player_right_bottom_pos = Vector2( dst_player_right_sx, dst_player_bottom_sy );

		left_x_line_collide = isCrossingOtherLineSegment( src_player_left_bottom_pos, src_player_left_top_pos,
														  dst_player_left_bottom_pos, dst_player_right_bottom_pos, true );


		//src_player_right_line with dst_player_bottom_line
		Vector2 src_player_right_bottom_pos = Vector2( src_player_right_sx, src_player_bottom_sy );
		Vector2 src_player_right_top_pos = Vector2( src_player_right_sx, src_player_moved_top_sy );

		right_x_line_collide = isCrossingOtherLineSegment( src_player_right_bottom_pos, src_player_right_top_pos,
														   dst_player_left_bottom_pos, dst_player_right_bottom_pos, true );

		collide_from_down = left_x_line_collide || right_x_line_collide;
	}

	return collide_from_down;
}

void Collider::collidePlayerWithGimmicks( std::vector< PlayerPtr > players ) {
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		GimmickPtr gimmick = gimmicks[ i ];
		for ( int j = 0; j < players.size( ); j++ ) {
			PlayerPtr src_player = players[ j ];
			if ( src_player->getReason( ) == Player::DEAD_REASON::ENEMY ) {
				continue;
			}
			collidePlayerWithGimmickSortType( src_player, gimmick );
		}
	}
}

void Collider::collidePlayerWithGimmickSortType( PlayerPtr player, GimmickPtr gimmick ) {
	switch ( gimmick->getType( ) ) {
	case Gimmick::TYPE::ACID:
		collidePlayerWithAcid( player, gimmick );
		break;
	case Gimmick::TYPE::GOAL:
		collidePlayerWithGoal( player, gimmick );
		break;
	case Gimmick::TYPE::RESPAWN:
		collidePlayerWithRespawn( player, gimmick );
		break;
	case Gimmick::TYPE::FAN:
		collidePlayerWithGimmick( player, gimmick );
		break;
	case Gimmick::TYPE::NEEDLE:
		collidePlayerWithGimmick( player, gimmick );
		break;
	case Gimmick::TYPE::DECELERATE_NET:
		collidePlayerWithDecelerateNet( player, gimmick );
		break;
	case Gimmick::TYPE::STICKY_NET:
		collidePlayerWithStickyNet( player, gimmick );
		break;
	}
}

void Collider::collidePlayerWithAcid( PlayerPtr player, GimmickPtr acid ) {
	LogPtr log = Task::get< Log >( );
	if ( ( collidePlayerWithGimmickFromLeft( player, acid ) ||
		 collidePlayerWithGimmickFromRight( player, acid ) ||
		 collidePlayerWithGimmickFromUp( player, acid ) ||
		 collidePlayerWithGimmickFromDown( player, acid ) ||
		 collidePlayerWithGimmickInside( player, acid ) ) ) {

		if ( player->getReason( ) == Player::DEAD_REASON::NONE ||
			 player->getReason( ) == Player::DEAD_REASON::ACID ) {
			log->status( "acid_hit" );
			player->setDead( false );
			player->setDeadReason( Player::DEAD_REASON::ACID );
			collidePlayerWhenDie( player );
			player->addSoakedCount( );
			player->setGravityCount( ) = 30;
			//浮上処理
			if ( player->getNowSoakedCount( ) > 5 && !player->isCollapse( ) ) {
				double float_vec_y = ( double )player->getNowSoakedCount( ) / 3;
				if ( acid->getPos( ).y - player->getSize( ).y / 2 > player->getPos( ).y - float_vec_y ) {
					float_vec_y = player->getPos( ).y - ( acid->getPos( ).y - player->getSize( ).y / 2 );
				}
				player->setVec( ) -= Vector2( 0, float_vec_y );
			}
		}

		if ( player->getReason( ) == Player::DEAD_REASON::SUICIDE &&
			 collidePlayerWithGimmickInside( player, acid ) ) {
			player->setDelete( );
			player->setDeadReason( Player::DEAD_REASON::ACID );
		}
	}
}

void Collider::collidePlayerWithDecelerateNet( PlayerPtr player, GimmickPtr decelerate_net ) {
	LogPtr log = Task::get< Log >( );
	if ( collidePlayerWithGimmickFromLeft( player, decelerate_net ) ||
		 collidePlayerWithGimmickFromRight( player, decelerate_net ) ||
		 collidePlayerWithGimmickFromUp( player, decelerate_net ) ||
		 collidePlayerWithGimmickFromDown( player, decelerate_net ) ||
		 collidePlayerWithGimmickInside( player, decelerate_net ) ) {
		log->status( "net_hit" );
		decelerate_net->setDelete( );
		player->speedDeceleration( );
	}
}

void Collider::collidePlayerWithStickyNet( PlayerPtr player, GimmickPtr stickey_net ) {
	LogPtr log = Task::get< Log >( );
	if ( collidePlayerWithGimmickFromLeft( player, stickey_net ) ||
		 collidePlayerWithGimmickFromRight( player, stickey_net ) ||
		 collidePlayerWithGimmickFromUp( player, stickey_net ) ||
		 collidePlayerWithGimmickFromDown( player, stickey_net ) ||
		 collidePlayerWithGimmickInside( player, stickey_net ) ) {
		log->status( "net_hit" );
		player->setDead( true );
		stickey_net->setDelete( );
		player->setDeadReason( Player::DEAD_REASON::STICKY_NET );
	}
}



void Collider::collidePlayerWithFan( PlayerPtr player, GimmickPtr fan ) {
	double player_left_sx = player->getPos( ).x;
	double player_right_sx = player->getPos( ).x + player->getSize( ).x;
	double player_top_sy = player->getPos( ).y;
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	double fan_left_sx = fan->getPos( ).x;
	double fan_right_sx = fan->getPos( ).x + fan->getSize( ).x;
	double fan_top_sy = fan->getPos( ).y;
	double fan_bottom_sy = fan->getPos( ).y + fan->getSize( ).y;

	double adjust_vec_x = player->getVec( ).x;
	double adjust_vec_y = player->getVec( ).y;
	switch ( _dir ) {
	case Collider::COLLIDE_DIR::LEFT:
		adjust_vec_x = fan_left_sx - player_right_sx - 1;
		if ( adjust_vec_x < 0.0 ) {
			adjust_vec_x = 0.0;
		}
		break;
	case Collider::COLLIDE_DIR::RIGHT:
		adjust_vec_x = fan_right_sx - player_left_sx + 1;
		if ( adjust_vec_x > 0.0 ) {
			adjust_vec_x = 0.0;
		}
		break;
	case Collider::COLLIDE_DIR::UP:
		adjust_vec_y = fan_top_sy - player_bottom_sy - 1;
		if ( adjust_vec_y < 0.0 ) {
			adjust_vec_y = 0.0;
		}
		player->setStanding( true );
		//落下死
		if ( player->getPos( ).y > player->getMaximumPosY( ) + DEAD_DOWNWARD_HEIGHT ) {
			player->setDead( true );
			collidePlayerWhenDie( player );
		}
		break;
	case Collider::COLLIDE_DIR::DOWN:
		adjust_vec_y = fan_bottom_sy - player_top_sy + 1;
		if ( adjust_vec_y > 0.0 ) {
			adjust_vec_y = 0.0;
		}
		break;
	}

	if ( adjust_vec_y > 0 ) {
		player->setStanding( false );
	}

	if ( _dir != COLLIDE_DIR::NONE ) {
		player->setVec( ) = Vector2( adjust_vec_x, adjust_vec_y );
	}
}

void Collider::setupWind( FanPtr fan ) {
	_winds[ fan ] = new Wind( );
	switch ( fan->getDir( ) ) {
	case Gimmick::DIR::LEFT:
		_winds[ fan ]->edge1_start_pos = fan->getPos( );
		_winds[ fan ]->edge2_start_pos = fan->getPos( ) + Vector2( 0, fan->getSize( ).y );
		_winds[ fan ]->center_start_pos = fan->getPos( ) + Vector2( 0, fan->getSize( ).y / 2 );
		break;
	case Gimmick::DIR::RIGHT:
		_winds[ fan ]->edge1_start_pos = fan->getPos( ) + Vector2( fan->getSize( ).x, 0 );
		_winds[ fan ]->edge2_start_pos = fan->getPos( ) + Vector2( fan->getSize( ).x, fan->getSize( ).y );
		_winds[ fan ]->center_start_pos = fan->getPos( ) + Vector2( fan->getSize( ).x, fan->getSize( ).y / 2 );
		break;
	case Gimmick::DIR::UP:
		_winds[ fan ]->edge1_start_pos = fan->getPos( );
		_winds[ fan ]->edge2_start_pos = fan->getPos( ) + Vector2( fan->getSize( ).x, 0 );
		_winds[ fan ]->center_start_pos = fan->getPos( ) + Vector2( fan->getSize( ).x / 2, 0 );
		break;
	case Gimmick::DIR::DOWN:
		_winds[ fan ]->edge1_start_pos = fan->getPos( ) + Vector2( 0, fan->getSize( ).y );
		_winds[ fan ]->edge2_start_pos = fan->getPos( ) + Vector2( fan->getSize( ).x, fan->getSize( ).y );
		_winds[ fan ]->center_start_pos = fan->getPos( ) + Vector2( fan->getSize( ).x / 2, fan->getSize( ).y );
		break;
	}
	_winds[ fan ]->edge1_end_pos = _winds[ fan ]->edge1_start_pos + fan->getVec( );
	_winds[ fan ]->edge2_end_pos = _winds[ fan ]->edge2_start_pos + fan->getVec( );
	_winds[ fan ]->center_end_pos = _winds[ fan ]->center_start_pos + fan->getVec( );

	_winds[ fan ]->edge1_stop_pos = _winds[ fan ]->edge1_end_pos;
	_winds[ fan ]->edge2_stop_pos = _winds[ fan ]->edge2_end_pos;
	_winds[ fan ]->center_stop_pos = _winds[ fan ]->center_end_pos;


	interceptWindWithGrounds( fan->getDir( ), _winds[ fan ]->edge1_start_pos, _winds[ fan ]->edge1_stop_pos );
	interceptWindWithGrounds( fan->getDir( ), _winds[ fan ]->edge2_start_pos, _winds[ fan ]->edge2_stop_pos );
	interceptWindWithGrounds( fan->getDir( ), _winds[ fan ]->center_start_pos, _winds[ fan ]->center_stop_pos );

	interceptWindWithGimmicks( fan->getDir( ), _winds[ fan ]->edge1_start_pos, _winds[ fan ]->edge1_stop_pos );
	interceptWindWithGimmicks( fan->getDir( ), _winds[ fan ]->edge2_start_pos, _winds[ fan ]->edge2_stop_pos );
	interceptWindWithGimmicks( fan->getDir( ), _winds[ fan ]->center_start_pos, _winds[ fan ]->center_stop_pos );

}

void Collider::collideWithWinds( std::vector<PlayerPtr> players ) {
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::FAN ) {
			for ( int j = 0; j < players.size( ); j++ ) {
				PlayerPtr src_player = players[ j ];
				collideWithWind( src_player, std::dynamic_pointer_cast< Fan >( gimmicks[ i ] ) );
			}
		}
	}
}

void Collider::collideWithWind( PlayerPtr player, FanPtr fan ) {
	if ( !fan->isOn( ) ) {
		return;
	}

	Vector2 player_start_pos;
	Vector2 player_end_pos;
	switch ( fan->getDir( ) ) {
	case Gimmick::DIR::LEFT:
		player_start_pos = player->getPos( ) + Vector2( player->getSize( ).x, 0 );
		player_end_pos = player->getPos( ) + player->getSize( );
		break;
	case Gimmick::DIR::RIGHT:
		player_start_pos = player->getPos( );
		player_end_pos = player->getPos( ) + Vector2( 0, player->getSize( ).y );
		break;
	case Gimmick::DIR::UP:
		player_start_pos = player->getPos( ) + Vector2( 0, player->getSize( ).y );
		player_end_pos = player->getPos( ) + player->getSize( );
		break;
	case Gimmick::DIR::DOWN:
		player_start_pos = player->getPos( );
		player_end_pos = player->getPos( ) + Vector2( player->getSize( ).x, 0 );
		break;
	}

	Vector2 adjust_pos;
	int wind_power = 24;

	if ( isCrossingOtherLineSegment( _winds[ fan ]->edge1_start_pos, _winds[ fan ]->edge1_stop_pos, player_start_pos, player_end_pos, true ) ) {
		switch ( fan->getDir( ) ) {
		case Gimmick::DIR::LEFT:
			adjust_pos.x = ( _winds[ fan ]->edge1_end_pos.x - ( player->getPos( ).x + player->getSize( ).x ) ) / wind_power;
			if ( adjust_pos.x < _winds[ fan ]->edge1_stop_pos.x - ( player->getPos( ).x + player->getSize( ).x ) ) {
				adjust_pos.x = _winds[ fan ]->edge1_stop_pos.x - ( player->getPos( ).x + player->getSize( ).x );
			}
			break;
		case Gimmick::DIR::RIGHT:
			adjust_pos.x = ( _winds[ fan ]->edge1_end_pos.x - player->getPos( ).x ) / wind_power;
			if ( adjust_pos.x > _winds[ fan ]->edge1_stop_pos.x - player->getPos( ).x ) {
				adjust_pos.x = _winds[ fan ]->edge1_stop_pos.x - player->getPos( ).x;
			}
			break;
		case Gimmick::DIR::UP:
			adjust_pos.y = ( _winds[ fan ]->edge1_end_pos.y - ( player->getPos( ).y + player->getSize( ).y ) ) / wind_power;
			if ( adjust_pos.y > 0 ) {
				adjust_pos.y = 0;
			}
			if ( adjust_pos.y < _winds[ fan ]->edge1_stop_pos.y - ( player->getPos( ).y + player->getSize( ).y ) ) {
				adjust_pos.y = _winds[ fan ]->edge1_stop_pos.y - ( player->getPos( ).y + player->getSize( ).y );
			}
			break;
		case Gimmick::DIR::DOWN:
			adjust_pos.y = ( _winds[ fan ]->edge1_end_pos.y - player->getPos( ).y ) / wind_power;
			if ( adjust_pos.y < 0 ) {
				adjust_pos.y = 0;
			}
			if ( adjust_pos.y > _winds[ fan ]->edge1_stop_pos.y - player->getPos( ).y ) {
				adjust_pos.y = _winds[ fan ]->edge1_stop_pos.y - player->getPos( ).y;
			}
			break;
		}
	}

	if ( isCrossingOtherLineSegment( _winds[ fan ]->edge2_start_pos, _winds[ fan ]->edge2_stop_pos, player_start_pos, player_end_pos, true ) ) {
		switch ( fan->getDir( ) ) {
		case Gimmick::DIR::LEFT:
			adjust_pos.x = ( _winds[ fan ]->edge2_end_pos.x - ( player->getPos( ).x + player->getSize( ).x ) ) / wind_power;
			if ( adjust_pos.x < _winds[ fan ]->edge2_stop_pos.x - ( player->getPos( ).x + player->getSize( ).x ) ) {
				adjust_pos.x = _winds[ fan ]->edge2_stop_pos.x - ( player->getPos( ).x + player->getSize( ).x );
			}
			break;
		case Gimmick::DIR::RIGHT:
			adjust_pos.x = ( _winds[ fan ]->edge2_end_pos.x - player->getPos( ).x ) / wind_power;
			if ( adjust_pos.x > _winds[ fan ]->edge2_stop_pos.x - player->getPos( ).x ) {
				adjust_pos.x = _winds[ fan ]->edge2_stop_pos.x - player->getPos( ).x;
			}
			break;
		case Gimmick::DIR::UP:
			adjust_pos.y = ( _winds[ fan ]->edge2_end_pos.y - ( player->getPos( ).y + player->getSize( ).y ) ) / wind_power;
			if ( adjust_pos.y > 0 ) {
				adjust_pos.y = 0;
			}
			if ( adjust_pos.y < _winds[ fan ]->edge2_stop_pos.y - ( player->getPos( ).y + player->getSize( ).y ) ) {
				adjust_pos.y = _winds[ fan ]->edge2_stop_pos.y - ( player->getPos( ).y + player->getSize( ).y );
			}
			break;
		case Gimmick::DIR::DOWN:
			adjust_pos.y = ( _winds[ fan ]->edge2_end_pos.y - player->getPos( ).y ) / wind_power;
			if ( adjust_pos.y < 0 ) {
				adjust_pos.y = 0;
			}
			if ( adjust_pos.y > _winds[ fan ]->edge2_stop_pos.y - player->getPos( ).y ) {
				adjust_pos.y = _winds[ fan ]->edge2_stop_pos.y - player->getPos( ).y;
			}
			break;
		}
	}

	if ( isCrossingOtherLineSegment( _winds[ fan ]->center_start_pos, _winds[ fan ]->center_stop_pos, player_start_pos, player_end_pos, true ) ) {
		switch ( fan->getDir( ) ) {
		case Gimmick::DIR::LEFT:
			adjust_pos.x = ( _winds[ fan ]->center_end_pos.x - ( player->getPos( ).x + player->getSize( ).x ) ) / wind_power;
			if ( adjust_pos.x < _winds[ fan ]->center_stop_pos.x - ( player->getPos( ).x + player->getSize( ).x ) ) {
				adjust_pos.x = _winds[ fan ]->center_stop_pos.x - ( player->getPos( ).x + player->getSize( ).x );
			}
			break;
		case Gimmick::DIR::RIGHT:
			adjust_pos.x = ( _winds[ fan ]->center_end_pos.x - player->getPos( ).x ) / wind_power;
			if ( adjust_pos.x > _winds[ fan ]->center_stop_pos.x - player->getPos( ).x ) {
				adjust_pos.x = _winds[ fan ]->center_stop_pos.x - player->getPos( ).x;
			}
			break;
		case Gimmick::DIR::UP:
			adjust_pos.y = ( _winds[ fan ]->center_end_pos.y - ( player->getPos( ).y + player->getSize( ).y ) ) / wind_power;
			if ( adjust_pos.y > 0 ) {
				adjust_pos.y = 0;
			}
			if ( adjust_pos.y < _winds[ fan ]->center_stop_pos.y - ( player->getPos( ).y + player->getSize( ).y ) ) {
				adjust_pos.y = _winds[ fan ]->center_stop_pos.y - ( player->getPos( ).y + player->getSize( ).y );
			}
			break;
		case Gimmick::DIR::DOWN:
			adjust_pos.y = ( _winds[ fan ]->center_end_pos.y - player->getPos( ).y ) / wind_power;
			if ( adjust_pos.y < 0 ) {
				adjust_pos.y = 0;
			}
			if ( adjust_pos.y > _winds[ fan ]->center_stop_pos.y - player->getPos( ).y ) {
				adjust_pos.y = _winds[ fan ]->center_stop_pos.y - player->getPos( ).y;
			}
			break;
		}
	}

	if ( adjust_pos.x != 0 || adjust_pos.y != 0 ) {
		if ( adjust_pos.y < 0 ) {
			player->setStanding( false );
		}
		player->setGravityCount( ) = 30;
		player->setVec( ) += adjust_pos;
	}
}

void Collider::interceptWindWithGrounds( Gimmick::DIR dir, Vector2 start_pos, Vector2& end_pos ) {
	std::vector< MapData::Ground* > grounds = _map_data->getGrounds( );
	for ( int i = 0; i < grounds.size( ); i++ ) {
		MapData::Ground* ground = grounds[ i ];
		Vector2 ground_start_pos = Vector2( );
		Vector2 ground_end_pos = Vector2( );
		switch ( dir ) {
		case Gimmick::DIR::LEFT:
			ground_start_pos = ground->pos + Vector2( ground->actual_size.x, 0 );
			ground_end_pos = ground->pos + Vector2( ground->actual_size.x, ground->actual_size.y );
			break;
		case Gimmick::DIR::RIGHT:
			ground_start_pos = ground->pos;
			ground_end_pos = ground->pos + Vector2( 0, ground->actual_size.y );
			break;
		case Gimmick::DIR::UP:
			ground_start_pos = ground->pos + Vector2( 0, ground->actual_size.y );
			ground_end_pos = ground->pos + Vector2( ground->actual_size.x, ground->actual_size.y );
			break;
		case Gimmick::DIR::DOWN:
			ground_start_pos = ground->pos;
			ground_end_pos = ground->pos + Vector2( ground->actual_size.x, 0 );
			break;
		}
		if ( isCrossingOtherLineSegment( start_pos, end_pos, ground_start_pos, ground_end_pos, true ) ) {
			switch ( dir ) {
			case Gimmick::DIR::LEFT:
			case Gimmick::DIR::RIGHT:
				end_pos = Vector2( ground_start_pos.x, end_pos.y );
				break;
			case Gimmick::DIR::UP:
			case Gimmick::DIR::DOWN:
				end_pos = Vector2( end_pos.x, ground_start_pos.y );
				break;
			}
		}
	}
}

void Collider::interceptWindWithGimmicks( Gimmick::DIR dir, Vector2 start_pos, Vector2& end_pos ) {
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		GimmickPtr gimmick = gimmicks[ i ];
		if ( gimmick->getType( ) != Gimmick::TYPE::FAN ) {
			continue;
		}
		Vector2 gimmick_start_pos = Vector2( );
		Vector2 gimmick_end_pos = Vector2( );
		switch ( dir ) {
		case Gimmick::DIR::LEFT:
			gimmick_start_pos = gimmick->getPos( ) + Vector2( gimmick->getSize( ).x, 0 );
			gimmick_end_pos = gimmick->getPos( ) + Vector2( gimmick->getSize( ).x, gimmick->getSize( ).y );
			break;
		case Gimmick::DIR::RIGHT:
			gimmick_start_pos = gimmick->getPos( );
			gimmick_end_pos = gimmick->getPos( ) + Vector2( 0, gimmick->getSize( ).y );
			break;
		case Gimmick::DIR::UP:
			gimmick_start_pos = gimmick->getPos( ) + Vector2( 0, gimmick->getSize( ).y );
			gimmick_end_pos = gimmick->getPos( ) + Vector2( gimmick->getSize( ).x, gimmick->getSize( ).y );
			break;
		case Gimmick::DIR::DOWN:
			gimmick_start_pos = gimmick->getPos( );
			gimmick_end_pos = gimmick->getPos( ) + Vector2( gimmick->getSize( ).x, 0 );
			break;
		}
		if ( isCrossingOtherLineSegment( start_pos, end_pos, gimmick_start_pos, gimmick_end_pos, true ) ) {
			switch ( dir ) {
			case Gimmick::DIR::LEFT:
			case Gimmick::DIR::RIGHT:
				end_pos = Vector2( gimmick_start_pos.x, end_pos.y );
				break;
			case Gimmick::DIR::UP:
			case Gimmick::DIR::DOWN:
				end_pos = Vector2( end_pos.x, gimmick_start_pos.y );
				break;
			}
		}
	}
}

void Collider::collidePlayerWithNeedle( PlayerPtr player, GimmickPtr needle ) {
	double player_left_sx = player->getPos( ).x;
	double player_right_sx = player->getPos( ).x + player->getSize( ).x;
	double player_top_sy = player->getPos( ).y;
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	double needle_left_sx = needle->getPos( ).x + NEEDLE_COLLIDE_AJDUST;
	double needle_right_sx = needle->getPos( ).x + needle->getSize( ).x - NEEDLE_COLLIDE_AJDUST;
	double needle_top_sy = needle->getPos( ).y + NEEDLE_COLLIDE_AJDUST;
	double needle_bottom_sy = needle->getPos( ).y + needle->getSize( ).y - NEEDLE_COLLIDE_AJDUST;

	double adjust_vec_x = player->getVec( ).x;
	double adjust_vec_y = player->getVec( ).y;
	if ( needle->getDir( ) == convertColliderDir( ) ) {
		if ( player->getState( ) == Player::STATE::ALIVE ) {
			player->setDead( false );
			switch ( _dir ) {
			case Collider::COLLIDE_DIR::LEFT:
				if ( needle_top_sy + 10 >= player_bottom_sy ) {
					player->setPos( ) = Vector2( needle_left_sx - player->getSize( ).x / 2, needle_top_sy - player->getSize( ).y + 10 );
				} else {
					player->setPos( ) = Vector2( needle_left_sx - player->getSize( ).x / 2, player->getPos( ).y );
				}
				break;
			case Collider::COLLIDE_DIR::RIGHT:
				if ( needle_top_sy + 10 >= player_bottom_sy ) {
					player->setPos( ) = Vector2( needle_left_sx + player->getSize( ).x / 2, needle_top_sy - player->getSize( ).y + 10 );
				} else {
					player->setPos( ) = Vector2( needle_left_sx + player->getSize( ).x / 2, player->getPos( ).y );
				}
				break;
			case Collider::COLLIDE_DIR::UP:
				player->setPos( ) = Vector2( player->getPos( ).x, needle_bottom_sy - player->getSize( ).y - 1 );
				break;
			case Collider::COLLIDE_DIR::DOWN:
				player->setPos( ) = Vector2( player->getPos( ).x, needle_top_sy + 1 );
				break;
			}
			player->setDeadReason( Player::DEAD_REASON::NEEDLE );
		}
	} else {
		switch ( _dir ) {
		case Collider::COLLIDE_DIR::LEFT:
			adjust_vec_x = needle_left_sx - player_right_sx - 1;
			/*if ( adjust_vec_x < 0.0 ) {
				adjust_vec_x = 0.0;
			}*/
			break;
		case Collider::COLLIDE_DIR::RIGHT:
			adjust_vec_x = needle_right_sx - player_left_sx + 1;
			/*if ( adjust_vec_x > 0.0 ) {
				adjust_vec_x = 0.0;
			}*/
			break;
		case Collider::COLLIDE_DIR::UP:
			adjust_vec_y = needle_top_sy - player_bottom_sy - 1;
			if ( adjust_vec_y < 0.0 ) {
				adjust_vec_y = 0.0;
			}
			player->setStanding( true );
			//落下死
			if ( player->getPos( ).y > player->getMaximumPosY( ) + DEAD_DOWNWARD_HEIGHT ) {
				player->setDead( true );
				collidePlayerWhenDie( player );
			}
			break;
		case Collider::COLLIDE_DIR::DOWN:
			adjust_vec_y = needle_bottom_sy - player_top_sy + 1;
			/*if ( adjust_vec_y > 0.0 ) {
				adjust_vec_y = 0.0;
			}*/
			break;
		}
	}

	if ( adjust_vec_y > 0 ) {
		player->setStanding( false );
	}
	if ( _dir != COLLIDE_DIR::NONE ) {
		player->setVec( ) = Vector2( adjust_vec_x, adjust_vec_y );
	}
}

void Collider::collidePlayerWithGimmick( PlayerPtr player, GimmickPtr gimmick ) {
	LogPtr log = Task::get< Log >( );

	_dir = COLLIDE_DIR::NONE;

	_collide_up = false;
	_collide_down = false;
	_collide_left = false;
	_collide_right = false;

	double player_left_sx = player->getPos( ).x;
	double player_right_sx = player->getPos( ).x + player->getSize( ).x;
	double player_top_sy = player->getPos( ).y;
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	double gimmick_left_sx = gimmick->getPos( ).x;
	double gimmick_right_sx = gimmick->getPos( ).x + gimmick->getSize( ).x;
	double gimmick_top_sy = gimmick->getPos( ).y;
	double gimmick_bottom_sy = gimmick->getPos( ).y + gimmick->getSize( ).y;

	//上下
	{
		if ( collidePlayerWithGimmickFromUp( player, gimmick ) ) {
			//log->status( "hit_up" );
			_dir = COLLIDE_DIR::UP;
			_collide_up = true;
		}
		if ( collidePlayerWithGimmickFromDown( player, gimmick ) ) {
			//log->status( "hit_down" );
			_dir = COLLIDE_DIR::DOWN;
			_collide_down = true;
		}

		if ( _collide_up && _collide_down ) {
			//vecが貫通しているとき
			//log->status( "hit_up_down" );
			double p_bottom_to_g_top = gimmick_top_sy - player_bottom_sy;
			double p_bottom_to_g_bottom = gimmick_bottom_sy - player_bottom_sy;
			if ( p_bottom_to_g_top < p_bottom_to_g_bottom ) {
				_dir = COLLIDE_DIR::UP;
				_collide_down = false;
			} else {
				_dir = COLLIDE_DIR::DOWN;
				_collide_up = false;
			}
		}

	}

	//左右
	{
		if ( collidePlayerWithGimmickFromLeft( player, gimmick ) ) {
			//log->status( "hit_left" );
			_dir = COLLIDE_DIR::LEFT;
			_collide_left = true;
		}
		if ( collidePlayerWithGimmickFromRight( player, gimmick ) ) {
			//log->status( "hit_right" );
			_dir = COLLIDE_DIR::RIGHT;
			_collide_right = true;
		}

		if ( _collide_left && _collide_right ) {
			//vecが貫通しているとき
			//log->status( "hit_left_right" );
			double p_left_to_g_left = gimmick_left_sx - player_left_sx;
			double p_left_to_g_right = gimmick_right_sx - player_left_sx;
			if ( p_left_to_g_left < p_left_to_g_right ) {
				_dir = COLLIDE_DIR::LEFT;
				_collide_right = false;
			} else {
				_dir = COLLIDE_DIR::RIGHT;
				_collide_left = false;
			}
		}
	}

	//角に当たったとき
	if ( ( _collide_up || _collide_down ) && ( _collide_left || _collide_right ) ) {
		collideCornerWithGimmick( player, gimmick );
	}

	if ( _collide_up || _collide_down || _collide_left || _collide_right ) {
		switch ( gimmick->getType( ) ) {
		case Gimmick::TYPE::FAN:
			collidePlayerWithFan( player, gimmick );
			break;
		case Gimmick::TYPE::NEEDLE:
			collidePlayerWithNeedle( player, gimmick );
			break;
		}
	}
}

void Collider::collideCornerWithGimmick( PlayerPtr player, GimmickPtr gimmick ) {
	double player_top_sy = player->getPos( ).y;
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	double gimmick_top_sy = gimmick->getPos( ).y;
	double gimmick_bottom_sy = gimmick->getPos( ).y + gimmick->getSize( ).y;

	if ( _collide_up && _collide_left ) {
		if ( player_bottom_sy > gimmick_top_sy ) {
			_dir = COLLIDE_DIR::LEFT;
		} else {
			_dir = COLLIDE_DIR::UP;
		}
	}
	if ( _collide_up && _collide_right ) {
		if ( player_bottom_sy > gimmick_top_sy ) {
			_dir = COLLIDE_DIR::RIGHT;
		} else {
			_dir = COLLIDE_DIR::UP;
		}

	}
	if ( _collide_down && _collide_left ) {
		if ( player_top_sy < gimmick_bottom_sy ) {
			_dir = COLLIDE_DIR::LEFT;
		} else {
			_dir = COLLIDE_DIR::DOWN;
		}
	}
	if ( _collide_down && _collide_right ) {
		if ( player_top_sy < gimmick_bottom_sy ) {
			_dir = COLLIDE_DIR::RIGHT;
		} else {
			_dir = COLLIDE_DIR::DOWN;
		}
	}
}

bool Collider::collidePlayerWithGimmickFromLeft( PlayerPtr player, GimmickPtr gimmick ) {
	bool collide_from_left = false;

	double player_left_sx = player->getPos( ).x;

	double player_moved_right_sx = player->getPos( ).x + player->getSize( ).x + player->getVec( ).x;
	double player_top_sy = player->getPos( ).y;
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;


	double gimmick_left_sx = gimmick->getPos( ).x;
	double gimmick_top_sy = gimmick->getPos( ).y;
	double gimmick_bottom_sy = gimmick->getPos( ).y + gimmick->getSize( ).y;
	if ( gimmick->getType( ) == Gimmick::TYPE::NEEDLE ) {
		if ( gimmick->getDir( ) == Gimmick::DIR::LEFT || gimmick->getDir( ) == Gimmick::DIR::RIGHT ) {
			gimmick_top_sy = gimmick->getPos( ).y + NEEDLE_COLLIDE_AJDUST;
			gimmick_bottom_sy = gimmick->getPos( ).y + gimmick->getSize( ).y - NEEDLE_COLLIDE_AJDUST;
		} else {
			gimmick_left_sx = gimmick->getPos( ).x + NEEDLE_COLLIDE_AJDUST;
		}
	}

	if ( player_top_sy < gimmick_top_sy && player_bottom_sy > gimmick_bottom_sy ) {
		//player_height > gimmick_height
		if ( player_moved_right_sx > gimmick_left_sx && player_left_sx < gimmick_left_sx ) {
			collide_from_left = true;
		} else {
			collide_from_left = false;
		}
	} else {
		bool top_y_line_collide = false;
		bool bottom_y_line_collide = false;

		//player_top_line with gimmick_left_line
		Vector2 player_left_top_pos = Vector2( player_left_sx, player_top_sy );
		Vector2 player_right_top_pos = Vector2( player_moved_right_sx, player_top_sy );

		Vector2 gimmick_left_top_pos = Vector2( gimmick_left_sx, gimmick_top_sy );
		Vector2 gimmick_left_bottom_pos = Vector2( gimmick_left_sx, gimmick_bottom_sy );

		top_y_line_collide = isCrossingOtherLineSegment( player_left_top_pos, player_right_top_pos,
														 gimmick_left_top_pos, gimmick_left_bottom_pos, true );


		//player_bottom_line with gimmick_left_line
		Vector2 player_left_bottom_pos = Vector2( player_left_sx, player_bottom_sy );
		Vector2 player_right_bottom_pos = Vector2( player_moved_right_sx, player_bottom_sy );

		bottom_y_line_collide = isCrossingOtherLineSegment( player_left_bottom_pos, player_right_bottom_pos,
															gimmick_left_top_pos, gimmick_left_bottom_pos, true );

		collide_from_left = top_y_line_collide || bottom_y_line_collide;
	}
	return collide_from_left;
}

bool Collider::collidePlayerWithGimmickFromRight( PlayerPtr player, GimmickPtr gimmick ) {
	bool collide_from_right = false;

	double player_right_sx = player->getPos( ).x + player->getSize( ).x;

	double player_moved_left_sx = player->getPos( ).x + player->getVec( ).x;

	double player_top_sy = player->getPos( ).y;
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;


	double gimmick_right_sx = gimmick->getPos( ).x + gimmick->getSize( ).x;
	double gimmick_top_sy = gimmick->getPos( ).y;
	double gimmick_bottom_sy = gimmick->getPos( ).y + gimmick->getSize( ).y;
	if ( gimmick->getType( ) == Gimmick::TYPE::NEEDLE ) {
		if ( gimmick->getDir( ) == Gimmick::DIR::LEFT || gimmick->getDir( ) == Gimmick::DIR::RIGHT ) {
			gimmick_top_sy = gimmick->getPos( ).y + NEEDLE_COLLIDE_AJDUST;
			gimmick_bottom_sy = gimmick->getPos( ).y + gimmick->getSize( ).y - NEEDLE_COLLIDE_AJDUST;
		} else {
			gimmick_right_sx = gimmick->getPos( ).x + gimmick->getSize( ).x - NEEDLE_COLLIDE_AJDUST;
		}
	}

	if ( player_top_sy < gimmick_top_sy && player_bottom_sy > gimmick_bottom_sy ) {
		//player_height > gimmick_height
		if ( player_moved_left_sx < gimmick_right_sx && player_right_sx > gimmick_right_sx ) {
			collide_from_right = true;
		} else {
			collide_from_right = false;
		}
	} else {
		bool top_y_line_collide = false;
		bool bottom_y_line_collide = false;

		//player_top_line with gimmick_right_line
		Vector2 player_right_top_pos = Vector2( player_right_sx, player_top_sy );
		Vector2 player_left_top_pos = Vector2( player_moved_left_sx, player_top_sy );

		Vector2 gimmick_right_top_pos = Vector2( gimmick_right_sx, gimmick_top_sy );
		Vector2 gimmick_right_bottom_pos = Vector2( gimmick_right_sx, gimmick_bottom_sy );

		top_y_line_collide = isCrossingOtherLineSegment( player_right_top_pos, player_left_top_pos,
														 gimmick_right_top_pos, gimmick_right_bottom_pos, true );


		//player_bottom_line with gimmick_right_line
		Vector2 player_right_bottom_pos = Vector2( player_right_sx, player_bottom_sy );
		Vector2 player_left_bottom_pos = Vector2( player_moved_left_sx, player_bottom_sy );

		bottom_y_line_collide = isCrossingOtherLineSegment( player_right_bottom_pos, player_left_bottom_pos,
															gimmick_right_top_pos, gimmick_right_bottom_pos, true );

		collide_from_right = top_y_line_collide || bottom_y_line_collide;
	}
	return collide_from_right;
}

bool Collider::collidePlayerWithGimmickFromUp( PlayerPtr player, GimmickPtr gimmick ) {
	bool collide_from_up = false;

	double player_top_sy = player->getPos( ).y;
	double player_left_sx = player->getPos( ).x;
	double player_right_sx = player->getPos( ).x + player->getSize( ).x;

	double player_moved_bottom_sy = player->getPos( ).y + player->getSize( ).y + player->getVec( ).y;


	double gimmick_top_sy = gimmick->getPos( ).y;
	double gimmick_left_sx = gimmick->getPos( ).x;
	double gimmick_right_sx = gimmick->getPos( ).x + gimmick->getSize( ).x;
	if ( gimmick->getType( ) == Gimmick::TYPE::NEEDLE ) {
		if ( gimmick->getDir( ) == Gimmick::DIR::UP || gimmick->getDir( ) == Gimmick::DIR::DOWN ) {
			gimmick_left_sx = gimmick->getPos( ).x + NEEDLE_COLLIDE_AJDUST;
			gimmick_right_sx = gimmick->getPos( ).x + gimmick->getSize( ).x - NEEDLE_COLLIDE_AJDUST;
		} else {
			gimmick_top_sy = gimmick->getPos( ).y + NEEDLE_COLLIDE_AJDUST;
		}
	}

	if ( player_left_sx < gimmick_left_sx && player_right_sx > gimmick_right_sx ) {
		//player_width > gimmick_width
		if ( player_moved_bottom_sy >= gimmick_top_sy && player_top_sy < gimmick_top_sy ) {
			collide_from_up = true;
		} else {
			collide_from_up = false;
		}
	} else {
		bool left_x_line_collide = false;
		bool right_x_line_collide = false;
		//player_left_line with gimmick_top_line
		Vector2 player_left_top_pos = Vector2( player_left_sx, player_top_sy );
		Vector2 player_left_bottom_pos = Vector2( player_left_sx, player_moved_bottom_sy );

		Vector2 gimmick_left_top_pos = Vector2( gimmick_left_sx, gimmick_top_sy );
		Vector2 gimmick_right_top_pos = Vector2( gimmick_right_sx, gimmick_top_sy );

		left_x_line_collide = isCrossingOtherLineSegment( player_left_top_pos, player_left_bottom_pos,
														  gimmick_left_top_pos, gimmick_right_top_pos, true );


		//player_right_line with gimmick_top_line
		Vector2 player_right_top_pos = Vector2( player_right_sx, player_top_sy );
		Vector2 player_right_bottom_pos = Vector2( player_right_sx, player_moved_bottom_sy );

		right_x_line_collide = isCrossingOtherLineSegment( player_right_top_pos, player_right_bottom_pos,
														   gimmick_left_top_pos, gimmick_right_top_pos, true );

		collide_from_up = left_x_line_collide || right_x_line_collide;
	}
	return collide_from_up;
}

bool Collider::collidePlayerWithGimmickFromDown( PlayerPtr player, GimmickPtr gimmick ) {
	bool collide_from_down = false;

	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	double player_left_sx = player->getPos( ).x;
	double player_right_sx = player->getPos( ).x + player->getSize( ).x;

	double player_moved_top_sy = player->getPos( ).y + player->getVec( ).y;


	double gimmick_bottom_sy = gimmick->getPos( ).y + gimmick->getSize( ).y;
	double gimmick_left_sx = gimmick->getPos( ).x;
	double gimmick_right_sx = gimmick->getPos( ).x + gimmick->getSize( ).x;
	if ( gimmick->getType( ) == Gimmick::TYPE::NEEDLE ) {
		if ( gimmick->getDir( ) == Gimmick::DIR::UP || gimmick->getDir( ) == Gimmick::DIR::DOWN ) {
			gimmick_left_sx = gimmick->getPos( ).x + NEEDLE_COLLIDE_AJDUST;
			gimmick_right_sx = gimmick->getPos( ).x + gimmick->getSize( ).x - NEEDLE_COLLIDE_AJDUST;
		} else {
			gimmick_bottom_sy = gimmick->getPos( ).y + gimmick->getSize( ).y - NEEDLE_COLLIDE_AJDUST;
		}
	}

	if ( player_left_sx < gimmick_left_sx && player_right_sx > gimmick_right_sx ) {
		//player_width > gimmick_width
		if ( player_moved_top_sy < gimmick_bottom_sy && player_bottom_sy > gimmick_bottom_sy ) {
			collide_from_down = true;
		} else {
			collide_from_down = false;
		}
	} else {
		bool left_x_line_collide = false;
		bool right_x_line_collide = false;

		//player_left_line with gimmick_bottom_line
		Vector2 player_left_bottom_pos = Vector2( player_left_sx, player_bottom_sy );
		Vector2 player_left_top_pos = Vector2( player_left_sx, player_moved_top_sy );

		Vector2 gimmick_left_bottom_pos = Vector2( gimmick_left_sx, gimmick_bottom_sy );
		Vector2 gimmick_right_bottom_pos = Vector2( gimmick_right_sx, gimmick_bottom_sy );

		left_x_line_collide = isCrossingOtherLineSegment( player_left_bottom_pos, player_left_top_pos,
														  gimmick_left_bottom_pos, gimmick_right_bottom_pos, true );


		//player_right_line with gimmick_bottom_line
		Vector2 player_right_bottom_pos = Vector2( player_right_sx, player_bottom_sy );
		Vector2 player_right_top_pos = Vector2( player_right_sx, player_moved_top_sy );

		right_x_line_collide = isCrossingOtherLineSegment( player_right_bottom_pos, player_right_top_pos,
														   gimmick_left_bottom_pos, gimmick_right_bottom_pos, true );

		collide_from_down = left_x_line_collide || right_x_line_collide;
	}
	return collide_from_down;
}

bool Collider::collidePlayerWithGimmickInside( PlayerPtr player, GimmickPtr gimmick ) {
	double player_moved_left_sx = player->getPos( ).x + player->getVec( ).x;
	double player_moved_right_sx = player->getPos( ).x + player->getSize( ).x + player->getVec( ).x;
	double player_moved_top_sy = player->getPos( ).y + player->getVec( ).y;
	double player_moved_bottom_sy = player->getPos( ).y + player->getSize( ).y + player->getVec( ).y;

	double gimmick_left_sx = gimmick->getPos( ).x;
	double gimmick_right_sx = gimmick->getPos( ).x + gimmick->getSize( ).x;
	double gimmick_top_sy = gimmick->getPos( ).y;
	double gimmick_bottom_sy = gimmick->getPos( ).y + gimmick->getSize( ).y;

	return player_moved_left_sx >= gimmick_left_sx &&
		player_moved_right_sx <= gimmick_right_sx &&
		player_moved_top_sy >= gimmick_top_sy &&
		player_moved_bottom_sy <= gimmick_bottom_sy;
}

void Collider::collidePlayerWithGoal( PlayerPtr player, GimmickPtr goal ) {
	SystemPtr system = Task::get< System >( );
	if ( player->getPos( ).x > goal->getPos( ).x + goal->getSize( ).x / 6 &&
		 player->getPos( ).x + player->getSize( ).x < goal->getPos( ).x + goal->getSize( ).x * 5 / 6 &&
		 player->getPos( ).y > goal->getPos( ).y &&
		 player->getPos( ).y + player->getSize( ).y < goal->getPos( ).y + goal->getSize( ).y &&
		 player->getState( ) == Player::STATE::ALIVE &&
		 player->isStanding( ) ) {
		player->setStanding( true );
		system->setClear( );
	}
	if ( player->getState( ) == Player::STATE::DEAD ) {
		if ( collidePlayerWithGimmickFromLeft( player, goal ) ||
			 collidePlayerWithGimmickFromRight( player, goal ) ||
			 collidePlayerWithGimmickFromUp( player, goal ) ||
			 collidePlayerWithGimmickFromDown( player, goal ) ||
			 collidePlayerWithGimmickInside( player, goal ) ) {
			player->setDelete( );

		}
	} else {
		if ( collidePlayerWithGimmickFromLeft( player, goal ) ||
			 collidePlayerWithGimmickFromRight( player, goal ) ||
			 collidePlayerWithGimmickFromUp( player, goal ) ||
			 collidePlayerWithGimmickFromDown( player, goal ) ||
			 collidePlayerWithGimmickInside( player, goal ) ) {
			player->setExplosive( false );
		}
	}
}

void Collider::collidePlayerWithRespawn( PlayerPtr player, GimmickPtr respawn ) {
	if ( player->getState( ) == Player::STATE::DEAD ) {
		if ( collidePlayerWithGimmickFromLeft( player, respawn ) ||
			 collidePlayerWithGimmickFromRight( player, respawn ) ||
			 collidePlayerWithGimmickFromUp( player, respawn ) ||
			 collidePlayerWithGimmickFromDown( player, respawn ) ||
			 collidePlayerWithGimmickInside( player, respawn ) ) {
			player->setDelete( );
		}
	} else {
		if ( collidePlayerWithGimmickFromLeft( player, respawn ) ||
			 collidePlayerWithGimmickFromRight( player, respawn ) ||
			 collidePlayerWithGimmickFromUp( player, respawn ) ||
			 collidePlayerWithGimmickFromDown( player, respawn ) ||
			 collidePlayerWithGimmickInside( player, respawn ) ) {
			player->setExplosive( false );
		}
	}
}

void Collider::collidePlayerWithEnemies( std::vector<PlayerPtr> players ) {
	std::vector< EnemyPtr > enemies = _map_data->getEnemies( );
	for ( int i = 0; i < enemies.size( ); i++ ) {
		EnemyPtr enemy = enemies[ i ];
		for ( int j = 0; j < players.size( ); j++ ) {
			PlayerPtr src_player = players[ j ];
			collidePlayerWithEnemy( src_player, enemy );
		}
	}
}

void Collider::collidePlayerWithEnemy( PlayerPtr player, EnemyPtr enemy ) {
	LogPtr log = Task::get< Log >( );
	if ( collidePlayerWithEnemyFromLeft( player, enemy ) ||
		 collidePlayerWithEnemyFromRight( player, enemy ) ||
		 collidePlayerWithEnemyFromUp( player, enemy ) ||
		 collidePlayerWithEnemyFromDown( player, enemy ) ) {

		log->status( "enemy_hit" );
		if ( enemy->getStatus( ) == Enemy::STATUS::HARMFUL ) {
			if ( player->getState( ) == Player::STATE::ALIVE ) {
				player->setDeadReason( Player::DEAD_REASON::ENEMY );
				player->setDead( false );
				player->setPos( ) = enemy->getPos( ) - Vector2( 0, 100 );
				enemy->setChange( true );
				enemy->setStatus( Enemy::STATUS::HARMLESSNESS );

			}
		} else {
			if ( player->getState( ) == Player::STATE::DEAD && player->getReason( ) == Player::DEAD_REASON::ENEMY ) {
				player->setVec( ) = enemy->getVec( );
				enemy->capturePlayer( player );
			}
		}
	}
}

bool Collider::collidePlayerWithEnemyFromLeft( PlayerPtr player, EnemyPtr enemy ) {
	bool collide_from_left = false;

	double player_left_sx = player->getPos( ).x;

	double player_moved_right_sx = player->getPos( ).x + player->getSize( ).x;
	double player_top_sy = player->getPos( ).y + player->getVec( ).y;
	if ( player->getVec( ).y > 0 ) {
		player_top_sy = player->getPos( ).y;
	}
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y + player->getVec( ).y;
	if ( player->getVec( ).y < 0 ) {
		player_bottom_sy = player->getPos( ).y + player->getSize( ).y;
	}

	double enemy_left_sx = enemy->getPos( ).x;
	double enemy_top_sy = enemy->getPos( ).y;
	double enemy_bottom_sy = enemy->getPos( ).y + enemy->getSize( ).y;

	if ( player_top_sy < enemy_top_sy && player_bottom_sy > enemy_bottom_sy ) {
		//player_height > gimmick_height
		if ( player_moved_right_sx > enemy_left_sx && player_left_sx < enemy_left_sx ) {
			collide_from_left = true;
		} else {
			collide_from_left = false;
		}
	} else {
		bool top_y_line_collide = false;
		bool bottom_y_line_collide = false;

		//player_top_line with gimmick_left_line
		Vector2 player_left_top_pos = Vector2( player_left_sx, player_top_sy );
		Vector2 player_right_top_pos = Vector2( player_moved_right_sx, player_top_sy );

		Vector2 enemy_left_top_pos = Vector2( enemy_left_sx, enemy_top_sy );
		Vector2 enemy_left_bottom_pos = Vector2( enemy_left_sx, enemy_bottom_sy );

		top_y_line_collide = isCrossingOtherLineSegment( player_left_top_pos, player_right_top_pos,
														 enemy_left_top_pos, enemy_left_bottom_pos, false );


		//player_bottom_line with gimmick_left_line
		Vector2 player_left_bottom_pos = Vector2( player_left_sx, player_bottom_sy );
		Vector2 player_right_bottom_pos = Vector2( player_moved_right_sx, player_bottom_sy );

		bottom_y_line_collide = isCrossingOtherLineSegment( player_left_bottom_pos, player_right_bottom_pos,
															enemy_left_top_pos, enemy_left_bottom_pos, false );

		collide_from_left = top_y_line_collide || bottom_y_line_collide;
	}
	return collide_from_left;
}

bool Collider::collidePlayerWithEnemyFromRight( PlayerPtr player, EnemyPtr enemy ) {
	bool collide_from_right = false;

	double player_right_sx = player->getPos( ).x + player->getSize( ).x;

	double player_moved_left_sx = player->getPos( ).x;
	double player_top_sy = player->getPos( ).y + player->getVec( ).y;
	if ( player->getVec( ).y > 0 ) {
		player_top_sy = player->getPos( ).y;
	}
	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y + player->getVec( ).y;
	if ( player->getVec( ).y < 0 ) {
		player_bottom_sy = player->getPos( ).y + player->getSize( ).y;
	}

	double enemy_top_sy = enemy->getPos( ).y;
	double enemy_bottom_sy = enemy->getPos( ).y + enemy->getSize( ).y;
	double enemy_right_sx = enemy->getPos( ).x + enemy->getSize( ).x;

	if ( player_top_sy < enemy_top_sy && player_bottom_sy > enemy_bottom_sy ) {
		//player_height > gimmick_height
		if ( player_moved_left_sx < enemy_right_sx && player_right_sx > enemy_right_sx ) {
			collide_from_right = true;
		} else {
			collide_from_right = false;
		}
	} else {
		bool top_y_line_collide = false;
		bool bottom_y_line_collide = false;

		//player_top_line with gimmick_right_line
		Vector2 player_right_top_pos = Vector2( player_right_sx, player_top_sy );
		Vector2 player_left_top_pos = Vector2( player_moved_left_sx, player_top_sy );

		Vector2 enemy_right_top_pos = Vector2( enemy_right_sx, enemy_top_sy );
		Vector2 enemy_right_bottom_pos = Vector2( enemy_right_sx, enemy_bottom_sy );

		top_y_line_collide = isCrossingOtherLineSegment( player_right_top_pos, player_left_top_pos,
														 enemy_right_top_pos, enemy_right_bottom_pos, false );


		//player_bottom_line with gimmick_right_line
		Vector2 player_right_bottom_pos = Vector2( player_right_sx, player_bottom_sy );
		Vector2 player_left_bottom_pos = Vector2( player_moved_left_sx, player_bottom_sy );

		bottom_y_line_collide = isCrossingOtherLineSegment( player_right_bottom_pos, player_left_bottom_pos,
															enemy_right_top_pos, enemy_right_bottom_pos, false );

		collide_from_right = top_y_line_collide || bottom_y_line_collide;
	}
	return collide_from_right;
}

bool Collider::collidePlayerWithEnemyFromUp( PlayerPtr player, EnemyPtr enemy ) {
	bool collide_from_up = false;

	double player_top_sy = player->getPos( ).y;

	double player_left_sx = player->getPos( ).x + player->getVec( ).x;
	if ( player->getVec( ).x > 0 ) {
		player_left_sx = player->getPos( ).x;
	}
	double player_right_sx = player->getPos( ).x + player->getSize( ).x + player->getVec( ).x;
	if ( player->getVec( ).x < 0 ) {
		player_right_sx = player->getPos( ).x + player->getSize( ).x;
	}
	double player_moved_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	double enemy_left_sx = enemy->getPos( ).x;
	double enemy_right_sx = enemy->getPos( ).x + enemy->getSize( ).x;
	double enemy_top_sy = enemy->getPos( ).y;

	if ( player_left_sx < enemy_left_sx && player_right_sx > enemy_right_sx ) {
		//player_width > gimmick_width
		if ( player_moved_bottom_sy >= enemy_top_sy && player_top_sy < enemy_top_sy ) {
			collide_from_up = true;
		} else {
			collide_from_up = false;
		}
	} else {
		bool left_x_line_collide = false;
		bool right_x_line_collide = false;
		//player_left_line with gimmick_top_line
		Vector2 player_left_top_pos = Vector2( player_left_sx, player_top_sy );
		Vector2 player_left_bottom_pos = Vector2( player_left_sx, player_moved_bottom_sy );

		Vector2 enemy_left_top_pos = Vector2( enemy_left_sx, enemy_top_sy );
		Vector2 enemy_right_top_pos = Vector2( enemy_right_sx, enemy_top_sy );

		left_x_line_collide = isCrossingOtherLineSegment( player_left_top_pos, player_left_bottom_pos,
														  enemy_left_top_pos, enemy_right_top_pos, false );


		//player_right_line with gimmick_top_line
		Vector2 player_right_top_pos = Vector2( player_right_sx, player_top_sy );
		Vector2 player_right_bottom_pos = Vector2( player_right_sx, player_moved_bottom_sy );

		right_x_line_collide = isCrossingOtherLineSegment( player_right_top_pos, player_right_bottom_pos,
														   enemy_left_top_pos, enemy_right_top_pos, false );

		collide_from_up = left_x_line_collide || right_x_line_collide;
	}
	return collide_from_up;
}

bool Collider::collidePlayerWithEnemyFromDown( PlayerPtr player, EnemyPtr enemy ) {
	bool collide_from_down = false;

	double player_bottom_sy = player->getPos( ).y + player->getSize( ).y;

	double player_left_sx = player->getPos( ).x + player->getVec( ).x;
	if ( player->getVec( ).x > 0 ) {
		player_left_sx = player->getPos( ).x;
	}
	double player_right_sx = player->getPos( ).x + player->getSize( ).x + player->getVec( ).x;
	if ( player->getVec( ).x < 0 ) {
		player_right_sx = player->getPos( ).x + player->getSize( ).x;
	}
	double player_moved_top_sy = player->getPos( ).y;

	double enemy_left_sx = enemy->getPos( ).x;
	double enemy_right_sx = enemy->getPos( ).x + enemy->getSize( ).x;
	double enemy_bottom_sy = enemy->getPos( ).y + enemy->getSize( ).y;

	if ( player_left_sx < enemy_left_sx && player_right_sx > enemy_right_sx ) {
		//player_width > gimmick_width
		if ( player_moved_top_sy < enemy_bottom_sy && player_bottom_sy > enemy_bottom_sy ) {
			collide_from_down = true;
		} else {
			collide_from_down = false;
		}
	} else {
		bool left_x_line_collide = false;
		bool right_x_line_collide = false;

		//player_left_line with gimmick_bottom_line
		Vector2 player_left_bottom_pos = Vector2( player_left_sx, player_bottom_sy );
		Vector2 player_left_top_pos = Vector2( player_left_sx, player_moved_top_sy );

		Vector2 enemy_left_bottom_pos = Vector2( enemy_left_sx, enemy_bottom_sy );
		Vector2 enemy_right_bottom_pos = Vector2( enemy_right_sx, enemy_bottom_sy );

		left_x_line_collide = isCrossingOtherLineSegment( player_left_bottom_pos, player_left_top_pos,
														  enemy_left_bottom_pos, enemy_right_bottom_pos, false );


		//player_right_line with gimmick_bottom_line
		Vector2 player_right_bottom_pos = Vector2( player_right_sx, player_bottom_sy );
		Vector2 player_right_top_pos = Vector2( player_right_sx, player_moved_top_sy );

		right_x_line_collide = isCrossingOtherLineSegment( player_right_bottom_pos, player_right_top_pos,
														   enemy_left_bottom_pos, enemy_right_bottom_pos, false );

		collide_from_down = left_x_line_collide || right_x_line_collide;
	}
	return collide_from_down;
}

bool Collider::isCrossingOtherLineSegment( Vector2 line_segment1_start_pos, Vector2 line_segment1_end_pos, Vector2 line_segment2_start_pos, Vector2 line_segment2_end_pos, bool just_consider ) {
	return isStraddlingOtherLineSegment( line_segment1_start_pos, line_segment1_end_pos,
										 line_segment2_start_pos, line_segment2_end_pos, just_consider ) &&
		isStraddlingOtherLineSegment( line_segment2_start_pos, line_segment2_end_pos,
									  line_segment1_start_pos, line_segment1_end_pos, just_consider );
}

bool Collider::isStraddlingOtherLineSegment( Vector2 src_line_segment_start_pos, Vector2 src_line_segment_end_pos, Vector2 dst_line_segment_start_pos, Vector2 dst_line_segment_end_pos, bool just_consider ) {
	bool straddling = false;

	Vector2 src_start_to_src_end = src_line_segment_end_pos - src_line_segment_start_pos;
	Vector2 src_start_to_dst_start = dst_line_segment_start_pos - src_line_segment_start_pos;
	Vector2 src_start_to_dst_end = dst_line_segment_end_pos - src_line_segment_start_pos;

	if ( just_consider ) {
		straddling = getCrossProduct( src_start_to_src_end, src_start_to_dst_start ) * getCrossProduct( src_start_to_src_end, src_start_to_dst_end ) <= 0;
	} else {
		straddling = getCrossProduct( src_start_to_src_end, src_start_to_dst_start ) * getCrossProduct( src_start_to_src_end, src_start_to_dst_end ) < 0;
	}
	return straddling;
}

double Collider::getCrossProduct( Vector2 vec1, Vector2 vec2 ) const {
	return ( vec1.x * vec2.y ) - ( vec2.x * vec1.y );
}

void Collider::collidePlayerWhenDie( PlayerPtr player ) {
	bool adjusted = true;
	if ( !player->isDieAdjusted( ) ) {
		std::vector< MapData::Ground* > grounds = _map_data->getGrounds( );
		for ( int j = 0; j < grounds.size( ); j++ ) {
			MapData::Ground* ground = grounds[ j ];
			if ( ( player->getPos( ).y > ground->pos.y &&
				 player->getPos( ).y < ground->pos.y + ground->actual_size.y ) ||
				 ( player->getPos( ).y + player->getSize( ).y > ground->pos.y &&
				 player->getPos( ).y + player->getSize( ).y < ground->pos.y + ground->actual_size.y ) ) {

				if ( player->getPos( ).x <= ground->pos.x + ground->actual_size.x &&
					 player->getPos( ).x + player->getSize( ).x >= ground->pos.x + ground->actual_size.x ) {
					player->setPos( ) = Vector2( ground->pos.x + ground->actual_size.x + 1, player->getPos( ).y );
					adjusted = false;
				}

				if ( player->getPos( ).x + player->getSize( ).x >= ground->pos.x &&
					 player->getPos( ).x <= ground->pos.x ) {
					player->setPos( ) = Vector2( ground->pos.x - player->getSize( ).x - 1, player->getPos( ).y );
					adjusted = false;
				}
			}
		}
		std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
		for ( int i = 0; i < gimmicks.size( ); i++ ) {
			GimmickPtr gimmick = gimmicks[ i ];
			if ( gimmick->getType( ) == Gimmick::TYPE::FAN ) {
				if ( ( player->getPos( ).y > gimmick->getPos( ).y &&
					 player->getPos( ).y < gimmick->getPos( ).y + gimmick->getSize( ).y ) ||
					 ( player->getPos( ).y + player->getSize( ).y > gimmick->getPos( ).y &&
					 player->getPos( ).y + player->getSize( ).y < gimmick->getPos( ).y + gimmick->getSize( ).y ) ) {

					if ( player->getPos( ).x <= gimmick->getPos( ).x + gimmick->getSize( ).x &&
						 player->getPos( ).x + player->getSize( ).x >= gimmick->getPos( ).x + gimmick->getSize( ).x ) {
						player->setPos( ) = Vector2( gimmick->getPos( ).x + gimmick->getSize( ).x + 1, player->getPos( ).y );
						adjusted = false;
					}

					if ( player->getPos( ).x + player->getSize( ).x >= gimmick->getPos( ).x &&
						 player->getPos( ).x <= gimmick->getPos( ).x ) {
						player->setPos( ) = Vector2( gimmick->getPos( ).x - player->getSize( ).x - 1, player->getPos( ).y );
						adjusted = false;
					}
				}
			}
		}

		std::vector< PlayerPtr > players = _map_data->getPlayers( );
		for ( int j = 0; j < players.size( ); j++ ) {
			PlayerPtr dst_player = players[ j ];
			if ( player == dst_player ) {
				continue;
			}
			if ( ( player->getPos( ).y > dst_player->getPos( ).y &&
				 player->getPos( ).y < dst_player->getPos( ).y + dst_player->getSize( ).y ) ||
				 ( player->getPos( ).y + player->getSize( ).y > dst_player->getPos( ).y &&
				 player->getPos( ).y + player->getSize( ).y < dst_player->getPos( ).y + dst_player->getSize( ).y ) ) {

				if ( player->getPos( ).x <= dst_player->getPos( ).x + dst_player->getSize( ).x &&
					 player->getPos( ).x + player->getSize( ).x >= dst_player->getPos( ).x + dst_player->getSize( ).x ) {
					player->setPos( ) = Vector2( dst_player->getPos( ).x + dst_player->getSize( ).x + 1, player->getPos( ).y );
					adjusted = false;
				}

				if ( player->getPos( ).x + player->getSize( ).x >= dst_player->getPos( ).x &&
					 player->getPos( ).x <= dst_player->getPos( ).x ) {
					player->setPos( ) = Vector2( dst_player->getPos( ).x - player->getSize( ).x - 1, player->getPos( ).y );
					adjusted = false;
				}
			}
		}
		if ( adjusted ) {
			player->setDieAdjusted( );
		}
		player->setVec( ) = Vector2( 0, player->getVec( ).y );
	}
}

Gimmick::DIR Collider::convertColliderDir( ) const {
	Gimmick::DIR dir = Gimmick::DIR::NONE;
	switch ( _dir ) {
	case Collider::COLLIDE_DIR::LEFT:
		dir = Gimmick::DIR::LEFT;
		break;
	case Collider::COLLIDE_DIR::RIGHT:
		dir = Gimmick::DIR::RIGHT;
		break;
	case Collider::COLLIDE_DIR::UP:
		dir = Gimmick::DIR::UP;
		break;
	case Collider::COLLIDE_DIR::DOWN:
		dir = Gimmick::DIR::DOWN;
		break;
	}
	return dir;
}

