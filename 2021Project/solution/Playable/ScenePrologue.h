#pragma once
#include "Scene.h"
#include "Image.h"

PTR( System );
PTR( Controller );
PTR( Transition );
PTR( ViewerPrologue );
PTR( Player );

class ScenePrologue : public Scene {
public:
	ScenePrologue( );
	virtual ~ScenePrologue( );
private:
	void initialize( ) override;
	void update( ) override;
private:
	void playerMove( );
private:

	SystemPtr _system;
	PlayerPtr _player;
	ControllerPtr _controller;
	TransitionPtr _transition;
	ViewerProloguePtr _viewer_prologue;
};

