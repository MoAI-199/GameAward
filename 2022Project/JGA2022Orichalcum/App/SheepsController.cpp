#include "SheepsController.h"
#include "SheepsManager.h"

SheepsController::SheepsController( SheepsManagerPtr manager ) :
	_manager( manager ) {
}

SheepsController::~SheepsController( ) {
}

int SheepsController::getSheepNum( ) const {
	return _manager->getMaxNum( );
}

int SheepsController::getLeadPlayer( ) const {
	return _manager->getLeadPlayer( );
}

double SheepsController::getSize( int idx ) const {
	return _manager->getSize( idx );
}

double SheepsController::getAngle( int idx ) const {
	return _manager->getAngle( idx );
}

Vector2 SheepsController::getPos( int idx ) const {
	return _manager->getPos( idx );
}

Sheep::COLOR_TYPE SheepsController::getColorType( int idx ) const {
	return _manager->getColorType( idx );
}

double SheepsController::getViewSize( int idx ) const {
	return _manager->getViewSize( idx );
}

double SheepsController::getScale( int idx ) const{
	return _manager->getScale( idx );
}
