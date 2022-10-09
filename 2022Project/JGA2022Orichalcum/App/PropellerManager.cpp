#include "PropellerManager.h"
#include "define.h"
#include "DeviceController.h"
#include "Application.h"

static const double SPEED = 5.0;
static const double ACCEL_SPEED = 0.5;
static const double ACCEL_LIMIT = 10.0;
static const int WING_LENGTH = 9;
static const int DECELERATION_INTERVAL = 5000;


PropellerManager::PropellerManager( DeviceControllerPtr device_controller ) {

	_status.shaft_pos = Vector2( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 );
	
	for ( int i = 0; i < WING_LENGTH; i++ ) {
		Wing wing1;
		wing1.pos = _status.shaft_pos - Vector2( 0.0, i * ( wing1.size * 1.50 ) );
		_status.wing1.push_back( wing1 );
		Wing wing2;
		wing2.pos = _status.shaft_pos + Vector2( i * ( wing2.size * 1.25 ), i * wing2.size );
		_status.wing2.push_back( wing2 );
		Wing wing3;
		wing3.pos = _status.shaft_pos + Vector2( -( i * ( wing3.size * 1.25 ) ), i * wing3.size );
		_status.wing3.push_back( wing3 );
	}
	_rotate_accel = 0.0;
	_before_pushed_time = 0;
}

PropellerManager::~PropellerManager( ) {
}

void PropellerManager::update( ) {
	rotateWings( );
	move( );
}

PropellerManager::Status PropellerManager::getStatus( ) const {
	return _status;
}

double PropellerManager::getRotateAccel( ) const {
	return _rotate_accel;
}

void PropellerManager::rotateWings( ) {
	bool push_a = _device->isPressedA( );
	bool push_b = _device->isPressedB( );

	ApplicationPtr app = Application::getInstance( );

	if ( push_a ) {
		_before_pushed_time = app->getNowCount( );
		if ( _rotate_accel < ACCEL_LIMIT ) {
			_rotate_accel += ACCEL_SPEED;
		}
	}

	if ( push_b ) {
		_before_pushed_time = app->getNowCount( );
		if ( _rotate_accel > -ACCEL_LIMIT ) {
			_rotate_accel += -ACCEL_SPEED;
		}
	}

	if ( !push_a && !push_b ) {
		int now = app->getNowCount( );
		if ( now >= _before_pushed_time + DECELERATION_INTERVAL ) {
			if ( _rotate_accel < 0 ) {
				_rotate_accel += ACCEL_SPEED;
			} 
			if( _rotate_accel > 0 ) {
				_rotate_accel += -ACCEL_SPEED;
			}
		}
	}

	//回転スピードradianを指定
	double speed_rad = _rotate_accel * ( PI / 360 );

	int wing_num = ( int )_status.wing1.size( );

	for ( int i = 0; i < wing_num; i++ ) {
		int idx = i;
		Vector2 now_pos = _status.wing1[ i ].pos;
		Vector2 new_pos;
		new_pos.x =
			( now_pos.x - _status.shaft_pos.x ) * cos( speed_rad ) -
			( now_pos.y - _status.shaft_pos.y ) * sin( speed_rad );
		new_pos.y =
			( now_pos.x - _status.shaft_pos.x ) * sin( speed_rad ) +
			( now_pos.y - _status.shaft_pos.y ) * cos( speed_rad );

		Vector2 base_pos( _status.shaft_pos.x, _status.shaft_pos.y );
		_status.wing1[ i ].pos = new_pos.add( base_pos );

		now_pos = _status.wing2[ i ].pos;

		new_pos.x =
			( now_pos.x - _status.shaft_pos.x ) * cos( speed_rad ) -
			( now_pos.y - _status.shaft_pos.y ) * sin( speed_rad );
		new_pos.y =
			( now_pos.x - _status.shaft_pos.x ) * sin( speed_rad ) +
			( now_pos.y - _status.shaft_pos.y ) * cos( speed_rad );
		_status.wing2[ i ].pos = new_pos.add( base_pos );

		now_pos = _status.wing3[ i ].pos;

		new_pos.x =
			( now_pos.x - _status.shaft_pos.x ) * cos( speed_rad ) -
			( now_pos.y - _status.shaft_pos.y ) * sin( speed_rad );
		new_pos.y =
			( now_pos.x - _status.shaft_pos.x ) * sin( speed_rad ) +
			( now_pos.y - _status.shaft_pos.y ) * cos( speed_rad );
		_status.wing3[ i ].pos = new_pos.add( base_pos );
	}
}

void PropellerManager::move( ) {
	Vector2 vec;
	bool input = _device->isLeftStickLeftDown( );
	if ( input ) {
		vec.x += -SPEED;
	}

	input = _device->isLeftStickRightDown( );
	if ( input ) {
		vec.x += SPEED;
	}

	input = _device->isLeftStickLowerDown( );
	if ( input ) {
		vec.y += -SPEED;
	}

	input = _device->isLeftStickUpperDown( );
	if ( input ) {
		vec.y += SPEED;
	}
	
	_status.shaft_pos += vec;

	int wing_num = ( int )_status.wing1.size( );
	for ( int i = 0; i < wing_num; i++ ) {
		_status.wing1[ i ].pos += vec;
		_status.wing2[ i ].pos += vec;
		_status.wing3[ i ].pos += vec;
	}

}
