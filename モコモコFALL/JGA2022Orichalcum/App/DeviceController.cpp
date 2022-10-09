#include "DeviceController.h"
#include "Keyboard.h"
#include "Device.h"


DeviceController::DeviceController( ) {
}

DeviceController::~DeviceController( ) {
}

bool DeviceController::isHoldArrowUp( ) const {
    KeyboardPtr key = Task::get< Keyboard >( );
    return key->isHoldKey( "ARROW_UP");
}

bool DeviceController::isHoldArrowDown( ) const {
    KeyboardPtr key = Task::get< Keyboard >( );
    return key->isHoldKey( "ARROW_DOWN" );
}

bool DeviceController::isHoldArrowLeft( ) const {
    KeyboardPtr key = Task::get< Keyboard >( );
    return key->isHoldKey( "ARROW_LEFT" );
}

bool DeviceController::isHoldArrowRight( ) const {
    KeyboardPtr key = Task::get< Keyboard >( );
    return key->isHoldKey( "ARROW_RIGHT" );
}

bool DeviceController::isLeftStickRightDown( ) const {
    DevicePtr device = Task::get< Device >( );
    char data = device->getDirX( );
    if ( data > 0 ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isLeftStickLeftDown( ) const {
    DevicePtr device = Task::get< Device >( );
    char data = device->getDirX( );
    if ( data < 0 ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isLeftStickUpperDown( ) const {
    DevicePtr device = Task::get< Device >( );
    char data = device->getDirY( );
    if ( data > 0 ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isLeftStickLowerDown( ) const {
    DevicePtr device = Task::get< Device >( );
    char data = device->getDirY( );
    if ( data < 0 ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isRightStickLeftDown( ) const {
    DevicePtr device = Task::get< Device >( );
    char data = device->getRightDirX( );
    if ( data > 0 ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isRightStickRightDown( ) const {
    DevicePtr device = Task::get< Device >( );
    char data = device->getRightDirX( );
    if ( data < 0 ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isRightStickUpperDown( ) const {
    DevicePtr device = Task::get< Device >( );
    char data = device->getRightDirY( );
    if ( data > 0 ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isRightStickLowerDown( ) const {
    DevicePtr device = Task::get< Device >( );
    char data = device->getRightDirY( );
    if ( data < 0 ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isPressedA( ) const {
    DevicePtr device = Task::get< Device >( );
    bool pressed = device->getPush( ) & BUTTON_A;
    if ( pressed ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isPressedB( ) const {
    DevicePtr device = Task::get< Device >( );
    bool pressed = device->getPush( ) & BUTTON_B;
    if ( pressed ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isPressedC( ) const {
    DevicePtr device = Task::get< Device >( );
    bool pressed = device->getPush( ) & BUTTON_C;
    if ( pressed ) {
        return true;
    } else {
        return false;
    }
}

bool DeviceController::isPressedD( ) const {
    DevicePtr device = Task::get< Device >( );
    bool pressed = device->getPush( ) & BUTTON_D;
    if ( pressed ) {
        return true;
    } else {
        return false;
    }
}
