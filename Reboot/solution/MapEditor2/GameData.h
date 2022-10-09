#pragma once
#include <vector>
#include "Define.h"
#include "mathematics.h"
#include <string>
#include <map>

class GameData {
public:
	GameData( );
	virtual ~GameData( );
public:
	struct Ground {
		Vector2 _pos;
		Vector2 _size;
	};

	struct Gimmick {
		enum class TYPE {
			GOAL,
			RESPAWN,
			ACID,
			NEEDLE,
			FAN,
			DECELERATE_NET,
			STICKY_NET
		};
		enum class DIR {
			NONE,
			LEFT,
			RIGHT,
			UP,
			DOWN
		};
		Vector2 _pos;
		Vector2 _size;
		TYPE _type;
		DIR _dir;
	};

	struct Fan {
		Gimmick* _gimmick;
		double _vec;
		bool _start_state;
		bool _auto_switching;
		int _on_time;
		int _off_time;
	};

	struct Enemy {
		enum class MOVE_PATTERN {
			CLOCKWISE,
			VERTICAL,
			HORIZONTAL,
			COUNTER_CLOCKWISE
		};
		Vector2 _pos;
		Vector2 _size;
		Vector2 _vec;
		MOVE_PATTERN _move_pattern;
	};

	enum class STATE {
		NONE,
		SAVE_SUCCESS,
		SAVE_FAILURE,
		LOAD_SUCCESS,
		LOAD_FAILURE
	};
public:
	void updateState( );
	STATE getState( ) const;
	int getWidth( ) const;
	int getHeight( ) const;
	int& setWidth( );
	int& setHeight( );
public:
	void createGround( Vector2 pos, Vector2 size );
	std::vector< Ground* > getGrounds( ) const;
	int getGroundsSize( ) const;
	void deleteGround( int select );
public:
	void createGimmick( Vector2 pos, Vector2 size, Gimmick::TYPE type, Gimmick::DIR dir );
	std::vector< Gimmick* > getGimmicks( ) const;
	int getGimmicksSize( ) const;
	void deleteGimmick( int select );
public:
	void createFan( Vector2 pos, Vector2 size, Gimmick::DIR dir );
	std::vector< Fan* > getFans( ) const;
	int getFansSize( ) const;
	void deleteFan( int select );
public:
	void createEnemy( Vector2 pos, Enemy::MOVE_PATTERN move_pattern );
	std::vector< Enemy* > getEnemies( ) const;
	int getEnemiesSize( ) const;
	void deleteEnemy( int select );
public:
	void createBetweenObject( Vector2 pos, Vector2 size );
	std::vector< Ground* > getBetweenObjects( ) const;
	int getBetweenObjectsSize( ) const;
	void deleteBetweenObject( int select );
public:
	void save( std::string filename );
	void load( std::string filename );
	void oldLoad( std::string filename );
private:
	std::vector< Ground* > _grounds;
	std::vector< Gimmick* > _gimmicks;
	std::vector< Fan* > _fans;
	std::vector< Enemy* > _enemies;
	std::vector< Ground* > _between_objects;
	int _reset_count;
	STATE _state;
	int _width;
	int _height;
};

