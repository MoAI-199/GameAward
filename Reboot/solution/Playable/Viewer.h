#pragma once
#include "smart_ptr.h"
#include <array>
#include <vector>
#include <map>
#include "mathematics.h"
#include "Player.h"
#include "Define.h"

PTR( Player );
PTR( MapData );
PTR( Image );
PTR( Gimmick );
PTR( Camera );
PTR( Menu );
PTR( System );
PTR( Transition );
PTR( Collider );
PTR( Controller );

class Viewer {
public:
	Viewer( MapDataConstPtr map_data, CameraConstPtr camera, MenuPtr menu, ColliderPtr collider, TransitionPtr transition, ControllerPtr controller );
	virtual ~Viewer( );
public:
	void draw( );
	void skipClear( );
	int getViewClearCount( ) const;
	int getEndingWaitCount( ) const;
private:
	void drawBackGround( ) const;
	void drawPlayer( );
	void drawGrounds( ) const;
	void drawGimmick( ) const;
	void drawGimmickAnime( );
	void drawGoal( Vector2 pos, Vector2 player_pos, Player::STATE player_state, Vector2 goal_size );
	void drawBubbles( Vector2 pos, int acid_num, double acid_size, int amount_of_acid );
	void drawSpreadBubble( Vector2 pos, int acid_num, double acid_size, int amount_of_acid );
	void drawWideBubble( Vector2 pos, int acid_num, double acid_size, int amount_of_acid );
	void drawRespawn( );
	void drawClear( );
	void drawNumber( double pos_x, double pos_y, int number ) const;
	void drawMenu( ) const;
	void drawTutorial( );
	void drawEnding( ) const;
	void drawMovieLine( );
	void drawEndingText( );
	void drawEndingGirl( );
	void drawWind( );
	void drawPlayerDieEffectAnime( PlayerPtr player );
	void drawPlayerDeleteEffectAnime( );
	void drawEnemyAnime( );
	void drawOperation( );
	void drawCreditList( );
	void drawGauge( );
	void drawShadow( ) const;
	void drawBetweenObject( ) const;

	void drawClearIrisOut( ) const;
	void drawClearIrisIn( ) const;
	void drawClearText( ) const;
	void drawClearResultText( ) const;
	void drawClearStar( );
	void drawClearPlayerHead( );
	void drawClearConveyor( );

	void animePlayerWalkRight( Vector2 pos );
	void animePlayerWalkLeft( Vector2 pos );
	void animePlayerIdleRight( Vector2 pos );
	void animePlayerIdleLeft( Vector2 pos );
	void animePlayerJumpRight( Vector2 pos );
	void animePlayerJumpLeft( Vector2 pos );

	void animePlayerRight( Vector2 pos, bool move, bool jump, bool stand );
	void animePlayerLeft( Vector2 pos, bool move, bool jump, bool stand );
	void animeHarmfulEnemyLeft( Vector2 pos );
	void animeHarmfulEnemyRight( Vector2 pos );
	void animeDeactivatedEnemyLeft( Vector2 pos );
	void animeDeactivatedEnemyRight( Vector2 pos );
	void animeEnemyModeChangeLeft( Vector2 pos, int enemy_size );
	void animeEnemyModeChangeRight( Vector2 pos, int enemy_size );

	void skipEndingButton( );

private:
	void setupVariable( );
	void setupPlayer( );
	void setupWalk( );
	void setupIdle( );
	void setupGoal( );
	void setupJump( );
	void setupFloor( );
	void setupBetweenObject( );
	void setupGimmick( );
	void setupEnemy( );
	void setupWind( );
	void setupBubble( );
	void setupRandom( );
	void setupRespawn( );
	void setupDieEffect( );
	void setupConveyor( );
private:
	MapDataConstPtr _map_data;
	CameraConstPtr _camera;
	MenuPtr _menu;
	ColliderPtr _collider;
	TransitionPtr _transition;
	ControllerPtr _controller;
	ImagePtr _death_player_right;
	ImagePtr _death_player_left;
	ImagePtr _background;
	ImagePtr _text_window;
	ImagePtr _clear_text;
	ImagePtr _death_count;
	ImagePtr _save_text;
	ImagePtr _number;
	ImagePtr _sticky_net;
	ImagePtr _decelerate_net;
	ImagePtr _ui_menu;
	ImagePtr _ui_menu_base;
	ImagePtr _ui_retire;
	ImagePtr _ui_return_game;
	ImagePtr _ui_retire_confirm;
	ImagePtr _ui_yes;
	ImagePtr _ui_no;
	ImagePtr _needle_top;
	ImagePtr _needle_down;
	ImagePtr _needle_right;
	ImagePtr _needle_left;
	ImagePtr _star;
	ImagePtr _doughnut_star;
	ImagePtr _continue_text;
	ImagePtr _blue_window;
	ImagePtr _tutorial_window;
	ImagePtr _operation_method;
	ImagePtr _operation_method2;
	ImagePtr _sky;
	ImagePtr _dead_mountain;
	ImagePtr _light;
	ImagePtr _exclamation;
	ImagePtr _operation;
	ImagePtr _ui_operation;
	ImagePtr _suicide_level_1;
	ImagePtr _suicide_level_2;
	ImagePtr _suicide_level_3;
	ImagePtr _icon_bomb;
	ImagePtr _left_head;
	ImagePtr _right_head;
	ImagePtr _shadow;
	ImagePtr _procession_player;
	ImagePtr _procession_dark;
	ImagePtr _ban_bomb;
	ImagePtr _acid;
	ImagePtr _iris_screen;
	ImagePtr _clear_background;
	ImagePtr _skip_text;
	ImagePtr _button_a_64px;
	ImagePtr _ending_bg;
	ImagePtr  _ui_credit;
	ImagePtr  _credit_list;
	ImagePtr _end_text;
	ImagePtr _true;
	ImagePtr _ending_head;

