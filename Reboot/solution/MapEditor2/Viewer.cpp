#include "Viewer.h"
#include "Application.h"
#include "Drawer.h"
#include "Editor.h"
#include "GameData.h"
#include "Mouse.h"
#include "Image.h"
#include "Camera.h"

Viewer::Viewer( GameDataConstPtr data, EditorConstPtr editor, CameraConstPtr camera ) :
	_data( data ),
	_editor( editor ),
	_camera( camera ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_background = drawer->createImage( "Resource/Image/background/BG6.png" );
	//_floor.push_back( drawer->createImage( "Resource/Image/ground/floor2.png" ) );
	_floor.push_back( drawer->createImage( "Resource/Image/ground/fill_variation_01_db.png" ) );
	_floor.push_back( drawer->createImage( "Resource/Image/ground/fill_variation_02_db.png" ) );
	_floor.push_back( drawer->createImage( "Resource/Image/ground/fill_variation_03_db.png" ) );
	_floor.push_back( drawer->createImage( "Resource/Image/ground/fill_variation_04_db.png" ) );
	//_floor_top.push_back( drawer->createImage( "Resource/Image/ground/top_floor.png" ) );
	_floor_top.push_back( drawer->createImage( "Resource/Image/ground/top_edge_variation_01_db.png" ) );
	_floor_top.push_back( drawer->createImage( "Resource/Image/ground/top_edge_variation_02_db.png" ) );
	_floor_top.push_back( drawer->createImage( "Resource/Image/ground/top_edge_variation_03_db.png" ) );

	_between_wall.push_back( drawer->createImage( "Resource/Image/ground/between_object_1.png" ) );
	_between_wall.push_back( drawer->createImage( "Resource/Image/ground/between_object_2.png" ) );
	_between_wall.push_back( drawer->createImage( "Resource/Image/ground/between_object_3.png" ) );
	_between_wall.push_back( drawer->createImage( "Resource/Image/ground/between_object_4.png" ) );

	_goal = drawer->createImage( "Resource/Image/goal/door_08.png" );
	_acid = drawer->createImage( "Resource/Image/gimmick/toxic_water_repeating.png" );
	_respawn = drawer->createImage( "Resource/Image/Respawn/fill/lab_door_05.png" );
	_decelerate_net = drawer->createImage( "Resource/Image/gimmick/decelerate_net.png" );
	_sticky_net = drawer->createImage( "Resource/image/gimmick/sticky_net.png" );
	_enemy = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_000.png" );
	_needle_top = drawer->createImage( "Resource/image/gimmick/needle_up_2.png" );
	_needle_down = drawer->createImage( "Resource/image/gimmick/needle_down_2.png" );
	_needle_right = drawer->createImage( "Resource/image/gimmick/needle_right_2.png" );
	_needle_left = drawer->createImage( "Resource/image/gimmick/needle_left_2.png" );
	_fan_top = drawer->createImage( "Resource/used_by_MapEditor/fan_up.png" );
	_fan_down = drawer->createImage( "Resource/used_by_MapEditor/fan_down.png" );
	_fan_right = drawer->createImage( "Resource/used_by_MapEditor/fan_right.png" );
	_fan_left = drawer->createImage( "Resource/used_by_MapEditor/fan_left.png" );

	_goal_plate = drawer->createImage( "Resource/used_by_MapEditor/goal.png" );
	_respawn_plate = drawer->createImage( "Resource/used_by_MapEditor/respawn.png" );
	_acid_plate = drawer->createImage( "Resource/used_by_MapEditor/acid.png" );
	_needle_plate = drawer->createImage( "Resource/used_by_MapEditor/needle.png" );
	_fan_plate = drawer->createImage( "Resource/used_by_MapEditor/fan.png" );
	_sticky_net_plate = drawer->createImage( "Resource/used_by_MapEditor/net1.png" );
	_decelerate_net_plate = drawer->createImage( "Resource/used_by_MapEditor/net2.png" );
	_enemy_plate = drawer->createImage( "Resource/used_by_MapEditor/enemy.png" );
}

Viewer::~Viewer( ) {

}

