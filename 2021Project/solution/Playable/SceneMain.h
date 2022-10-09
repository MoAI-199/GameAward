#pragma once
#include "Scene.h"

PTR( Player );
PTR( Viewer );
PTR( MapData );
PTR( Collider );
PTR( Camera );
PTR( Music );
PTR( Controller );
PTR( Menu );
PTR( Transition );


class SceneMain : public Scene {
public:
	SceneMain( );
	virtual ~SceneMain( );
private:
	void initialize( ) override;
	void update( ) override;
	void finalize( ) override;
private:
	void endingUpdate( ) ;
	void startIris( );
private:
	PlayerPtr _player;
	ViewerPtr _viewer;
	MapDataPtr _map_data;
	ColliderPtr _collider;
	CameraPtr _camera;
	ControllerPtr _controller;
	MenuPtr _menu;
	TransitionPtr _transition;
	bool _first_time;
	bool _skip_ending;
	bool _fade_clear;
};

