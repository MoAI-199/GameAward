#include "Collision.h"
#include "SheepsManager.h"
#include "CloudsManager.h"
#include "GimmickManager.h"
#include "GimmickManager.h"
#include "FundamentalGameSystem.h"
#include "define.h"
#include <cmath>
#include "PropellerManager.h"

Collision::Collision( SheepsManagerPtr sheeps_manager,
					  CloudsManagerPtr clouds_manager,
					  GimmickManagerPtr gimmick_manager,
					  FundamentalGameSystemPtr fundamental_fame_system,
					  PropellerManagerPtr propeller_manager ) :
	_sheeps_manager( sheeps_manager ),
	_clouds_manager( clouds_manager ),
	_gimmick_manager( gimmick_manager ),
	_fundamental_game_system( fundamental_fame_system ),
	_propeller_manager( propeller_manager ) {
}

Collision::~Collision( ) {
}

void Collision::update( ) {
	resetIdxList( );
	//各メゾットのネストを減らすため
	for ( int i = 0; i < _sheeps_manager->getMaxNum( ); i++ ) {
		int player_idx = i;
		updatePlayerToPlayer( player_idx );
		updatePlayerToCloud( player_idx );
		updatePlayerToGimmick( player_idx );
		updatePlayerToPropeller( player_idx );
	}
	updateExecutionLevelUp( );

}

Collision::IdxList Collision::getIdxList( int idx ) const {
	return _idx_list.at( idx );
}

int Collision::getIdxListMaxSize( ) const {
	return ( int )_idx_list.size( );
}

bool Collision::isHitCircleToCircle( Vector2 pos1, Vector2 pos2 ) const {
	/*なんか下記コード差がある気がするだから残す。
	double length1 = std::pow( pos1.x - pos2.x, 2 ) + std::pow( pos1.y - pos2.y, 2 );
	double length2 = std::pow( CLOUD_RADIUS_SIZE + _players_manager->getSize( 0 ), 2 );
	if ( length1 < length2 ) {
		return true;
	}
	return false;
	*/
	double length1 = ( pos1 - pos2 ).getLength( );
	double length2 = CLOUD_RADIUS_SIZE + _sheeps_manager->getSize( 0 );
	if ( length1 < length2 ) {
		return true;
	}
	return false;
}

bool Collision::isHitCircleToCircle( Vector2 pos1, Vector2 pos2, double size ) const {
	double length1 = ( pos1 - pos2 ).getLength( );
	double length2 = _propeller_manager->getStatus( ).wing1[ 0 ].size + size;
	if ( length1 < length2 ) {
		return true;
	}
	return false;
}

bool Collision::isOverlapCircleToCircle( Vector2 pos1,
										 Vector2 pos2,
										 double size ) const {
	double length1 = ( pos1 - pos2 ).getLength( );
	if ( length1 < size ) {
		return true;
	}
	return false;
}

bool Collision::isHitCircleToSquare( Vector2 circle_pos,
									 int circle_range,
									 Vector2 square_pos2,
									 int square_width,
									 int square_height ) const {
	//四角の座標
	double x1 = square_pos2.x - square_width;
	double y1 = square_pos2.y - square_height;
	double x2 = square_pos2.x + square_width;
	double y2 = square_pos2.y + square_height;
	//羊の座標
	double xs = circle_pos.x;
	double ys = circle_pos.y;
	//円の半径
	double r = circle_range;

	//辺の判定
	//左右上下
	if ( ( xs > x1 ) && ( xs < x2 ) && ( ys > y1 - r ) && ( ys < y2 + r ) ) {
		return true;
	}
	if ( ( xs > x1 - r ) && ( xs < x2 + r ) && ( ys > y1 ) && ( ys < y2 ) ) {
		return true;
	}
	//角の判定
	//左上
	if ( ( std::pow( ( x1 - xs ), 2 ) + std::pow( ( y1 - ys ), 2 ) ) < std::pow( r, 2 ) ) {
		return true;
	}
	//右上
	if ( ( std::pow( ( x2 - xs ), 2 ) + std::pow( ( y1 - ys ), 2 ) ) < std::pow( r, 2 ) ) {
		return true;
	}
	//右下
	if ( ( std::pow( ( x2 - xs ), 2 ) + std::pow( ( y2 - ys ), 2 ) ) < std::pow( r, 2 ) ) {
		return true;
	}
	//左下
	if ( ( std::pow( ( x1 - xs ), 2 ) + std::pow( ( y2 - ys ), 2 ) ) < std::pow( r, 2 ) ) {
		return true;
	}

	return false;
}

void Collision::updateExecutionLevelUp( ) {
	if ( !_fundamental_game_system->isLevelUp( ) ) {
		return;
	}
	_idx_list.clear( );
}

