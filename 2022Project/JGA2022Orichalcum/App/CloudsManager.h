#pragma once
#include"smart_ptr.h"
#include "mathematics.h"
#include <vector>
#include "Cloud.h"

PTR( Cloud );

class CloudsManager {
public:
	enum class DIR_ROTATE {
		LEFT,
		RIGHT,
		STOP,
	};
public:
	CloudsManager( );
	virtual ~CloudsManager( );
public:
	void update( );
	void addList( CloudPtr cloud );
	void deleteCloud( int idx );
	void clearAll( );
	void setPos( int idx, Vector2 pos );
	void setVec( int idx, Vector2 vec );
	void setValid( int idx, bool valid );
	void setRotateDir( DIR_ROTATE dir_rotate );

	int getMaxNum( ) const;
	double getScale( int idx ) const;
	DIR_ROTATE getDir( ) const;
	Vector2 getVec( int idx ) const;
	Vector2 getPos( int idx ) const;
	Vector2 getBaseLine( int idx ) const;
	bool getValid( int idx ) const;

private:
	std::vector< CloudPtr >_list;
	DIR_ROTATE _now_dir_rotate;
};

