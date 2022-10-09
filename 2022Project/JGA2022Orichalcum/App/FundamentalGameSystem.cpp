#include "FundamentalGameSystem.h"
#include "mathematics.h"
#include "define.h"
#include "CloudsManager.h"
#include "SheepsManager.h"
#include "sheepFactory.h"
#include "Log.h"
#include "Tool.h"
#include "Keyboard.h"

FundamentalGameSystem::FundamentalGameSystem( SheepsManagerPtr sheeps_manager,
											  CloudsManagerPtr clouds_manager,
											  SheepFactoryPtr sheep_factory ) :
	_sheeps_manager( sheeps_manager ),
	_clouds_manager( clouds_manager ),
	_sheep_factory( sheep_factory ) {
	_now_level = 0;
	_moko_rate = 0;
	_levelup = false;
	_interval = false;
}

FundamentalGameSystem::~FundamentalGameSystem( ) {
}

void FundamentalGameSystem::update( ) {
	updateMokoRate( );
	updateInterval( );
	_levelup = false;
	if ( _moko_rate > 90 ) {
		_levelup = true;
	}

	#if debug
	KeyboardPtr key = Task::get< Keyboard >( );
	if ( key->isPushKey( "L" ) ) {
		upLevel( );
		_levelup = true;
	} else {
		_levelup = false;
	}
	#endif
}

bool FundamentalGameSystem::isLevelUp( ) const {
	return _levelup;
}

bool FundamentalGameSystem::isInterval( ) const {
	return _interval;
}

int FundamentalGameSystem::getLevel( ) const {
	return _now_level;
}

int FundamentalGameSystem::getMokoRate( ) const {
	return _moko_rate;
}



void FundamentalGameSystem::updateInterval( ) {
	ApplicationPtr app = Task::get< Application >( );
	if ( !_interval ) {
		_now_millisecond = app->getNowCount( );
		return;
	}
	//３秒間のインターバル
	if ( _now_millisecond < app->getNowCount( ) - 3000 ) {
		_interval = false;
	}
}

void FundamentalGameSystem::updateMokoRate( ) {
	#if debug
	LogPtr log = Task::get< Log >( );
	log->status( "rate:%d\n", _moko_rate );
	log->status( "level:%d\n", _now_level );
	#endif
	double rate = 0;
	double inside_num = 0;
	Vector2 org_pos( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 );//形態の原点

	//形態の半径
	double from_radius = ( _clouds_manager->getPos( 0 ) - org_pos ).getLength( );
	//形態の面積
	double from_area = from_radius * from_radius * PI;

	//すべての羊の面積
	double sheep_all_area = 0.0;
	for ( int i = 0; i < _sheeps_manager->getMaxNum( ); i++ ) {
		int idx = i;
		double sheep_radius = _sheeps_manager->getSize( idx );
		sheep_all_area += sheep_radius * sheep_radius * PI;
	}

	for ( int i = 0; i < _sheeps_manager->getMaxNum( ); i++ ) {
		int idx = i;
		//中心からの羊の距離
		double length = ( org_pos - _sheeps_manager->getPos( idx ) ).getLength( );
		if ( length < from_radius ) {
			inside_num++;
		}
	}
	rate = ( double )( sheep_all_area / from_area * 100.0 );
	setMokoRate( rate );
}

void FundamentalGameSystem::upLevel( ) {
	_now_level++;
	_interval = true;
}

void FundamentalGameSystem::setMokoRate( double rate ) {
	_moko_rate = ( int )rate;
}