void Viewer::draw( ) const {
	drawBackGround( );
	drawBetweenObjects( );
	drawGrounds( );
	drawGimmicks( );
	drawFans( );
	drawEnemies( );
	drawSelectArea( );
	drawNowSelect( );
	drawLange( );
	drawText( );
	drawSelecter( );
}

void Viewer::drawBackGround( ) const {
	ApplicationPtr app = Application::getInstance( );
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	drawer->setBlend( scope, Drawer::BLEND::ADD, 0.5 );
	_background->draw( 0, 0 );
}

void Viewer::drawGrounds( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< GameData::Ground* > grounds = _data->getGrounds( );
	for ( int i = 0; i < grounds.size( ); i++ ) {
		GameData::Ground* ground = grounds[ i ];
		for ( int i = 0; i < ground->_size.x; i += GROUND_IMAGE_SIZE ) {
			for ( int j = 0; j < ground->_size.y; j += GROUND_IMAGE_SIZE ) {
				int image_num = i / GROUND_IMAGE_SIZE + j / GROUND_IMAGE_SIZE;
				int adjust_y = 22;
				Vector2 adjust_pos = Vector2( ground->_pos.x + i, ground->_pos.y - adjust_y + j ) - _camera->getPos( );

				if ( j == 0 ) {
					if ( ground->_size.x - i < GROUND_IMAGE_SIZE ) {
						_floor_top[ image_num % _floor_top.size( ) ]->drawRect( adjust_pos, 0, 0, ground->_size.x - i, GROUND_IMAGE_SIZE );
					} else if ( ground->_size.y - adjust_y - j < GROUND_IMAGE_SIZE ) {
						_floor_top[ image_num % _floor_top.size( ) ]->drawRect( adjust_pos, 0, 0, GROUND_IMAGE_SIZE, ground->_size.y - j );
					} else {
						_floor_top[ image_num % _floor_top.size( ) ]->draw( adjust_pos );
					}
				} else {
					if ( ground->_size.x - i < GROUND_IMAGE_SIZE ) {
						_floor[ image_num % _floor.size( ) ]->drawRect( adjust_pos, 0, 0, ground->_size.x - i, GROUND_IMAGE_SIZE );
					} else if ( ground->_size.x - j < GROUND_IMAGE_SIZE ) {
						_floor[ image_num % _floor.size( ) ]->drawRect( adjust_pos, 0, 0, GROUND_IMAGE_SIZE, ground->_size.y - j );
					} else {
						_floor[ image_num % _floor.size( ) ]->draw( adjust_pos );
					}
				}
			}
		}
	}
}

void Viewer::drawGimmicks( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		GameData::Gimmick* gimmick = gimmicks[ i ];
		Vector2 adjust_pos = gimmick->_pos - _camera->getPos( );
		switch ( gimmick->_type ) {
		case GameData::Gimmick::TYPE::GOAL:
			_goal->draw( adjust_pos );
			break;
		case GameData::Gimmick::TYPE::RESPAWN:
			_respawn->draw( adjust_pos );
			break;
		case GameData::Gimmick::TYPE::ACID:
			for ( int k = 0; k < gimmick->_size.x; k += 1920 ) {
				for ( int j = 0; j < gimmick->_size.y; j += 1080 ) {
					int tx = 10;
					int ty = 10;
					if ( k > 0 ) {
						tx = 308;
					}
					if ( j > 0 ) {
						ty = 250;
					}
					_acid->drawRect( ( int )adjust_pos.x + k, ( int )adjust_pos.y + j, tx, ty, ( int )gimmick->_size.x - k, ( int )gimmick->_size.y - j );
				}
			}
			break;
		case GameData::Gimmick::TYPE::NEEDLE:
			switch ( gimmick->_dir ) {
			case GameData::Gimmick::DIR::UP:
				for ( int k = 0; k < gimmick->_size.x; k += ( int )NEEDLE_SIZE.x ) {
					_needle_top->draw( ( int )adjust_pos.x + k, ( int )adjust_pos.y );

				}
				break;
			case  GameData::Gimmick::DIR::DOWN:
				for ( int k = 0; k < gimmicks[ i ]->_size.x; k += ( int )NEEDLE_SIZE.x ) {
					_needle_down->draw( ( int )adjust_pos.x + k, ( int )adjust_pos.y );
				}
				break;
			case GameData::Gimmick::DIR::LEFT:
				for ( int k = 0; k < gimmicks[ i ]->_size.y; k += ( int )NEEDLE_SIZE.x ) {
					_needle_left->draw( ( int )adjust_pos.x, ( int )adjust_pos.y + k );
				}
				break;
			case  GameData::Gimmick::DIR::RIGHT:
				for ( int k = 0; k < gimmicks[ i ]->_size.y; k += ( int )NEEDLE_SIZE.x ) {
					_needle_right->draw( ( int )adjust_pos.x, ( int )adjust_pos.y + k );
				}
				break;
			}
			break;
		case GameData::Gimmick::TYPE::DECELERATE_NET:
			_decelerate_net->draw( adjust_pos );
			break;
		case GameData::Gimmick::TYPE::STICKY_NET:
			_sticky_net->draw( adjust_pos );
			break;
		}
	}
}

