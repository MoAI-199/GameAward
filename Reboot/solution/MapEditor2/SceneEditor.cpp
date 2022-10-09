#include "SceneEditor.h"
#include "Editor.h"
#include "GameData.h"
#include "Viewer.h"
#include "Camera.h"

SCENE( SceneEditor );

void SceneEditor::initialize( ) {
	_data = GameDataPtr( new GameData( ) );
	_camera = CameraPtr( new Camera( ) );
	_editor = EditorPtr( new Editor( _data, _camera ) );
	_viewer = ViewerPtr( new Viewer( _data, _editor, _camera ) );
}

void SceneEditor::update( ) {
	_camera->update( );
	_editor->update( );
	_data->updateState( );
	_viewer->draw( );
}
