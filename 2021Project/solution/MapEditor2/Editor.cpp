#include "Editor.h"
#include "GameData.h"
#include "Keyboard.h"
#include "Drawer.h"
#include "Mouse.h"
#include "Define.h"
#include "Camera.h"

Editor::Editor( GameDataPtr data, CameraPtr camera ) :
	_data( data ),
	_camera( camera ) {
	_temp_pos1 = Vector2( );
	_temp_pos2 = Vector2( );
	_mouse_hold_left = false;
	_mouse_hold_right = false;
	_select = -1;
	_save_distance = Vector2( );
	_select_type = 0;
	_mode = MODE::SINGLE;
}

Editor::~Editor( ) {

}

void Editor::update( ) {
	selectCreateType( );
	selectSingle( );
	selectMultiple( );
	switch ( _mode ) {
	case Editor::MODE::SINGLE:
		switch ( _select_type ) {
		case 0:
			createGroundToMouse( );
			moveSelectedGround( );
			tweakSelectedGround( );
			deleteSelectedGround( );
			//createGroundToNumber( );
			break;
		case 8:
			createEnemyToMouse( );
			moveSelectedEnemy( );
			tweakSelectedEnemy( );
			deleteSelectedEnemy( );
			changeSelectedEnemyMovePattern( );
			changeSelectedEnemyVecX( );
			changeSelectedEnemyVecY( );
			break;
		case 9:
			createBetweenObjectToMouse( );
			moveSelectedBetweenObject( );
			tweakSelectedBetweenObject( );
			deleteSelectedBetweenObject( );
			break;
		default:
			createGimmickToMouse( );
			moveSelectedGimmick( );
			tweakSelectedGimmick( );
			deleteSelectedGimmick( );
			changeSelectedGimmickDir( );
			changeSelectedFanData( );
			break;
		}
		break;
	case Editor::MODE::MULTIPLE:
		moveSelectedMultiple( );
		tweakSelectedMultiple( );
		changeSelectedMultipleData( );
		changeSelectedMultipleDir( );
		break;
	}
	save( );
	load( );
	oldLoad( );
	changeWidth( );
	changeHeight( );
}

Vector2 Editor::getTempPos1( ) const {
	return _temp_pos1;
}

Vector2 Editor::getTempPos2( ) const {
	return _temp_pos2;
}

int Editor::getSelect( ) const {
	return _select;
}

int Editor::getSelectType( ) const {
	return _select_type;
}

Editor::MODE Editor::getMode( ) const {
	return _mode;
}

std::vector< int > Editor::getMultipleSelects( ) const {
	return _multiple_selects;
}


void Editor::createGroundToMouse( ) {
	MousePtr mouse = Task::get< Mouse >( );
	if ( mouse->isPushLeftButton( ) ) {
		_temp_pos1 = mouse->getPos( );
	}
	if ( mouse->isHoldLeftButton( ) && _select == -1 ) {
		_mouse_hold_left = true;
	} else if ( _mouse_hold_left ) {
		_temp_pos2 = mouse->getPos( );
		Vector2 size = _temp_pos2 - _temp_pos1;
		if ( size.x > 0 && size.y > 0 ) {
			if ( ( int )size.x % GROUND_IMAGE_SIZE > GROUND_IMAGE_SIZE / 2 ) {
				size = Vector2( ( ( int )( size.x / GROUND_IMAGE_SIZE ) + 1 ) * GROUND_IMAGE_SIZE, size.y );
			} else {
				size = Vector2( ( int )( size.x / GROUND_IMAGE_SIZE ) * GROUND_IMAGE_SIZE, size.y );
				if ( size.x == 0 ) {
					size = Vector2( GROUND_IMAGE_SIZE, size.y );
				}
			}
			if ( ( int )size.y % GROUND_IMAGE_SIZE > GROUND_IMAGE_SIZE / 2 ) {
				size = Vector2( size.x, ( ( int )( size.y / GROUND_IMAGE_SIZE ) + 1 ) * GROUND_IMAGE_SIZE );
			} else {
				size = Vector2( size.x, ( int )( size.y / GROUND_IMAGE_SIZE ) * GROUND_IMAGE_SIZE );
				if ( size.y == 0 ) {
					size = Vector2( size.x, GROUND_IMAGE_SIZE );
				}
			}
			_data->createGround( _temp_pos1 + _camera->getPos( ), size );
			_select = _data->getGroundsSize( ) - 1;
		}
		_mouse_hold_left = false;
	}
}

void Editor::createGroundToNumber( ) {
	MousePtr mouse = Task::get< Mouse >( );
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( mouse->isPushRightButton( ) && _select == -1 ) {
		DrawerConstPtr drawer = Task::getConst< Drawer >( );
		drawer->drawString( 0, 20, "create_Ground_To_Number" );
		drawer->drawString( 0, 50, "width" );
		std::string width_str = Application::getInstance( )->inputString( 0, 80 );
		if ( width_str.size( ) == 0 ) {
			return;
		}
		int width = convertFromStringToNumber( width_str );

		drawer->drawString( 0, 110, "height" );
		std::string height_str = Application::getInstance( )->inputString( 0, 140 );
		if ( height_str.size( ) == 0 ) {
			return;
		}
		int height = convertFromStringToNumber( height_str );
		_data->createGround( mouse->getPos( ), Vector2( width, height ) );

	}
}

