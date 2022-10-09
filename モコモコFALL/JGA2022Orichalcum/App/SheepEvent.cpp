#include "SheepEvent.h"
#include "Collision.h"
#include "SheepsManager.h"
#include "CloudsManager.h"
#include "Keyboard.h"
#include "define.h"
#include "Log.h"
#include "Tool.h"
#include "SheepFactory.h"
#include "Application.h"
#include "FundamentalGameSystem.h"
#include "PropellerManager.h"

static const double OUT_RANGE = 50.0;

SheepEvent::SheepEvent( CollisionPtr collision,
						SheepsManagerPtr sheeps_manager,
						CloudsManagerPtr clouds_manager,
						SheepFactoryPtr sheep_factory,
						FundamentalGameSystemPtr game_system,
						PropellerManagerPtr propeller_manager ) :
	_collision( collision ),
	_sheeps_manager( sheeps_manager ),
	_clouds_manager( clouds_manager ),
	_sheep_factory( sheep_factory ),
	_game_system( game_system ),
	_propeller_manager( propeller_manager ) {
	ApplicationPtr app = Application::getInstance( );
	_before_deploy_time = app->getNowCount( );
	_deploy_interval = 500;

	initCreate( );
}

SheepEvent::~SheepEvent( ) {
}

void SheepEvent::update( ) {
	updateExecutionLevelUp( );
	if ( _game_system->isInterval( ) ) {
		return;
	}
	isDebugMode( );
	updateMove( );
	updateCollision( );

	ApplicationPtr app = Application::getInstance( );
	int now = app->getNowCount( );
	if ( now >= _before_deploy_time + _deploy_interval * 1000 ) {
		deployRandomPos( );
		_before_deploy_time = app->getNowCount( );
	}
}

void SheepEvent::setDeployInterval( int interval ) {
	_deploy_interval = interval;
}

void SheepEvent::updateExecutionLevelUp( ) {
	if ( !_game_system->isLevelUp( ) ) {
		return;
	}
	_sheeps_manager->clearAll( );
	Vector2 vec;
	vec.x = 10 - Tool::getRand( 20 );
	vec.y = 10 - Tool::getRand( 20 );
	_sheep_factory->deploy( Vector2( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ), vec );
}

void SheepEvent::updateCollision( ) {
	//プレイヤーが関係ないなら処理しない
	for ( int i = 0; i < _collision->getIdxListMaxSize( ); i++ ) {
		int list_idx = i;
		Collision::IdxList idx_list = _collision->getIdxList( list_idx );
		if ( idx_list.sheep1 == -1 ) {
			return;
		}

		if ( idx_list.sheep2 != -1 ) {
			//羊と羊の処理
			reflectiveToPlayer( idx_list.sheep1, idx_list.sheep2 );
		}
		if ( idx_list.cloud != -1 ) {
			//羊と雲の処理
			reflectiveToCloud( idx_list.sheep1, idx_list.cloud );
		}
		if ( idx_list.wing1 != -1 ) {
			reflectiveToWing1( idx_list.sheep1, idx_list.wing1 );
		}
		if ( idx_list.wing2 != -1 ) {
			reflectiveToWing2( idx_list.sheep1, idx_list.wing2 );
		}
		if ( idx_list.wing3 != -1 ) {
			reflectiveToWing3( idx_list.sheep1, idx_list.wing3 );
		}
		if ( idx_list.shaft ) {
			reflectiveToShaft( idx_list.sheep1 );
		}
	}
}

void SheepEvent::updateMove( ) {
	for (int i = 0; i < _sheeps_manager->getMaxNum( ); i++) {
		Vector2 now_pos = _sheeps_manager->getPos( i );
		_sheeps_manager->setPos( i, now_pos + _sheeps_manager->getVec( i ) );

	}
}

void SheepEvent::reflectiveToCloud( int player_idx, int cloud_idx ) {

	Vector2 player_pos = _sheeps_manager->getPos( player_idx );
	Vector2 cloud_pos = _clouds_manager->getPos( cloud_idx );
	Vector2 normal_vec = ( player_pos.sub( cloud_pos ) );
	Vector2 direction_vec = _sheeps_manager->getVec( player_idx );
	Vector2 cloud_vec = _clouds_manager->getVec( cloud_idx );
	Vector2 new_vec = ( normal_vec + direction_vec ) + cloud_vec;
	//speed 下げると早くなる.シンプルに足しただけだと値が大きすぎるのでspeedで割る
	double speed = 4;
	new_vec.x /= speed;
	new_vec.y /= speed;
	//スピードの最大値の設定
	new_vec.x = new_vec.x < -MAX_SPEED ? -MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y < -MAX_SPEED ? -MAX_SPEED : new_vec.y;
	new_vec.x = new_vec.x > MAX_SPEED ? MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y > MAX_SPEED ? MAX_SPEED : new_vec.y;

	_sheeps_manager->setVec( player_idx, new_vec );
}

