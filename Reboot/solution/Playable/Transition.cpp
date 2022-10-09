#include "Transition.h"
#include "Image.h"
#include "Drawer.h"
#include "Define.h"

Transition::Transition( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_iris = drawer->createImage( "Resource/iris_2.png" );
	_transition = false;
	_scaling = true;
	_finished = false;
	_stop = false;
	_scale = IRIS_SCALE_LIMIT;

}

Transition::~Transition( ) {
}

void Transition::initialize( ) {
}

void Transition::update( ) {
	if ( !_transition ) {
		return;
	}
	//drawFinishedTransition( );
	switch ( _type ) {
	case Transition::TYPE::IRIS:
		drawIris( );
		break;
	case Transition::TYPE::FADE:
		drawFade( );
		break;
	}
}

bool Transition::isTransition( ) {
	return _transition;
}

bool Transition::isScaling( ) {
	return _scaling;
}

bool Transition::isStop( ) {
	return _stop;
}

void Transition::drawIris( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );
	_iris->drawExtend( app->getScreenWidth( ) * -_scale + _target_sx - _cx,
						 app->getScreenHeight( ) * -_scale + _target_sy - _cy,
						 app->getScreenWidth( ) * _scale + _target_sx - _cx,
						 app->getScreenHeight( ) * _scale + _target_sy - _cy );

	if ( _io == IO::OUT ) {

		if ( _circle_ratio > _scale && _stop_time != 0 ) {
			_stop_time--;
			_stop = true;
		} else {
			_scale -= _speed;
		}

		if ( _scale < 1 ) {
			_scale = 1;
			drawer->drawCircle( _target_sx - _cx, _target_sy - _cy, 100, Color( 0, 0, 0 ), true );
			_scaling = false;
			_finished = true;
		}
	} else {

		if ( _circle_ratio < _scale && _stop_time != 0 ) {
			_stop_time--;
		} else {
			_scale += _speed;
		}

		if ( _scale > IRIS_SCALE_LIMIT ) {
			_scale = IRIS_SCALE_LIMIT;
			_scaling = false;
			_transition = false;
		}
	}
}

void Transition::drawFade( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );
	if ( _io == IO::OUT ) {
		_ratio += _speed;
		if ( _ratio >= 1.0 ) {
			_scaling = false;
			_finished = true;
		}
	} else {
		_ratio -= _speed;
		if ( _ratio <= 0.0 ) {
			_scaling = false;
			_transition = false;
		}
	}

	Drawer::Scope scope = drawer->createScope( );
	drawer->setBlend( scope, Drawer::BLEND::ALPHA, _ratio );
	if ( _color == COLOR::WHITE ) {
		drawer->drawBox( 0, 0, app->getScreenWidth( ), app->getScreenHeight( ), Color( 1, 1, 1 ), true );
	} else {
		drawer->drawBox( 0, 0, app->getScreenWidth( ), app->getScreenHeight( ), Color( 0, 0, 0 ), true );
	}
}

void Transition::drawFinishedTransition( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );

	if ( _finished ) {
		drawer->drawBox( 0, 0, app->getScreenWidth( ), app->getScreenHeight( ), Color( 1, 0, 0 ), true );
	}
}

void Transition::setIrisIn( int sx, int sy, int camera_x, int camera_y, double speed, int stop_time, int circle_ratio ) {
	_transition = true;
	_scaling = true;

	_target_sx = sx;
	_target_sy = sy;
	_cx = camera_x;
	_cy = camera_y;
	_speed = speed;
	_stop_time = stop_time;
	_circle_ratio = circle_ratio;

	_io = IO::IN;
	_scale = 0;
	_type = TYPE::IRIS;
}

void Transition::setIrisOut( int sx, int sy, int camera_x, int camera_y, double speed, int stop_time, int circle_ratio ) {
	_transition = true;
	_scaling = true;

	_target_sx = sx;
	_target_sy = sy;
	_cx = camera_x;
	_cy = camera_y;
	_speed = speed;
	_stop_time = stop_time;
	_circle_ratio = circle_ratio;

	_io = IO::OUT;
	_scale = IRIS_SCALE_LIMIT;
	_type = TYPE::IRIS;
}

void Transition::setFadeIn( double speed, COLOR color ) {
	_transition = true;
	_scaling = true;

	_color = color;
	_speed = speed;

	_io = IO::IN;
	_ratio = 1.0;
	_type = TYPE::FADE;
}

void Transition::setFadeOut( double speed, COLOR color ) {
	_transition = true;
	_scaling = true;

	_color = color;
	_speed = speed;

	_io = IO::OUT;
	_ratio = 0.0;
	_type = TYPE::FADE;
}