void Collision::updatePlayerToPlayer( int idx ) {
	for ( int i = 0; i < _sheeps_manager->getMaxNum( ); i++ ) {
		if ( idx == i ) {
			continue;
		}
		int target_idx = i;
		Vector2 player_pos1 = _sheeps_manager->getPos( idx );
		Vector2 player_pos2 = _sheeps_manager->getPos( target_idx );
		if ( isHitCircleToCircle( player_pos1, player_pos2 ) ) {
			IdxList idx_list;
			idx_list.sheep1 = idx;
			idx_list.sheep2 = target_idx;
			_idx_list.push_back( idx_list );
			return;
		}
	}
}

void Collision::updatePlayerToCloud( int idx ) {
	for ( int i = 0; i < _clouds_manager->getMaxNum( ); i++ ) {
		int cloud_idx = i;
		Vector2 player_pos = _sheeps_manager->getPos( idx );
		Vector2 cloud_pos = _clouds_manager->getPos( cloud_idx );
		if ( !_clouds_manager->getValid( cloud_idx ) ) {
			continue;
		}
		if ( isHitCircleToCircle( player_pos, cloud_pos ) ) {
			IdxList idx_list;
			idx_list.sheep1 = idx;
			idx_list.cloud = cloud_idx;
			_idx_list.push_back( idx_list );

			return;
		}
	}
}

void Collision::updatePlayerToGimmick( int idx ) {
	for ( int i = 0; i < _gimmick_manager->getMaxNum( ); i++ ) {
		int gimmick_idx = i;
		Vector2 player_pos1 = _sheeps_manager->getPos( idx );
		Vector2 gimmick_pos = _gimmick_manager->getPos( gimmick_idx );
		Gimmick::TYPE type = _gimmick_manager->getType( gimmick_idx );
		double size = _gimmick_manager->getSize( gimmick_idx );
		
		/*ギミック穴の特例処理*/
		bool hit = isOverlapCircleToCircle( player_pos1, gimmick_pos, size );
		if ( type == Gimmick::TYPE::HOLE ) {
			if ( hit ) {
				for ( int j = 0; j < _idx_list.size( ); j++ ) {
					if ( gimmick_idx == _idx_list.at( j ).gimmick ) {
						gimmick_idx = -1;
					}
				}
				IdxList idx_list;
				idx_list.sheep1 = idx;
				idx_list.gimmick = gimmick_idx;
				_idx_list.push_back( idx_list );
			}
			return;
		}

		hit = isHitCircleToCircle( player_pos1, gimmick_pos );
		if ( hit ) {
			//同時に接触した際に片方を除外する
			for ( int j = 0; j < _idx_list.size( ); j++ ) {
				if ( gimmick_idx == _idx_list.at( j ).gimmick ) {
					gimmick_idx = -1;
				}
			}
			IdxList idx_list;
			idx_list.sheep1 = idx;
			idx_list.gimmick = gimmick_idx;
			_idx_list.push_back( idx_list );

			return;
		}
	}
}

void Collision::updatePlayerToPropeller( int idx ) {
	PropellerManager::Status status = _propeller_manager->getStatus( );
	int wing_num = ( int )status.wing1.size( );
	Vector2 sheep_pos = _sheeps_manager->getPos( idx );

	bool hit = isHitCircleToCircle( sheep_pos, status.shaft_pos, status.shaft_size );
	if ( hit ) {
		IdxList idx_list;
		idx_list.sheep1 = idx;
		idx_list.shaft = true;
		_idx_list.push_back( idx_list );
		return;
	}

	for ( int i = 0; i < wing_num; i++ ) {
		Vector2 wing1_pos = status.wing1[ i ].pos;
		double wing_size = status.wing1[ i ].size;
		hit = isHitCircleToCircle( sheep_pos, wing1_pos, wing_size );
		if ( hit ) {
			IdxList idx_list;
			idx_list.sheep1 = idx;
			idx_list.wing1 = i;
			_idx_list.push_back( idx_list );
			return;
		}
	}
	for ( int i = 0; i < wing_num; i++ ) {
		Vector2 wing2_pos = status.wing2[ i ].pos;
		double wing_size = status.wing1[ i ].size;
		hit = isHitCircleToCircle( sheep_pos, wing2_pos, wing_size );
		if ( hit ) {
			IdxList idx_list;
			idx_list.sheep1 = idx;
			idx_list.wing2 = i;
			_idx_list.push_back( idx_list );
			return;
		}
	}
	for ( int i = 0; i < wing_num; i++ ) {
		Vector2 wing3_pos = status.wing3[ i ].pos;
		double wing_size = status.wing1[ i ].size;
		hit = isHitCircleToCircle( sheep_pos, wing3_pos, wing_size );
		if ( hit ) {
			IdxList idx_list;
			idx_list.sheep1 = idx;
			idx_list.wing3 = i;
			_idx_list.push_back( idx_list );
			return;
		}
	}
}


void Collision::resetIdxList( ) {
	_idx_list.clear( );
}
