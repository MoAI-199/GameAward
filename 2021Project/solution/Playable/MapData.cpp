#include "MapData.h"
#include "Gimmick.h"
#include "Player.h"
#include "Define.h"
#include "Enemy.h"
#include "Fan.h"
#include "Binary.h"
#include "Application.h"
#include "Log.h"
#include "Menu.h"
#include "Exception.h"
#include "Music.h"
#include "StageManager.h"

static const int START_POS_X = 100;
static const int START_POS_Y = 50;

const std::string DIRECTORY_DATA = "Resource/MapData/";
const std::string EXTENSION = ".dat";
/*debug
debug_acid
debug_fan
debug_net1
debug_net2
debug_needle
debug_enemy
*/
/*main
main_stage_01//移動
main_stage_02//落下死
main_stage_20//自爆tutorial
main_stage_03//酸
main_stage_10//足場の両端に棘
kuso_stage//上るやつ
main_stage_05//棘で壁に張り付く
main_stage_08//階段と敵
main_stage_09//てきがいっぱい、横長
main_stage_07//ファンでふわ敵よける
main_stage_006//初見殺しの下を通るステージ
fan_s2_60fps//くねくねファン
main_stage_19.3//洗濯機
main_stage_33_ver6//ファンで死体をぶっ飛ばして渡る
main_stage_21//ねばねばネットのステージ
main_stage_18//むずいやつファンで上がっていく
main_stage_40//自爆で上る。2ルート
*/
MapData::MapData( int stage_num, ControllerPtr controller, MenuPtr menu ) :
	_controller( controller ),
	_menu( menu ) {
	_width = 4000;
	_height = 4000;
	_stage[ 0 ] =  "main_stage_01";
	_stage[ 1 ] =  "main_stage_02";
	_stage[ 2 ] =  "main_stage_20";
	_stage[ 3 ] =  "main_stage_03";
	_stage[ 4 ] =  "main_stage_10";
	_stage[ 5 ] = "kuso_stage";
	_stage[ 6 ] =  "main_stage_05";
	_stage[ 7 ] =  "main_stage_08";
	_stage[ 8 ] =  "main_stage_09";
	_stage[ 9 ] =  "main_stage_07";
	_stage[ 10 ] =  "main_stage_006";
	_stage[ 11 ] = "fan_s2_60fps";
	_stage[ 12 ] = "main_stage_19.3.1";
	_stage[ 13 ] = "main_stage_33_ver7";
	_stage[ 14 ] = "main_stage_40";
	_stage[ 15 ] = "main_stage_18";
	_stage[ STAGE_NUM ] = "ending_stage";//STAGE_NUM
	_delete_player_num = 0;
	_spawned = true;
	_reset_count = 0;
	_respawn_count = 0;

	ApplicationPtr app( Application::getInstance( ) );
	if ( _stage.size( ) - 1 < stage_num ) {
#ifdef _DEBUG
		EXCEPTION( true, "ERROR[out of range] fail loaded file" );
#endif
		return;
	}
	/*//ending_debug
	StageManagerPtr stage_manager = Task::get< StageManager >( );
	stage_manager->setStageNum( STAGE_NUM );
	load( _stage[ STAGE_NUM ] );
	*/
	load( _stage[ stage_num ] );
	createPlayer( );
}

MapData::~MapData( ) {
}

void MapData::update( ) {
	if ( _menu->isMenu( ) ) {
		return;
	}
	deletePlayer( );
	updatePlayer( );
	deleteGimmick( );
	deleteEnemy( );
	sortPlayersInOrderOfPosY( );
	updateFan( );
	updateEnemy( );
	if ( _spawned ) {
		_reset_count++;
		if ( _reset_count > 20 ) {
			_reset_count = 0;
			_spawned = false;
		}
	}
}

void MapData::updateFan( ) {
	for ( int i = 0; i < _gimmicks.size( ); i++ ) {
		if ( _gimmicks[ i ]->getType( ) == Gimmick::TYPE::FAN ) {
			std::dynamic_pointer_cast< Fan >( _gimmicks[ i ] )->update( );
		}
	}
}