void SheepEvent::reflectiveToPlayer( int player1_idx, int player2_idx ) {
	Vector2 sheep1_pos = _sheeps_manager->getPos( player1_idx );
	Vector2 sheep1_vec = _sheeps_manager->getVec( player1_idx );
	Vector2 sheep2_pos = _sheeps_manager->getPos( player2_idx );
	Vector2 sheep2_vec = _sheeps_manager->getVec( player2_idx );
	Vector2 normal1_vec = ( sheep1_pos.sub( sheep2_pos ) );
	Vector2 new_player1_vec = ( normal1_vec + sheep1_vec );
	Vector2 normal2_vec = ( sheep2_pos.sub( sheep1_pos ) );
	Vector2 new_player2_vec = ( normal2_vec + sheep2_vec );

	double speed = 5;
	new_player1_vec.x /= speed;
	new_player1_vec.y /= speed;
	new_player2_vec.x /= speed;
	new_player2_vec.y /= speed;

	/*読みにくかったら、ごめんなさい*/
	new_player1_vec.x = new_player1_vec.x < -MAX_SPEED ? -MAX_SPEED : new_player1_vec.x;
	new_player1_vec.y = new_player1_vec.y < -MAX_SPEED ? -MAX_SPEED : new_player1_vec.y;
	new_player1_vec.x = new_player1_vec.x > MAX_SPEED ? MAX_SPEED : new_player1_vec.x;
	new_player1_vec.y = new_player1_vec.y > MAX_SPEED ? MAX_SPEED : new_player1_vec.y;

	new_player2_vec.x = new_player2_vec.x < -MAX_SPEED ? -MAX_SPEED : new_player2_vec.x;
	new_player2_vec.y = new_player2_vec.y < -MAX_SPEED ? -MAX_SPEED : new_player2_vec.y;
	new_player2_vec.x = new_player2_vec.x > MAX_SPEED ? MAX_SPEED : new_player2_vec.x;
	new_player2_vec.y = new_player2_vec.y > MAX_SPEED ? MAX_SPEED : new_player2_vec.y;

	_sheeps_manager->setVec( player1_idx, new_player1_vec );
	_sheeps_manager->setVec( player2_idx, new_player2_vec );
}

void SheepEvent::reflectiveToWing1( int player_idx, int wing_idx ) {
	Vector2 sheep_pos = _sheeps_manager->getPos( player_idx );
	Vector2 wing_pos = _propeller_manager->getStatus( ).wing1[ wing_idx ].pos;
	Vector2 normal_vec = ( sheep_pos.sub( wing_pos ) );
	Vector2 direction_vec = _sheeps_manager->getVec( player_idx );
	Vector2 new_vec = ( normal_vec + direction_vec );

	double speed = _propeller_manager->getRotateAccel( ) * 10;
	new_vec.x += speed;
	new_vec.y += speed;
	new_vec.x = new_vec.x < -MAX_SPEED ? -MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y < -MAX_SPEED ? -MAX_SPEED : new_vec.y;

	new_vec.x = new_vec.x > MAX_SPEED ? MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y > MAX_SPEED ? MAX_SPEED : new_vec.y;

	_sheeps_manager->setVec( player_idx, new_vec );
}

void SheepEvent::reflectiveToWing2( int player_idx, int wing_idx ) {
	Vector2 sheep_pos = _sheeps_manager->getPos( player_idx );
	Vector2 wing_pos = _propeller_manager->getStatus( ).wing2[ wing_idx ].pos;
	Vector2 normal_vec = ( sheep_pos.sub( wing_pos ) );
	Vector2 direction_vec = _sheeps_manager->getVec( player_idx );
	Vector2 new_vec = ( normal_vec + direction_vec );

	double speed = _propeller_manager->getRotateAccel( ) * 10;
	new_vec.x += speed;
	new_vec.y += speed;
	new_vec.x = new_vec.x < -MAX_SPEED ? -MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y < -MAX_SPEED ? -MAX_SPEED : new_vec.y;

	new_vec.x = new_vec.x > MAX_SPEED ? MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y > MAX_SPEED ? MAX_SPEED : new_vec.y;

	_sheeps_manager->setVec( player_idx, new_vec );
}

