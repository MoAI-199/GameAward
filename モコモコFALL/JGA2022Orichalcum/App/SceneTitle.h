#pragma once
#include "Scene.h"
#include "Image.h"

class SceneTitle :  public Scene {
public:
	SceneTitle( );
	virtual ~SceneTitle( );
public:
	void update( ) override;
private:
	void drawTaitoru( );
	void changeScene( );
private:
	const int SCREEN_HEIGHT;
	const int SCREEN_WIDTH;
	ImagePtr _taitoru;
};

// sample
