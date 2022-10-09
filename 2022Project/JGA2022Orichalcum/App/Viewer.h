#pragma once
#include "Color.h"
#include "Sheep.h"
#include "smart_ptr.h"
#include "Gimmick.h"
#include "ColorBall.h"
#include "Image.h"
#include <vector>
#include <array>

PTR( CloudsController );
PTR( SheepsController );
PTR( Cursor );
PTR( GimmickController );
PTR( PropellerManager );
PTR( FundamentalGameSystem );

using namespace std;

class Viewer {
public:
	Viewer(
		SheepsControllerPtr sheeps_controller,
		CloudsControllerPtr clouds_controller,
		GimmickControllerPtr gimmick_controller, 
		PropellerManagerPtr propeller_manager,
		FundamentalGameSystemPtr game_system );
	virtual ~Viewer( );
public:
	void draw( );
private:
	enum class USER_INTERFACE {
		LEVEL,
		MOKO_RATE,
	};
private:
	void loadImages();
	void drawBgMain( );
	void drawSheeps( );
	void drawClouds( );
	void drawGimmicks( );
	void drawCoin( Vector2 pos, double size );
	void drawCarrot( Vector2 pos, double size );
	void drawPropeller( );
	void drawUserInterface( Vector2 pos, int num, USER_INTERFACE ui );
	Color specifyColorBall( ColorBall::COLOR_TYPE type );
	ImagePtr specifySheep( int sheep_num, double size,double angle, Sheep::COLOR_TYPE type );
	ImagePtr changeSheepSize( ImagePtr sheep, Vector2 pos, double size, double angle );
private:
	SheepsControllerPtr _sheeps_controller;
	CloudsControllerPtr _clouds_controller;
	GimmickControllerPtr _gimmick_controller;
	PropellerManagerPtr _propeller_manager;
	FundamentalGameSystemPtr _game_system;
private:
	ImagePtr _img_bg_main;
	ImagePtr _img_cloud;
	ImagePtr _img_sheep_none;
	ImagePtr _img_sheep_red;
	ImagePtr _img_sheep_bule;
	ImagePtr _img_sheep_yellow;
	ImagePtr _img_sheep_green;
	ImagePtr _img_sheep_purple;
	ImagePtr _img_sheep_orange;
	ImagePtr _img_sheep_secret;
	ImagePtr _img_propeller;
	ImagePtr _img_heart;
	ImagePtr _img_board;
	array< ImagePtr, 10 > _img_number;
	array< ImagePtr, 2 > _img_ui;
private:
	double _heart_extend;
	bool _heart_reverse;
	//debug
	//double size;
	double s;
};