void Editor::selectSingle( ) {
	bool select = false;
	MousePtr mouse = Task::get< Mouse >( );
	if ( mouse->isPushLeftButton( ) ) {
		std::vector< GameData::Ground* > grounds = _data->getGrounds( );
		for ( int i = 0; i < grounds.size( ); i++ ) {
			if ( grounds[ i ]->_pos.x <= mouse->getPos( ).x + _camera->getPos( ).x &&
				 grounds[ i ]->_pos.x + grounds[ i ]->_size.x >= mouse->getPos( ).x + _camera->getPos( ).x &&
				 grounds[ i ]->_pos.y <= mouse->getPos( ).y + _camera->getPos( ).y &&
				 grounds[ i ]->_pos.y + grounds[ i ]->_size.y >= mouse->getPos( ).y + _camera->getPos( ).y ) {
				_select = i;
				select = true;
				_select_type = 0;
			}
		}

		std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
		for ( int i = 0; i < gimmicks.size( ); i++ ) {
			if ( gimmicks[ i ]->_pos.x <= mouse->getPos( ).x + _camera->getPos( ).x &&
				 gimmicks[ i ]->_pos.x + gimmicks[ i ]->_size.x >= mouse->getPos( ).x + _camera->getPos( ).x &&
				 gimmicks[ i ]->_pos.y <= mouse->getPos( ).y + _camera->getPos( ).y &&
				 gimmicks[ i ]->_pos.y + gimmicks[ i ]->_size.y >= mouse->getPos( ).y + _camera->getPos( ).y ) {
				_select = i;
				select = true;
				_select_type = ( int )( gimmicks[ i ]->_type ) + 1;
			}
		}

		std::vector< GameData::Fan* > fans = _data->getFans( );
		for ( int i = 0; i < fans.size( ); i++ ) {
			if ( fans[ i ]->_gimmick->_pos.x <= mouse->getPos( ).x + _camera->getPos( ).x &&
				 fans[ i ]->_gimmick->_pos.x + fans[ i ]->_gimmick->_size.x >= mouse->getPos( ).x + _camera->getPos( ).x &&
				 fans[ i ]->_gimmick->_pos.y <= mouse->getPos( ).y + _camera->getPos( ).y &&
				 fans[ i ]->_gimmick->_pos.y + fans[ i ]->_gimmick->_size.y >= mouse->getPos( ).y + _camera->getPos( ).y ) {
				_select = i + _data->getGimmicksSize( );
				select = true;
				_select_type = ( int )( GameData::Gimmick::TYPE::FAN ) + 1;
			}
		}

		std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
		for ( int i = 0; i < enemies.size( ); i++ ) {
			if ( enemies[ i ]->_pos.x <= mouse->getPos( ).x + _camera->getPos( ).x &&
				 enemies[ i ]->_pos.x + enemies[ i ]->_size.x >= mouse->getPos( ).x + _camera->getPos( ).x &&
				 enemies[ i ]->_pos.y <= mouse->getPos( ).y + _camera->getPos( ).y &&
				 enemies[ i ]->_pos.y + enemies[ i ]->_size.y >= mouse->getPos( ).y + _camera->getPos( ).y ) {
				_select = i;
				select = true;
				_select_type = 8;
			}
		}

		std::vector< GameData::Ground* > between_objects = _data->getBetweenObjects( );
		for ( int i = 0; i < between_objects.size( ); i++ ) {
			if ( between_objects[ i ]->_pos.x <= mouse->getPos( ).x + _camera->getPos( ).x &&
				 between_objects[ i ]->_pos.x + between_objects[ i ]->_size.x >= mouse->getPos( ).x + _camera->getPos( ).x &&
				 between_objects[ i ]->_pos.y <= mouse->getPos( ).y + _camera->getPos( ).y &&
				 between_objects[ i ]->_pos.y + between_objects[ i ]->_size.y >= mouse->getPos( ).y + _camera->getPos( ).y ) {
				_select = i;
				select = true;
				_select_type = 9;
			}
		}

		if ( !select ) {
			_select = -1;
		}
		_mode = MODE::SINGLE;
	}
}

void Editor::selectMultiple( ) {
	MousePtr mouse = Task::get< Mouse >( );
	if ( _mode == MODE::SINGLE ) {
		for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
			_multiple_selects.erase( _multiple_selects.begin( ) );
		}
	}
	if ( mouse->isPushRightButton( ) ) {
		_temp_pos1 = mouse->getPos( );
	}
	if ( mouse->isHoldRightButton( ) ) {
		_mouse_hold_right = true;
	} else if ( _mouse_hold_right ) {
		_temp_pos2 = mouse->getPos( );
		Vector2 size = _temp_pos2 - _temp_pos1;
		if ( size.x > 0 && size.y > 0 ) {
			std::vector< GameData::Ground* > grounds = _data->getGrounds( );
			for ( int i = 0; i < grounds.size( ); i++ ) {
				if ( grounds[ i ]->_pos.x >= _temp_pos1.x + _camera->getPos( ).x &&
					 grounds[ i ]->_pos.x + grounds[ i ]->_size.x <= _temp_pos2.x + _camera->getPos( ).x &&
					 grounds[ i ]->_pos.y >= _temp_pos1.y + _camera->getPos( ).y &&
					 grounds[ i ]->_pos.y + grounds[ i ]->_size.y <= _temp_pos2.y + _camera->getPos( ).y ) {
					_multiple_selects.push_back( i );
				}
			}

			std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
			for ( int i = 0; i < gimmicks.size( ); i++ ) {
				if ( gimmicks[ i ]->_pos.x >= _temp_pos1.x + _camera->getPos( ).x &&
					 gimmicks[ i ]->_pos.x + gimmicks[ i ]->_size.x <= _temp_pos2.x + _camera->getPos( ).x &&
					 gimmicks[ i ]->_pos.y >= _temp_pos1.y + _camera->getPos( ).y &&
					 gimmicks[ i ]->_pos.y + gimmicks[ i ]->_size.y <= _temp_pos2.y + _camera->getPos( ).y ) {
					_multiple_selects.push_back( i + grounds.size( ) );
				}
			}

			std::vector< GameData::Fan* > fans = _data->getFans( );
			for ( int i = 0; i < fans.size( ); i++ ) {
				if ( fans[ i ]->_gimmick->_pos.x >= _temp_pos1.x + _camera->getPos( ).x &&
					 fans[ i ]->_gimmick->_pos.x + fans[ i ]->_gimmick->_size.x <= _temp_pos2.x + _camera->getPos( ).x &&
					 fans[ i ]->_gimmick->_pos.y >= _temp_pos1.y + _camera->getPos( ).y &&
					 fans[ i ]->_gimmick->_pos.y + fans[ i ]->_gimmick->_size.y <= _temp_pos2.y + _camera->getPos( ).y ) {
					_multiple_selects.push_back( i + grounds.size( ) + gimmicks.size( ) );
				}
			}

			std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
			for ( int i = 0; i < enemies.size( ); i++ ) {
				if ( enemies[ i ]->_pos.x >= _temp_pos1.x + _camera->getPos( ).x &&
					 enemies[ i ]->_pos.x + enemies[ i ]->_size.x <= _temp_pos2.x + _camera->getPos( ).x &&
					 enemies[ i ]->_pos.y >= _temp_pos1.y + _camera->getPos( ).y &&
					 enemies[ i ]->_pos.y + enemies[ i ]->_size.y <= _temp_pos2.y + _camera->getPos( ).y ) {
					_multiple_selects.push_back( i + grounds.size( ) + gimmicks.size( ) + fans.size( ) );
				}
			}

		}
		if ( _multiple_selects.size( ) != 0 ) {
			_mode = MODE::MULTIPLE;
		}
		_mouse_hold_right = false;
	}
}

void Editor::moveSelectedGround( ) {
	MousePtr mouse = Task::get< Mouse >( );
	if ( _select >= 0 && _select < _data->getGroundsSize( ) ) {
		std::vector< GameData::Ground* > grounds = _data->getGrounds( );
		if ( mouse->isPushLeftButton( ) ) {
			_save_distance = grounds[ _select ]->_pos - mouse->getPos( );
		}
		if ( mouse->isHoldLeftButton( ) ) {
			grounds[ _select ]->_pos = mouse->getPos( ) + _save_distance;
		}
	}
}

