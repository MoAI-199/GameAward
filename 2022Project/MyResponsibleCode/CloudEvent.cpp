#include "FundamentalGameSystem.h"
#include "CloudEvent.h"
#include "CloudFactory.h"
#include "CloudsManager.h"
#include "DeviceController.h"
#include "Exception.h"
#include "define.h"
#include "mathematics.h"
#include "Keyboard.h"
#include "Log.h"
#include "Tool.h"

const int DEFORMATION_TIME = 500;

CloudEvent::CloudEvent( CloudFactoryPtr cloud_factory,
						CloudsManagerPtr clouds_manager,
						DeviceControllerPtr device_controller,
						FundamentalGameSystemPtr always_game_system ) :
	_cloud_factory( cloud_factory ),
	_clouds_manager( clouds_manager ),
	_device_controller( device_controller ),
	_game_system( always_game_system ) {
	_base_pos = Vector2( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 );//原点
	_form_switch = false;
	_animation = false;
	_form_circle = false;
	_form_square = false;
	_form_count_timer = 0;
	_start_millisecont = 0;
	_speed = 1.0;
	creates( 4 + 4 * 2 );
	placementFirstCircle( );
}

CloudEvent::~CloudEvent( ) {
}

void CloudEvent::update( ) {
	updateExecutionLevelUp( );
	if ( _game_system->isInterval( ) ) {
		return;
	}
	
	if ( !_animation ) {
		updateMove( );
		_vec.clear( );
	} else {
		moveAnimation( );
		_animation = !isAnimationEnd( );
		if ( !_animation ) {
			if ( _form_circle ) {
				placementFirstCircle( );
			} else if ( _form_square ) {
				placementFirstSquare( );
			}
		}
	}
}

void CloudEvent::updateMove( ) {
	int level = _game_system->getLevel( ) + 1;
	static int grade = 0;
	if ( level % 3 == 0 && grade < 2 ) {
		grade++;
	}
	moveGrade( ( MOVE_GRADE )grade );
}

void CloudEvent::updateForm( ) {
	switchFormFlag( );
	switchForm( );
}

void CloudEvent::updateExecutionLevelUp( ) {
	if ( !_game_system->isLevelUp( ) ) {
		return;
	}
	ApplicationPtr app = Task::get< Application >( );
	_start_millisecont = app->getNowCount( );
	_clouds_manager->clearAll( );
	//個数
	static int add_cloud;
	if ( _game_system->getLevel( ) % 2 == 0 &&
		 add_cloud < 20 ) {
		add_cloud++;
	}
	int base_num = 12;
	creates( base_num + add_cloud );

	//穴
	std::vector< int > array = { 0 };
	static int cloud_invalid_num;//穴の数
	if ( _game_system->getLevel( ) % 5 == 0 ) {
		if ( cloud_invalid_num < 8 ) {
			cloud_invalid_num++;
		}
	}
	//ランダムな位置にすると穴が小さくなるため穴の量を増やす
	if ( _game_system->getLevel( ) > 40 ) {
		for ( int i = 0; i < cloud_invalid_num + 5; i++ ) {
			array.push_back( Tool::getRand( _clouds_manager->getMaxNum( ) ) );
		}
	} else {
		for ( int i = 0; i < cloud_invalid_num; i++ ) {
			array.push_back( Tool::getRand( i ) );
		}
	}
	makeHole( ( int )array.size( ), array );
	array.clear( );

	//形
	int now_num = _clouds_manager->getMaxNum( );
	if ( now_num % 4 == 0 ) {
		placementFirstSquare( );
	} else {
		placementFirstCircle( );
	}

	static bool circle = false;
	if ( _game_system->getLevel( ) > 40 ) {
		if ( circle ) {
			placementFirstSquare( );
		} else {
			placementFirstCircle( );
		}
		circle = !circle;
	}
}

void CloudEvent::creates( int num ) {
	EXCEPTION( num < 12, "warning num" );
	for ( int i = 0; i < num; i++ ) {
		int idx = i;
		_cloud_factory->addDeploy( );
	}
}

