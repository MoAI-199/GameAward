#include "GimmickEvent.h"
#include "Collision.h"
#include "GimmickManager.h"
#include "Gimmick.h"
#include "GimmickFactory.h"
#include "SheepFactory.h"
#include "SheepsManager.h"
#include "Sheep.h"
#include "CloudsManager.h"
#include "FundamentalGameSystem.h"
#include "Tool.h"
#include "Application.h"
#include <algorithm>
#include "define.h"

static const int GIMMICK_TYPE_NUM = 3;
static const int COLLOR_NUM = 6;
static const double OUT_RANGE = 50.0;
static const double FALL_SPEED = 0.05;

GimmickEvent::GimmickEvent( CollisionPtr collisiton,
							SheepsManagerPtr players_manager,
							GimmickManagerPtr gimmick_manager,
							SheepFactoryPtr sheep_factory,
							GimmickFactoryPtr gimmick_factory,
							CloudsManagerPtr clouds_manager,
							FundamentalGameSystemPtr fundamental_game_system ) :
	_collision( collisiton ),
	_sheeps_manager( players_manager ),
	_gimmicks_manager( gimmick_manager ),
	_sheep_factory( sheep_factory ),
	_gimmick_factory( gimmick_factory ),
	_clouds_manager( clouds_manager ),
	_game_system( fundamental_game_system ) {
	ApplicationPtr app = Application::getInstance( );
	_before_deploy_time = app->getNowCount( );
	_deploy_interval = 500;
}

GimmickEvent::~GimmickEvent( ) {
}

void GimmickEvent::update( ) {
	updateExecutionLevelUp( );
	if ( _game_system->isInterval( ) ) {
		return;
	}
	setDeployInterval( 1 );
	updateAction( );
	updateCreateGimmick( );
	updateMove( );
}

void GimmickEvent::setDeployInterval( int interval ) {
	_deploy_interval = interval;
}

void GimmickEvent::updateExecutionLevelUp( ) {
	if ( !_game_system->isLevelUp( ) ) {
		return;
	}
	_gimmicks_manager->clearAll( );
}

void GimmickEvent::updateMove( ) {
	for ( int i = 0; i < _gimmicks_manager->getMaxNum( ); i++ ) {
		int idx = i;
		Vector2 now_pos = _gimmicks_manager->getPos( idx );
		_gimmicks_manager->setPos( idx, now_pos + _gimmicks_manager->getVec( idx ) );
	}
}

void GimmickEvent::updateCreateGimmick( ) {
	ApplicationPtr app = Application::getInstance( );
	int now = app->getNowCount( );
	if ( now >= _before_deploy_time + _deploy_interval * 1000 ) {
		deployRandomGimmick( );
		_before_deploy_time = app->getNowCount( );
	}
}

void GimmickEvent::updateAction( ) {
	for ( int i = 0; i < _collision->getIdxListMaxSize( ); i++ ) {
		int idx = i;
		Collision::IdxList gimmick_idx = _collision->getIdxList( idx );
		int deleted_gimmick_num = ( int )_deleted_gimmick_idx.size( );
		for ( int j = 0; j < deleted_gimmick_num; j++ ) {
			if ( gimmick_idx.gimmick > _deleted_gimmick_idx[ j ] ) {
				int diff = abs( gimmick_idx.gimmick - _deleted_gimmick_idx[ j ] );
				gimmick_idx.gimmick -= diff;
			}
		}
		if ( gimmick_idx.gimmick == -1 ) {
			continue;
		}
		actionGimmick( gimmick_idx );
	}
	_deleted_gimmick_idx.clear( );

}

void GimmickEvent::actionGimmick( Collision::IdxList idx_list ) {
	Gimmick::TYPE type = _gimmicks_manager->getType( idx_list.gimmick );
	switch ( type ) {
	case Gimmick::TYPE::NONE:
		break;
	case Gimmick::TYPE::COIN:
		actionHeart( idx_list.gimmick, idx_list.sheep1 );
		break;
	case Gimmick::TYPE::HOLE:
		actionHole( idx_list.gimmick, idx_list.sheep1 );
		break;
	case Gimmick::TYPE::COLOR_BALL:
		actionColorBall( idx_list.gimmick, idx_list.sheep1 );
		break;
	case Gimmick::TYPE::CARROT:
		actionCarrot( idx_list.gimmick, idx_list.sheep1 );
		break;
	}
}

