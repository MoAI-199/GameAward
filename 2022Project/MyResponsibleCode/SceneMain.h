#pragma once
#include "Scene.h"
#include "smart_ptr.h"

PTR( Viewer );
PTR( DeviceController );
PTR( Collision );

PTR( SheepsManager );
PTR( SheepFactory );
PTR( SheepsController );
PTR( SheepEvent );

PTR( CloudEvent );
PTR( CloudFactory );
PTR( CloudsManager );
PTR( CloudsController );

PTR( GimmickController );
PTR( GimmickManager );
PTR( GimmickFactory );
PTR( GimmickEvent );

PTR( BarManager );
PTR( PropellerManager );

PTR( FundamentalGameSystem );


class SceneMain : public Scene {
public:
	SceneMain( );
	virtual ~SceneMain( );
private:
	void update( );
private:
	void changeScene( );
	void setNextScene( );
private:
	CloudFactoryPtr _cloud_factory;
	CloudsManagerPtr _clouds_manager;
	CloudsControllerPtr _clouds_controller;
	CloudEventPtr _cloud_event;
	GimmickControllerPtr _gimmick_controller;
	GimmickManagerPtr _gimmicks_manager;
	GimmickFactoryPtr _gimmick_factory;
	GimmickEventPtr _gimmick_event;
	SheepFactoryPtr _sheep_factory;
	SheepsManagerPtr _sheeps_manager;
	SheepsControllerPtr _sheeps_controller;
	SheepEventPtr _sheep_event;
	ViewerPtr _viewer;
	CollisionPtr _collision;
	PropellerManagerPtr _propeller_manager;
	FundamentalGameSystemPtr _game_system;
	DeviceControllerPtr _device_controller;
	//debug
	int time_count;
};