void MapData::updateEnemy( ) {
	for ( int i = 0; i < _enemies.size( ); i++ ) {
		_enemies[ i ]->update( );
	}
}

void MapData::updatePlayer( ) {
	LogPtr log = Task::get< Log >( );
	StageManagerPtr stage_manager = Task::get< StageManager >( );
	int suicider_count = 0;
	for ( int i = 0; i < _players.size( ); i++ ) {
		if ( _stage[ stage_manager->getStageNum( ) ] == "ending_stage" ) {
			_players[ i ]->endingUpdate( );
		} else {
			_players[ i ]->update( );
			if ( _players[ i ]->getState( ) == Player::STATE::DEAD && !_players[ i ]->isInherited( ) ) {
				_respawn_count++;
				if ( _respawn_count > RESPAWN_TIME ) {
					createPlayer( );
					_players[ i ]->setInherited( );
					_respawn_count = 0;
				}
			}
		}
		if ( _players[ i ]->getState( ) == Player::STATE::ALIVE ) {
			if ( _players[ i ]->getPos( ).x < 0 ) {
				_players[ i ]->setPos( ) = Vector2( 0, _players[ i ]->setPos( ).y );
			}
			if ( _players[ i ]->getPos( ).x + _players[ i ]->getSize( ).x > _width ) {
				_players[ i ]->setPos( ) = Vector2( _width - _players[ i ]->getSize( ).x, _players[ i ]->setPos( ).y );
			}
			log->status( "standing:%d", _players[ i ]->isStanding( ) );
		} else if ( _players[ i ]->getReason( ) == Player::DEAD_REASON::SUICIDE ) {
			suicider_count++;
		}
	}

	if ( suicider_count > MAX_SUICIDER_NUM ) {
		int player_num = -1;
		int after_suicide_existence_count = 0;
		for ( int i = 0; i < _players.size( ); i++ ) {
			if ( _players[ i ]->getReason( ) == Player::DEAD_REASON::SUICIDE ) {
				if ( after_suicide_existence_count < _players[ i ]->getAfterSuicideExistenceCount( ) ) {
					after_suicide_existence_count = _players[ i ]->getAfterSuicideExistenceCount( );
					player_num = i;
				}
			}
		}
		_players[ player_num ]->setDelete( );
	}
}

std::vector< MapData::Ground* > MapData::getGrounds( ) const {
	return _grounds;
}

std::vector< GimmickPtr > MapData::getGimmicks( ) const {
	return _gimmicks;
}

std::vector< PlayerPtr > MapData::getPlayers( ) const {
	return _players;
}

std::vector< EnemyPtr> MapData::getEnemies( ) const {
	return _enemies;
}

std::vector<MapData::BetweenObject*> MapData::getBetweenObjects( ) const {
	return _between_objects;
}

void MapData::createGround( Vector2 pos, Vector2 size ) {
	Ground* ground = new Ground( );
	ground->pos = pos;
	ground->view_size = size;
	ground->actual_size = size - Vector2( 0, ADJUST_GROUND_Y );
	_grounds.push_back( ground );
}

void MapData::createGimmick( Vector2 pos, Vector2 size, Gimmick::TYPE type ) {
	if ( !isNeedDirToGimmck( type ) && type != Gimmick::TYPE::FAN ) {
		_gimmicks.push_back( GimmickPtr( new Gimmick( pos, size, type, Gimmick::DIR::NONE ) ) );
	}
}

void MapData::createGimmick( Vector2 pos, Vector2 size, Gimmick::TYPE type, Gimmick::DIR dir ) {
	if ( isNeedDirToGimmck( type ) && type != Gimmick::TYPE::FAN ) {
		_gimmicks.push_back( GimmickPtr( new Gimmick( pos, size, type, dir ) ) );
	}
}

void MapData::createFan( Vector2 pos, Gimmick::DIR dir, double vec, bool start_state, bool auto_switching, int on_time, int off_time ) {
	Vector2 size = FAN_SIZE;
	if ( dir == Gimmick::DIR::LEFT || dir == Gimmick::DIR::RIGHT ) {
		size = Vector2( size.y, size.x );
	}
	_gimmicks.push_back( GimmickPtr( new Fan( pos, size, dir, vec, start_state, auto_switching, on_time, off_time ) ) );
}

