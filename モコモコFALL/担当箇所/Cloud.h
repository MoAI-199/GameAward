#pragma once
#include "mathematics.h"

class Cloud {
public:
	struct Status {
		Vector2 pos;
		Vector2 vec;
		Vector2 base_line;
		double scale;
		bool valid;
	}_status;
	
public:
	Cloud( Vector2 base_pos, double scale );
	virtual ~Cloud( );
public:
	void update( );
	Vector2 getPos( )const;
	Vector2 geBaseLdine( ) const;
	double getScale( ) const;
	bool getValid( ) const;

	void setPos( Vector2 pos );
	void setVec( Vector2 vec );
	void setValid( bool valid );
private:
	Vector2 _befor_vec;

};

