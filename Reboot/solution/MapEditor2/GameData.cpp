#include "GameData.h"
#include "Application.h"
#include "Binary.h"
#include <direct.h>
#include "Define.h"
#include "Drawer.h"

const std::string DIRECTORY_DATA = "Resource/MapData/";
const std::string EXTENSION = ".dat";

GameData::GameData( ) {
	_state = STATE::NONE;
	_reset_count = 0;
	_width = 1920;
	_height = 1080;
}

GameData::~GameData( ) {

}


void GameData::createGround( Vector2 pos, Vector2 size ) {
	Ground* ground = new Ground( );
	ground->_pos = pos;
	ground->_size = size;
	_grounds.push_back( ground );
}

void GameData::updateState( ) {
	if ( _state != STATE::NONE ) {
		_reset_count++;
		if ( _reset_count > 90 ) {
			_reset_count = 0;
			_state = STATE::NONE;
		}
	}
}

std::vector< GameData::Ground* > GameData::getGrounds( ) const {
	return _grounds;
}

int GameData::getGroundsSize( ) const {
	return _grounds.size( );
}

GameData::STATE GameData::getState( ) const {
	return _state;
}

int GameData::getWidth( ) const {
	return _width;
}

int GameData::getHeight( ) const {
	return _height;
}

void GameData::deleteGround( int select ) {
	_grounds.erase( _grounds.begin( ) + select );
}

void GameData::createGimmick( Vector2 pos, Vector2 size, Gimmick::TYPE type, Gimmick::DIR dir ) {
	Gimmick* gimmick = new Gimmick( );
	gimmick->_pos = pos;
	gimmick->_size = size;
	gimmick->_type = type;
	gimmick->_dir = dir;
	_gimmicks.push_back( gimmick );
}

void GameData::createFan( Vector2 pos, Vector2 size, Gimmick::DIR dir ) {
	Gimmick* gimmick = new Gimmick( );
	gimmick->_pos = pos;
	gimmick->_size = size;
	gimmick->_type = Gimmick::TYPE::FAN;
	gimmick->_dir = dir;

	Fan* fan = new Fan( );
	fan->_gimmick = gimmick;
	fan->_vec = 100;
	fan->_start_state = true;
	fan->_auto_switching = false;
	fan->_on_time = 0;
	fan->_off_time = 0;
	_fans.push_back( fan );
}

std::vector< GameData::Fan* > GameData::getFans( ) const {
	return _fans;
}

int GameData::getFansSize( ) const {
	return _fans.size( );
}

void GameData::deleteFan( int select ) {
	_fans.erase( _fans.begin( ) + select );
}

void GameData::createEnemy( Vector2 pos, Enemy::MOVE_PATTERN move_pattern ) {
	Enemy* enemy = new Enemy( );
	enemy->_pos = pos;
	enemy->_vec = Vector2( 0, 0 );
	enemy->_size = ENEMY_SIZE;
	enemy->_move_pattern = move_pattern;
	_enemies.push_back( enemy );
}

std::vector< GameData::Enemy* > GameData::getEnemies( ) const {
	return _enemies;
}

int GameData::getEnemiesSize( ) const {
	return _enemies.size( );
}

void GameData::deleteEnemy( int select ) {
	_enemies.erase( _enemies.begin( ) + select );
}

void GameData::createBetweenObject( Vector2 pos, Vector2 size ) {
	Ground* between_object = new Ground( );
	between_object->_pos = pos;
	between_object->_size = size;
	_between_objects.push_back( between_object );
}

std::vector< GameData::Ground*> GameData::getBetweenObjects( ) const {
	return _between_objects;
}

int GameData::getBetweenObjectsSize( ) const {
	return _between_objects.size( );
}

void GameData::deleteBetweenObject( int select ) {
	_between_objects.erase( _between_objects.begin( ) + select );
}

std::vector< GameData::Gimmick* > GameData::getGimmicks( ) const {
	return _gimmicks;
}

int GameData::getGimmicksSize( ) const {
	return _gimmicks.size( );
}

void GameData::deleteGimmick( int select ) {
	_gimmicks.erase( _gimmicks.begin( ) + select );
}

int& GameData::setHeight( ) {
	return _height;
}

int& GameData::setWidth( ) {
	return _width;
}