void Viewer::drawFans( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< GameData::Fan* > fans = _data->getFans( );
	for ( int i = 0; i < fans.size( ); i++ ) {
		GameData::Fan* fan = fans[ i ];
		Vector2 adjust_pos = fan->_gimmick->_pos - _camera->getPos( );
		switch ( fan->_gimmick->_dir ) {
		case  GameData::Gimmick::DIR::UP:
			_fan_top->draw( adjust_pos );
			break;
		case  GameData::Gimmick::DIR::DOWN:
			_fan_down->draw( adjust_pos );
			break;
		case  GameData::Gimmick::DIR::LEFT:
			_fan_left->draw( adjust_pos );
			break;
		case GameData::Gimmick::DIR::RIGHT:
			_fan_right->draw( adjust_pos );
			break;
		}
	}
}

void Viewer::drawEnemies( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
	for ( int i = 0; i < enemies.size( ); i++ ) {
		GameData::Enemy* enemy = enemies[ i ];
		Vector2 adjust_pos = enemy->_pos - _camera->getPos( );
		_enemy->draw( adjust_pos - Vector2( 30, 20 ) );
	}
}

void Viewer::drawBetweenObjects( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< GameData::Ground* > between_objects = _data->getBetweenObjects( );
	for ( int i = 0; i < between_objects.size( ); i++ ) {
		GameData::Ground* between_object = between_objects[ i ];
		for ( int i = 0; i < between_object->_size.x; i += GROUND_IMAGE_SIZE ) {
			for ( int j = 0; j < between_object->_size.y; j += GROUND_IMAGE_SIZE ) {
				int image_num = i / GROUND_IMAGE_SIZE + j / GROUND_IMAGE_SIZE;
				Vector2 adjust_pos = Vector2( between_object->_pos.x + i, between_object->_pos.y + j ) - _camera->getPos( );

				if ( j == 0 ) {
					if ( between_object->_size.x - i < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, between_object->_size.x - i, GROUND_IMAGE_SIZE );
					} else if ( between_object->_size.y - j < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, GROUND_IMAGE_SIZE, between_object->_size.y - j );
					} else {
						_between_wall[ image_num % _between_wall.size( ) ]->draw( adjust_pos );
					}
				} else {
					if ( between_object->_size.x - i < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, between_object->_size.x - i, GROUND_IMAGE_SIZE );
					} else if ( between_object->_size.y - j < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, GROUND_IMAGE_SIZE, between_object->_size.y - j );
					} else {
						_between_wall[ image_num % _between_wall.size( ) ]->draw( adjust_pos );
					}
				}
			}
		}
	}
}

