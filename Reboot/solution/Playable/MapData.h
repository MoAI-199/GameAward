#pragma once
#include "smart_ptr.h"
#include <vector>
#include <string>
#include <map>
#include "mathematics.h"
#include "Gimmick.h"
#include "Enemy.h"

PTR( Gimmick );
PTR( Player );
PTR( Enemy );
PTR( Controller );
PTR( System );
PTR( Menu );

class MapData {
public:
	MapData( int stage_num, ControllerPtr controller, MenuPtr menu );
	virtual ~MapData( );
public:
	struct Ground {
		Vector2 pos;
		Vector2 view_size;
		Vector2 actual_size;
	};
	struct BetweenObject {
		Vector2 pos;
		Vector2 size;
	};
public:
	void update( );
	void updateFan( );
	void updateEnemy( );
	void updatePlayer( );
	std::vector< Ground* > getGrounds( ) const;
	std::vector< GimmickPtr > getGimmicks( ) const;
	std::vector< PlayerPtr > getPlayers( ) const;
	std::vector< EnemyPtr > getEnemies( ) const;
	std::vector< BetweenObject* > getBetweenObjects( ) const;
	void createGround( Vector2 pos, Vector2 size );
	void createGimmick( Vector2 pos, Vector2 size, Gimmick::TYPE type );
	void createGimmick( Vector2 pos, Vector2 size, Gimmick::TYPE type, Gimmick::DIR dir );
	void createFan( Vector2 pos, Gimmick::DIR dir, double vec, bool start_state, bool auto_switching, int on_time, int off_time );
	void createPlayer( );
	void createEnemy( Vector2 pos, Enemy::MOVE_PATTERN move, Vector2 vec );
	void createBetweenObject( Vector2 pos, Vector2 size );
	int getDeadPlayerNum( ) const;
	bool isSpawned( ) const;
	int getWidth( ) const;
	int getHeight( ) const;
	int getRespawnCount( ) const;
	std::string getStageName( int stage_num ) const;
private:
	void load( std::string filename );
	void deletePlayer( );
	void deleteEnemy( );
	void deleteGimmick( );
	void sortPlayersInOrderOfPosY( );
	bool isNeedDirToGimmck( Gimmick::TYPE type ) const;
private:
	std::vector< Ground* > _grounds;
	std::vector< GimmickPtr > _gimmicks;
	std::vector< PlayerPtr > _players;
	std::vector< EnemyPtr > _enemies;
	std::vector< BetweenObject* > _between_objects;
	std::map< int, std::string > _stage;
	std::string _ending_map;
	int _delete_player_num;
	bool _spawned;
	int _reset_count;
	int _width;
	int _height;
	int _respawn_count;
	ControllerPtr _controller;
	MenuPtr _menu;
};

