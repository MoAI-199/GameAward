#pragma once
#include "Scene.h"
#include "Image.h"
#include "Application.h"

class SceneResult : public Scene {
public:
	SceneResult( );
	~SceneResult( );
public:
	void update( ) override;
private:
	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
	ImagePtr _gauge;
	ImagePtr _result_board;
	ImagePtr _word1;
};