void MapData::createPlayer( ) {
	for ( int i = 0; i < _gimmicks.size( ); i++ ) {
		if ( _gimmicks[ i ]->getType( ) == Gimmick::TYPE::RESPAWN ) {
			int add_respawn_pos_y = 15;
			_players.push_back( PlayerPtr( new Player( _gimmicks[ i ]->getPos( ) + Vector2( _gimmicks[ i ]->getSize( ).x / 2 - PLAYER_SIZE_X / 2, _gimmicks[ i ]->getSize( ).y - PLAYER_SIZE_Y + add_respawn_pos_y ), _controller ) ) );
			_spawned = true;
		}
	}
}

void MapData::createEnemy( Vector2 pos, Enemy::MOVE_PATTERN pattern, Vector2 vec ) {
	_enemies.push_back( EnemyPtr( new Enemy( pos, pattern, vec ) ) );
}

void MapData::createBetweenObject( Vector2 pos, Vector2 size ) {
	BetweenObject* between_object = new BetweenObject( );
	between_object->pos = pos;
	between_object->size = size;
	_between_objects.push_back( between_object );
}

void MapData::deletePlayer( ) {
	for ( int i = 0; i < _players.size( ); i++ ) {
		if ( _players[ i ]->isDeleted( ) && _players[ i ]->isInherited( ) ) {
			_players.erase( _players.begin( ) + i );
			_delete_player_num++;
		}
	}
}

void MapData::deleteEnemy( ) {
	for ( int i = 0; i < _enemies.size( ); i++ ) {
		if ( _enemies[ i ]->isDeleted( ) ) {
			_enemies.erase( _enemies.begin( ) + i );
		}
	}
}

void MapData::deleteGimmick( ) {
	for ( int i = 0; i < _gimmicks.size( ); i++ ) {
		if ( _gimmicks[ i ]->isDeleted( ) ) {
			_gimmicks.erase( _gimmicks.begin( ) + i );
		}
	}
}

int MapData::getDeadPlayerNum( ) const {
	return ( int )( _players.size( ) - 1 + _delete_player_num );
}

bool MapData::isSpawned( ) const {
	return _spawned;
}

int MapData::getWidth( ) const {
	return _width;
}

int MapData::getHeight( ) const {
	return _height;
}

int MapData::getRespawnCount( ) const {
	return _respawn_count;
}

std::string MapData::getStageName( int stage_num ) const {
	if ( stage_num > STAGE_NUM ) {
		return std::string( );
	}
	return _stage.at( stage_num );
}


void MapData::sortPlayersInOrderOfPosY( ) {
	if ( _players.size( ) == 0 ) {
		return;
	}
	for ( int i = 0; i < _players.size( ) - 1; i++ ) {
		if ( _players[ i ]->getPos( ).y > _players[ ( __int64 )i + 1 ]->getPos( ).y ) {
			PlayerPtr back_up = _players[ i ];
			_players[ i ] = _players[ ( __int64 )i + 1 ];
			_players[ ( __int64 )i + 1 ] = back_up;
			i = 0;
		}
	}
}



bool MapData::isNeedDirToGimmck( Gimmick::TYPE type ) const {
	bool need = false;
	switch ( type ) {
	case Gimmick::TYPE::NEEDLE:
	case Gimmick::TYPE::FAN:
		need = true;
		break;
	}
	return need;
}