	std::array< ImagePtr, 4 > _floor;
	std::array< ImagePtr, 2 > _floor_top;
	std::array< ImagePtr, 4 > _between_wall;
	std::array< ImagePtr, 22 > _player_walk_right;
	std::array< ImagePtr, 22 > _player_walk_left;
	std::array< ImagePtr, 15 > _player_idle_right;
	std::array< ImagePtr, 15 > _player_idle_left;
	std::array< ImagePtr, 8 > _goal_anime;
	std::array< ImagePtr, 10 > _player_jump_right;
	std::array< ImagePtr, 10 > _player_jump_left;
	std::array< ImagePtr, 20 > _harmful_enemy_move_left;
	std::array< ImagePtr, 20 > _harmful_enemy_move_right;
	std::array< ImagePtr, 20 > _deactivated_enemy_move_left;
	std::array< ImagePtr, 20 > _deactivated_enemy_move_right;
	std::array< ImagePtr, 24 > _wind_up;
	std::array< ImagePtr, 24 > _wind_down;
	std::array< ImagePtr, 24 > _wind_right;
	std::array< ImagePtr, 24 > _wind_left;
	std::array< ImagePtr, 13 > _spread_bubble;
	std::array< ImagePtr, 8 > _wide_bubble;
	std::array< ImagePtr, 8 > _respawn_anime;
	std::array< ImagePtr, PLAYER_DIE_EFFECT_IMAGE_NUM > _die_effect_anime;
	std::array< ImagePtr, 10 > _mode_change_left;
	std::array< ImagePtr, 10 > _mode_change_right;
	std::array< ImagePtr, 2 > _button_switch;
	std::array< ImagePtr, 3 > _conveyor;
	std::array< ImagePtr, 2 > _fan_top;
	std::array< ImagePtr, 2 > _fan_down;
	std::array< ImagePtr, 2 > _fan_right;
	std::array< ImagePtr, 2 > _fan_left;

	std::vector< int > _random_pos;
	std::vector< int > _random_bubble_type;
	std::vector< int > _mode_change_image_num;
	std::map< GimmickPtr, int > _fan_image_num;
	std::map< GimmickPtr, int > _fan_wait_time;

private:
	int _wait_time;
	int _goal_num_y;
	int _goal_num_x;
	int _respawn_num;
	int _respawn_wait_time;
	int _player_walk_right_num;
	int _player_walk_right_wait_time;
	int _player_walk_left_num;
	int _player_walk_left_wait_time;
	int _player_idle_num;
	int _player_idle_wait_time;
	int _goal_wait_time;
	int _goal_image_num;
	int _jump_image_num;
	int _jump_wait_time;
	int _view_clear_count;
	int _view_save_count;
	int _enemy_wait_time;
	int _enemy_image_num;
	int _wind_wait_time;
	int _wind_image_num;
	int _spread_bubble_image_num;
	int _small_bubble_image_num;
	int _wide_bubble_image_num;
	int _spread_bubble_wait_time;
	int _small_bubble_wait_time;
	int _wide_bubble_wait_time;
	int _amount_of_acid;
	int _amount_of_fan;
	int _mode_change_wait_time;
	double _ratio;
	double _tutorial_scale;
	double _opacity;
	int _player_die_effect_time;
	int _player_die_effect_image_num;
	int _player_delete_effect_time;
	int _player_delete_effect_image_num;
	Vector2 _delete_player_pos;
	Vector2 _delete_player_size;
	bool _draw_delete_effect;
	bool _first;
	bool _pushed_a;
	int _movie_line_count;
	int _suicide_count;
	int _suicide_count_level;
	int _level_change_time;
	int _switch_button_count;
	int _switch_count;
	int _switch_image_num;
	int _clear_head_pos_x;
	int _move_head;
	int _conveyor_wait_time;
	int _conveyor_image_num;
	int _player_head_wait_time;
	int _ending_wait_count;

	std::array< double, 3 > _angle;
	std::array< double, 3 > _scale;
	std::array< bool, 3 > _star_first;
};