void Viewer::drawSelectArea( ) const {
	MousePtr mouse = Task::get< Mouse >( );
	DrawerPtr drawer = Task::get< Drawer >( );
	if ( mouse->isHoldLeftButton( ) && _editor->getSelect( ) == -1 ) {
		Vector2 size = mouse->getPos( ) - _editor->getTempPos1( );
		if ( size.x > 0 && size.y > 0 ) {
			drawer->drawBox( _editor->getTempPos1( ).x, _editor->getTempPos1( ).y,
							 mouse->getPos( ).x, mouse->getPos( ).y,
							 Color( 1.0, 0.0, 0.0 ), false );
		}
	}

	if ( mouse->isHoldRightButton( ) ) {
		Vector2 size = mouse->getPos( ) - _editor->getTempPos1( );
		if ( size.x > 0 && size.y > 0 ) {
			drawer->drawBox( _editor->getTempPos1( ).x, _editor->getTempPos1( ).y,
							 mouse->getPos( ).x, mouse->getPos( ).y,
							 Color( 1.0, 0.0, 0.0 ), false );
		}
	}
}

void Viewer::drawNowSelect( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	if ( _editor->getMode( ) == Editor::MODE::SINGLE ) {
		if ( _editor->getSelectType( ) == 0 ) {
			if ( _editor->getSelect( ) >= 0 && _editor->getSelect( ) < _data->getGroundsSize( ) ) {
				std::vector< GameData::Ground* > grounds = _data->getGrounds( );
				Vector2 adjust_pos = grounds[ _editor->getSelect( ) ]->_pos - _camera->getPos( );
				drawer->drawBox( adjust_pos.x, adjust_pos.y,
								 adjust_pos.x + grounds[ _editor->getSelect( ) ]->_size.x,
								 adjust_pos.y + grounds[ _editor->getSelect( ) ]->_size.y,
								 Color( 1.0, 0.0, 0.0 ), false );
			}
		} else if ( _editor->getSelectType( ) == 8 ) {
			if ( _editor->getSelect( ) >= 0 && _editor->getSelect( ) < _data->getEnemiesSize( ) ) {
				std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
				Vector2 adjust_pos = enemies[ _editor->getSelect( ) ]->_pos - _camera->getPos( );
				drawer->drawBox( adjust_pos.x, adjust_pos.y,
								 adjust_pos.x + enemies[ _editor->getSelect( ) ]->_size.x,
								 adjust_pos.y + enemies[ _editor->getSelect( ) ]->_size.y,
								 Color( 1.0, 0.0, 0.0 ), false );
			}
		} else if ( _editor->getSelectType( ) == 9 ) {
			if ( _editor->getSelect( ) >= 0 && _editor->getSelect( ) < _data->getBetweenObjectsSize( ) ) {
				std::vector< GameData::Ground* > between_objects = _data->getBetweenObjects( );
				Vector2 adjust_pos = between_objects[ _editor->getSelect( ) ]->_pos - _camera->getPos( );
				drawer->drawBox( adjust_pos.x, adjust_pos.y,
								 adjust_pos.x + between_objects[ _editor->getSelect( ) ]->_size.x,
								 adjust_pos.y + between_objects[ _editor->getSelect( ) ]->_size.y,
								 Color( 1.0, 0.0, 0.0 ), false );
			}
		} else {
			if ( _editor->getSelect( ) >= 0 && _editor->getSelect( ) < _data->getGimmicksSize( ) ) {
				std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
				Vector2 adjust_pos = gimmicks[ _editor->getSelect( ) ]->_pos - _camera->getPos( );
				drawer->drawBox( adjust_pos.x, adjust_pos.y,
								 adjust_pos.x + gimmicks[ _editor->getSelect( ) ]->_size.x,
								 adjust_pos.y + gimmicks[ _editor->getSelect( ) ]->_size.y,
								 Color( 1.0, 0.0, 0.0 ), false );
			}
			if ( _editor->getSelect( ) >= _data->getGimmicksSize( ) && _editor->getSelect( ) < _data->getFansSize( ) + _data->getGimmicksSize( ) ) {
				std::vector< GameData::Fan* > fans = _data->getFans( );
				int select = _editor->getSelect( ) - _data->getGimmicksSize( );
				Vector2 adjust_pos = fans[ select ]->_gimmick->_pos - _camera->getPos( );
				drawer->drawBox( adjust_pos.x, adjust_pos.y,
								 adjust_pos.x + fans[ select ]->_gimmick->_size.x,
								 adjust_pos.y + fans[ select ]->_gimmick->_size.y,
								 Color( 1.0, 0.0, 0.0 ), false );
			}
		}
	} else if ( _editor->getMode( ) == Editor::MODE::MULTIPLE ) {
		std::vector< int > multiple_selects = _editor->getMultipleSelects( );
		for ( int i = 0; i < multiple_selects.size( ); i++ ) {
			if ( multiple_selects[ i ] < _data->getGroundsSize( ) ) {
				std::vector< GameData::Ground* > grounds = _data->getGrounds( );
				Vector2 adjust_pos = grounds[ multiple_selects[ i ] ]->_pos - _camera->getPos( );
				drawer->drawBox( adjust_pos.x, adjust_pos.y,
								 adjust_pos.x + grounds[ multiple_selects[ i ] ]->_size.x,
								 adjust_pos.y + grounds[ multiple_selects[ i ] ]->_size.y,
								 Color( 1.0, 0.0, 0.0 ), false );
			} else if ( multiple_selects[ i ] < _data->getGroundsSize( ) + _data->getGimmicksSize( ) ) {
				std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
				int select = multiple_selects[ i ] - _data->getGroundsSize( );
				Vector2 adjust_pos = gimmicks[ select ]->_pos - _camera->getPos( );
				drawer->drawBox( adjust_pos.x, adjust_pos.y,
								 adjust_pos.x + gimmicks[ select ]->_size.x,
								 adjust_pos.y + gimmicks[ select ]->_size.y,
								 Color( 1.0, 0.0, 0.0 ), false );
			} else if ( multiple_selects[ i ] < _data->getGroundsSize( ) + _data->getGimmicksSize( ) + _data->getFansSize( ) ) {
				std::vector< GameData::Fan* > fans = _data->getFans( );
				int select = multiple_selects[ i ] - _data->getGroundsSize( ) - _data->getGimmicksSize( );
				Vector2 adjust_pos = fans[ select ]->_gimmick->_pos - _camera->getPos( );
				drawer->drawBox( adjust_pos.x, adjust_pos.y,
								 adjust_pos.x + fans[ select ]->_gimmick->_size.x,
								 adjust_pos.y + fans[ select ]->_gimmick->_size.y,
								 Color( 1.0, 0.0, 0.0 ), false );
			} else if ( multiple_selects[ i ] < _data->getGroundsSize( ) + _data->getGimmicksSize( ) + _data->getFansSize( ) + _data->getEnemiesSize( ) ) {
				std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
				int select = multiple_selects[ i ] - _data->getGroundsSize( ) - _data->getGimmicksSize( ) - _data->getFansSize( );
				Vector2 adjust_pos = enemies[ select ]->_pos - _camera->getPos( );
				drawer->drawBox( adjust_pos.x, adjust_pos.y,
								 adjust_pos.x + enemies[ select ]->_size.x,
								 adjust_pos.y + enemies[ select ]->_size.y,
								 Color( 1.0, 0.0, 0.0 ), false );
			}
		}
	}
}