void CloudEvent::placementFirstCircle( ) {
	double radius = 16.0 * _clouds_manager->getMaxNum( );
	for ( int i = 0; i < _clouds_manager->getMaxNum( ); i++ ) {
		double angle = ( double )i * ( 360.0 * 2 / _clouds_manager->getMaxNum( ) );
		double rad = angle * ( PI / 360 );
		Vector2 pos;
		pos.x = radius * cos( rad );
		pos.y = radius * sin( rad );
		_clouds_manager->setPos( i, _base_pos.add( pos ) );
	}
}

void CloudEvent::placementFirstSquare( ) {
	int max_num = _clouds_manager->getMaxNum( );
	int area = max_num / 4;
	Vector2 new_pos;
	int idx = 0;
	//top
	for ( int i = 0; i < area; i++ ) {
		new_pos.x = ( i + 1 ) * ( CLOUD_RADIUS_SIZE * 2 );
		_clouds_manager->setPos( idx, _base_pos.add( new_pos ) );
		idx++;
	}
	//bottom
	for ( int i = 0; i < area; i++ ) {
		new_pos.x = i * ( CLOUD_RADIUS_SIZE * 2 );
		new_pos.y = area * ( CLOUD_RADIUS_SIZE * 2 );
		_clouds_manager->setPos( idx, _base_pos.add( new_pos ) );
		idx++;
	}
	//left
	for ( int i = 0; i < area; i++ ) {
		new_pos.x = 0;
		new_pos.y = i * ( CLOUD_RADIUS_SIZE * 2 );
		_clouds_manager->setPos( idx, _base_pos.add( new_pos ) );
		idx++;
	}
	//right
	for ( int i = 0; i < area; i++ ) {
		new_pos.x = area * ( CLOUD_RADIUS_SIZE * 2 );
		new_pos.y = ( i + 1 ) * ( CLOUD_RADIUS_SIZE * 2 );
		_clouds_manager->setPos( idx, _base_pos.add( new_pos ) );
		idx++;
	}
	//中心の調整
	Vector2 sub_pos( ( area * ( CLOUD_RADIUS_SIZE * 2 ) ) / 2,
					 ( area * ( CLOUD_RADIUS_SIZE * 2 ) ) / 2 );
	for ( int i = 0; i < max_num; i++ ) {
		Vector2 now_pos = _clouds_manager->getPos( i );
		_clouds_manager->setPos( i, now_pos.sub( sub_pos ) );
	}
}

std::vector< Vector2 > CloudEvent::placementCircle( ) {
	//マジックナンバーを使用（計算方法がわからん）16
	//最大数に応じて半径を変更
	std::vector<Vector2> next_pos;
	double radius = 16.0 * _clouds_manager->getMaxNum( );
	for ( int i = 0; i < _clouds_manager->getMaxNum( ); i++ ) {
		double angle = ( double )i * ( 360.0 * 2 / _clouds_manager->getMaxNum( ) );
		double rad = angle * ( PI / 360 );
		Vector2 pos;
		pos.x = radius * cos( rad );
		pos.y = radius * sin( rad );

		next_pos.push_back( _base_pos.add( pos ) );
	}
	return next_pos;
}