void MapData::load( std::string filename ) {
	filename += EXTENSION;
	filename = DIRECTORY_DATA + filename;

	BinaryPtr binary( new Binary );

	ApplicationPtr app( Application::getInstance( ) );
	if ( !app->loadBinary( filename, binary ) ) {
#ifdef _DEBUG
		EXCEPTION( !binary, "ERROR[file::load] fail loaded file" );
#endif
		return;
	}

	binary->read( ( void* )&_width, sizeof( int ) );
	binary->read( ( void* )&_height, sizeof( int ) );

	{
		int grounds_size = ( int )_grounds.size( );
		for ( int i = 0; i < grounds_size; i++ ) {
			_grounds.erase( _grounds.begin( ) );
		}

		binary->read( ( void* )&grounds_size, sizeof( int ) );

		for ( int i = 0; i < grounds_size; i++ ) {
			Vector2 pos = Vector2( );
			binary->read( ( void* )&pos, sizeof( Vector2 ) );
			Vector2 size = Vector2( );
			binary->read( ( void* )&size, sizeof( Vector2 ) );
			createGround( pos, size );
		}
	}


	{
		int gimmicks_size = ( int )_gimmicks.size( );
		for ( int i = 0; i < gimmicks_size; i++ ) {
			_gimmicks.erase( _gimmicks.begin( ) );
		}

		binary->read( ( void* )&gimmicks_size, sizeof( int ) );

		for ( int i = 0; i < gimmicks_size; i++ ) {
			Vector2 pos = Vector2( );
			binary->read( ( void* )&pos, sizeof( Vector2 ) );
			Vector2 size = Vector2( );
			binary->read( ( void* )&size, sizeof( Vector2 ) );
			int type = 0;
			binary->read( ( void* )&type, sizeof( int ) );
			int dir = 0;
			binary->read( ( void* )&dir, sizeof( int ) );
			if ( isNeedDirToGimmck( ( Gimmick::TYPE )type ) ) {
				createGimmick( pos, size, ( Gimmick::TYPE )type, ( Gimmick::DIR )dir );
			} else {
				createGimmick( pos, size, ( Gimmick::TYPE )type );
			}
		}

		int fans_size = 0;
		binary->read( ( void* )&fans_size, sizeof( int ) );

		for ( int i = 0; i < fans_size; i++ ) {
			Vector2 pos = Vector2( );
			binary->read( ( void* )&pos, sizeof( Vector2 ) );
			Vector2 size = Vector2( );
			binary->read( ( void* )&size, sizeof( Vector2 ) );
			int type = 0;
			binary->read( ( void* )&type, sizeof( int ) );
			int dir = 0;
			binary->read( ( void* )&dir, sizeof( int ) );

			double vec = 0.0;
			binary->read( ( void* )&vec, sizeof( double ) );
			bool start_state = false;
			binary->read( ( void* )&start_state, sizeof( bool ) );
			bool auto_switching = false;
			binary->read( ( void* )&auto_switching, sizeof( bool ) );
			int on_time = 0;
			binary->read( ( void* )&on_time, sizeof( int ) );
			int off_time = 0;
			binary->read( ( void* )&off_time, sizeof( int ) );

			createFan( pos, ( Gimmick::DIR )dir, vec, start_state, auto_switching, on_time, off_time );
		}
	}

	{
		int enemies_size = ( int )_enemies.size( );
		for ( int i = 0; i < enemies_size; i++ ) {
			_enemies.erase( _enemies.begin( ) );
		}

		binary->read( ( void* )&enemies_size, sizeof( int ) );

		for ( int i = 0; i < enemies_size; i++ ) {
			Vector2 pos = Vector2( );
			binary->read( ( void* )&pos, sizeof( Vector2 ) );

			Vector2 vec = Vector2( );
			binary->read( ( void* )&vec, sizeof( Vector2 ) );

			int move_pattern = 0;
			binary->read( ( void* )&move_pattern, sizeof( int ) );
			createEnemy( pos, ( Enemy::MOVE_PATTERN )move_pattern, vec );
		}
	}


	{
		int between_objects_size = ( int )_between_objects.size( );
		for ( int i = 0; i < between_objects_size; i++ ) {
			_between_objects.erase( _between_objects.begin( ) );
		}

		binary->read( ( void* )&between_objects_size, sizeof( int ) );

		for ( int i = 0; i < between_objects_size; i++ ) {
			Vector2 pos = Vector2( );
			binary->read( ( void* )&pos, sizeof( Vector2 ) );
			Vector2 size = Vector2( );
			binary->read( ( void* )&size, sizeof( Vector2 ) );
			createBetweenObject( pos, size );
		}
	}
}