void SheepEvent::reflectiveToWing3( int player_idx, int wing_idx ) {
	Vector2 sheep_pos = _sheeps_manager->getPos( player_idx );
	Vector2 wing_pos = _propeller_manager->getStatus( ).wing3[ wing_idx ].pos;
	Vector2 normal_vec = ( sheep_pos.sub( wing_pos ) );
	Vector2 direction_vec = _sheeps_manager->getVec( player_idx );
	Vector2 new_vec = ( normal_vec + direction_vec );

	double speed = _propeller_manager->getRotateAccel( ) * 10;
	new_vec.x += speed;
	new_vec.y += speed;
	new_vec.x = new_vec.x < -MAX_SPEED ? -MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y < -MAX_SPEED ? -MAX_SPEED : new_vec.y;

	new_vec.x = new_vec.x > MAX_SPEED ? MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y > MAX_SPEED ? MAX_SPEED : new_vec.y;

	_sheeps_manager->setVec( player_idx, new_vec );
}

void SheepEvent::reflectiveToShaft( int player_idx ) {
	Vector2 player_pos = _sheeps_manager->getPos( player_idx );
	Vector2 shaft_pos = _propeller_manager->getStatus( ).shaft_pos;
	Vector2 normal_vec = ( player_pos.sub( shaft_pos ) );
	Vector2 direction_vec = _sheeps_manager->getVec( player_idx );
	Vector2 new_vec = ( normal_vec + direction_vec );
	//speed 下げると早くなる.シンプルに足しただけだと値が大きすぎるのでspeedで割る
	double speed = 4;
	new_vec.x /= speed;
	new_vec.y /= speed;
	//スピードの最大値の設定
	new_vec.x = new_vec.x < -MAX_SPEED ? -MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y < -MAX_SPEED ? -MAX_SPEED : new_vec.y;
	new_vec.x = new_vec.x > MAX_SPEED ? MAX_SPEED : new_vec.x;
	new_vec.y = new_vec.y > MAX_SPEED ? MAX_SPEED : new_vec.y;

	_sheeps_manager->setVec( player_idx, new_vec );
}

void SheepEvent::deployRandomPos( ) {
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

	Vector2 diff;
	diff.x = SCREEN_WIDTH / 2 - pos.x;
	diff.y = SCREEN_HEIGHT / 2 - pos.y;

	double radian = atan2( diff.y, diff.x );
	Vector2 vec;
	vec.x = cos( radian ) * 4;
	vec.y = sin( radian ) * 4;

	_sheep_factory->deploy( pos, vec );
}

void SheepEvent::initCreate( ){
	Vector2 vec;
	vec.x = 10 - Tool::getRand( 20 );
	vec.y = 10 - Tool::getRand( 20 );
	_sheep_factory->deploy( Vector2( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ), vec );

}

bool SheepEvent::isInto( Vector2 vec1, Vector2 vec2 ) {
	double length_deff = ( vec1 - vec2 ).getLength( );
	double length = Vector2( CLOUD_RADIUS_SIZE, 0 ).getLength( );
	return length_deff / 2 < length;
}

bool SheepEvent::isDebugMode( ) {
	KeyboardPtr key = Task::get<Keyboard>( );
	int player_num = _sheeps_manager->getMaxNum( );
	if ( player_num == 0 ) {
		return false;
	}
	double speed = 15;
	//一番目のプレイヤーを操作する
	Vector2 vec = _sheeps_manager->getVec( 0 );
	if ( key->isHoldKey( "D" ) ) {
		vec = Vector2( speed, 0.0 );
		_sheeps_manager->setVec( 0, vec );
		return true;
	}
	if ( key->isHoldKey( "A" ) ) {
		vec = Vector2( -speed, 0.0 );
		_sheeps_manager->setVec( 0, vec );
		return true;
	}
	if ( key->isHoldKey( "W" ) ) {
		vec = Vector2( 0.0, -speed );
		_sheeps_manager->setVec( 0, vec );
		return true;
	}
	if ( key->isHoldKey( "S" ) ) {
		vec = Vector2( 0.0, speed );
		_sheeps_manager->setVec( 0, vec );
		return true;
	}
	return false;
}