void Editor::tweakSelectedGround( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( _select >= 0 && _select < _data->getGroundsSize( ) ) {
		std::vector< GameData::Ground* > grounds = _data->getGrounds( );
		if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {
			if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
				grounds[ _select ]->_pos += Vector2( -GROUND_IMAGE_SIZE, 0 );
				grounds[ _select ]->_size += Vector2( GROUND_IMAGE_SIZE, 0 );
			} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
				grounds[ _select ]->_size += Vector2( GROUND_IMAGE_SIZE, 0 );
			} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
				grounds[ _select ]->_pos += Vector2( 0, -GROUND_IMAGE_SIZE );
				grounds[ _select ]->_size += Vector2( 0, GROUND_IMAGE_SIZE );
			} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
				grounds[ _select ]->_size += Vector2( 0, GROUND_IMAGE_SIZE );
			}
		} else if ( keyboard->isHoldKey( "SPACE" ) ) {
			if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
				grounds[ _select ]->_pos += Vector2( GROUND_IMAGE_SIZE, 0 );
				grounds[ _select ]->_size += Vector2( -GROUND_IMAGE_SIZE, 0 );
			} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
				grounds[ _select ]->_size += Vector2( -GROUND_IMAGE_SIZE, 0 );
			} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
				grounds[ _select ]->_pos += Vector2( 0, GROUND_IMAGE_SIZE );
				grounds[ _select ]->_size += Vector2( 0, -GROUND_IMAGE_SIZE );
			} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
				grounds[ _select ]->_size += Vector2( 0, -GROUND_IMAGE_SIZE );
			}
			if ( grounds[ _select ]->_size.x == 0 || grounds[ _select ]->_size.y == 0 ) {
				_data->deleteGround( _select );
				_select = -1;
			}
		} else {
			if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
				grounds[ _select ]->_pos += Vector2( -1, 0 );
			} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
				grounds[ _select ]->_pos += Vector2( 1, 0 );
			} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
				grounds[ _select ]->_pos += Vector2( 0, -1 );
			} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
				grounds[ _select ]->_pos += Vector2( 0, 1 );
			}
		}
	}
}

void Editor::deleteSelectedGround( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( keyboard->isPushKey( "BACK_SPACE" ) ) {
		if ( _select >= 0 && _select < _data->getGroundsSize( ) ) {
			_data->deleteGround( _select );
			_select = -1;
		}
	}
}

void Editor::createGimmickToMouse( ) {
	MousePtr mouse = Task::get< Mouse >( );
	if ( mouse->isPushLeftButton( ) ) {
		_temp_pos1 = mouse->getPos( );
	}
	if ( mouse->isHoldLeftButton( ) && _select == -1 ) {
		_mouse_hold_left = true;
	} else if ( _mouse_hold_left ) {
		_temp_pos2 = mouse->getPos( );
		Vector2 size = _temp_pos2 - _temp_pos1;
		if ( size.x > 0 && size.y > 0 ) {
			if ( ( int )size.x % GROUND_IMAGE_SIZE > GROUND_IMAGE_SIZE / 2 ) {
				size = Vector2( ( ( int )( size.x / GROUND_IMAGE_SIZE ) + 1 ) * GROUND_IMAGE_SIZE, size.y );
			} else {
				size = Vector2( ( int )( size.x / GROUND_IMAGE_SIZE ) * GROUND_IMAGE_SIZE, size.y );
				if ( size.x == 0 ) {
					size = Vector2( GROUND_IMAGE_SIZE, size.y );
				}
			}
			if ( ( int )size.y % GROUND_IMAGE_SIZE > GROUND_IMAGE_SIZE / 2 ) {
				size = Vector2( size.x, ( ( int )( size.y / GROUND_IMAGE_SIZE ) + 1 ) * GROUND_IMAGE_SIZE );
			} else {
				size = Vector2( size.x, ( int )( size.y / GROUND_IMAGE_SIZE ) * GROUND_IMAGE_SIZE );
				if ( size.y == 0 ) {
					size = Vector2( size.x, GROUND_IMAGE_SIZE );
				}
			}

			int select_adjust = -1;
			switch ( ( GameData::Gimmick::TYPE )( _select_type - 1 ) ) {
			case GameData::Gimmick::TYPE::GOAL:
			case GameData::Gimmick::TYPE::RESPAWN:
			case GameData::Gimmick::TYPE::DECELERATE_NET:
			case GameData::Gimmick::TYPE::STICKY_NET:
				_data->createGimmick( _temp_pos1 + _camera->getPos( ), getGimmickSize( _select_type - 1 ),
									  ( GameData::Gimmick::TYPE )( _select_type - 1 ), GameData::Gimmick::DIR::NONE );
				break;
			case GameData::Gimmick::TYPE::ACID:
				_data->createGimmick( _temp_pos1 + _camera->getPos( ), size,
									  ( GameData::Gimmick::TYPE )( _select_type - 1 ), GameData::Gimmick::DIR::NONE );
				break;
			case GameData::Gimmick::TYPE::NEEDLE:
				_data->createGimmick( _temp_pos1 + _camera->getPos( ), getGimmickSize( _select_type - 1 ),
									  ( GameData::Gimmick::TYPE )( _select_type - 1 ), GameData::Gimmick::DIR::UP );
				break;
			case GameData::Gimmick::TYPE::FAN:
				_data->createFan( _temp_pos1 + _camera->getPos( ), getGimmickSize( _select_type - 1 ), GameData::Gimmick::DIR::UP );
				select_adjust = _data->getFansSize( ) - 1;
				break;
			}
			_select = _data->getGimmicksSize( ) + select_adjust;
		}
		_mouse_hold_left = false;
	}
}

void Editor::moveSelectedGimmick( ) {
	MousePtr mouse = Task::get< Mouse >( );
	if ( _select >= 0 && _select < _data->getGimmicksSize( ) ) {
		std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
		if ( mouse->isPushLeftButton( ) ) {
			_save_distance = gimmicks[ _select ]->_pos - mouse->getPos( );
		}
		if ( mouse->isHoldLeftButton( ) ) {
			gimmicks[ _select ]->_pos = mouse->getPos( ) + _save_distance;
		}
	}

	//fan
	if ( _select >= _data->getGimmicksSize( ) && _select < _data->getFansSize( ) + _data->getGimmicksSize( ) ) {
		std::vector< GameData::Fan* > fans = _data->getFans( );
		if ( mouse->isPushLeftButton( ) ) {
			_save_distance = fans[ _select - _data->getGimmicksSize( ) ]->_gimmick->_pos - mouse->getPos( );
		}
		if ( mouse->isHoldLeftButton( ) ) {
			fans[ _select - _data->getGimmicksSize( ) ]->_gimmick->_pos = mouse->getPos( ) + _save_distance;
		}
	}
}