void Viewer::drawText( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	MousePtr mouse = Task::get< Mouse >( );
	drawer->drawString( 0, 0, "now Select:%d", _editor->getSelect( ) );
	drawer->drawString( 0, 20, "camera_pos_x:%.1lf", _camera->getPos( ).x );
	drawer->drawString( 0, 40, "camera_pos_y:%.1lf", _camera->getPos( ).y );

	int size = 0;
	if ( _editor->getSelectType( ) == 0 ) {
		size = _data->getGroundsSize( );
	} else if ( _editor->getSelectType( ) == 8 ) {
		size = _data->getEnemiesSize( );
	} else if ( _editor->getSelectType( ) == 9 ) {
		size = _data->getBetweenObjectsSize( );
	} else {
		size = _data->getGimmicksSize( ) + _data->getFansSize( );
	}
	if ( _editor->getSelect( ) <= -1 || _editor->getSelect( ) >= size ) {
		if ( mouse->isHoldLeftButton( ) ) {
			drawer->drawString( 0, 60, "pos_x :%.1lf", _editor->getTempPos1( ).x );
			drawer->drawString( 0, 80, "pos_y :%.1lf", _editor->getTempPos1( ).y );
			drawer->drawString( 0, 100, "size_x:%.1lf", mouse->getPos( ).x - _editor->getTempPos1( ).x );
			drawer->drawString( 0, 120, "size_y:%.1lf", mouse->getPos( ).y - _editor->getTempPos1( ).y );
		}
	} else {
		if ( _editor->getSelectType( ) == 0 ) {
			std::vector< GameData::Ground* > grounds = _data->getGrounds( );
			drawer->drawString( 0, 60, "now_select_ground_pos_x :%.1lf", grounds[ _editor->getSelect( ) ]->_pos.x );
			drawer->drawString( 0, 80, "now_select_ground_pos_y :%.1lf", grounds[ _editor->getSelect( ) ]->_pos.y );
			drawer->drawString( 0, 100, "now_select_ground_size_x:%.1lf", grounds[ _editor->getSelect( ) ]->_size.x );
			drawer->drawString( 0, 120, "now_select_ground_size_y:%.1lf", grounds[ _editor->getSelect( ) ]->_size.y );
		} else if ( _editor->getSelectType( ) == 8 ) {
			std::vector< GameData::Enemy* > enemies = _data->getEnemies( );
			drawer->drawString( 0, 60, "now_select_enemy_pos_x :%.1lf", enemies[ _editor->getSelect( ) ]->_pos.x );
			drawer->drawString( 0, 80, "now_select_enemy_pos_y :%.1lf", enemies[ _editor->getSelect( ) ]->_pos.y );
			drawer->drawString( 0, 100, "now_select_enemy_size_x:%.1lf", enemies[ _editor->getSelect( ) ]->_size.x );
			drawer->drawString( 0, 120, "now_select_enemy_size_y:%.1lf", enemies[ _editor->getSelect( ) ]->_size.y );
			drawer->drawString( 0, 140, "now_select_enemy_move_pattern:%d", enemies[ _editor->getSelect( ) ]->_move_pattern );
			drawer->drawString( 0, 160, "now_select_enemy_vec_x:%.1lf", enemies[ _editor->getSelect( ) ]->_vec.x );
			drawer->drawString( 0, 180, "now_select_enemy_vec_y:%.1lf", enemies[ _editor->getSelect( ) ]->_vec.y );
		} else if ( _editor->getSelectType( ) == 9 ) {
			std::vector< GameData::Ground* > between_objects = _data->getBetweenObjects( );
			drawer->drawString( 0, 60, "now_select_between_object_pos_x :%.1lf", between_objects[ _editor->getSelect( ) ]->_pos.x );
			drawer->drawString( 0, 80, "now_select_between_object_pos_y :%.1lf", between_objects[ _editor->getSelect( ) ]->_pos.y );
			drawer->drawString( 0, 100, "now_select_between_object_size_x:%.1lf", between_objects[ _editor->getSelect( ) ]->_size.x );
			drawer->drawString( 0, 120, "now_select_between_object_size_y:%.1lf", between_objects[ _editor->getSelect( ) ]->_size.y );
		} else {
			if ( _editor->getSelect( ) >= _data->getGimmicksSize( ) && _editor->getSelect( ) < _data->getFansSize( ) + _data->getGimmicksSize( ) ) {
				std::vector< GameData::Fan* > fans = _data->getFans( );
				int select = _editor->getSelect( ) - _data->getGimmicksSize( );
				drawer->drawString( 0, 60, "now_select_fan_pos_x :%.1lf", fans[ select ]->_gimmick->_pos.x );
				drawer->drawString( 0, 80, "now_select_fan_pos_y :%.1lf", fans[ select ]->_gimmick->_pos.y );
				drawer->drawString( 0, 100, "now_select_fan_size_x:%.1lf", fans[ select ]->_gimmick->_size.x );
				drawer->drawString( 0, 120, "now_select_fan_size_y:%.1lf", fans[ select ]->_gimmick->_size.y );
				drawer->drawString( 0, 140, "now_select_fan_dir:%d", fans[ select ]->_gimmick->_dir );
				drawer->drawString( 0, 160, "now_select_fan_vec:%.1lf", fans[ select ]->_vec );
				drawer->drawString( 0, 180, "now_select_fan_on:%d", fans[ select ]->_start_state );
				drawer->drawString( 0, 200, "now_select_fan_auto_switching:%d", fans[ select ]->_auto_switching );
				drawer->drawString( 0, 220, "now_select_fan_on_time:%d", fans[ select ]->_on_time );
				drawer->drawString( 0, 240, "now_select_fan_off_time:%d", fans[ select ]->_off_time );
			} else {
				std::vector< GameData::Gimmick* > gimmicks = _data->getGimmicks( );
				drawer->drawString( 0, 60, "now_select_gimmick_pos_x :%.1lf", gimmicks[ _editor->getSelect( ) ]->_pos.x );
				drawer->drawString( 0, 80, "now_select_gimmick_pos_y :%.1lf", gimmicks[ _editor->getSelect( ) ]->_pos.y );
				drawer->drawString( 0, 100, "now_select_gimmick_size_x:%.1lf", gimmicks[ _editor->getSelect( ) ]->_size.x );
				drawer->drawString( 0, 120, "now_select_gimmick_size_y:%.1lf", gimmicks[ _editor->getSelect( ) ]->_size.y );
				drawer->drawString( 0, 140, "now_select_gimmick_dir:%d", gimmicks[ _editor->getSelect( ) ]->_dir );
			}
		}
	}

	ApplicationPtr app = Application::getInstance( );
	int x = app->getScreenWidth( ) / 2 - 100;
	int y = app->getScreenHeight( ) / 2;
	switch ( _data->getState( ) ) {
	case GameData::STATE::SAVE_SUCCESS:
		drawer->drawString( x, y, "セーブしました" );
		break;
	case GameData::STATE::SAVE_FAILURE:
		drawer->drawString( x, y, "セーブ失敗" );
		break;
	case GameData::STATE::LOAD_SUCCESS:
		drawer->drawString( x, y, "ロードしました" );
		break;
	case GameData::STATE::LOAD_FAILURE:
		drawer->drawString( x, y, "ロード失敗" );
		break;
	}
}

