#pragma once
#include "Scene.h"
#include "smart_ptr.h"

PTR( GameData );
PTR( Editor );
PTR( Viewer );
PTR( Camera );

class SceneEditor : public Scene {
public:
	void initialize( ) override;
	void update( ) override;
private:
	GameDataPtr _data;
	EditorPtr _editor;
	ViewerPtr _viewer;
	CameraPtr _camera;
};