void Editor::tweakSelectedGimmick( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( _select >= 0 && _select < _data->getGimmicksSize( ) ) {
		std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
		int adjust_size = 0;
		switch ( gimmicks[ _select ]->_type ) {
		case GameData::Gimmick::TYPE::ACID:
			adjust_size = GROUND_IMAGE_SIZE;
			break;
		case GameData::Gimmick::TYPE::NEEDLE:
			adjust_size = NEEDLE_SIZE.x;
			break;
		}
		if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {

			if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
				if ( gimmicks[ _select ]->_type == GameData::Gimmick::TYPE::NEEDLE &&
					 ( gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::LEFT ||
					 gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::RIGHT ) ) {
					return;
				}
				gimmicks[ _select ]->_pos += Vector2( -adjust_size, 0 );
				gimmicks[ _select ]->_size += Vector2( adjust_size, 0 );
			} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
				if ( gimmicks[ _select ]->_type == GameData::Gimmick::TYPE::NEEDLE &&
					 ( gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::LEFT ||
					 gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::RIGHT ) ) {
					return;
				}
				gimmicks[ _select ]->_size += Vector2( adjust_size, 0 );
			} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
				if ( gimmicks[ _select ]->_type == GameData::Gimmick::TYPE::NEEDLE &&
					 ( gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::UP ||
					 gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::DOWN ) ) {
					return;
				}
				gimmicks[ _select ]->_pos += Vector2( 0, -adjust_size );
				gimmicks[ _select ]->_size += Vector2( 0, adjust_size );
			} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
				if ( gimmicks[ _select ]->_type == GameData::Gimmick::TYPE::NEEDLE &&
					 ( gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::UP ||
					 gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::DOWN ) ) {
					return;
				}
				gimmicks[ _select ]->_size += Vector2( 0, adjust_size );
			}
		} else if ( keyboard->isHoldKey( "SPACE" ) ) {
			if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
				if ( gimmicks[ _select ]->_type == GameData::Gimmick::TYPE::NEEDLE &&
					 ( gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::LEFT ||
					 gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::RIGHT ) ) {
					return;
				}
				gimmicks[ _select ]->_pos += Vector2( adjust_size, 0 );
				gimmicks[ _select ]->_size += Vector2( -adjust_size, 0 );
			} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
				if ( gimmicks[ _select ]->_type == GameData::Gimmick::TYPE::NEEDLE &&
					 ( gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::LEFT ||
					 gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::RIGHT ) ) {
					return;
				}
				gimmicks[ _select ]->_size += Vector2( -adjust_size, 0 );
			} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
				if ( gimmicks[ _select ]->_type == GameData::Gimmick::TYPE::NEEDLE &&
					 ( gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::UP ||
					 gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::DOWN ) ) {
					return;
				}
				gimmicks[ _select ]->_pos += Vector2( 0, adjust_size );
				gimmicks[ _select ]->_size += Vector2( 0, -adjust_size );
			} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
				if ( gimmicks[ _select ]->_type == GameData::Gimmick::TYPE::NEEDLE &&
					 ( gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::UP ||
					 gimmicks[ _select ]->_dir == GameData::Gimmick::DIR::DOWN ) ) {
					return;
				}
				gimmicks[ _select ]->_size += Vector2( 0, -adjust_size );
			}
			if ( gimmicks[ _select ]->_size.x == 0 || gimmicks[ _select ]->_size.y == 0 ) {
				_data->deleteGimmick( _select );
				_select = -1;
			}
		} else {
			if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
				gimmicks[ _select ]->_pos += Vector2( -1, 0 );
			} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
				gimmicks[ _select ]->_pos += Vector2( 1, 0 );
			} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
				gimmicks[ _select ]->_pos += Vector2( 0, -1 );
			} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
				gimmicks[ _select ]->_pos += Vector2( 0, 1 );
			}
		}
	}

	//fan
	if ( _select >= _data->getGimmicksSize( ) && _select < _data->getFansSize( ) + _data->getGimmicksSize( ) ) {
		std::vector< GameData::Fan* > fans = _data->getFans( );
		int select = _select - _data->getGimmicksSize( );
		if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
			fans[ select ]->_gimmick->_pos += Vector2( -1, 0 );
		} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
			fans[ select ]->_gimmick->_pos += Vector2( 1, 0 );
		} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
			fans[ select ]->_gimmick->_pos += Vector2( 0, -1 );
		} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
			fans[ select ]->_gimmick->_pos += Vector2( 0, 1 );
		}
	}
}

void Editor::deleteSelectedGimmick( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( keyboard->isPushKey( "BACK_SPACE" ) ) {
		if ( _select >= 0 && _select < _data->getGimmicksSize( ) ) {
			_data->deleteGimmick( _select );
			_select = -1;
		}
		if ( _select >= _data->getGimmicksSize( ) && _select < _data->getFansSize( ) + _data->getGimmicksSize( ) ) {
			_data->deleteFan( _select - _data->getGimmicksSize( ) );
			_select = -1;
		}
	}
}

void Editor::changeSelectedGimmickDir( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );

	GameData::Gimmick* gimmick = new GameData::Gimmick( );
	bool need_dir = false;
	GameData::Gimmick::DIR dir = GameData::Gimmick::DIR::NONE;

	if ( _select >= 0 && _select < _data->getGimmicksSize( ) ) {
		if ( _data->getGimmicks( )[ _select ]->_type == GameData::Gimmick::TYPE::NEEDLE ) {
			gimmick = _data->getGimmicks( )[ _select ];
			need_dir = true;
		}
	} else if ( _select >= _data->getGimmicksSize( ) && _select < _data->getFansSize( ) + _data->getGimmicksSize( ) ) {
		std::vector< GameData::Fan* > fans = _data->getFans( );
		int select = _select - _data->getGimmicksSize( );
		gimmick = fans[ select ]->_gimmick;
		need_dir = true;
	}

	if ( !need_dir ) {
		return;
	}

	if ( keyboard->isPushKey( "NUM4" ) ) {
		dir = GameData::Gimmick::DIR::LEFT;
	} else if ( keyboard->isPushKey( "NUM6" ) ) {
		dir = GameData::Gimmick::DIR::RIGHT;
	} else if ( keyboard->isPushKey( "NUM8" ) ) {
		dir = GameData::Gimmick::DIR::UP;
	} else if ( keyboard->isPushKey( "NUM2" ) ) {
		dir = GameData::Gimmick::DIR::DOWN;
	}

	if ( keyboard->isPushKey( "NUM4" ) || keyboard->isPushKey( "NUM6" ) ||
		 keyboard->isPushKey( "NUM8" ) || keyboard->isPushKey( "NUM2" ) ) {

		if ( gimmick->_type == GameData::Gimmick::TYPE::FAN ) {
			switch ( dir ) {
			case GameData::Gimmick::DIR::LEFT:
			case GameData::Gimmick::DIR::RIGHT:
				gimmick->_size = Vector2( FAN_SIZE.y, FAN_SIZE.x );
				break;
			case GameData::Gimmick::DIR::UP:
			case GameData::Gimmick::DIR::DOWN:
				gimmick->_size = FAN_SIZE;
				break;
			}
		} else if ( gimmick->_type == GameData::Gimmick::TYPE::NEEDLE ) {
			if ( ( ( gimmick->_dir == GameData::Gimmick::DIR::LEFT || gimmick->_dir == GameData::Gimmick::DIR::RIGHT ) &&
				 ( dir == GameData::Gimmick::DIR::UP || dir == GameData::Gimmick::DIR::DOWN ) ) ||
				 ( ( gimmick->_dir == GameData::Gimmick::DIR::UP || gimmick->_dir == GameData::Gimmick::DIR::DOWN ) &&
				 ( dir == GameData::Gimmick::DIR::LEFT || dir == GameData::Gimmick::DIR::RIGHT ) ) ) {
				gimmick->_size = Vector2( gimmick->_size.y, gimmick->_size.x );
			}
		}
		gimmick->_dir = dir;
	}
}

