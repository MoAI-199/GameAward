#pragma once
#include "Gimmick.h"
#include "smart_ptr.h"

PTR( Fan );

class Fan : public Gimmick {
public:
	Fan( Vector2 pos, Vector2 size, DIR dir, double vec, bool start_state, bool auto_switching, int on_time, int off_time );
	virtual ~Fan( );
	void update( );
	Vector2 getVec( ) const;
	bool isOn( ) const;
	int getCount( )const;
	int getOffTime( ) const;
	int getOnTime( ) const;
private:
	double _vec;
	bool _on;
	bool _auto_switching;
	int _count;
	int _on_time;
	int _off_time;
};