std::vector< Vector2 > CloudEvent::placementSquare( ) {
	int max_num = _clouds_manager->getMaxNum( );
	std::vector< Vector2 > next_pos( max_num );
	int area = max_num / 4;
	Vector2 new_pos;
	int idx = 0;
	//top
	for ( int i = 0; i < area; i++ ) {
		new_pos.x = ( i + 1 ) * ( CLOUD_RADIUS_SIZE * 2 );
		next_pos[ idx ] = _base_pos.add( new_pos );
		idx++;
	}
	//bottom
	for ( int i = 0; i < area; i++ ) {
		new_pos.x = i * ( CLOUD_RADIUS_SIZE * 2 );
		new_pos.y = area * ( CLOUD_RADIUS_SIZE * 2 );
		next_pos[ idx ] = _base_pos.add( new_pos );
		idx++;
	}
	//left
	for ( int i = 0; i < area; i++ ) {
		new_pos.x = 0;
		new_pos.y = i * ( CLOUD_RADIUS_SIZE * 2 );
		next_pos[ idx ] = _base_pos.add( new_pos );
		idx++;
	}
	//right
	for ( int i = 0; i < area; i++ ) {
		new_pos.x = area * ( CLOUD_RADIUS_SIZE * 2 );
		new_pos.y = ( i + 1 ) * ( CLOUD_RADIUS_SIZE * 2 );
		next_pos[ idx ] = _base_pos.add( new_pos );
		idx++;
	}
	//中心の調整
	Vector2 sub_pos( ( area * ( CLOUD_RADIUS_SIZE * 2 ) ) / 2,
					 ( area * ( CLOUD_RADIUS_SIZE * 2 ) ) / 2 );
	std::vector< Vector2 > ret;
	for ( int i = 0; i < max_num; i++ ) {
		Vector2 now_pos = next_pos[ i ];
		ret.push_back( now_pos.sub( sub_pos ) );
	}
	return ret;
}

void CloudEvent::placementNew( ) {
	//テスト段階の形です。
	int idx = 0;
	int max_num = _clouds_manager->getMaxNum( );
	int area = max_num / 3;
	Vector2 new_pos;
	double radius = 16.0 * _clouds_manager->getMaxNum( );
	for ( int i = 0; i < max_num / 2; i++ ) {
		double angle = ( double )i * ( 360.0 * 2 / ( max_num / 2 ) ) - 180;
		double rad = angle * ( PI / 360 );
		Vector2 pos;
		pos.x = radius * cos( rad );
		pos.y = radius * sin( rad );
		Vector2 right_pos( ( pos.x + SCREEN_WIDTH / 15 ), pos.y );
		_clouds_manager->setPos( idx, _base_pos.add( right_pos ) );
		idx++;
	}
	for ( int i = 0; i < max_num / 2; i++ ) {
		double angle = ( double )i * ( 360.0 * 2 / ( max_num / 2 ) ) + 180;
		double rad = angle * ( PI / 360 );
		Vector2 pos;
		pos.x = radius * cos( rad );
		pos.y = radius * sin( rad );

		Vector2 left_pos( ( pos.x - SCREEN_WIDTH / 15 ), pos.y );
		_clouds_manager->setPos( idx, _base_pos.add( left_pos ) );
		idx++;
	}
}

void CloudEvent::makeHole( int array_size, std::vector< int > ary ) {
	for ( int i = 0; i < array_size; i++ ) {
		int idx = ary[ i ];
		_clouds_manager->setValid( idx, false );
	}
}

