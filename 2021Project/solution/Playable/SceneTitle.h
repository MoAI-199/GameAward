#pragma once
#include "Scene.h"
#include "smart_ptr.h"
#include <array>

PTR( Image );
PTR( Controller );
PTR( System );
PTR( Music );
PTR( Transition );

class SceneTitle : public Scene {
public:
	enum class MODE {
		NORMAL,
		LOAD,
		PROLORG
	};
public:
	SceneTitle( );
	virtual ~SceneTitle( );
public:
	void initialize( ) override;
	void update( ) override;
	void finalize( ) override;
private:
	void draw( );
	void drawTitle( );
	void drawNormal( );
	void drawLoad( );
	void drawNumber( double pos_x, double pos_y, int number ) const;
	void drawExtendNumber( double pos1_x, double pos1_y, double size, int number ) const;
	void next( );
	void chooseLoad( );
	void selectLoad( );
	void backNormal( );
	void drawAnime( );

	void setupAnime( );
	void setupTextAnime( );
private:
	ImagePtr _start;
	ImagePtr _save_data_frame;
	ImagePtr _save_data_text;
	ImagePtr _number;
	ImagePtr _background;
	ImagePtr _back;
	ImagePtr _back_frame;
	ImagePtr _b_botton;
	ImagePtr _window;
	ImagePtr _please_push_botton;
	ImagePtr _new;
	std::array< ImagePtr, 24 > _title_palyer_anime;
	std::array< ImagePtr, 2 > _button_switch;
	std::array< ImagePtr, 8 > _title_text_anime;

	int _select_data;
	int _anime_wait_time;
	int _anime_image_num;
	int _next_loop_wait_time;
	int _switch_count;
	int _switch_image_num;
	int _title_text_wait_time;
	int _title_text_image_num;
	int _title_text_delay_time;
	bool _extend;
	bool _selected_data;
	bool _first;
	std::array< int, 3 > _degree_of_progress;
	MODE _mode;
	ControllerPtr _controller;
	SystemPtr _system;
	TransitionPtr _transition;
	MusicPtr _music;
};