void Editor::changeSelectedFanData( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( _select >= _data->getGimmicksSize( ) && _select < _data->getFansSize( ) + _data->getGimmicksSize( ) ) {
		std::vector< GameData::Fan* > fans = _data->getFans( );
		GameData::Fan* fan = fans[ _select - _data->getGimmicksSize( ) ];
		if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {
			if ( keyboard->isPushKey( "1" ) ) {
				changeSelectedFanVec( fan );
			} else if ( keyboard->isPushKey( "2" ) ) {
				changeSelectedFanStartState( fan );
			} else if ( keyboard->isPushKey( "3" ) ) {
				changeSelectedFanAutoSwitching( fan );
			} else if ( keyboard->isPushKey( "4" ) ) {
				changeSelectedFanOntime( fan );
			} else if ( keyboard->isPushKey( "5" ) ) {
				changeSelectedFanOffTime( fan );
			}
		}

	}
}

void Editor::changeSelectedFanVec( GameData::Fan* fan ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "fan_vec" );
	std::string vec_str = Application::getInstance( )->inputString( 0, 80 );
	if ( vec_str.size( ) == 0 ) {
		return;
	}
	fan->_vec = convertFromStringToNumber( vec_str );
}

void Editor::changeSelectedFanStartState( GameData::Fan* fan ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "start_state : on->1, off->0" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	fan->_start_state = convertFromStringToNumber( str );
}

void Editor::changeSelectedFanAutoSwitching( GameData::Fan* fan ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "auto_switching : on->1, off->0" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	fan->_auto_switching = convertFromStringToNumber( str );
}

void Editor::changeSelectedFanOntime( GameData::Fan* fan ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "on_time" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	fan->_on_time = convertFromStringToNumber( str );
}

void Editor::changeSelectedFanOffTime( GameData::Fan* fan ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "off_time" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	fan->_off_time = convertFromStringToNumber( str );
}

void Editor::createEnemyToMouse( ) {
	MousePtr mouse = Task::get< Mouse >( );
	if ( mouse->isPushLeftButton( ) ) {
		_temp_pos1 = mouse->getPos( );
	}
	if ( mouse->isHoldLeftButton( ) && _select == -1 ) {
		_mouse_hold_left = true;
	} else if ( _mouse_hold_left ) {
		_temp_pos2 = mouse->getPos( );
		Vector2 size = _temp_pos2 - _temp_pos1;
		if ( size.x > 0 && size.y > 0 ) {
			_data->createEnemy( _temp_pos1 + _camera->getPos( ), GameData::Enemy::MOVE_PATTERN::CLOCKWISE );
			_select = _data->getEnemiesSize( ) - 1;
		}
		_mouse_hold_left = false;
	}
}

void Editor::moveSelectedEnemy( ) {
	MousePtr mouse = Task::get< Mouse >( );
	if ( _select >= 0 && _select < _data->getEnemiesSize( ) ) {
		std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
		if ( mouse->isPushLeftButton( ) ) {
			_save_distance = enemies[ _select ]->_pos - mouse->getPos( );
		}
		if ( mouse->isHoldLeftButton( ) ) {
			enemies[ _select ]->_pos = mouse->getPos( ) + _save_distance;
		}
	}
}

void Editor::tweakSelectedEnemy( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( _select >= 0 && _select < _data->getEnemiesSize( ) ) {
		std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
		if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
			enemies[ _select ]->_pos += Vector2( -1, 0 );
		} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
			enemies[ _select ]->_pos += Vector2( 1, 0 );
		} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
			enemies[ _select ]->_pos += Vector2( 0, -1 );
		} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
			enemies[ _select ]->_pos += Vector2( 0, 1 );
		}
	}
}

void Editor::deleteSelectedEnemy( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( keyboard->isPushKey( "BACK_SPACE" ) ) {
		if ( _select >= 0 && _select < _data->getEnemiesSize( ) ) {
			_data->deleteEnemy( _select );
			_select = -1;
		}
	}
}

void Editor::changeSelectedEnemyMovePattern( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( _select >= 0 && _select < _data->getEnemiesSize( ) ) {
		if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {
			if ( keyboard->isPushKey( "1" ) ) {
				DrawerConstPtr drawer = Task::getConst< Drawer >( );
				drawer->drawString( 0, 50, "move_pattern : Clockwise->0, Vertical->1, Horizontal->2, CounterClockwise->3" );
				std::string str = Application::getInstance( )->inputString( 0, 80 );
				if ( str.size( ) == 0 || convertFromStringToNumber( str ) > 3 ) {
					return;
				}
				std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
				enemies[ _select ]->_move_pattern = ( GameData::Enemy::MOVE_PATTERN )convertFromStringToNumber( str );
			}
		}
	}
}

void Editor::changeSelectedEnemyVecX( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( _select >= 0 && _select < _data->getEnemiesSize( ) ) {
		if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {
			if ( keyboard->isPushKey( "2" ) ) {
				DrawerConstPtr drawer = Task::getConst< Drawer >( );
				drawer->drawString( 0, 50, "vec_x" );
				std::string str = Application::getInstance( )->inputString( 0, 80 );
				if ( str.size( ) == 0 ) {
					return;
				}
				std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
				enemies[ _select ]->_vec.x = convertFromStringToNumber( str );
			}
		}
	}
}

void Editor::changeSelectedEnemyVecY( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( _select >= 0 && _select < _data->getEnemiesSize( ) ) {
		if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {
			if ( keyboard->isPushKey( "3" ) ) {
				DrawerConstPtr drawer = Task::getConst< Drawer >( );
				drawer->drawString( 0, 50, "vec_y" );
				std::string str = Application::getInstance( )->inputString( 0, 80 );
				if ( str.size( ) == 0 ) {
					return;
				}
				std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
				enemies[ _select ]->_vec.y = convertFromStringToNumber( str );
			}
		}
	}
}

void Editor::moveSelectedMultiple( ) {

}