void GameData::save( std::string filename ) {
	_mkdir( DIRECTORY_DATA.c_str( ) );

	filename += EXTENSION;
	filename = DIRECTORY_DATA + filename;

	BinaryPtr binary( new Binary );
	int ground_size = ( int )( sizeof( Vector2 ) * 2 );
	int gimmick_size = ( int )( sizeof( Vector2 ) * 2 + sizeof( int ) * 2 );
	int fan_size = ( int )( sizeof( double ) + sizeof( bool ) * 2 + sizeof( int ) * 2 ) + gimmick_size;
	int enemy_size = ( int )( sizeof( Vector2 ) * 2 + sizeof( int ) );
	int between_object_size = ( int )( sizeof( Vector2 ) * 2 );

	int size = ( int )( sizeof( int ) * 7 + ground_size * _grounds.size( ) + gimmick_size * _gimmicks.size( )
						+ fan_size * _fans.size( ) + enemy_size * _enemies.size( ) + between_object_size * _between_objects.size( ) );
	binary->ensure( size );

	binary->write( ( void* )&_width, sizeof( int ) );
	binary->write( ( void* )&_height, sizeof( int ) );

	int grounds_size = _grounds.size( );
	binary->write( ( void* )&grounds_size, sizeof( int ) );

	for ( int i = 0; i < _grounds.size( ); i++ ) {
		binary->write( ( void* )&_grounds[ i ]->_pos, sizeof( Vector2 ) );
		binary->write( ( void* )&_grounds[ i ]->_size, sizeof( Vector2 ) );
	}

	int gimmicks_size = _gimmicks.size( );
	binary->write( ( void* )&gimmicks_size, sizeof( int ) );

	for ( int i = 0; i < _gimmicks.size( ); i++ ) {
		binary->write( ( void* )&_gimmicks[ i ]->_pos, sizeof( Vector2 ) );
		binary->write( ( void* )&_gimmicks[ i ]->_size, sizeof( Vector2 ) );
		int type = ( int )_gimmicks[ i ]->_type;
		binary->write( ( void* )&type, sizeof( int ) );
		int dir = ( int )_gimmicks[ i ]->_dir;
		binary->write( ( void* )&dir, sizeof( int ) );
	}

	int fans_size = _fans.size( );
	binary->write( ( void* )&fans_size, sizeof( int ) );

	for ( int i = 0; i < _fans.size( ); i++ ) {
		binary->write( ( void* )&_fans[ i ]->_gimmick->_pos, sizeof( Vector2 ) );
		binary->write( ( void* )&_fans[ i ]->_gimmick->_size, sizeof( Vector2 ) );
		int type = ( int )_fans[ i ]->_gimmick->_type;
		binary->write( ( void* )&type, sizeof( int ) );
		int dir = ( int )_fans[ i ]->_gimmick->_dir;
		binary->write( ( void* )&dir, sizeof( int ) );

		binary->write( ( void* )&_fans[ i ]->_vec, sizeof( double ) );
		binary->write( ( void* )&_fans[ i ]->_start_state, sizeof( bool ) );
		binary->write( ( void* )&_fans[ i ]->_auto_switching, sizeof( bool ) );
		binary->write( ( void* )&_fans[ i ]->_on_time, sizeof( int ) );
		binary->write( ( void* )&_fans[ i ]->_off_time, sizeof( int ) );
	}

	int enemies_size = _enemies.size( );
	binary->write( ( void* )&enemies_size, sizeof( int ) );

	for ( int i = 0; i < _enemies.size( ); i++ ) {
		binary->write( ( void* )&_enemies[ i ]->_pos, sizeof( Vector2 ) );
		binary->write( ( void* )&_enemies[ i ]->_vec, sizeof( Vector2 ) );
		int move_pattern = ( int )_enemies[ i ]->_move_pattern;
		binary->write( ( void* )&move_pattern, sizeof( int ) );
	}

	int between_objects_size = _between_objects.size( );
	binary->write( ( void* )&between_objects_size, sizeof( int ) );

	for ( int i = 0; i < _between_objects.size( ); i++ ) {
		binary->write( ( void* )&_between_objects[ i ]->_pos, sizeof( Vector2 ) );
		binary->write( ( void* )&_between_objects[ i ]->_size, sizeof( Vector2 ) );
	}

	ApplicationPtr app( Application::getInstance( ) );
	app->saveBinary( filename, binary );

	DrawerPtr drawer = Task::get< Drawer >( );
	struct stat buf;
	if ( stat( filename.c_str( ), &buf ) == 0 ) {
		_state = STATE::SAVE_SUCCESS;
	} else {
		_state = STATE::SAVE_FAILURE;
	}
}

