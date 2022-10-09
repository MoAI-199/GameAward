#pragma once
#include "mathematics.h"
#include "Sheep.h"
#include "smart_ptr.h"
#include <vector>

PTR( Sheep );

class SheepsManager {
	friend class PlayersOperator;

public:
	SheepsManager( );
	virtual ~SheepsManager( );
public:
	void update( );
	void addSheep( SheepPtr player );
	void setColorType( int idx, Sheep::COLOR_TYPE type );
	void setVec( int idx, Vector2 vec );
	void setPos( int idx, Vector2 pos );
	void setSwapPlayersPos( int first_idx,
							int second_idx,
							Vector2 first_pos,
							Vector2 second_pos );
	void setViewSize( int idx, double size );
	void setScale( int idx, double scale );
	void deleteOutOfScreenSheep( int idx );
	void addDeleteList( int idx );
	void clearAll( );
	Vector2 getPos( int idx ) const;
	Vector2 getVec( int idx ) const;
	Sheep::COLOR_TYPE getColorType( int idx ) const;
	int getMaxNum( ) const;
	int getLeadPlayer( ) const;
	bool isColorStorage( int idx, Sheep::COLOR_TYPE type );
	double getSize( int idx ) const;
	double getAngle( int idx ) const;
	double getViewSize( int idx ) const;
	double getScale( int idx ) const;
private:
	void deletePlayer( int idx );
private:
	std::vector< SheepPtr > _list;
	std::vector< int > _delete_list;
};

