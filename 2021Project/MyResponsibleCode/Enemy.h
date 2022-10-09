#pragma once
#include "mathematics.h"
#include "Player.h"


class Enemy {
public:
	enum class STATUS{
		HARMLESSNESS,
		HARMFUL
	};
	enum class MOVE_PATTERN {
		CLOCK_WISE,
		VERTICAL,
		HORIZONTAL,
		COUNTER_CLOCK_WISE
	};
	enum class DIR {
		RIGHT,
		LEFT
	};
	Enemy( Vector2 pos ,MOVE_PATTERN move,Vector2 vec );
	virtual ~Enemy( );
public:
	void update( );
	void move( );
	void movePattern( );
	void capturePlayer( PlayerPtr player );
public:
	void setStatus( STATUS status );
	void setChange( bool change );
	bool isDeleted( ) const;
	bool isChange( ) const;
private:
	void modeChange( );
public:
	STATUS getStatus( ) const;
	DIR getDir( ) const;
	Vector2 getPos( ) const;
	Vector2 getSize( ) const;
	Vector2 getVec( ) const;
private:
	Vector2 _pos;
	Vector2 _size;
	Vector2 _vec;
	Vector2 _vec_move;
	int _time;
	int _change_time;
	double _angle;
	STATUS _status;
	MOVE_PATTERN _move_pattern;
	DIR _dir;
	bool _delete;
	bool _change;
};

