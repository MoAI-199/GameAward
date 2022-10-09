#pragma once
#include "mathematics.h"

class Gimmick {
public:
	Gimmick( );
	virtual ~Gimmick( );
public:
	enum class TYPE {
		NONE,
		COIN,
		COLOR_BALL,
		HOLE,
		CARROT,
	};
	struct Status {

		Vector2 pos = Vector2( );
		Vector2 vec = Vector2( );
		TYPE type = TYPE::NONE;
		double size = 0.0;
		bool mode = false;;
		Status( ) {
			type = TYPE::NONE;
			size = 0.0;
		}
		Status( Vector2 new_pos, TYPE new_type, double new_size ) {
			pos = new_pos;
			type = new_type;
			size = new_size;
		}
	};
public:
	void update( );

	void setPos( Vector2 pos );
	void setSize( double size );
	void setType( TYPE type );
	double getSize( ) const;
	Vector2 getPos( ) const;
	Vector2 getVec( ) const;
	
	TYPE getType( ) const;
private:
	Status _status;
};

