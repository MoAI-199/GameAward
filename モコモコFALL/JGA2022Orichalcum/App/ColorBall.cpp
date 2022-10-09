#include "ColorBall.h"

ColorBall::ColorBall( ) {
	_color_type = COLOR_TYPE::NONE;
}

ColorBall::~ColorBall( ) {
}

void ColorBall::setColorType( COLOR_TYPE type ) {
	_color_type = type;
}

ColorBall::COLOR_TYPE ColorBall::getColorType( ) const {
	return _color_type;
}