void Editor::tweakSelectedMultiple( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	std::vector< GameData::Ground* > grounds = _data->getGrounds( );
	std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
	std::vector< GameData::Fan* > fans = _data->getFans( );
	std::vector< GameData::Enemy* > enemies = _data->getEnemies( );

	Vector2 pos = Vector2( 0, 0 );
	if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
		pos += Vector2( -1, 0 );
	} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
		pos += Vector2( 1, 0 );
	} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
		pos += Vector2( 0, -1 );
	} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
		pos += Vector2( 0, 1 );
	}
	if ( pos != Vector2( 0, 0 ) ) {
		for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
			if ( _multiple_selects[ i ] < grounds.size( ) ) {
				grounds[ _multiple_selects[ i ] ]->_pos += pos;
			} else if ( _multiple_selects[ i ] < grounds.size( ) + gimmicks.size( ) ) {
				gimmicks[ _multiple_selects[ i ] - grounds.size( ) ]->_pos += pos;
			} else if ( _multiple_selects[ i ] < grounds.size( ) + gimmicks.size( ) + fans.size( ) ) {
				fans[ _multiple_selects[ i ] - grounds.size( ) - gimmicks.size( ) ]->_gimmick->_pos += pos;
			} else if ( _multiple_selects[ i ] < grounds.size( ) + gimmicks.size( ) + fans.size( ) + enemies.size( ) ) {
				enemies[ _multiple_selects[ i ] - grounds.size( ) - gimmicks.size( ) - fans.size( ) ]->_pos += pos;
			}
		}
	}
}

void Editor::changeSelectedMultipleData( ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	int grounds_size = _data->getGroundsSize( );
	int gimmicks_size = _data->getGimmicksSize( );
	std::vector< GameData::Fan* > fans = _data->getFans( );
	std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
	bool at_fan = false;
	bool at_enemy = false;
	for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
		if ( _multiple_selects[ i ] >= grounds_size + gimmicks_size &&
			 _multiple_selects[ i ] < grounds_size + gimmicks_size + fans.size( ) ) {
			at_fan = true;
		} else if ( _multiple_selects[ i ] < grounds_size + gimmicks_size + fans.size( ) + enemies.size( ) ) {
			at_enemy = true;
		}
	}

	if ( at_fan && at_enemy ) {

	}
	std::string str;
	if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {
		if ( at_fan ) {
			if ( keyboard->isPushKey( "1" ) ) {
				changeSelectedMultipleFanVec( );
			} else if ( keyboard->isPushKey( "2" ) ) {
				changeSelectedMultipleFanStartState( );
			} else if ( keyboard->isPushKey( "3" ) ) {
				changeSelectedMultipleFanAutoSwitching( );
			} else if ( keyboard->isPushKey( "4" ) ) {
				changeSelectedMultipleFanOnTime( );
			} else if ( keyboard->isPushKey( "5" ) ) {
				changeSelectedMultipleFanOffTime( );
			}
		} else if ( at_enemy ) {
			if ( keyboard->isPushKey( "1" ) ) {
				changeSelectedMultipleEnemyMovePattern( );
			} else if ( keyboard->isPushKey( "2" ) ) {
				changeSelectedMultipleEnemyVecX( );
			} else if ( keyboard->isPushKey( "3" ) ) {
				changeSelectedMultipleEnemyVecY( );
			}
		}
	}
}

void Editor::changeSelectedMultipleFanVec( ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "fan_vec" );
	std::string vec_str = Application::getInstance( )->inputString( 0, 80 );
	if ( vec_str.size( ) == 0 ) {
		return;
	}
	int grounds_size = _data->getGroundsSize( );
	int gimmicks_size = _data->getGimmicksSize( );
	std::vector< GameData::Fan* > fans = _data->getFans( );
	for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
		if ( _multiple_selects[ i ] >= grounds_size + gimmicks_size &&
			 _multiple_selects[ i ] < grounds_size + gimmicks_size + fans.size( ) ) {
			fans[ _multiple_selects[ i ] - grounds_size - gimmicks_size ]->_vec = convertFromStringToNumber( vec_str );
		}
	}
}

void Editor::changeSelectedMultipleFanStartState( ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "start_state : on->1, off->0" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	int grounds_size = _data->getGroundsSize( );
	int gimmicks_size = _data->getGimmicksSize( );
	std::vector< GameData::Fan* > fans = _data->getFans( );
	for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
		if ( _multiple_selects[ i ] >= grounds_size + gimmicks_size &&
			 _multiple_selects[ i ] < grounds_size + gimmicks_size + fans.size( ) ) {
			fans[ _multiple_selects[ i ] - grounds_size - gimmicks_size ]->_start_state = convertFromStringToNumber( str );
		}
	}
}

void Editor::changeSelectedMultipleFanAutoSwitching( ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "auto_switching : on->1, off->0" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	int grounds_size = _data->getGroundsSize( );
	int gimmicks_size = _data->getGimmicksSize( );
	std::vector< GameData::Fan* > fans = _data->getFans( );
	for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
		if ( _multiple_selects[ i ] >= grounds_size + gimmicks_size &&
			 _multiple_selects[ i ] < grounds_size + gimmicks_size + fans.size( ) ) {
			fans[ _multiple_selects[ i ] - grounds_size - gimmicks_size ]->_auto_switching = convertFromStringToNumber( str );
		}
	}
}

void Editor::changeSelectedMultipleFanOnTime( ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "on_time" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	int grounds_size = _data->getGroundsSize( );
	int gimmicks_size = _data->getGimmicksSize( );
	std::vector< GameData::Fan* > fans = _data->getFans( );
	for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
		if ( _multiple_selects[ i ] >= grounds_size + gimmicks_size &&
			 _multiple_selects[ i ] < grounds_size + gimmicks_size + fans.size( ) ) {
			fans[ _multiple_selects[ i ] - grounds_size - gimmicks_size ]->_on_time = convertFromStringToNumber( str );
		}
	}
}

void Editor::changeSelectedMultipleFanOffTime( ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "off_time" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	int grounds_size = _data->getGroundsSize( );
	int gimmicks_size = _data->getGimmicksSize( );
	std::vector< GameData::Fan* > fans = _data->getFans( );
	for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
		if ( _multiple_selects[ i ] >= grounds_size + gimmicks_size &&
			 _multiple_selects[ i ] < grounds_size + gimmicks_size + fans.size( ) ) {
			fans[ _multiple_selects[ i ] - grounds_size - gimmicks_size ]->_off_time = convertFromStringToNumber( str );
		}
	}
}

