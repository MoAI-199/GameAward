#include "CloudsController.h"
#include "CloudsManager.h"

CloudsController::CloudsController( CloudsManagerPtr manager ) :
	_manager( manager ) {

}

CloudsController::~CloudsController( ) {
}

Vector2 CloudsController::getPos( int idx ) const {
	return _manager->getPos( idx );
}

double CloudsController::getScale( int idx ) const {
	return _manager->getScale( idx );
}

int CloudsController::getMaxSize( ) const {
	return _manager->getMaxNum( );
}

bool CloudsController::getValid( int idx ) const {
	return _manager->getValid( idx );
}



