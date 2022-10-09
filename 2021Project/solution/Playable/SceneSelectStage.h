#pragma once
#include "Scene.h"
#include <array>
#include "Define.h"

PTR( Controller );
PTR( Image );
PTR( Transition );

class SceneSelectStage : public Scene {
public:
	SceneSelectStage( );
	virtual ~SceneSelectStage( );
private:
	void initialize( ) override;
	void update( ) override;
	void finalize( ) override;
private:
	void select( );
	void inToTheSelectStage( );
	void backTitle( );
	void setupWalk( );
private:
	void draw( );
	void drawBackGround( );
	void drawStageUI( );
	void drawStrataUI( );
	void drawScoreUI( ) const;
	void drawBack( ) const;
	void drawNumber( double pos_x, double pos_y, int number ) const;
	void drawExtendNumber( double pos1_x, double pos1_y, double size, int number ) const;
	void moveCamera( );
	void moveAnimePlayer( );
	void walkPlayerOut( );
	void walkPlayerIn( );
	void unlookEffectMove( );
	void unlookStageEffect( );
private:
	int _now_select_stage_num;
	int _before_select_strata;
	int _before_select_stage;
	int _strata_anime_count;
	int _before_select_stage_icon_anime_count;
	int _now_select_stage_icon_anime_count;
	int _camera_y;
	int _wait_time_close;
	int wait_time_open;
	int _wait_select_count;
	int _player_wait_time;
	int _player_image_num;
	int _player_pos_x;
	int _player_pos_y;
	int _wait_criteria_time;
	int _unlook_image_num;
	int _unlook_image_wait_time;
	int _window_anime_image_num;
	int _move;
	bool _selected;
	bool _backed;
	bool _draw_effect;
	ImagePtr _background;
	ImagePtr _number;
	ImagePtr _player_img;
	ImagePtr _star;
	ImagePtr _doughnut_star;
	ImagePtr _strata_text;
	ImagePtr _strata_window;
	ImagePtr _dark_strata_window;
	ImagePtr _icon_base;
	ImagePtr _back;
	ImagePtr _back_frame;
	ImagePtr _b_botton;
	std::array < ImagePtr, 8 > _icon_door;
	std::array < ImagePtr, 6 > _spark;
	std::array < ImagePtr, 14 > _window_anime;
	std::array < ImagePtr, 18 > _energy_field;
	std::array < ImagePtr, 22 > _player_walk_right;
	ControllerPtr _controller;
	TransitionPtr _transition;
	std::array< Vector2, STAGE_NUM > _stage_ui_pos;
};