void GameData::load( std::string filename ) {
	filename += EXTENSION;
	filename = DIRECTORY_DATA + filename;

	BinaryPtr binary( new Binary );

	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app( Application::getInstance( ) );
	if ( !app->loadBinary( filename, binary ) ) {
		_state = STATE::LOAD_FAILURE;
		return;
	} else {
		_state = STATE::LOAD_SUCCESS;
	}


	binary->read( ( void* )&_width, sizeof( int ) );
	binary->read( ( void* )&_height, sizeof( int ) );

	{
		int grounds_size = _grounds.size( );
		for ( int i = 0; i < grounds_size; i++ ) {
			_grounds.erase( _grounds.begin( ) );
		}

		binary->read( ( void* )&grounds_size, sizeof( int ) );

		for ( int i = 0; i < grounds_size; i++ ) {
			Ground* ground = new Ground( );
			binary->read( ( void* )&ground->_pos, sizeof( Vector2 ) );
			binary->read( ( void* )&ground->_size, sizeof( Vector2 ) );
			_grounds.push_back( ground );
		}
	}


	{
		int gimmicks_size = _gimmicks.size( );
		for ( int i = 0; i < gimmicks_size; i++ ) {
			_gimmicks.erase( _gimmicks.begin( ) );
		}

		binary->read( ( void* )&gimmicks_size, sizeof( int ) );

		for ( int i = 0; i < gimmicks_size; i++ ) {
			Gimmick* gimmick = new Gimmick( );
			binary->read( ( void* )&gimmick->_pos, sizeof( Vector2 ) );
			binary->read( ( void* )&gimmick->_size, sizeof( Vector2 ) );
			int type = 0;
			binary->read( ( void* )&type, sizeof( int ) );
			gimmick->_type = ( Gimmick::TYPE )type;
			int dir = 0;
			binary->read( ( void* )&dir, sizeof( int ) );
			gimmick->_dir = ( Gimmick::DIR )dir;
			_gimmicks.push_back( gimmick );
		}
	}

	{
		int fans_size = _fans.size( );
		for ( int i = 0; i < fans_size; i++ ) {
			_fans.erase( _fans.begin( ) );
		}

		binary->read( ( void* )&fans_size, sizeof( int ) );

		for ( int i = 0; i < fans_size; i++ ) {
			Gimmick* gimmick = new Gimmick( );
			binary->read( ( void* )&gimmick->_pos, sizeof( Vector2 ) );
			binary->read( ( void* )&gimmick->_size, sizeof( Vector2 ) );
			int type = 0;
			binary->read( ( void* )&type, sizeof( int ) );
			gimmick->_type = ( Gimmick::TYPE )type;
			int dir = 0;
			binary->read( ( void* )&dir, sizeof( int ) );
			gimmick->_dir = ( Gimmick::DIR )dir;

			Fan* fan = new Fan( );
			fan->_gimmick = gimmick;
			binary->read( ( void* )&fan->_vec, sizeof( double ) );
			binary->read( ( void* )&fan->_start_state, sizeof( bool ) );
			binary->read( ( void* )&fan->_auto_switching, sizeof( bool ) );
			binary->read( ( void* )&fan->_on_time, sizeof( int ) );
			binary->read( ( void* )&fan->_off_time, sizeof( int ) );

			_fans.push_back( fan );
		}
	}

	{
		int enemies_size = _enemies.size( );
		for ( int i = 0; i < enemies_size; i++ ) {
			_enemies.erase( _enemies.begin( ) );
		}

		binary->read( ( void* )&enemies_size, sizeof( int ) );

		for ( int i = 0; i < enemies_size; i++ ) {
			Enemy* enemy = new Enemy( );
			binary->read( ( void* )&enemy->_pos, sizeof( Vector2 ) );

			binary->read( ( void* )&enemy->_vec, sizeof( Vector2 ) );

			enemy->_size = ENEMY_SIZE;
			int move_pattern = 0;
			binary->read( ( void* )&move_pattern, sizeof( int ) );
			enemy->_move_pattern = ( Enemy::MOVE_PATTERN )move_pattern;
			_enemies.push_back( enemy );
		}
	}

	{
		int between_objects_size = _between_objects.size( );
		for ( int i = 0; i < between_objects_size; i++ ) {
			_between_objects.erase( _between_objects.begin( ) );
		}

		binary->read( ( void* )&between_objects_size, sizeof( int ) );

		for ( int i = 0; i < between_objects_size; i++ ) {
			Ground* between_object = new Ground( );
			binary->read( ( void* )&between_object->_pos, sizeof( Vector2 ) );
			binary->read( ( void* )&between_object->_size, sizeof( Vector2 ) );
			_between_objects.push_back( between_object );
		}
	}
}