void Editor::changeSelectedMultipleEnemyMovePattern( ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "move_pattern : Circle->0, Vertical->1, Horizontal->2" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 || convertFromStringToNumber( str ) > 2 ) {
		return;
	}
	int grounds_size = _data->getGroundsSize( );
	int gimmicks_size = _data->getGimmicksSize( );
	int fans_size = _data->getFansSize( );
	std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
	for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
		if ( _multiple_selects[ i ] >= grounds_size + gimmicks_size + fans_size &&
			 _multiple_selects[ i ] < grounds_size + gimmicks_size + fans_size + enemies.size( ) ) {
			enemies[ _multiple_selects[ i ] - grounds_size - gimmicks_size - fans_size ]->_move_pattern = ( GameData::Enemy::MOVE_PATTERN )convertFromStringToNumber( str );
		}
	}
}

void Editor::changeSelectedMultipleEnemyVecX( ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "vec_x" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	int grounds_size = _data->getGroundsSize( );
	int gimmicks_size = _data->getGimmicksSize( );
	int fans_size = _data->getFansSize( );
	std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
	for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
		if ( _multiple_selects[ i ] >= grounds_size + gimmicks_size + fans_size &&
			 _multiple_selects[ i ] < grounds_size + gimmicks_size + fans_size + enemies.size( ) ) {
			enemies[ _multiple_selects[ i ] - grounds_size - gimmicks_size - fans_size ]->_vec.x = convertFromStringToNumber( str );
		}
	}
}

void Editor::changeSelectedMultipleEnemyVecY( ) {
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	drawer->drawString( 0, 50, "vec_y" );
	std::string str = Application::getInstance( )->inputString( 0, 80 );
	if ( str.size( ) == 0 ) {
		return;
	}
	int grounds_size = _data->getGroundsSize( );
	int gimmicks_size = _data->getGimmicksSize( );
	int fans_size = _data->getFansSize( );
	std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
	for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
		if ( _multiple_selects[ i ] >= grounds_size + gimmicks_size + fans_size &&
			 _multiple_selects[ i ] < grounds_size + gimmicks_size + fans_size + enemies.size( ) ) {
			enemies[ _multiple_selects[ i ] - grounds_size - gimmicks_size - fans_size ]->_vec.y = convertFromStringToNumber( str );
		}
	}
}

void Editor::changeSelectedMultipleDir( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );

	GameData::Gimmick::DIR dir = GameData::Gimmick::DIR::NONE;
	if ( keyboard->isPushKey( "NUM4" ) ) {
		dir = GameData::Gimmick::DIR::LEFT;
	} else if ( keyboard->isPushKey( "NUM6" ) ) {
		dir = GameData::Gimmick::DIR::RIGHT;
	} else if ( keyboard->isPushKey( "NUM8" ) ) {
		dir = GameData::Gimmick::DIR::UP;
	} else if ( keyboard->isPushKey( "NUM2" ) ) {
		dir = GameData::Gimmick::DIR::DOWN;
	}

	if ( keyboard->isPushKey( "NUM4" ) || keyboard->isPushKey( "NUM6" ) ||
		 keyboard->isPushKey( "NUM8" ) || keyboard->isPushKey( "NUM2" ) ) {
		int grounds_size = _data->getGroundsSize( );
		std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
		std::vector< GameData::Fan* > fans = _data->getFans( );
		for ( int i = 0; i < _multiple_selects.size( ); i++ ) {
			if ( _multiple_selects[ i ] >= grounds_size &&
				 _multiple_selects[ i ] < grounds_size + gimmicks.size( ) ) {
				if ( gimmicks[ _multiple_selects[ i ] - grounds_size ]->_type == GameData::Gimmick::TYPE::NEEDLE ) {
					gimmicks[ _multiple_selects[ i ] - grounds_size ]->_dir = dir;
					switch ( dir ) {
					case GameData::Gimmick::DIR::LEFT:
					case GameData::Gimmick::DIR::RIGHT:
						gimmicks[ _multiple_selects[ i ] - grounds_size ]->_size = Vector2( NEEDLE_SIZE.y, NEEDLE_SIZE.x );
						break;
					case GameData::Gimmick::DIR::UP:
					case GameData::Gimmick::DIR::DOWN:
						gimmicks[ _multiple_selects[ i ] - grounds_size ]->_size = NEEDLE_SIZE;
						break;
					}
				}
			} else if ( _multiple_selects[ i ] < grounds_size + gimmicks.size( ) + fans.size( ) ) {
				fans[ _multiple_selects[ i ] - grounds_size - gimmicks.size( ) ]->_gimmick->_dir = dir;
				switch ( dir ) {
				case GameData::Gimmick::DIR::LEFT:
				case GameData::Gimmick::DIR::RIGHT:
					fans[ _multiple_selects[ i ] - grounds_size - gimmicks.size( ) ]->_gimmick->_size = Vector2( FAN_SIZE.y, FAN_SIZE.x );
					break;
				case GameData::Gimmick::DIR::UP:
				case GameData::Gimmick::DIR::DOWN:
					fans[ _multiple_selects[ i ] - grounds_size - gimmicks.size( ) ]->_gimmick->_size = FAN_SIZE;
					break;
				}
			}
		}
	}
}

void Editor::createBetweenObjectToMouse( ) {
	MousePtr mouse = Task::get< Mouse >( );
	if ( mouse->isPushLeftButton( ) ) {
		_temp_pos1 = mouse->getPos( );
	}
	if ( mouse->isHoldLeftButton( ) && _select == -1 ) {
		_mouse_hold_left = true;
	} else if ( _mouse_hold_left ) {
		_temp_pos2 = mouse->getPos( );
		Vector2 size = _temp_pos2 - _temp_pos1;
		if ( size.x > 0 && size.y > 0 ) {
			if ( ( int )size.x % GROUND_IMAGE_SIZE > GROUND_IMAGE_SIZE / 2 ) {
				size = Vector2( ( ( int )( size.x / GROUND_IMAGE_SIZE ) + 1 ) * GROUND_IMAGE_SIZE, size.y );
			} else {
				size = Vector2( ( int )( size.x / GROUND_IMAGE_SIZE ) * GROUND_IMAGE_SIZE, size.y );
				if ( size.x == 0 ) {
					size = Vector2( GROUND_IMAGE_SIZE, size.y );
				}
			}
			if ( ( int )size.y % GROUND_IMAGE_SIZE > GROUND_IMAGE_SIZE / 2 ) {
				size = Vector2( size.x, ( ( int )( size.y / GROUND_IMAGE_SIZE ) + 1 ) * GROUND_IMAGE_SIZE );
			} else {
				size = Vector2( size.x, ( int )( size.y / GROUND_IMAGE_SIZE ) * GROUND_IMAGE_SIZE );
				if ( size.y == 0 ) {
					size = Vector2( size.x, GROUND_IMAGE_SIZE );
				}
			}
			_data->createBetweenObject( _temp_pos1 + _camera->getPos( ), size );
			_select = _data->getBetweenObjectsSize( ) - 1;
		}
		_mouse_hold_left = false;
	}
}

