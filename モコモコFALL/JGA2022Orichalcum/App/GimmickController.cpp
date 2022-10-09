#include "GimmickController.h"
#include "GimmickManager.h"


GimmickController::GimmickController( GimmickManagerPtr manager ) :
	_manager( manager ) {
}

GimmickController::~GimmickController( ) {
}

int GimmickController::getGimmickNum( ) {
	return _manager->getMaxNum( );
}

Vector2 GimmickController::getPos( int idx ) const {
	return _manager->getPos( idx );
}

double GimmickController::getSize( int idx ) const {
	return  _manager->getSize( idx );
}

Gimmick::TYPE GimmickController::getType( int idx ) const {
	return _manager->getType( idx );
}

ColorBall::COLOR_TYPE GimmickController::getColorType( int idx ) const {
	return  _manager->getColorType( idx );
}