void CloudEvent::moveGrade( MOVE_GRADE move_grade ) {
	//向きとスピード
	static double rad = _speed * ( PI / 360 );
	static bool dir = false;
	ApplicationPtr app = Task::get< Application >( );
	switch ( move_grade ) {
	case MOVE_GRADE::BEGINNER:
		if ( _game_system->isLevelUp( ) ) {
			//ランダムに回転方向変更
			rad = Tool::getRand( 2 ) ? 1.0 * ( PI / 360 ) : -1.0 * ( PI / 360 );
		}
		break;
	case MOVE_GRADE::NOVICE:
		//１０秒毎に向きが変わる
		if ( app->getNowCount( ) - _start_millisecont > 10000 ) {
			_start_millisecont = app->getNowCount( );
			dir = !dir;
		}
		rad = dir ? _speed * ( PI / 360 ) : -_speed * ( PI / 360 );
		break;
	case MOVE_GRADE::DIFFICULT:
		static int change_timing = 5000;
		//3～10秒で変化
		if ( app->getNowCount( ) - _start_millisecont > change_timing ) {
			change_timing = 3000 + Tool::getRand( 7000 );
			_start_millisecont = app->getNowCount( );
			//type = Tool::getRand( 3 );
			dir = !dir;
			_speed = 1 + Tool::getRand( 3 );
		}
		rad = dir ? _speed * ( PI / 360 ) : -_speed * ( PI / 360 );
	}

	for ( int i = 0; i < _clouds_manager->getMaxNum( ); i++ ) {
		int idx = i;
		Vector2 now_pos = _clouds_manager->getPos( idx );
		Vector2 new_pos;
		new_pos.x =
			( now_pos.x - _base_pos.x ) * cos( rad ) -
			( now_pos.y - _base_pos.y ) * sin( rad );
		new_pos.y =
			( now_pos.x - _base_pos.x ) * sin( rad ) +
			( now_pos.y - _base_pos.y ) * cos( rad );
		Vector2 base_pos( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 );
		_clouds_manager->setPos( idx, new_pos.add( base_pos ) );
	}

	//回転方向を保存
	CloudsManager::DIR_ROTATE now_dir_rotate;
	if ( rad > 0 ) {
		now_dir_rotate = CloudsManager::DIR_ROTATE::LEFT;
	} else if ( rad < 0 ) {
		now_dir_rotate = CloudsManager::DIR_ROTATE::RIGHT;
	} else {
		now_dir_rotate = CloudsManager::DIR_ROTATE::STOP;
	}
	_clouds_manager->setRotateDir( now_dir_rotate );
	
}

void CloudEvent::moveAnimation( ) {
	for ( int i = 0; i < _clouds_manager->getMaxNum( ); i++ ) {
		Vector2 pos = _clouds_manager->getPos( i ) + _vec[ i ];
		_clouds_manager->setPos( i, pos );
	}
}
void CloudEvent::moveFormCircle( ) {
	CloudsManagerPtr square_pos = _clouds_manager;
	_moved_pos = placementCircle( );
	for ( int i = 0; i < _clouds_manager->getMaxNum( ); i++ ) {
		Vector2 new_square_pos = square_pos->getPos( i );
		Vector2 new_circle_pos = _moved_pos.at( i );

		Vector2 diff = new_square_pos - new_circle_pos;

		_vec.push_back( Vector2( -diff.x / 50, -diff.y / 50 ) );
	}
	_animation = true;
}

void CloudEvent::moveFormSquare( ) {
	CloudsManagerPtr circle_pos = _clouds_manager;
	_moved_pos = placementSquare( );
	for ( int i = 0; i < _clouds_manager->getMaxNum( ); i++ ) {
		Vector2 new_square_pos = _moved_pos.at( i );
		Vector2 new_circle_pos = circle_pos->getPos( i );

		Vector2 diff = new_circle_pos - new_square_pos;

		_vec.push_back( Vector2( -diff.x / 50, -diff.y / 50 ) );
	}
	_animation = true;
}

bool CloudEvent::isAnimationEnd( ) {
	for ( int i = 0; i < _clouds_manager->getMaxNum( ); i++ ) {
		Vector2 diff;
		diff.x = abs( _clouds_manager->getPos( i ).x - _moved_pos[ i ].x );
		diff.y = abs( _clouds_manager->getPos( i ).y - _moved_pos[ i ].y );
		return diff.x < 2 && diff.y < 2;
	}
	return false;
}


void CloudEvent::switchFormFlag( ) {
	if ( _form_count_timer > DEFORMATION_TIME ) {
		_form_switch = !_form_switch;
		_form_count_timer = 0;
	} else {
		_form_count_timer++;
	}
}

void CloudEvent::switchForm( ) {
	if ( _form_count_timer <= DEFORMATION_TIME ) {
		return;
	}
	if ( _form_switch == true ) {
		moveFormCircle( );
		_form_circle = true;
		_form_square = false;
	} else {
		moveFormSquare( );
		_form_circle = false;
		_form_square = true;
	}
}