void Editor::moveSelectedBetweenObject( ) {
	MousePtr mouse = Task::get< Mouse >( );
	if ( _select >= 0 && _select < _data->getBetweenObjectsSize( ) ) {
		std::vector< GameData::Ground* > between_objects = _data->getBetweenObjects( );
		if ( mouse->isPushLeftButton( ) ) {
			_save_distance = between_objects[ _select ]->_pos - mouse->getPos( );
		}
		if ( mouse->isHoldLeftButton( ) ) {
			between_objects[ _select ]->_pos = mouse->getPos( ) + _save_distance;
		}
	}
}

void Editor::tweakSelectedBetweenObject( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( _select >= 0 && _select < _data->getBetweenObjectsSize( ) ) {
		std::vector< GameData::Ground* > between_objects = _data->getBetweenObjects( );
		if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {
			if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
				between_objects[ _select ]->_pos += Vector2( -GROUND_IMAGE_SIZE, 0 );
				between_objects[ _select ]->_size += Vector2( GROUND_IMAGE_SIZE, 0 );
			} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
				between_objects[ _select ]->_size += Vector2( GROUND_IMAGE_SIZE, 0 );
			} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
				between_objects[ _select ]->_pos += Vector2( 0, -GROUND_IMAGE_SIZE );
				between_objects[ _select ]->_size += Vector2( 0, GROUND_IMAGE_SIZE );
			} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
				between_objects[ _select ]->_size += Vector2( 0, GROUND_IMAGE_SIZE );
			}
		} else if ( keyboard->isHoldKey( "SPACE" ) ) {
			if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
				between_objects[ _select ]->_pos += Vector2( GROUND_IMAGE_SIZE, 0 );
				between_objects[ _select ]->_size += Vector2( -GROUND_IMAGE_SIZE, 0 );
			} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
				between_objects[ _select ]->_size += Vector2( -GROUND_IMAGE_SIZE, 0 );
			} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
				between_objects[ _select ]->_pos += Vector2( 0, GROUND_IMAGE_SIZE );
				between_objects[ _select ]->_size += Vector2( 0, -GROUND_IMAGE_SIZE );
			} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
				between_objects[ _select ]->_size += Vector2( 0, -GROUND_IMAGE_SIZE );
			}
			if ( between_objects[ _select ]->_size.x == 0 || between_objects[ _select ]->_size.y == 0 ) {
				_data->deleteBetweenObject( _select );
				_select = -1;
			}
		} else {
			if ( keyboard->isPushKey( "ARROW_LEFT" ) ) {
				between_objects[ _select ]->_pos += Vector2( -1, 0 );
			} else if ( keyboard->isPushKey( "ARROW_RIGHT" ) ) {
				between_objects[ _select ]->_pos += Vector2( 1, 0 );
			} else if ( keyboard->isPushKey( "ARROW_UP" ) ) {
				between_objects[ _select ]->_pos += Vector2( 0, -1 );
			} else if ( keyboard->isPushKey( "ARROW_DOWN" ) ) {
				between_objects[ _select ]->_pos += Vector2( 0, 1 );
			}
		}
	}
}

void Editor::deleteSelectedBetweenObject( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( keyboard->isPushKey( "BACK_SPACE" ) ) {
		if ( _select >= 0 && _select < _data->getBetweenObjectsSize( ) ) {
			_data->deleteBetweenObject( _select );
			_select = -1;
		}
	}
}

void Editor::changeWidth( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {
		if ( keyboard->isPushKey( "W" ) ) {
			DrawerConstPtr drawer = Task::getConst< Drawer >( );
			drawer->drawString( 0, 50, "width" );
			std::string width_str = Application::getInstance( )->inputString( 0, 80 );
			if ( width_str.size( ) == 0 ) {
				return;
			}
			_data->setWidth( ) = convertFromStringToNumber( width_str );
		}
	}
}

void Editor::changeHeight( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( keyboard->isHoldKey( "LEFT_CTRL" ) || keyboard->isHoldKey( "RIGHT_CTRL" ) ) {
		if ( keyboard->isPushKey( "H" ) ) {
			DrawerConstPtr drawer = Task::getConst< Drawer >( );
			drawer->drawString( 0, 50, "height" );
			std::string height_str = Application::getInstance( )->inputString( 0, 80 );
			if ( height_str.size( ) == 0 ) {
				return;
			}
			_data->setHeight( ) = convertFromStringToNumber( height_str );
		}
	}
}

void Editor::selectCreateType( ) {
	ApplicationPtr app = Application::getInstance( );
	MousePtr mouse = Task::get< Mouse >( );
	int x = app->getScreenWidth( ) - 80;
	for ( int i = 0; i < 10; i++ ) {
		int y = 160 + 8 * ( i + 1 + i * 2 ) + 64 * i;
		if ( mouse->isPushLeftButton( ) ) {
			if ( mouse->getPos( ).x >= x && mouse->getPos( ).x <= x + 64 &&
				 mouse->getPos( ).y >= y && mouse->getPos( ).y <= y + 64 ) {
				_select_type = i;
			}
		}
	}
}

Vector2 Editor::getGimmickSize( int gimmick_type ) {
	Vector2 size = Vector2( );
	switch ( gimmick_type ) {
	case 0:
		size = GOAL_SIZE;
		break;
	case 1:
		size = RESPAWN_SIZE;
		break;
	case 2:
		break;
	case 3:
		size = NEEDLE_SIZE;
		break;
	case 4:
		size = FAN_SIZE;
		break;
	case 5:
		size = DECELERATE_NET_SIZE;
		break;
	case 6:
		size = STICKY_NET_SIZE;
		break;
	}
	return size;
}

int Editor::convertFromStringToNumber( std::string str ) {
	int num = 0;
	for ( int i = 0; i < str.size( ); i++ ) {
		num += ( int )str[ i ] - 48;
		num *= 10;
	}
	return num / 10;
}


void Editor::save( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	if ( keyboard->isPushKey( "F5" ) && !keyboard->isPushKey( "F6" ) ) {
		drawer->drawString( 0, 20, "save" );
		std::string filename = Application::getInstance( )->inputString( 0, 40 );
		if ( filename.size( ) == 0 ) {
			return;
		}
		_data->save( filename );
	}
}

void Editor::load( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	if ( keyboard->isPushKey( "F6" ) && !keyboard->isPushKey( "F5" ) ) {
		drawer->drawString( 0, 20, "load" );
		std::string filename = Application::getInstance( )->inputString( 0, 40 );
		if ( filename.size( ) == 0 ) {
			return;
		}
		_data->load( filename );
	}
}

void Editor::oldLoad( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	DrawerConstPtr drawer = Task::getConst< Drawer >( );
	if ( keyboard->isPushKey( "F7" ) && !keyboard->isPushKey( "F5" ) && !keyboard->isPushKey( "F6" ) ) {
		drawer->drawString( 0, 20, "oldload" );
		std::string filename = Application::getInstance( )->inputString( 0, 40 );
		if ( filename.size( ) == 0 ) {
			return;
		}
		_data->oldLoad( filename );
	}
}
