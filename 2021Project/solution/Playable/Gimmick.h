#pragma once
#include "mathematics.h"

class Gimmick {
public:
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
public:
	Gimmick( Vector2 pos, Vector2 size, TYPE type, DIR dir );
	virtual ~Gimmick( );
public:
	Vector2 getPos( ) const;
	Vector2 getSize( ) const;
	TYPE getType( ) const;
	DIR getDir( ) const;
	bool isDeleted( ) const;
public:
	void setDelete( );
private:
	Vector2 _pos;
	Vector2 _size;
	TYPE _type;
	DIR _dir;
	bool _delete;
};

