#include "Menu.h"
#include "Controller.h"
#include "StageManager.h"
#include "Music.h"
#include "Define.h"

Menu::Menu( ControllerPtr controller ) :
	_controller( controller ) {

	_menu = false;
	_retire_mode = false;
	_confirm_select = false;
	_retire = false;
	_operation_mode = false;
	_credit_mode = false;
	_select = 0;
}

Menu::~Menu( ) {
}

void Menu::update( ) {
	if ( !_retire_mode && !_operation_mode && !_credit_mode ) {
		changeMenu( );
	}
	if ( _menu ) {
		if ( _retire_mode ) {
			confirmRetire( );
		} else if ( _operation_mode ) {
			operationChange( );
		} else if ( _credit_mode ) {
			creditChange( );
		} else {
			choiseChange( );
			choose( );
		}
	} else {
		_retire_mode = false;
		_confirm_select = false;
		_retire = false;
		_operation_mode = false;
		_credit_mode = false;
		_select = 0;
	}

}

bool Menu::isMenu( ) const {
	return _menu;
}

void Menu::changeMenu( ) {
	StageManagerPtr manager = Task::get< StageManager >( );
	MusicPtr music = Task::get< Music >( );
	if ( manager->getStageNum( ) != STAGE_NUM ) {
		if ( _controller->getPushX( ) || ( _menu && _controller->getPushB( ) ) ) {
			music->cancelSE( );
			_menu = !_menu;
			_select = 0;
		}
	}
}


void Menu::choose( ) {
	MusicPtr music = Task::get< Music >( );
	if ( _controller->getPushA( ) ) {
		switch ( _select ) {
		case 0:
			music->cancelSE( );
			_menu = false;
			break;
		case 1:
			music->decisionSE( );
			_retire_mode = true;
			break;
		case 2:
			music->decisionSE( );
			_operation_mode = true;
			break;
		case 3:
			music->decisionSE( );
			_credit_mode = true;
			break;
		}
	}
}

void Menu::choiseChange( ) {
	MusicPtr music = Task::get< Music >( );
	if ( !_controller->isTilted( ) ) {
		if ( _controller->getStickDown( ) && _select < 3 ) {
			music->cursorSE( );
			_select++;
		}
		if ( _controller->getStickUp( ) && _select > 0 ) {
			music->cursorSE( );
			_select--;
		}
	}
}

void Menu::confirmRetire( ) {
	MusicPtr music = Task::get< Music >( );
	if ( !_controller->isTilted( ) ) {
		if ( _controller->getStickLeft( ) ) {
			music->cursorSE( );
			_confirm_select = true;
		}
		if ( _controller->getStickRight( ) ) {
			music->cursorSE( );
			_confirm_select = false;
		}
	}
	if ( _controller->getPushA( ) ) {
		if ( _confirm_select ) {
			music->decisionSE( );
			_retire = true;
		} else {
			music->cancelSE( );
			_retire_mode = false;
		}
	}
	if ( _controller->getPushB( ) ) {
		music->cancelSE( );
		_retire_mode = false;
		_confirm_select = false;
	}
}

void Menu::operationChange( ) {
	MusicPtr music = Task::get< Music >( );
	if ( _controller->getPushA( ) || _controller->getPushB( ) ) {
		music->cancelSE( );
		_operation_mode = false;
	}
}

void Menu::creditChange( ) {
	MusicPtr music = Task::get< Music >( );
	if ( _controller->getPushA( ) || _controller->getPushB( ) ) {
		music->cancelSE( );
		_credit_mode = false;
	}
}

void Menu::menuClose( ) {
	_menu = false;
}

int Menu::getSelectingButton( ) {
	return _select;
}

bool Menu::isRetire( ) const {
	return _retire;
}

bool Menu::isRetireMode( ) const {
	return _retire_mode;
}

bool Menu::isRetireConfirm( ) const {
	return _confirm_select;
}

bool Menu::isOperationMode( ) const {
	return _operation_mode;
}

bool Menu::isCreditMode( ) const {
	return _credit_mode;
}