void Viewer::drawSelecter( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );
	int x = app->getScreenWidth( ) - 80;
	for ( int i = 0; i < 10; i++ ) {
		int y = 160 + 8 * ( i + 1 + i * 2 ) + 64 * i;
		switch ( i ) {
		case 0:
			_floor_top[ 0 ]->drawExtend( x, y, x + 64, y + 64 );
			break;
		case 1:
			_goal_plate->draw( x, y );
			break;
		case 2:
			_respawn_plate->draw( x, y );
			break;
		case 3:
			_acid_plate->draw( x, y );
			break;
		case 4:
			_needle_plate->draw( x, y );
			break;
		case 5:
			_fan_plate->draw( x, y );
			break;
		case 6:
			_decelerate_net_plate->draw( x, y );
			break;
		case 7:
			_sticky_net_plate->draw( x, y );
			break;
		case 8:
			_enemy_plate->draw( x, y );
			break;
		case 9:
			drawer->drawBox( x, y, x + 64, y + 64, Color( 0.0, 1.0, 0.0 ), true );
			break;
		}
		if ( i == _editor->getSelectType( ) ) {
			drawer->drawBox( x - 8, y - 8, x + 80 - 8, y + 80 - 8, Color( 0.0, 1.0, 0.0 ), false );
		}
	}
}

void Viewer::drawLange( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	Vector2 adjust_pos = Vector2( 0, 0 ) - _camera->getPos( );
	drawer->drawBox( adjust_pos.x, adjust_pos.y,
					 adjust_pos.x + _data->getWidth( ),
					 adjust_pos.y + _data->getHeight( ),
					 Color( 0.0, 0.0, 1.0 ), false );
}
