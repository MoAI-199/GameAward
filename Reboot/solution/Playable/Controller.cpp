#include "Controller.h"
#include "Device.h"

Controller::Controller( ) {
	_tilted = false;
}

Controller::~Controller( ) {

}

void Controller::update( ) {
	getPushA( );
	getPushB( );
	getPushX( );
	getPushY( );

	getStickUp( );
	getStickDown( );
	getStickLeft( );
	getStickRight( );

	updateTilted( );
}

bool Controller::getPushA( ) const {
	DevicePtr device = Task::get< Device >( );
	if ( device->getPush( ) & BUTTON_A ) {
		return true;
	} else {
		return false;
	}
}

bool Controller::getPushB( ) const {
	DevicePtr device = Task::get< Device >( );
	if ( device->getPush( ) & BUTTON_B ) {
		return true;
	} else {
		return false;
	}
}

bool Controller::getPushX( ) const {
	DevicePtr device = Task::get< Device >( );
	if ( device->getPush( ) & BUTTON_C ) {
		return true;
	} else {
		return false;
	}

}

bool Controller::getPushY( ) const {
	DevicePtr device = Task::get< Device >( );
	if ( device->getPush( ) & BUTTON_D ) {
		return true;
	} else {
		return false;
	}
}

bool Controller::getHoldB( ) const {
	DevicePtr device = Task::get< Device >( );
	if ( device->getButton( ) & BUTTON_B ) {
		return true;
	} else {
		return false;
	}
}

bool Controller::getStickUp( ) const {
	DevicePtr device = Task::get< Device >( );
	if ( device->getDirY( ) < 0 ) {
		return true;
	} else {
		return false;
	}
}

bool Controller::getStickDown( ) const {
	DevicePtr device = Task::get< Device >( );
	if ( device->getDirY( ) > 0 ) {
		return true;
	} else {
		return false;
	}
}

bool Controller::getStickLeft( ) const {
	DevicePtr device = Task::get< Device >( );
	if ( device->getDirX( ) < 0 ) {
		return true;
	} else {
		return false;
	}
}

bool Controller::getStickRight( ) const {
	DevicePtr device = Task::get< Device >( );
	if ( device->getDirX( ) > 0 ) {
		return true;
	} else {
		return false;
	}
}


void Controller::updateTilted( ) {
	DevicePtr device = Task::get< Device >( );
	if ( device->getDirX( ) == 0 &&
		 device->getDirY( ) == 0 ) {
		_tilted = false;
	} else {
		_tilted = true;
	}
}

void Controller::setTilted( bool tilted ) {
	_tilted = tilted;
}

bool Controller::isTilted( ) {
	return _tilted;
}