void GameData::oldLoad( std::string filename ) {
	filename += EXTENSION;
	filename = DIRECTORY_DATA + filename;

	BinaryPtr binary( new Binary );

	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app( Application::getInstance( ) );
	if ( !app->loadBinary( filename, binary ) ) {
		_state = STATE::LOAD_FAILURE;
		return;
	} else {
		_state = STATE::LOAD_SUCCESS;
	}


	binary->read( ( void* )&_width, sizeof( int ) );
	binary->read( ( void* )&_height, sizeof( int ) );

	{
		int grounds_size = _grounds.size( );
		for ( int i = 0; i < grounds_size; i++ ) {
			_grounds.erase( _grounds.begin( ) );
		}

		binary->read( ( void* )&grounds_size, sizeof( int ) );

		for ( int i = 0; i < grounds_size; i++ ) {
			Ground* ground = new Ground( );
			binary->read( ( void* )&ground->_pos, sizeof( Vector2 ) );
			binary->read( ( void* )&ground->_size, sizeof( Vector2 ) );
			_grounds.push_back( ground );
		}
	}


	{
		int gimmicks_size = _gimmicks.size( );
		for ( int i = 0; i < gimmicks_size; i++ ) {
			_gimmicks.erase( _gimmicks.begin( ) );
		}

		binary->read( ( void* )&gimmicks_size, sizeof( int ) );

		for ( int i = 0; i < gimmicks_size; i++ ) {
			Gimmick* gimmick = new Gimmick( );
			binary->read( ( void* )&gimmick->_pos, sizeof( Vector2 ) );
			binary->read( ( void* )&gimmick->_size, sizeof( Vector2 ) );
			int type = 0;
			binary->read( ( void* )&type, sizeof( int ) );
			gimmick->_type = ( Gimmick::TYPE )type;
			int dir = 0;
			binary->read( ( void* )&dir, sizeof( int ) );
			gimmick->_dir = ( Gimmick::DIR )dir;
			_gimmicks.push_back( gimmick );
		}
	}

	{
		int fans_size = _fans.size( );
		for ( int i = 0; i < fans_size; i++ ) {
			_fans.erase( _fans.begin( ) );
		}

		binary->read( ( void* )&fans_size, sizeof( int ) );

		for ( int i = 0; i < fans_size; i++ ) {
			Gimmick* gimmick = new Gimmick( );
			binary->read( ( void* )&gimmick->_pos, sizeof( Vector2 ) );
			binary->read( ( void* )&gimmick->_size, sizeof( Vector2 ) );
			int type = 0;
			binary->read( ( void* )&type, sizeof( int ) );
			gimmick->_type = ( Gimmick::TYPE )type;
			int dir = 0;
			binary->read( ( void* )&dir, sizeof( int ) );
			gimmick->_dir = ( Gimmick::DIR )dir;

			Fan* fan = new Fan( );
			fan->_gimmick = gimmick;
			binary->read( ( void* )&fan->_vec, sizeof( double ) );
			binary->read( ( void* )&fan->_start_state, sizeof( bool ) );
			binary->read( ( void* )&fan->_auto_switching, sizeof( bool ) );
			binary->read( ( void* )&fan->_on_time, sizeof( int ) );
			binary->read( ( void* )&fan->_off_time, sizeof( int ) );

			_fans.push_back( fan );
		}
	}

	{
		int enemies_size = _enemies.size( );
		for ( int i = 0; i < enemies_size; i++ ) {
			_enemies.erase( _enemies.begin( ) );
		}

		binary->read( ( void* )&enemies_size, sizeof( int ) );

		for ( int i = 0; i < enemies_size; i++ ) {
			Enemy* enemy = new Enemy( );
			binary->read( ( void* )&enemy->_pos, sizeof( Vector2 ) );

			binary->read( ( void* )&enemy->_vec, sizeof( Vector2 ) );

			enemy->_size = ENEMY_SIZE;
			int move_pattern = 0;
			binary->read( ( void* )&move_pattern, sizeof( int ) );
			enemy->_move_pattern = ( Enemy::MOVE_PATTERN )move_pattern;
			_enemies.push_back( enemy );
		}
	}

}
