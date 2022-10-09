#include "Sheep.h"

static const double ROTATE_SPEED = 0.1;
static const double SHEEP_INIT_SIZE = 50.0;

Sheep::Sheep( ) {
}

Sheep::~Sheep( ) {
}

void Sheep::update( ) {
	//_status.pos += _status.vec;
	if ( _status.vec.x < 0 ) {
		_status.angle += -ROTATE_SPEED;
	} else {
		_status.angle += ROTATE_SPEED;
	}
}

void Sheep::setStatus( SheepStatus status ) {
	_status = status;
}

Vector2 Sheep::getPos( ) const {
	return _status.pos;
}

double Sheep::getSize( ) const {
	return SHEEP_INIT_SIZE * _status.scale;
}

double Sheep::getAngle( ) const {
	return _status.angle;
}

Vector2 Sheep::getVec( ) const {
	return _status.vec;
}

Sheep::COLOR_TYPE Sheep::getColorType( ) const {
	return _status.color_type;
}

double Sheep::getViewSize( ) const {
	return _status.view_size;
}

double Sheep::getScale( ) const {
	return _status.scale;
}

bool Sheep::isColorStrage( COLOR_TYPE type ) const {
	return _status.color_storage.at( type );
}

void Sheep::setScale( double scale ) {
	_status.scale = scale;
}

void Sheep::setPos( Vector2 pos ) {
	_status.pos = pos;
}

void Sheep::setVec( Vector2 vec ) {
	//ˆê’è”ˆÈã‚à‚µ‚­‚ÍˆÈ‰º‚É‚È‚Á‚½‚çmax_speed‚ÅŒÅ’è‚·‚é
	double new_vec_x = vec.x < -MAX_SPEED ? -MAX_SPEED : vec.x;
	double new_vec_y = vec.y < -MAX_SPEED ? -MAX_SPEED : vec.y;
	new_vec_x = vec.x > MAX_SPEED ? MAX_SPEED : vec.x;
	new_vec_y = vec.y > MAX_SPEED ? MAX_SPEED : vec.y;

	vec = Vector2( new_vec_x, new_vec_y );
	_status.vec = vec;
}

void Sheep::setColorType( COLOR_TYPE type ) {
	_status.color_type = type;
	_status.color_storage[ type ] = true;
}

void Sheep::setViewSize( double size ) {
	_status.view_size = size;
}