void GimmickEvent::actionHeart( int gimmick_idx, int player_idx ) {
	Vector2 vec;
	vec.x = 10 - Tool::getRand( 20 );
	vec.y = 10 - Tool::getRand( 20 );
	_sheep_factory->deploy( _sheeps_manager->getPos( player_idx ), vec );
	_gimmicks_manager->deleteGimmick( gimmick_idx );
	_deleted_gimmick_idx.push_back( gimmick_idx );
}

void GimmickEvent::actionCarrot( int gimmick_idx, int sheep_idx ) {
	double now_scale = _sheeps_manager->getScale( sheep_idx );
	//0.1倍大きくする
	_sheeps_manager->setScale( sheep_idx, now_scale + 0.1 );
	_gimmicks_manager->deleteGimmick( gimmick_idx );
	_deleted_gimmick_idx.push_back( gimmick_idx );
}

void GimmickEvent::actionColorBall( int gimmick_idx, int player_idx ) {
	ColorBall::COLOR_TYPE gimmick_color_type = _gimmicks_manager->getColorType( gimmick_idx );
	//enumの順番が同じという性質を利用※カラーの列挙をdefineファイルにしてもいいかも
	int player_color_type = ( int )gimmick_color_type;
	_sheeps_manager->setColorType( player_idx, ( Sheep::COLOR_TYPE )player_color_type );
	_gimmicks_manager->deleteGimmick( gimmick_idx );
	_deleted_gimmick_idx.push_back( gimmick_idx );
	//１つでもfalseならレインボー処理しない
	for ( int i = 0; i < ( int )Sheep::COLOR_TYPE::MAX - 1; i++ ) {
		if ( !_sheeps_manager->isColorStorage( player_idx, ( Sheep::COLOR_TYPE )i ) ) {
			return;
		}
	}
	_sheeps_manager->setColorType( player_idx, Sheep::COLOR_TYPE::SECRET );
}

void GimmickEvent::actionHole( int gimmick_idx, int player_idx ) {
	if ( isDeleteSheep( player_idx ) ) {
		_gimmicks_manager->deleteGimmick( gimmick_idx );
		_deleted_gimmick_idx.push_back( gimmick_idx );
		_sheeps_manager->addDeleteList( player_idx );
	}
}

void GimmickEvent::deployRandomGimmick( ) {
	Vector2 pos;
	int dir = Tool::getRand( 2 );

	if ( dir ) {
		int up = Tool::getRand( 2 );
		if ( up ) {
			pos.y = -OUT_RANGE;
		} else {
			pos.y = SCREEN_HEIGHT + OUT_RANGE;
		}
		pos.x = Tool::getRand( SCREEN_WIDTH );
	} else {
		int left = Tool::getRand( 2 );
		if ( left ) {
			pos.x = -OUT_RANGE;
		} else {
			pos.x = SCREEN_WIDTH + OUT_RANGE;
		}
		pos.y = Tool::getRand( SCREEN_HEIGHT );
	}

	int rand_type = Tool::getRand( GIMMICK_TYPE_NUM );
	switch ( rand_type ) {
	case 0:
		_gimmick_factory->deploy( pos, Gimmick::TYPE::COIN );
		break;
	case 1:
		_gimmick_factory->deploy( pos, Gimmick::TYPE::COLOR_BALL, ( ColorBall::COLOR_TYPE )Tool::getRand( COLLOR_NUM ) );
		break;
	case 2:
		_gimmick_factory->deploy( pos, Gimmick::TYPE::CARROT );
		break;
	}
}

bool GimmickEvent::isDeleteSheep( int idx ) {
	return animationFallSheep( idx ) < 0.1;
}

double GimmickEvent::animationFallSheep( int idx ) {
	double size = _sheeps_manager->getViewSize( idx );
	size += -FALL_SPEED;
	Vector2 vec = Vector2( 0, 0 );
	_sheeps_manager->setVec( idx, vec );
	_sheeps_manager->setViewSize( idx, size );
	return size;
}
