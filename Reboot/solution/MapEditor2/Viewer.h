#pragma once
#include "smart_ptr.h"
#include <map>
#include "Define.h"
#include "Color.h"
#include <vector>

PTR( GameData );
PTR( Editor );
PTR( Image );
PTR( Camera );

class Viewer {
public:
	Viewer( GameDataConstPtr data, EditorConstPtr editor, CameraConstPtr camera );
	virtual ~Viewer( );
public:
	void draw( ) const;
	void drawBackGround( ) const;
	void drawGrounds( ) const;
	void drawGimmicks( ) const;
	void drawFans( ) const;
	void drawEnemies( ) const;
	void drawBetweenObjects( ) const;
	void drawSelectArea( ) const;
	void drawNowSelect( ) const;
	void drawText( ) const;
	void drawSelecter( ) const;
	void drawLange( ) const;
private:
	GameDataConstPtr _data;
	EditorConstPtr _editor;
	CameraConstPtr _camera;

	ImagePtr _background;
	ImagePtr _goal_plate;
	ImagePtr _respawn_plate;
	ImagePtr _acid_plate;
	ImagePtr _needle_plate;
	ImagePtr _fan_plate;
	ImagePtr _decelerate_net_plate;
	ImagePtr _sticky_net_plate;
	ImagePtr _enemy_plate;

	ImagePtr _goal;
	ImagePtr _acid;
	ImagePtr _needle_top;
	ImagePtr _needle_down;
	ImagePtr _needle_right;
	ImagePtr _needle_left;
	ImagePtr _fan_top;
	ImagePtr _fan_down;
	ImagePtr _fan_right;
	ImagePtr _fan_left;
	ImagePtr _respawn;
	ImagePtr _decelerate_net;
	ImagePtr _sticky_net;
	ImagePtr _enemy;
	std::vector< ImagePtr > _floor;
	std::vector< ImagePtr > _floor_top;
	std::vector< ImagePtr > _between_wall;
};

