#include "Viewer.h"
#include "Player.h"
#include "Drawer.h"
#include "Image.h"
#include "MapData.h"
#include "Drawer.h"
#include "Application.h"
#include "Gimmick.h"
#include "Define.h"
#include "Camera.h"
#include "Enemy.h"
#include "Menu.h"
#include "System.h"
#include "Transition.h"
#include "Collider.h"
#include "Fan.h"
#include "StageManager.h"
#include "Score.h"
#include "Tool.h"
#include "Music.h"
#include "SaveData.h"
#include "Controller.h"

const int GROUND_IMAGE_SIZE = 128;
const int CLEAR_RESULT_ADJUST_Y = 50;
const Vector2 DEATH_COUNT_IMAGE_SIZE = Vector2( 900, 150 );
static const int CONVEYOR_SPEED = 6;

Viewer::Viewer( MapDataConstPtr map_data, CameraConstPtr camera, MenuPtr menu, ColliderPtr collider, TransitionPtr transition, ControllerPtr controller ) :
	_map_data( map_data ),
	_camera( camera ),
	_menu( menu ),
	_collider( collider ),
	_transition( transition ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_iris_screen = drawer->createImage( "Resource/blend.png" );
	_background = drawer->createImage( "Resource/Image/background/BG6.png" );
	_text_window = drawer->createImage( "Resource/Image/UI/strata_window_db.png" );
	_clear_text = drawer->createImage( "Resource/Image/UI/clear_text_3.png" );
	_death_count = drawer->createImage( "Resource/Image/UI/death_count.png" );
	_save_text = drawer->createImage( "Resource/Image/UI/save_text_dark_orange.png" );
	_number = drawer->createImage( "Resource/Image/image_number.png" );
	_ui_menu = drawer->createImage( "Resource/Image/UI/text_menu.png" );
	_ui_menu_base = drawer->createImage( "Resource/Image/UI/menu_base.png" );
	_ui_retire = drawer->createImage( "Resource/Image/UI/retire.png" );
	_ui_return_game = drawer->createImage( "Resource/Image/UI/return_game.png" );
	_ui_retire_confirm = drawer->createImage( "Resource/Image/UI/retire_confirm_big.png" );
	_ui_yes = drawer->createImage( "Resource/Image/UI/hai.png" );
	_ui_no = drawer->createImage( "Resource/Image/UI/iie.png" );
	_operation_method = drawer->createImage( "Resource/image/UI/operation_method_normal2.png" );
	_operation_method2 = drawer->createImage( "Resource/image/UI/operation_method_explosion3.png" );
	_star = drawer->createImage( "Resource/Image/UI/silver_star.png" );
	_doughnut_star = drawer->createImage( "Resource/Image/UI/doughnut_star.png" );
	_blue_window = drawer->createImage( "Resource/Image/UI/window_blue.png" );
	_tutorial_window = drawer->createImage( "Resource/Image/UI/strata_window.png" );
	_sky = drawer->createImage( "Resource/Image/Ending/sky_00198.jpg" );
	_light = drawer->createImage( "Resource/Image/Ending/light4.png" );
	_dead_mountain = drawer->createImage( "Resource/Image/Ending/dead_mountain.png" );
	_exclamation = drawer->createImage( "Resource/Image/UI/exclamation2.png" );
	_operation = drawer->createImage( "Resource/Image/operate2.png" );
	_ui_operation = drawer->createImage( "Resource/Image/UI/operation.png" );
	_suicide_level_1 = drawer->createImage( "Resource/image/UI/circle_level_1.png" );
	_suicide_level_2 = drawer->createImage( "Resource/image/UI/circle_level_2.png" );
	_suicide_level_3 = drawer->createImage( "Resource/image/UI/circle_level_3.png" );
	_icon_bomb = drawer->createImage( "Resource/image/UI/bomb2.png" );
	_shadow = drawer->createImage( "Resource/shadow.png" );
	_ban_bomb = drawer->createImage( "Resource/image/UI/ban_bomb.png" );
	_button_switch[ 0 ] = drawer->createImage( "Resource/Image/UI/a_button_false.png" );
	_button_switch[ 1 ] = drawer->createImage( "Resource/Image/UI/a_button_true.png" );
	_clear_background = drawer->createImage( "Resource/Image/background/title2_BG.png" );
	_skip_text = drawer->createImage( "Resource/Image/UI/skip.png" );
	_button_a_64px = drawer->createImage( "Resource/Image/UI/button_a_64px.png" );
	_ending_bg = drawer->createImage( "Resource/Image/background/ending_BG.png" );
	_ui_credit = drawer->createImage( "Resource/Image/UI/credit.png" );
	_credit_list = drawer->createImage( "Resource/Image/UI/credit_list.png" );
	_continue_text = drawer->createImage( "Resource/image/Ending/end_text9.png" );
	_end_text = drawer->createImage( "Resource/Image/Ending/end_text.png" );
	_true = drawer->createImage( "Resource/Image/Ending/girl.png" );
	setupFloor( );
	setupBetweenObject( );
	setupGoal( );
	setupPlayer( );
	setupGimmick( );
	setupEnemy( );
	setupWind( );
	setupBubble( );
	setupRandom( );
	setupRespawn( );
	setupDieEffect( );
	setupConveyor( );
	//変数初期化
	setupVariable( );

	for ( int i = 0; i < 3; i++ ) {
		_angle[ i ] = 0;
		_scale[ i ] = 0;
		_star_first[ i ] = false;
	}

	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	std::vector< EnemyPtr > enemies = _map_data->getEnemies( );
	for ( int i = 0; i < enemies.size( ); i++ ) {
		_mode_change_image_num.push_back( 0 );
	}
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::ACID ) {
			_amount_of_acid++;
		}
		if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::FAN ) {
			_fan_image_num[ gimmicks[ i ] ] = 0;
			_fan_wait_time[ gimmicks[ i ] ] = 0;
		}
	}
	if ( _amount_of_acid == 1 ) {
		_amount_of_acid = 2;
	}

}

Viewer::~Viewer( ) {
}

void Viewer::setupVariable( ) {

	_wait_time = 0;
	_goal_num_x = 0;
	_goal_num_y = 0;
	_player_walk_right_num = 0;
	_player_walk_right_wait_time = 0;
	_player_walk_left_num = 0;
	_player_walk_left_wait_time = 0;
	_player_idle_num = 0;
	_player_idle_wait_time = 0;
	_goal_image_num = 0;
	_goal_wait_time = 0;
	_jump_image_num = 0;
	_jump_wait_time = 0;
	_view_clear_count = 0;
	_view_save_count = 0;
	_tutorial_scale = 1.0;
	_ratio = 1.0;
	_enemy_image_num = 0;
	_enemy_wait_time = 0;
	_wind_image_num = 0;
	_wind_wait_time = 0;
	_spread_bubble_wait_time = 0;
	_spread_bubble_image_num = 0;
	_small_bubble_image_num = 0;
	_wide_bubble_image_num = 0;
	_small_bubble_wait_time = 0;
	_wide_bubble_wait_time = 0;
	_respawn_num = 0;
	_respawn_wait_time = 0;
	_player_die_effect_time = 0;
	_player_die_effect_image_num = 0;
	_movie_line_count = 0;
	_first = false;
	_suicide_count_level = 0;
	_player_delete_effect_image_num = 0;
	_player_delete_effect_time = 0;
	_delete_player_pos = Vector2( 0, 0 );
	_delete_player_size = Vector2( 0, 0 );
	_draw_delete_effect = false;
	_mode_change_wait_time = 0;
	_switch_button_count = 0;
	_switch_image_num = 0;
	_switch_count = 0;
	_clear_head_pos_x = 0;
	_move_head = 0;
	_conveyor_wait_time = 0;
	_conveyor_image_num = 0;
	_player_head_wait_time = 0;
	_ending_wait_count = 0;
	_pushed_a = false;

	_opacity = 0;
}

void Viewer::setupPlayer( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_death_player_right = drawer->createImage( "Resource/Image/player/die_right_8.png" );
	_death_player_left = drawer->createImage( "Resource/Image/player/die_left_8.png" );
	_right_head = drawer->createImage( "Resource/Image/player/die_right_head_2.png" );
	_left_head = drawer->createImage( "Resource/Image/player/die_left_head_2.png" );
	_ending_head = drawer->createImage( "Resource/Image/Ending/head_right.png" );
	setupWalk( );
	setupIdle( );
	setupJump( );
}

void Viewer::setupWalk( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	//right
	_player_walk_right[ 0 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_000.png" );
	_player_walk_right[ 1 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_001.png" );
	_player_walk_right[ 2 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_002.png" );
	_player_walk_right[ 3 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_003.png" );
	_player_walk_right[ 4 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_004.png" );
	_player_walk_right[ 5 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_005.png" );
	_player_walk_right[ 6 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_006.png" );
	_player_walk_right[ 7 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_007.png" );
	_player_walk_right[ 8 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_008.png" );
	_player_walk_right[ 9 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_009.png" );
	_player_walk_right[ 10 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_010.png" );
	_player_walk_right[ 11 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_011.png" );
	_player_walk_right[ 12 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_012.png" );
	_player_walk_right[ 13 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_013.png" );
	_player_walk_right[ 14 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_014.png" );
	_player_walk_right[ 15 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_015.png" );
	_player_walk_right[ 16 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_016.png" );
	_player_walk_right[ 17 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_017.png" );
	_player_walk_right[ 18 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_018.png" );
	_player_walk_right[ 19 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_019.png" );
	_player_walk_right[ 20 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_020.png" );
	_player_walk_right[ 21 ] = drawer->createImage( "Resource/Image/player/walk_right/walk_021.png" );
	//left
	_player_walk_left[ 0 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_000.png" );
	_player_walk_left[ 1 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_001.png" );
	_player_walk_left[ 2 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_002.png" );
	_player_walk_left[ 3 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_003.png" );
	_player_walk_left[ 4 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_004.png" );
	_player_walk_left[ 5 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_005.png" );
	_player_walk_left[ 6 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_006.png" );
	_player_walk_left[ 7 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_007.png" );
	_player_walk_left[ 8 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_008.png" );
	_player_walk_left[ 9 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_009.png" );
	_player_walk_left[ 10 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_010.png" );
	_player_walk_left[ 11 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_011.png" );
	_player_walk_left[ 12 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_012.png" );
	_player_walk_left[ 13 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_013.png" );
	_player_walk_left[ 14 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_014.png" );
	_player_walk_left[ 15 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_015.png" );
	_player_walk_left[ 16 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_016.png" );
	_player_walk_left[ 17 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_017.png" );
	_player_walk_left[ 18 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_018.png" );
	_player_walk_left[ 19 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_019.png" );
	_player_walk_left[ 20 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_020.png" );
	_player_walk_left[ 21 ] = drawer->createImage( "Resource/Image/player/walk_left/walk_021.png" );

}

void Viewer::setupIdle( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	//right
	_player_idle_right[ 0 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_000.png" );
	_player_idle_right[ 1 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_001.png" );
	_player_idle_right[ 2 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_002.png" );
	_player_idle_right[ 3 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_003.png" );
	_player_idle_right[ 4 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_004.png" );
	_player_idle_right[ 5 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_005.png" );
	_player_idle_right[ 6 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_006.png" );
	_player_idle_right[ 7 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_007.png" );
	_player_idle_right[ 8 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_008.png" );
	_player_idle_right[ 9 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_009.png" );
	_player_idle_right[ 10 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_010.png" );
	_player_idle_right[ 11 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_011.png" );
	_player_idle_right[ 12 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_012.png" );
	_player_idle_right[ 13 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_013.png" );
	_player_idle_right[ 14 ] = drawer->createImage( "Resource/Image/player/idle_right/idle_014.png" );
	//left
	_player_idle_left[ 0 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_000.png" );
	_player_idle_left[ 1 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_001.png" );
	_player_idle_left[ 2 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_002.png" );
	_player_idle_left[ 3 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_003.png" );
	_player_idle_left[ 4 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_004.png" );
	_player_idle_left[ 5 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_005.png" );
	_player_idle_left[ 6 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_006.png" );
	_player_idle_left[ 7 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_007.png" );
	_player_idle_left[ 8 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_008.png" );
	_player_idle_left[ 9 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_009.png" );
	_player_idle_left[ 10 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_010.png" );
	_player_idle_left[ 11 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_011.png" );
	_player_idle_left[ 12 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_012.png" );
	_player_idle_left[ 13 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_013.png" );
	_player_idle_left[ 14 ] = drawer->createImage( "Resource/Image/player/idle_left/idle_014.png" );
}

void Viewer::setupGoal( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_goal_anime[ 0 ] = drawer->createImage( "Resource/Image/goal/door_01.png" );
	_goal_anime[ 1 ] = drawer->createImage( "Resource/Image/goal/door_02.png" );
	_goal_anime[ 2 ] = drawer->createImage( "Resource/Image/goal/door_03.png" );
	_goal_anime[ 3 ] = drawer->createImage( "Resource/Image/goal/door_04.png" );
	_goal_anime[ 4 ] = drawer->createImage( "Resource/Image/goal/door_05.png" );
	_goal_anime[ 5 ] = drawer->createImage( "Resource/Image/goal/door_06.png" );
	_goal_anime[ 6 ] = drawer->createImage( "Resource/Image/goal/door_07.png" );
	_goal_anime[ 7 ] = drawer->createImage( "Resource/Image/goal/door_08.png" );
}

void Viewer::setupJump( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_player_jump_right[ 0 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_000.png" );
	_player_jump_right[ 1 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_001.png" );
	_player_jump_right[ 2 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_002.png" );
	_player_jump_right[ 3 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_003.png" );
	_player_jump_right[ 4 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_004.png" );
	_player_jump_right[ 5 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_005.png" );
	_player_jump_right[ 6 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_006.png" );
	_player_jump_right[ 7 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_007.png" );
	_player_jump_right[ 8 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_008.png" );
	_player_jump_right[ 9 ] = drawer->createImage( "Resource/Image/player/jump_right/jump_009.png" );

	_player_jump_left[ 0 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_000.png" );
	_player_jump_left[ 1 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_001.png" );
	_player_jump_left[ 2 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_002.png" );
	_player_jump_left[ 3 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_003.png" );
	_player_jump_left[ 4 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_004.png" );
	_player_jump_left[ 5 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_005.png" );
	_player_jump_left[ 6 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_006.png" );
	_player_jump_left[ 7 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_007.png" );
	_player_jump_left[ 8 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_008.png" );
	_player_jump_left[ 9 ] = drawer->createImage( "Resource/Image/player/jump_left/jump_009.png" );
}

void Viewer::setupFloor( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	drawer->setBlend( scope, Drawer::BLEND::ADD, 0.2 );

	_floor[ 0 ] = drawer->createImage( "Resource/Image/ground/fill_variation_01_db.png" );
	_floor[ 1 ] = drawer->createImage( "Resource/Image/ground/fill_variation_02_db.png" );
	_floor[ 2 ] = drawer->createImage( "Resource/Image/ground/fill_variation_03_db.png" );
	_floor[ 3 ] = drawer->createImage( "Resource/Image/ground/fill_variation_04_db.png" );
	_floor_top[ 0 ] = drawer->createImage( "Resource/Image/ground/top_edge_variation_01_db.png" );
	_floor_top[ 1 ] = drawer->createImage( "Resource/Image/ground/top_edge_variation_02_db.png" );
}

void Viewer::setupBetweenObject( ) {
	DrawerPtr drawer = Task::get< Drawer >( );

	_between_wall[ 0 ] = drawer->createImage( "Resource/Image/ground/between_object_1.png" );
	_between_wall[ 1 ] = drawer->createImage( "Resource/Image/ground/between_object_2.png" );
	_between_wall[ 2 ] = drawer->createImage( "Resource/Image/ground/between_object_3.png" );
	_between_wall[ 3 ] = drawer->createImage( "Resource/Image/ground/between_object_4.png" );
}

void Viewer::setupGimmick( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_acid = drawer->createImage( "Resource/Image/gimmick/toxic_water_repeating.png" );//toxic_water_repeating
	_sticky_net = drawer->createImage( "Resource/Image/gimmick/sticky_net.png" );
	_decelerate_net = drawer->createImage( "Resource/Image/gimmick/decelerate_net.png" );
	_needle_top = drawer->createImage( "Resource/image/gimmick/needle_up_2.png" );//needle_up_2
	_needle_down = drawer->createImage( "Resource/image/gimmick/needle_down_2.png" );
	_needle_right = drawer->createImage( "Resource/image/gimmick/needle_right_2.png" );
	_needle_left = drawer->createImage( "Resource/image/gimmick/needle_left_2.png" );

	_fan_top[ 0 ] = drawer->createImage( "Resource/image/gimmick/fan_img_up1.png" );
	_fan_top[ 1 ] = drawer->createImage( "Resource/image/gimmick/fan_img_up2.png" );
	_fan_down[ 0 ] = drawer->createImage( "Resource/image/gimmick/fan_img_down1.png" );
	_fan_down[ 1 ] = drawer->createImage( "Resource/image/gimmick/fan_img_down2.png" );
	_fan_right[ 0 ] = drawer->createImage( "Resource/image/gimmick/fan_img_right1.png" );
	_fan_right[ 1 ] = drawer->createImage( "Resource/image/gimmick/fan_img_right2.png" );
	_fan_left[ 0 ] = drawer->createImage( "Resource/image/gimmick/fan_img_left1.png" );
	_fan_left[ 1 ] = drawer->createImage( "Resource/image/gimmick/fan_img_left2.png" );
}

void Viewer::setupEnemy( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_harmful_enemy_move_left[ 0 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_000.png" );
	_harmful_enemy_move_left[ 1 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_001.png" );
	_harmful_enemy_move_left[ 2 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_002.png" );
	_harmful_enemy_move_left[ 3 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_003.png" );
	_harmful_enemy_move_left[ 4 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_004.png" );
	_harmful_enemy_move_left[ 5 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_005.png" );
	_harmful_enemy_move_left[ 6 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_006.png" );
	_harmful_enemy_move_left[ 7 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_007.png" );
	_harmful_enemy_move_left[ 8 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_008.png" );
	_harmful_enemy_move_left[ 9 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_009.png" );
	_harmful_enemy_move_left[ 10 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_010.png" );
	_harmful_enemy_move_left[ 11 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_011.png" );
	_harmful_enemy_move_left[ 12 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_012.png" );
	_harmful_enemy_move_left[ 13 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_013.png" );
	_harmful_enemy_move_left[ 14 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_014.png" );
	_harmful_enemy_move_left[ 15 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_015.png" );
	_harmful_enemy_move_left[ 16 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_016.png" );
	_harmful_enemy_move_left[ 17 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_017.png" );
	_harmful_enemy_move_left[ 18 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_018.png" );
	_harmful_enemy_move_left[ 19 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_left_move/resize/__small_droid_activated_idle_019.png" );
	_harmful_enemy_move_right[ 0 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_000.png" );
	_harmful_enemy_move_right[ 1 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_001.png" );
	_harmful_enemy_move_right[ 2 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_002.png" );
	_harmful_enemy_move_right[ 3 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_003.png" );
	_harmful_enemy_move_right[ 4 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_004.png" );
	_harmful_enemy_move_right[ 5 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_005.png" );
	_harmful_enemy_move_right[ 6 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_006.png" );
	_harmful_enemy_move_right[ 7 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_007.png" );
	_harmful_enemy_move_right[ 8 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_008.png" );
	_harmful_enemy_move_right[ 9 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_009.png" );
	_harmful_enemy_move_right[ 10 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_010.png" );
	_harmful_enemy_move_right[ 11 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_011.png" );
	_harmful_enemy_move_right[ 12 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_012.png" );
	_harmful_enemy_move_right[ 13 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_013.png" );
	_harmful_enemy_move_right[ 14 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_014.png" );
	_harmful_enemy_move_right[ 15 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_015.png" );
	_harmful_enemy_move_right[ 16 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_016.png" );
	_harmful_enemy_move_right[ 17 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_017.png" );
	_harmful_enemy_move_right[ 18 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_018.png" );
	_harmful_enemy_move_right[ 19 ] = drawer->createImage( "Resource/image/gimmick/enemy/enemy_right_move/resize/__small_droid_activated_idle_019.png" );
	_deactivated_enemy_move_left[ 0 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_000.png" );
	_deactivated_enemy_move_left[ 1 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_001.png" );
	_deactivated_enemy_move_left[ 2 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_002.png" );
	_deactivated_enemy_move_left[ 3 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_003.png" );
	_deactivated_enemy_move_left[ 4 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_004.png" );
	_deactivated_enemy_move_left[ 5 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_005.png" );
	_deactivated_enemy_move_left[ 6 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_006.png" );
	_deactivated_enemy_move_left[ 7 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_007.png" );
	_deactivated_enemy_move_left[ 8 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_008.png" );
	_deactivated_enemy_move_left[ 9 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_009.png" );
	_deactivated_enemy_move_left[ 10 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_010.png" );
	_deactivated_enemy_move_left[ 11 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_011.png" );
	_deactivated_enemy_move_left[ 12 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_012.png" );
	_deactivated_enemy_move_left[ 13 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_013.png" );
	_deactivated_enemy_move_left[ 14 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_014.png" );
	_deactivated_enemy_move_left[ 15 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_015.png" );
	_deactivated_enemy_move_left[ 16 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_016.png" );
	_deactivated_enemy_move_left[ 17 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_017.png" );
	_deactivated_enemy_move_left[ 18 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_018.png" );
	_deactivated_enemy_move_left[ 19 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_left/resize/__small_droid_deactivated_idle_019.png" );
	_deactivated_enemy_move_right[ 0 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_000.png" );
	_deactivated_enemy_move_right[ 1 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_001.png" );
	_deactivated_enemy_move_right[ 2 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_002.png" );
	_deactivated_enemy_move_right[ 3 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_003.png" );
	_deactivated_enemy_move_right[ 4 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_004.png" );
	_deactivated_enemy_move_right[ 5 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_005.png" );
	_deactivated_enemy_move_right[ 6 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_006.png" );
	_deactivated_enemy_move_right[ 7 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_007.png" );
	_deactivated_enemy_move_right[ 8 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_008.png" );
	_deactivated_enemy_move_right[ 9 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_009.png" );
	_deactivated_enemy_move_right[ 10 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_010.png" );
	_deactivated_enemy_move_right[ 11 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_011.png" );
	_deactivated_enemy_move_right[ 12 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_012.png" );
	_deactivated_enemy_move_right[ 13 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_013.png" );
	_deactivated_enemy_move_right[ 14 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_014.png" );
	_deactivated_enemy_move_right[ 15 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_015.png" );
	_deactivated_enemy_move_right[ 16 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_016.png" );
	_deactivated_enemy_move_right[ 17 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_017.png" );
	_deactivated_enemy_move_right[ 18 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_018.png" );
	_deactivated_enemy_move_right[ 19 ] = drawer->createImage( "Resource/image/gimmick/enemy/deactivated_enemy_right/resize/__small_droid_deactivated_idle_019.png" );

	_mode_change_left[ 0 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_000.png" );
	_mode_change_left[ 1 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_001.png" );
	_mode_change_left[ 2 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_002.png" );
	_mode_change_left[ 3 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_003.png" );
	_mode_change_left[ 4 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_004.png" );
	_mode_change_left[ 5 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_005.png" );
	_mode_change_left[ 6 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_006.png" );
	_mode_change_left[ 7 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_007.png" );
	_mode_change_left[ 8 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_008.png" );
	_mode_change_left[ 9 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_left/resize/__small_droid_deactivate_009.png" );
	_mode_change_right[ 0 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_000.png" );
	_mode_change_right[ 1 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_001.png" );
	_mode_change_right[ 2 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_002.png" );
	_mode_change_right[ 3 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_003.png" );
	_mode_change_right[ 4 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_004.png" );
	_mode_change_right[ 5 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_005.png" );
	_mode_change_right[ 6 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_006.png" );
	_mode_change_right[ 7 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_007.png" );
	_mode_change_right[ 8 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_008.png" );
	_mode_change_right[ 9 ] = drawer->createImage( "Resource/image/gimmick/enemy/mode_change_right/resize/__small_droid_deactivate_009.png" );
}

void Viewer::setupWind( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_wind_up[ 0 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind0.png" );
	_wind_up[ 1 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind1.png" );
	_wind_up[ 2 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind2.png" );
	_wind_up[ 3 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind3.png" );
	_wind_up[ 4 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind4.png" );
	_wind_up[ 5 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind5.png" );
	_wind_up[ 6 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind6.png" );
	_wind_up[ 7 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind7.png" );
	_wind_up[ 8 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind8.png" );
	_wind_up[ 9 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind9.png" );
	_wind_up[ 10 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind10.png" );
	_wind_up[ 11 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind11.png" );
	_wind_up[ 12 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind12.png" );
	_wind_up[ 13 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind13.png" );
	_wind_up[ 14 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind14.png" );
	_wind_up[ 15 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind15.png" );
	_wind_up[ 16 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind16.png" );
	_wind_up[ 17 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind17.png" );
	_wind_up[ 18 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind18.png" );
	_wind_up[ 19 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind19.png" );
	_wind_up[ 20 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind20.png" );
	_wind_up[ 21 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind21.png" );
	_wind_up[ 22 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind22.png" );
	_wind_up[ 23 ] = drawer->createImage( "Resource/image/gimmick/wind/up/wind23.png" );
	_wind_down[ 0 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind0.png" );
	_wind_down[ 1 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind1.png" );
	_wind_down[ 2 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind2.png" );
	_wind_down[ 3 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind3.png" );
	_wind_down[ 4 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind4.png" );
	_wind_down[ 5 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind5.png" );
	_wind_down[ 6 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind6.png" );
	_wind_down[ 7 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind7.png" );
	_wind_down[ 8 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind8.png" );
	_wind_down[ 9 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind9.png" );
	_wind_down[ 10 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind10.png" );
	_wind_down[ 11 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind11.png" );
	_wind_down[ 12 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind12.png" );
	_wind_down[ 13 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind13.png" );
	_wind_down[ 14 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind14.png" );
	_wind_down[ 15 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind15.png" );
	_wind_down[ 16 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind16.png" );
	_wind_down[ 17 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind17.png" );
	_wind_down[ 18 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind18.png" );
	_wind_down[ 19 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind19.png" );
	_wind_down[ 20 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind20.png" );
	_wind_down[ 21 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind21.png" );
	_wind_down[ 22 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind22.png" );
	_wind_down[ 23 ] = drawer->createImage( "Resource/image/gimmick/wind/down/wind23.png" );
	_wind_right[ 0 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind0.png" );
	_wind_right[ 1 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind1.png" );
	_wind_right[ 2 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind2.png" );
	_wind_right[ 3 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind3.png" );
	_wind_right[ 4 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind4.png" );
	_wind_right[ 5 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind5.png" );
	_wind_right[ 6 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind6.png" );
	_wind_right[ 7 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind7.png" );
	_wind_right[ 8 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind8.png" );
	_wind_right[ 9 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind9.png" );
	_wind_right[ 10 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind10.png" );
	_wind_right[ 11 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind11.png" );
	_wind_right[ 12 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind12.png" );
	_wind_right[ 13 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind13.png" );
	_wind_right[ 14 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind14.png" );
	_wind_right[ 15 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind15.png" );
	_wind_right[ 16 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind16.png" );
	_wind_right[ 17 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind17.png" );
	_wind_right[ 18 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind18.png" );
	_wind_right[ 19 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind19.png" );
	_wind_right[ 20 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind20.png" );
	_wind_right[ 21 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind21.png" );
	_wind_right[ 22 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind22.png" );
	_wind_right[ 23 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind23.png" );
	_wind_left[ 0 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind0.png" );
	_wind_left[ 1 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind1.png" );
	_wind_left[ 2 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind2.png" );
	_wind_left[ 3 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind3.png" );
	_wind_left[ 4 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind4.png" );
	_wind_left[ 5 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind5.png" );
	_wind_left[ 6 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind6.png" );
	_wind_left[ 7 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind7.png" );
	_wind_left[ 8 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind8.png" );
	_wind_left[ 9 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind9.png" );
	_wind_left[ 10 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind10.png" );
	_wind_left[ 11 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind11.png" );
	_wind_left[ 12 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind12.png" );
	_wind_left[ 13 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind13.png" );
	_wind_left[ 14 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind14.png" );
	_wind_left[ 15 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind15.png" );
	_wind_left[ 16 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind16.png" );
	_wind_left[ 17 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind17.png" );
	_wind_left[ 18 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind18.png" );
	_wind_left[ 19 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind19.png" );
	_wind_left[ 20 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind20.png" );
	_wind_left[ 21 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind21.png" );
	_wind_left[ 22 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind22.png" );
	_wind_left[ 23 ] = drawer->createImage( "Resource/image/gimmick/wind/side/wind23.png" );
}

void Viewer::setupBubble( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_spread_bubble[ 0 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_01.png" );
	_spread_bubble[ 1 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_02.png" );
	_spread_bubble[ 2 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_03.png" );
	_spread_bubble[ 3 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_04.png" );
	_spread_bubble[ 4 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_05.png" );
	_spread_bubble[ 5 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_06.png" );
	_spread_bubble[ 6 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_07.png" );
	_spread_bubble[ 7 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_08.png" );
	_spread_bubble[ 8 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_09.png" );
	_spread_bubble[ 9 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_10.png" );
	_spread_bubble[ 10 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_11.png" );
	_spread_bubble[ 11 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_12.png" );
	_spread_bubble[ 12 ] = drawer->createImage( "Resource/image/gimmick/bubble/bubble_13.png" );
	_wide_bubble[ 0 ] = drawer->createImage( "Resource/image/gimmick/bubble/wide_bubble1.png" );
	_wide_bubble[ 1 ] = drawer->createImage( "Resource/image/gimmick/bubble/wide_bubble2.png" );
	_wide_bubble[ 2 ] = drawer->createImage( "Resource/image/gimmick/bubble/wide_bubble3.png" );
	_wide_bubble[ 3 ] = drawer->createImage( "Resource/image/gimmick/bubble/wide_bubble4.png" );
	_wide_bubble[ 4 ] = drawer->createImage( "Resource/image/gimmick/bubble/wide_bubble5.png" );
	_wide_bubble[ 5 ] = drawer->createImage( "Resource/image/gimmick/bubble/wide_bubble6.png" );
	_wide_bubble[ 6 ] = drawer->createImage( "Resource/image/gimmick/bubble/wide_bubble7.png" );
	_wide_bubble[ 7 ] = drawer->createImage( "Resource/image/gimmick/bubble/wide_bubble8.png" );
}

void Viewer::setupRandom( ) {
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		_random_pos.push_back( Tool::getRand( ( int )gimmicks[ i ]->getSize( ).x - BUBBLE_SIZE_X ) );
		_random_bubble_type.push_back( Tool::getRand( BUBBLE_TYPE ) );
		if ( _random_pos[ i ] < 7 ) {
			_random_pos[ i ] += 7;
		}
	}
}

void Viewer::setupRespawn( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_procession_player = drawer->createImage( "Resource/image/Respawn/procession_player2.png" );
	_procession_dark = drawer->createImage( "Resource/image/Respawn/procession_dark.png" );
	_respawn_anime[ 0 ] = drawer->createImage( "Resource/image/Respawn/lab_door_01.png" );
	_respawn_anime[ 1 ] = drawer->createImage( "Resource/image/Respawn/lab_door_02.png" );
	_respawn_anime[ 2 ] = drawer->createImage( "Resource/image/Respawn/lab_door_03.png" );
	_respawn_anime[ 3 ] = drawer->createImage( "Resource/image/Respawn/lab_door_04.png" );
	_respawn_anime[ 4 ] = drawer->createImage( "Resource/image/Respawn/lab_door_05.png" );
	_respawn_anime[ 5 ] = drawer->createImage( "Resource/image/Respawn/lab_door_06.png" );
	_respawn_anime[ 6 ] = drawer->createImage( "Resource/image/Respawn/lab_door_07.png" );
	_respawn_anime[ 7 ] = drawer->createImage( "Resource/image/Respawn/lab_door_08.png" );
}

void Viewer::setupDieEffect( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_die_effect_anime[ 0 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00000.png" );
	_die_effect_anime[ 1 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00001.png" );
	_die_effect_anime[ 2 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00002.png" );
	_die_effect_anime[ 3 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00003.png" );
	_die_effect_anime[ 4 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00004.png" );
	_die_effect_anime[ 5 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00005.png" );
	_die_effect_anime[ 6 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00006.png" );
	_die_effect_anime[ 7 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00007.png" );
	_die_effect_anime[ 8 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00008.png" );
	_die_effect_anime[ 9 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00009.png" );
	_die_effect_anime[ 10 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00010.png" );
	_die_effect_anime[ 11 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00011.png" );
	_die_effect_anime[ 12 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00012.png" );
	_die_effect_anime[ 13 ] = drawer->createImage( "Resource/image/player/die_effect/blast 3_00013.png" );
}

void Viewer::setupConveyor( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_conveyor[ 2 ] = drawer->createImage( "Resource/image/conveyor/conveyor_00.png" );
	_conveyor[ 1 ] = drawer->createImage( "Resource/image/conveyor/conveyor_01.png" );
	_conveyor[ 0 ] = drawer->createImage( "Resource/image/conveyor/conveyor_02.png" );

}

void Viewer::draw( ) {
	if ( _first && _view_clear_count < CLEAR_BASE_COUNT[ 0 ] / 2 ) {
		drawBackGround( );
		drawBetweenObject( );
		drawEnding( );
		drawGrounds( );
		drawEndingGirl( );
		drawRespawn( );
		drawGimmickAnime( );
		drawTutorial( );
		//drawShadow( );
		drawPlayer( );
		drawGimmick( );
		drawEnemyAnime( );
		drawWind( );
		drawGauge( );
	}
	drawClear( );
	_transition->update( );
	_first = true;
	drawEndingText( );
	drawMovieLine( );
	skipEndingButton( );
	drawMenu( );
	drawOperation( );
	drawCreditList( );
}

void Viewer::drawBackGround( ) const {
	Vector2 camera_pos = _camera->getPos( );
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	Vector2 bg_size = Vector2( 9000, 6000 );
	//drawer->setBlend( scope, Drawer::BLEND::ADD, BG_BRIGHTNESS );

	//move_background
	StageManagerPtr stage_manager = Task::get< StageManager >( );

	if ( _map_data->getStageName( stage_manager->getStageNum( ) ) != "ending_stage" ) {
		_background->draw( Vector2( 0, 0 ) - Vector2( camera_pos.x / 4, camera_pos.y / 4 ) );
	}

	//not_move_background
	//_background->draw( 0, 0 );
}

void Viewer::drawPlayer( ) {
	Vector2 camera_pos = _camera->getPos( );
	DrawerPtr drawer = Task::get< Drawer >( );
	StageManagerPtr manager = Task::get< StageManager >( );
	std::vector< PlayerPtr > players = _map_data->getPlayers( );
	for ( int i = 0; i < players.size( ); i++ ) {
		Vector2 spos = players[ i ]->getPos( );
		Vector2 adjust_pos = spos + Vector2( 0, 1 ) - camera_pos;
		Vector2 player_size = players[ i ]->getSize( );
		Vector2 extend = Vector2( 8, 15 );
		Vector2 inversion_extend = Vector2( extend.y, extend.x );

		if ( players[ i ]->getState( ) != Player::STATE::DEAD ) {
			switch ( players[ i ]->getDir( ) ) {
			case Player::DIR::RIGHT:
				animePlayerRight( adjust_pos + Vector2( -12, 0 ), players[ i ]->isMoving( ), players[ i ]->isJumping( ), players[ i ]->isStanding( ) );
				break;
			case Player::DIR::LEFT:
				animePlayerLeft( adjust_pos + Vector2( -12, 0 ), players[ i ]->isMoving( ), players[ i ]->isJumping( ), players[ i ]->isStanding( ) );
			}


		} else {
			if ( players[ i ]->isDeadLie( ) ) {
				//collapse_size_change
				/*if ( players[ i ]->isCollapse( ) && players[ i ]->getHP( ) < PLAYER_HP * 4 / 5 ) {
					const Vector2 PLAYER_IMAGE_SIZE = Vector2( 151, 80 );
					double scale = ( double )players[ i ]->getHP( ) / ( double )( PLAYER_HP * 4 / 5 );
					double width = PLAYER_IMAGE_SIZE.x - ( double )( ( PLAYER_HP * 4 / 5 - players[ i ]->getHP( ) ) / 2 );
					if ( width < PLAYER_IMAGE_SIZE.x / 2 ) {
						width = PLAYER_IMAGE_SIZE.x / 2;
					}
					switch ( players[ i ]->getDir( ) ) {
					case Player::DIR::RIGHT:
						_death_player_right->drawRectRotation( adjust_pos + Vector2( players[ i ]->getSize( ).x / 2, players[ i ]->getSize( ).y / 2 ) + add_pos_y,
															   scale, 0, ( int )PLAYER_IMAGE_SIZE.x / 2, ( int )PLAYER_IMAGE_SIZE.y / 2,
															   0, 0, ( int )width, ( int )PLAYER_IMAGE_SIZE.y );
						break;
					case Player::DIR::LEFT:
						_death_player_left->drawRectRotation( adjust_pos + Vector2( players[ i ]->getSize( ).x / 2, players[ i ]->getSize( ).y / 2 ) + add_pos_y,
															  scale, 0, ( int )PLAYER_IMAGE_SIZE.x / 2, ( int )PLAYER_IMAGE_SIZE.y / 2,
															  ( int )( PLAYER_IMAGE_SIZE.x - width ), 0, ( int )PLAYER_IMAGE_SIZE.x, ( int )PLAYER_IMAGE_SIZE.y );
						break;
					}
				} else {*/
				Vector2 add_lie_pos = Vector2( 0, 0 );
				switch ( players[ i ]->getDir( ) ) {
				case Player::DIR::RIGHT:
					_death_player_right->draw( adjust_pos + add_lie_pos );
					break;
				case Player::DIR::LEFT:
					_death_player_left->draw( adjust_pos + add_lie_pos );
					break;
				}
				//}
			} else {
				//collapse_size_change
				/*if ( players[ i ]->isCollapse( ) && players[ i ]->getHP( ) < PLAYER_HP * 4 / 5 ) {
					const double PI = 3.1415926535897932384626433832795f;
					const Vector2 PLAYER_IMAGE_SIZE = Vector2( 151, 80 );
					double scale = ( double )players[ i ]->getHP( ) / ( double )( PLAYER_HP * 4 / 5 );
					double height = PLAYER_IMAGE_SIZE.x - ( double )( ( PLAYER_HP * 4 / 5 - players[ i ]->getHP( ) ) / 2 );
					if ( height < PLAYER_IMAGE_SIZE.x / 2 ) {
						height = PLAYER_IMAGE_SIZE.x / 2;
					}
					switch ( players[ i ]->getDir( ) ) {
					case Player::DIR::RIGHT:
						_death_player_right->drawRectRotation( adjust_pos + Vector2( players[ i ]->getSize( ).x / 2, players[ i ]->getSize( ).y / 2 ),
															   scale, PI / 2, ( int )PLAYER_IMAGE_SIZE.x / 2, ( int )PLAYER_IMAGE_SIZE.y / 2,
															   0, 0, ( int )height, ( int )PLAYER_IMAGE_SIZE.y );
						break;
					case Player::DIR::LEFT:
						_death_player_left->drawRectRotation( adjust_pos + Vector2( players[ i ]->getSize( ).x / 2, players[ i ]->getSize( ).y / 2 ),
															  scale, -PI / 2, ( int )PLAYER_IMAGE_SIZE.x / 2, ( int )PLAYER_IMAGE_SIZE.y / 2,
															  ( int )( PLAYER_IMAGE_SIZE.x - height ), 0, ( int )PLAYER_IMAGE_SIZE.x, ( int )PLAYER_IMAGE_SIZE.y );
						break;
					}
				} else {*/
				Vector2 add_head_pos = Vector2( -10, -8 );
				switch ( players[ i ]->getDir( ) ) {
				case Player::DIR::RIGHT:
					if ( players[ i ]->getReason( ) == Player::DEAD_REASON::SUICIDE ) {

						if ( _map_data->getStageName( manager->getStageNum( ) ) == "ending_stage" ) {
							_ending_head->draw( adjust_pos + add_head_pos );
						} else {
							_right_head->draw( adjust_pos + add_head_pos );
						}
					} else {
						_death_player_right->drawRotation( adjust_pos + Vector2( players[ i ]->getSize( ).x / 2, players[ i ]->getSize( ).y / 2 ), PI / 2 );
					}
					break;
				case Player::DIR::LEFT:
					if ( players[ i ]->getReason( ) == Player::DEAD_REASON::SUICIDE ) {
						_left_head->draw( adjust_pos + add_head_pos );
					} else {
						_death_player_left->drawRotation( adjust_pos + Vector2( players[ i ]->getSize( ).x / 2, players[ i ]->getSize( ).y / 2 ), -PI / 2 );
					}
				}
				//}
			}
			if ( players[ i ]->getReason( ) != Player::DEAD_REASON::ACID ) {
				drawPlayerDieEffectAnime( players[ i ] );
			} else {
				if ( !players[ i ]->isDrawnDieEffdect( ) ) {
					_player_die_effect_image_num = 0;
				}
				players[ i ]->setDrawnDieEffect( );
			}
		}
#if _DEBUG
		drawer->drawBox( ( int )spos.x - ( int )camera_pos.x, ( int )spos.y - ( int )camera_pos.y, ( int )( spos.x - camera_pos.x + player_size.x ), ( int )( spos.y - camera_pos.y + player_size.y ), Color( 0, 1, 0 ), false );
#endif
	}
	drawPlayerDeleteEffectAnime( );
}

void Viewer::drawGrounds( ) const {
	Vector2 camera_pos = _camera->getPos( );
	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< MapData::Ground* > grounds = _map_data->getGrounds( );
	for ( int i = 0; i < grounds.size( ); i++ ) {
		MapData::Ground* ground = grounds[ i ];
		for ( int i = 0; i < ground->view_size.x; i += GROUND_IMAGE_SIZE ) {
			for ( int j = 0; j < ground->view_size.y; j += GROUND_IMAGE_SIZE ) {
				int image_num = i / GROUND_IMAGE_SIZE + j / GROUND_IMAGE_SIZE;
				Vector2 adjust_pos = ground->pos + Vector2( i, ( double )( j - ( __int64 )ADJUST_GROUND_Y ) ) - camera_pos;
				if ( j == 0 ) {
					if ( ground->view_size.x - i < GROUND_IMAGE_SIZE && ground->view_size.y - j < GROUND_IMAGE_SIZE ) {
						_floor_top[ image_num % _floor.size( ) ]->drawRect( adjust_pos, 0, 0, ( int )ground->view_size.x - i, ( int )ground->view_size.y - j );
					} else if ( ground->view_size.x - i < GROUND_IMAGE_SIZE ) {
						_floor_top[ image_num % _floor_top.size( ) ]->drawRect( adjust_pos, 0, 0, ( int )ground->view_size.x - i, GROUND_IMAGE_SIZE );
					} else if ( ground->view_size.y - ADJUST_GROUND_Y - j < GROUND_IMAGE_SIZE ) {
						_floor_top[ image_num % _floor_top.size( ) ]->drawRect( adjust_pos, 0, 0, GROUND_IMAGE_SIZE, ( int )ground->view_size.y - j );
					} else {
						_floor_top[ image_num % _floor_top.size( ) ]->draw( adjust_pos );
					}
				} else {
					if ( ground->view_size.x - i < GROUND_IMAGE_SIZE && ground->view_size.y - j < GROUND_IMAGE_SIZE ) {
						_floor[ image_num % _floor.size( ) ]->drawRect( adjust_pos, 0, 0, ( int )ground->view_size.x - i, ( int )ground->view_size.y - j );
					} else if ( ground->view_size.x - i < GROUND_IMAGE_SIZE ) {
						_floor[ image_num % _floor.size( ) ]->drawRect( adjust_pos, 0, 0, ( int )ground->view_size.x - i, GROUND_IMAGE_SIZE );
					} else if ( ground->view_size.y - j < GROUND_IMAGE_SIZE ) {
						_floor[ image_num % _floor.size( ) ]->drawRect( adjust_pos, 0, 0, GROUND_IMAGE_SIZE, ( int )ground->view_size.y - j );
					} else {
						_floor[ image_num % _floor.size( ) ]->draw( adjust_pos );
					}
				}
			}
		}
	}
}

void Viewer::drawGimmick( ) const {
	Vector2 camera_pos = _camera->getPos( );
	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		Gimmick::TYPE type = gimmicks[ i ]->getType( );
		Vector2 pos = gimmicks[ i ]->getPos( ) - camera_pos;
		Vector2 size = gimmicks[ i ]->getSize( ) + pos;
		switch ( type ) {
		case Gimmick::TYPE::ACID:
			for ( int k = 0; k < gimmicks[ i ]->getSize( ).x; k += 1920 ) {
				for ( int j = 0; j < gimmicks[ i ]->getSize( ).y; j += 1080 ) {
					int tx = 10;
					int ty = 10;
					if ( k > 0 ) {
						tx = 308;
					}
					if ( j > 0 ) {
						ty = 250;
					}
					_acid->drawRect( ( int )pos.x + k, ( int )pos.y + j, tx, ty, ( int )gimmicks[ i ]->getSize( ).x - k, ( int )gimmicks[ i ]->getSize( ).y - j );
				}
			}
			break;
		case Gimmick::TYPE::NEEDLE:
			switch ( gimmicks[ i ]->getDir( ) ) {
			case Gimmick::DIR::UP:
				for ( int k = 0; k < gimmicks[ i ]->getSize( ).x; k += ( int )NEEDLE_SIZE.x ) {
					/*if ( k < 0 ) {
						_needle_top->drawRect( ( int )pos.x, ( int )pos.y, -k, 0, NEEDLE_SIZE.x, NEEDLE_SIZE.y );
					} else if ( k + ( int )NEEDLE_SIZE.x > gimmicks[ i ]->getSize( ).x ) {
						_needle_top->drawRect( ( int )pos.x + k, ( int )pos.y, 0, 0, gimmicks[ i ]->getSize( ).x - k, NEEDLE_SIZE.y );
					} else {
						_needle_top->draw( ( int )pos.x + k, ( int )pos.y );
					}*/
					_needle_top->draw( ( int )pos.x + k, ( int )pos.y );
				}
				break;
			case Gimmick::DIR::DOWN:
				for ( int k = 0; k < gimmicks[ i ]->getSize( ).x; k += ( int )NEEDLE_SIZE.x ) {
					_needle_down->draw( ( int )pos.x + k, ( int )pos.y );
				}
				break;
			case Gimmick::DIR::LEFT:
				for ( int k = 0; k < gimmicks[ i ]->getSize( ).y; k += ( int )NEEDLE_SIZE.x ) {
					_needle_left->draw( ( int )pos.x, ( int )pos.y + k );
				}
				break;
			case Gimmick::DIR::RIGHT:
				for ( int k = 0; k < gimmicks[ i ]->getSize( ).y; k += ( int )NEEDLE_SIZE.x ) {
					_needle_right->draw( ( int )pos.x, ( int )pos.y + k );
				}
				break;
			}
			break;
		case Gimmick::TYPE::FAN:
			break;
		case Gimmick::TYPE::DECELERATE_NET:
			_decelerate_net->draw( pos );

			break;
		case Gimmick::TYPE::STICKY_NET:
			_sticky_net->draw( pos );
			break;
		}
	}
}

void Viewer::drawGimmickAnime( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Vector2 camera_pos = _camera->getPos( );
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	std::vector< PlayerPtr > players = _map_data->getPlayers( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		for ( int j = 0; j < players.size( ); j++ ) {
			if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::GOAL ) {
				drawGoal( gimmicks[ i ]->getPos( ) - camera_pos, players[ j ]->getPos( ) - camera_pos, players[ j ]->getState( ), gimmicks[ i ]->getSize( ) );
#if _DEBUG
				drawer->drawBox( ( int )( gimmicks[ i ]->getPos( ).x - camera_pos.x ), ( int )( gimmicks[ i ]->getPos( ).y - camera_pos.y ),
								 ( int )( gimmicks[ i ]->getPos( ).x + gimmicks[ i ]->getSize( ).x - camera_pos.x ), ( int )( gimmicks[ i ]->getPos( ).y + gimmicks[ i ]->getSize( ).y - camera_pos.y ),
								 Color( 1.0, 0.0, 0.0 ), false );
#endif
			}
		}
		switch ( gimmicks[ i ]->getType( ) ) {
		case Gimmick::TYPE::ACID:
			drawBubbles( gimmicks[ i ]->getPos( ) - camera_pos, i, gimmicks[ i ]->getSize( ).x, _amount_of_acid );
			break;
		case Gimmick::TYPE::FAN:
			Vector2 pos = gimmicks[ i ]->getPos( ) - camera_pos;
			Vector2 adjust = Vector2( -5, -5 );
			if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
				_fan_wait_time[ gimmicks[ i ] ]++;
				if ( std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->getOffTime( ) -
					 std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->getCount( ) < 40 &&
					 !std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->isOn( ) ) {
					_fan_wait_time[ gimmicks[ i ] ] %= 6;
				} else if ( !std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->isOn( ) ) {
					_fan_wait_time[ gimmicks[ i ] ] = 1;
				} else {
					_fan_wait_time[ gimmicks[ i ] ] %= 2;
				}
				if ( _fan_wait_time[ gimmicks[ i ] ] == 0 ) {
					_fan_image_num[ gimmicks[ i ] ]++;
				}
				if ( _fan_image_num[ gimmicks[ i ] ] > 1 ) {
					_fan_image_num[ gimmicks[ i ] ] = 0;
				}
				if ( std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->getOffTime( ) -
					 std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->getCount( ) > 40 &&
					 !std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->isOn( ) ) {
					_fan_image_num[ gimmicks[ i ] ] = 0;
				}
			}
			switch ( gimmicks[ i ]->getDir( ) ) {
			case Gimmick::DIR::UP:
				_fan_top[ _fan_image_num[ gimmicks[ i ] ] ]->draw( pos + adjust );
				break;
			case Gimmick::DIR::DOWN:
				_fan_down[ _fan_image_num[ gimmicks[ i ] ] ]->draw( pos + adjust );
				break;
			case Gimmick::DIR::LEFT:
				_fan_left[ _fan_image_num[ gimmicks[ i ] ] ]->draw( pos + adjust );
				break;
			case Gimmick::DIR::RIGHT:
				_fan_right[ _fan_image_num[ gimmicks[ i ] ] ]->draw( pos + adjust );
				break;
			}
			break;
		}
	}
}


void Viewer::drawClear( ) {
	ApplicationPtr app = Application::getInstance( );
	SystemPtr system = Task::get< System >( );

	const int STAR_IMAGE_SIZE = 128;

	if ( system->isClear( ) ) {
		if ( _view_clear_count >= CLEAR_BASE_COUNT[ 0 ] ) {
			_view_clear_count++;
		} else {
			if ( !_transition->isScaling( ) ) {
				_view_clear_count++;
			}
		}
		drawClearIrisOut( );
		drawClearIrisIn( );

		if ( _view_clear_count > CLEAR_BASE_COUNT[ 0 ] + 1 ) {
			_clear_background->draw( 0, 0 );
			drawClearPlayerHead( );
			drawClearConveyor( );
		}

		drawClearText( );
		drawClearResultText( );
		drawClearStar( );

		if ( _view_clear_count > CLEAR_BASE_COUNT[ 3 ] ) {
			_view_clear_count = CLEAR_BASE_COUNT[ 3 ];
			_view_save_count += 30;
			int wait_time = 3000;
			Vector2 pos = Vector2( ( double )( ( __int64 )app->getScreenWidth( ) - _view_save_count ), ( double )( ( __int64 )app->getScreenHeight( ) - 120 ) );
			if ( _view_save_count > 320 ) {
				pos = Vector2( ( double )( ( __int64 )app->getScreenWidth( ) - 320 ),
							   ( double )( ( __int64 )app->getScreenHeight( ) - 120 ) );
			}
			if ( _view_save_count < 320 + wait_time ) {
				_save_text->draw( pos );
			} else {
				_view_save_count = 320 + wait_time;
			}

			if ( _view_save_count > 320 + wait_time / 2 ) {
				_switch_button_count++;
				_switch_button_count %= 30;
				if ( _switch_button_count == 0 ) {
					_switch_image_num++;
				}
				if ( _switch_image_num > 1 ) {
					_switch_image_num = 0;
				}
				_button_switch[ _switch_image_num ]->draw( app->getScreenWidth( ) / 2 + ( int )DEATH_COUNT_IMAGE_SIZE.x / 2 + 130 - 60,
														   app->getScreenHeight( ) / 2 + ( int )DEATH_COUNT_IMAGE_SIZE.y + 125 - 30 - CLEAR_RESULT_ADJUST_Y );
			}
		}
	}

}

void Viewer::drawClearIrisOut( ) const {
	Vector2 camera_pos = _camera->getPos( );
	if ( !_transition->isTransition( ) && _view_clear_count < CLEAR_BASE_COUNT[ 0 ] ) {
		std::vector< PlayerPtr > players = _map_data->getPlayers( );
		for ( int i = 0; i < players.size( ); i++ ) {
			if ( players[ i ]->getState( ) == Player::STATE::ALIVE ) {
				_transition->setIrisOut( ( int )( players[ i ]->getPos( ).x + PLAYER_SIZE_X / 2 ),
										 ( int )( players[ i ]->getPos( ).y + PLAYER_SIZE_Y / 4 ),
										 ( int )camera_pos.x, ( int )camera_pos.y,
										 DEFAULT_IRIS_SPEED, 30,
										 PLAYER_SIZE_X / 4 );
			}
		}
	}
}

void Viewer::drawClearIrisIn( ) const {
	ApplicationPtr app = Application::getInstance( );
	if ( _view_clear_count == CLEAR_BASE_COUNT[ 0 ] ) {
		Vector2 camera_pos = _camera->getPos( );
		_transition->setIrisIn( app->getScreenWidth( ) / 2,
								app->getScreenHeight( ) / 2,
								0, 0,
								DEFAULT_IRIS_SPEED, 0, 30 );
	}
}

void Viewer::drawClearText( ) const {
	ApplicationPtr app = Application::getInstance( );
	const Vector2 CLEAR_TEXT_IMAGE_SIZE = Vector2( 850, 120 );
	if ( _view_clear_count > CLEAR_BASE_COUNT[ 1 ] ) {
		Vector2 anime_clear_text_size = Vector2( ( double )( ( ( __int64 )_view_clear_count - CLEAR_BASE_COUNT[ 1 ] ) * 100 ),
												 ( double )( CLEAR_TEXT_IMAGE_SIZE.y + 80 - ( ( __int64 )_view_clear_count - CLEAR_BASE_COUNT[ 1 ] ) * 10 ) );
		if ( _view_clear_count > CLEAR_BASE_COUNT[ 1 ] + 10 ) {
			anime_clear_text_size = CLEAR_TEXT_IMAGE_SIZE;
			/*if ( _view_clear_count > CLEAR_BASE_COUNT[ 1 ] - 10 ) {
				anime_clear_text_size = Vector2( ( double )( ( CLEAR_BASE_COUNT[ 1 ] - ( __int64 )_view_clear_count ) * 100 ),
														 ( double )( CLEAR_TEXT_IMAGE_SIZE.y + 80 - ( CLEAR_BASE_COUNT[ 1 ] - ( __int64 )_view_clear_count ) * 10 ) );
			}*/
		}
		Vector2 window_add = Vector2( 50, 30 );
		_text_window->drawExtend( ( int )( app->getScreenWidth( ) / 2 - anime_clear_text_size.x / 2 - window_add.x ), ( int )( app->getScreenHeight( ) / 5 - anime_clear_text_size.y / 2 - window_add.y ),
								  ( int )( app->getScreenWidth( ) / 2 + anime_clear_text_size.x / 2 + window_add.x ), ( int )( app->getScreenHeight( ) / 5 + anime_clear_text_size.y / 2 + window_add.y ) );
		_clear_text->drawExtend( ( int )( app->getScreenWidth( ) / 2 - anime_clear_text_size.x / 2 ), ( int )( app->getScreenHeight( ) / 5 - anime_clear_text_size.y / 2 ),
								 ( int )( app->getScreenWidth( ) / 2 + anime_clear_text_size.x / 2 ), ( int )( app->getScreenHeight( ) / 5 + anime_clear_text_size.y / 2 ) );
	}
}

void Viewer::drawClearResultText( ) const {
	ApplicationPtr app = Application::getInstance( );

	const Vector2 WINDOW_ADD_SIZE = Vector2( 500, 300 );

	int adjust_death_count_pos_x = 70;
	if ( _map_data->getDeadPlayerNum( ) >= 10 ) {
		adjust_death_count_pos_x = 110;
	}
	if ( _map_data->getDeadPlayerNum( ) >= 100 ) {
		adjust_death_count_pos_x = 150;
	}
	if ( _map_data->getDeadPlayerNum( ) >= 1000 ) {
		adjust_death_count_pos_x = 210;
	}

	if ( _view_clear_count > CLEAR_BASE_COUNT[ 2 ] ) {
		Vector2 anime_death_count_size = Vector2( ( double )( ( ( __int64 )_view_clear_count - CLEAR_BASE_COUNT[ 2 ] ) * 100 ),
												  ( double )( 200 - ( ( __int64 )_view_clear_count - CLEAR_BASE_COUNT[ 2 ] ) * 10 ) );
		if ( _view_clear_count > CLEAR_BASE_COUNT[ 2 ] + 10 ) {
			anime_death_count_size = DEATH_COUNT_IMAGE_SIZE;
		}
		_text_window->drawExtend( app->getScreenWidth( ) / 2 - ( int )anime_death_count_size.x / 2 - WINDOW_ADD_SIZE.x / 2, app->getScreenHeight( ) / 2 - 50 - CLEAR_RESULT_ADJUST_Y,
								  app->getScreenWidth( ) / 2 + ( int )anime_death_count_size.x / 2 + WINDOW_ADD_SIZE.x / 2, app->getScreenHeight( ) / 2 + ( int )anime_death_count_size.y + WINDOW_ADD_SIZE.y - 50 - CLEAR_RESULT_ADJUST_Y );

		_death_count->drawExtend( app->getScreenWidth( ) / 2 - ( int )anime_death_count_size.x / 2 - ( int )adjust_death_count_pos_x, app->getScreenHeight( ) / 2 - CLEAR_RESULT_ADJUST_Y,
								  app->getScreenWidth( ) / 2 + ( int )anime_death_count_size.x / 2 - ( int )adjust_death_count_pos_x, app->getScreenHeight( ) / 2 + ( int )anime_death_count_size.y - CLEAR_RESULT_ADJUST_Y );
	}
	if ( _view_clear_count > CLEAR_BASE_COUNT[ 2 ] + 10 ) {
		drawNumber( ( double )( app->getScreenWidth( ) / 2 + DEATH_COUNT_IMAGE_SIZE.x / 2 - adjust_death_count_pos_x - 10 ), app->getScreenHeight( ) / 2 - CLEAR_RESULT_ADJUST_Y, _map_data->getDeadPlayerNum( ) );
	}
}

void Viewer::drawClearStar( ) {
	ApplicationPtr app = Application::getInstance( );
	MusicPtr music = Task::get< Music >( );
	ScorePtr score = Task::get< Score >( );
	StageManagerPtr manager = Task::get< StageManager >( );
	const int STAR_IMAGE_SIZE = 128;
	int score_num = score->getScore( manager->getStageNum( ), _map_data->getDeadPlayerNum( ) );
	int interval = 200;
	int add_star_pos_x = 60;
	int add_star_pos_y = 70;
	double star_rotation_speed = 0.5;
	double star_scale_speed = 0.05;

	Vector2 star_pos = Vector2( ( double )( ( __int64 )app->getScreenWidth( ) / 2 - interval - STAR_IMAGE_SIZE / 2 + add_star_pos_x ), ( double )( ( __int64 )app->getScreenHeight( ) * 2 / 3 + add_star_pos_y ) - CLEAR_RESULT_ADJUST_Y );
	Vector2 doughnut_star_pos = Vector2( ( double )( ( __int64 )app->getScreenWidth( ) / 2 - interval - STAR_IMAGE_SIZE / 2 ), app->getScreenHeight( ) * 2 / 3 - CLEAR_RESULT_ADJUST_Y );
	//star
	if ( _view_clear_count > CLEAR_BASE_COUNT[ 2 ] + 25 ) {
		_angle[ 0 ] += star_rotation_speed;
		if ( _scale[ 0 ] <= 1.0 ) {
			_scale[ 0 ] += star_scale_speed;
		}
		if ( _scale[ 0 ] >= 1.0 ) {
			if ( score_num <= 0 ) {
				_doughnut_star->draw( doughnut_star_pos );
			} else {
				_star->draw( star_pos - Vector2( add_star_pos_x, add_star_pos_y ) );
				if ( !_star_first[ 0 ] ) {
					music->starGetSE( );
					_star_first[ 0 ] = true;
				}
			}
		} else {
			if ( score_num <= 0 ) {
				_doughnut_star->drawRectRotation( ( int )star_pos.x, ( int )star_pos.y, _scale[ 0 ], _angle[ 0 ], 64, 64, 0, 0, 128, 128 );
			} else {
				_star->drawRectRotation( ( int )star_pos.x, ( int )star_pos.y, _scale[ 0 ], _angle[ 0 ], 64, 64, 0, 0, 128, 128 );
			}
		}
	}
	if ( _view_clear_count > CLEAR_BASE_COUNT[ 2 ] + 25 + 20 ) {
		_angle[ 1 ] += star_rotation_speed;
		if ( _scale[ 1 ] < 1.0 ) {
			_scale[ 1 ] += star_scale_speed;
		}
		if ( _scale[ 1 ] > 1.0 ) {
			if ( score_num <= 1 ) {
				_doughnut_star->draw( ( int )doughnut_star_pos.x + interval, ( int )doughnut_star_pos.y );
			} else {
				_star->draw( star_pos.x + interval - add_star_pos_x, star_pos.y - add_star_pos_y );
				if ( !_star_first[ 1 ] ) {
					music->starGetSE( );
					_star_first[ 1 ] = true;
				}
			}
		} else {
			if ( score_num <= 1 ) {
				_doughnut_star->drawRectRotation( ( int )star_pos.x + interval, ( int )star_pos.y, _scale[ 1 ], _angle[ 1 ], 64, 64, 0, 0, 128, 128 );;
			} else {
				_star->drawRectRotation( ( int )star_pos.x + interval, ( int )star_pos.y, _scale[ 1 ], _angle[ 1 ], 64, 64, 0, 0, 128, 128 );
			}
		}
	}
	if ( _view_clear_count > CLEAR_BASE_COUNT[ 2 ] + 25 + 20 * 2 ) {
		_angle[ 2 ] += star_rotation_speed;
		if ( _scale[ 2 ] < 1.0 ) {
			_scale[ 2 ] += star_scale_speed;
		}
		if ( _scale[ 2 ] > 1.0 ) {
			if ( score_num <= 2 ) {
				_doughnut_star->draw( ( int )doughnut_star_pos.x + ( __int64 )interval * 2, ( int )doughnut_star_pos.y );
			} else {
				_star->draw( star_pos.x + ( __int64 )interval * 2 - add_star_pos_x, star_pos.y - add_star_pos_y );
				if ( !_star_first[ 2 ] ) {
					music->starGetSE( );
					_star_first[ 2 ] = true;
				}
			}
		} else {
			if ( score_num <= 2 ) {
				_doughnut_star->drawRectRotation( ( int )star_pos.x + ( __int64 )interval * 2, ( int )star_pos.y, _scale[ 2 ], _angle[ 2 ], 64, 64, 0, 0, 128, 128 );
			} else {
				_star->drawRectRotation( ( int )star_pos.x + ( __int64 )interval * 2, ( int )star_pos.y, _scale[ 2 ], _angle[ 2 ], 64, 64, 0, 0, 128, 128 );
			}
		}
	}
}

void Viewer::drawClearPlayerHead( ) {
	ApplicationPtr app = Application::getInstance( );
	int dead_num = _map_data->getDeadPlayerNum( );
	int interval = 100;
	_player_head_wait_time++;
	_player_head_wait_time %= 1;
	if ( _view_clear_count > CLEAR_BASE_COUNT[ 1 ] ) {
		if ( _player_head_wait_time == 0 ) {
			_move_head += 4;
		}

		for ( int i = 0; i < dead_num; i++ ) {
			_clear_head_pos_x = interval * i * -1 + _move_head;
			_right_head->draw( _clear_head_pos_x, app->getScreenHeight( ) - 110 );
		}

	}
}

void Viewer::drawClearConveyor( ) {
	ApplicationPtr app = Application::getInstance( );
	int conveyor_width = 210;
	_conveyor_wait_time++;
	_conveyor_wait_time %= CONVEYOR_SPEED;

	if ( _conveyor_wait_time == 0 ) {
		_conveyor_image_num++;
		_conveyor_image_num %= 3;
	}
	for ( int i = 0; i < app->getScreenWidth( ) / conveyor_width + 1; i++ ) {
		_conveyor[ _conveyor_image_num ]->draw( i * conveyor_width, app->getScreenHeight( ) - 40 );
	}
}

void Viewer::drawNumber( double pos_x, double pos_y, int number ) const {
	const int SIDE = 4;
	std::vector< int > number_per_digit;
	do {
		number_per_digit.push_back( number % 10 );
		number /= 10;
	} while ( number > 0 );

	Vector2 draw_pos = Vector2( pos_x, pos_y );
	for ( int i = ( int )number_per_digit.size( ) - 1; i >= 0; i-- ) {
		int col = number_per_digit[ i ] / SIDE;
		int row = number_per_digit[ i ] % SIDE;
		_number->drawRect( draw_pos, row * NUMBER_IMAGE_SIZE, col * NUMBER_IMAGE_SIZE, NUMBER_IMAGE_SIZE, NUMBER_IMAGE_SIZE );
		draw_pos += Vector2( ( __int64 )NUMBER_IMAGE_SIZE - 20, 0 );
	}
}

void Viewer::drawPlayerDieEffectAnime( PlayerPtr player ) {
	int effect_size = 225;
	if ( !player->isDrawnDieEffdect( ) ) {
		_player_die_effect_time++;
		_player_die_effect_time %= 2;
		if ( _player_die_effect_time == 0 ) {
			_player_die_effect_image_num++;
		}
		if ( _player_die_effect_image_num >= PLAYER_DIE_EFFECT_IMAGE_NUM ) {
			_player_die_effect_image_num = 0;
			player->setDrawnDieEffect( );
		} else {
			_die_effect_anime[ _player_die_effect_image_num ]->drawExtend( player->getPos( ) - Vector2( effect_size - player->getSize( ).x / 2, effect_size - player->getSize( ).y / 2 ) - _camera->getPos( ),
																		   player->getPos( ) + Vector2( effect_size + player->getSize( ).x / 2, effect_size + player->getSize( ).y / 2 ) - _camera->getPos( ) );
		}
	}
}

void Viewer::drawPlayerDeleteEffectAnime( ) {
	int effect_size = 225;
	MusicPtr music = Task::get< Music >( );
	std::vector< PlayerPtr > players = _map_data->getPlayers( );
	for ( int i = 0; i < players.size( ); i++ ) {
		if ( players[ i ]->isDeleted( ) &&
			 players[ i ]->isInherited( ) &&
			 players[ i ]->getPos( ).y < _map_data->getHeight( ) &&
			 players[ i ]->getReason( ) != Player::DEAD_REASON::ACID &&
			 players[ i ]->getReason( ) != Player::DEAD_REASON::ENEMY ) {
			_delete_player_pos = players[ i ]->getPos( );
			_delete_player_size = players[ i ]->getSize( );
			_draw_delete_effect = true;
			music->deadSE( );
		}
	}
	if ( _draw_delete_effect ) {
		_player_delete_effect_time++;
		_player_delete_effect_time %= 2;
		if ( _player_delete_effect_time == 0 ) {
			_player_delete_effect_image_num++;
		}
		if ( _player_delete_effect_image_num >= PLAYER_DIE_EFFECT_IMAGE_NUM ) {
			_player_delete_effect_image_num = 0;
			_draw_delete_effect = false;
		} else {
			_die_effect_anime[ _player_delete_effect_image_num ]->drawExtend( _delete_player_pos - Vector2( effect_size - _delete_player_size.x / 2, effect_size - _delete_player_size.y / 2 ) - _camera->getPos( ),
																			  _delete_player_pos + Vector2( effect_size + _delete_player_size.x / 2, effect_size + _delete_player_size.y / 2 ) - _camera->getPos( ) );
		}
	}
}

void Viewer::drawEnemyAnime( ) {
	Vector2 camera_pos = _camera->getPos( );

	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< EnemyPtr > enemies = _map_data->getEnemies( );
	for ( int i = 0; i < enemies.size( ); i++ ) {
		Vector2 add_pos = Vector2( -10, -10 );
		Vector2 pos = enemies[ i ]->getPos( ) - camera_pos + add_pos;
		Vector2 size = pos + enemies[ i ]->getSize( );

		if ( enemies[ i ]->getStatus( ) == Enemy::STATUS::HARMFUL ) {
			//drawer->drawBox( ( int )pos.x, ( int )pos.y, ( int )size.x, ( int )size.y, Color( 1, 0, 0 ), true );
			switch ( enemies[ i ]->getDir( ) ) {
			case Enemy::DIR::LEFT:
				animeHarmfulEnemyLeft( pos - Vector2( 30, 20 ) );
				break;
			case Enemy::DIR::RIGHT:
				animeHarmfulEnemyRight( pos - Vector2( 50, 20 ) );
				break;
			}
		} else {
			if ( enemies[ i ]->isChange( ) ) {
				switch ( enemies[ i ]->getDir( ) ) {
				case Enemy::DIR::LEFT:
					animeEnemyModeChangeLeft( pos - Vector2( 30, 20 ), i );
					break;
				case Enemy::DIR::RIGHT:
					animeEnemyModeChangeRight( pos - Vector2( 50, 20 ), i );
					break;
				}
			} else {
				_mode_change_image_num[ i ] = 0;
				switch ( enemies[ i ]->getDir( ) ) {
				case Enemy::DIR::LEFT:
					animeDeactivatedEnemyLeft( pos - Vector2( 30, 20 ) );
					break;
				case Enemy::DIR::RIGHT:
					animeDeactivatedEnemyRight( pos - Vector2( 50, 20 ) );
					break;
				}
			}

		}
	}
}

void Viewer::drawOperation( ) {
	ApplicationPtr app = Application::getInstance( );

	int pos_x = app->getScreenWidth( ) / 2 - 1280 / 2;
	int pos_y = app->getScreenHeight( ) / 3 - 100;
	if ( _menu->isOperationMode( ) ) {
		_operation->draw( pos_x, pos_y );
	}
}

void Viewer::drawCreditList( ) {
	ApplicationPtr app = Application::getInstance( );

	int pos_x = app->getScreenWidth( ) / 2 - 700 / 2;
	int pos_y = app->getScreenHeight( ) / 2 - 100;
	if ( _menu->isCreditMode( ) ) {
		_blue_window->drawExtend( pos_x - 50, pos_y - 50,
								  pos_x + 700 + 50, pos_y + 500 + 50 );
		_credit_list->draw( pos_x, pos_y );
	}
}

void Viewer::drawGauge( ) {
	Vector2 camera_pos = _camera->getPos( );
	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< PlayerPtr > players = _map_data->getPlayers( );
	Vector2 interval = Vector2( -10, -105 );
	Vector2 bomb_size = Vector2( 90, 90 );
	_level_change_time = 20;

	for ( int i = 0; i < players.size( ); i++ ) {
		Vector2 spos = players[ i ]->getPos( );
		Vector2 adjust_pos = spos + Vector2( 0, 1 ) - camera_pos;
		Vector2 bomb_pos = Vector2( adjust_pos + interval );
		Vector2 bomb_extend_size( adjust_pos + interval + bomb_size );
		_suicide_count = players[ i ]->getSuicideCount( );
		if ( bomb_pos.y < 0 ) {
			adjust_pos = Vector2( spos.x - camera_pos.y - interval.x + 90, -interval.y + 10 );
		}
		if ( players[ i ]->isCannotExplosive( ) ) {
			_ban_bomb->drawExtend( adjust_pos + interval, adjust_pos + interval + bomb_size );
		}
		if ( _suicide_count != 0 && players[ i ]->getState( ) == Player::STATE::ALIVE ) {
			_suicide_count_level = ( _suicide_count + _level_change_time ) / _level_change_time;
			switch ( _suicide_count_level ) {
			case 1:_suicide_level_1->draw( adjust_pos + interval );
				break;
			case 2:_suicide_level_2->draw( adjust_pos + interval );
				break;
			case 3:_suicide_level_3->draw( adjust_pos + interval );
				break;
			}

			_icon_bomb->draw( adjust_pos + interval + Vector2( 10, 10 ) );
		}
	}
}

void Viewer::drawShadow( ) const {
	std::vector< PlayerPtr > players = _map_data->getPlayers( );
	PlayerPtr alive_player = NULL;
	for ( int i = 0; i < players.size( ); i++ ) {
		if ( players[ i ]->getState( ) == Player::STATE::ALIVE ) {
			alive_player = players[ i ];
		}
	}

	int pos_y = _map_data->getHeight( ) + 200;
	if ( alive_player != NULL ) {
		std::vector< MapData::Ground* > grounds = _map_data->getGrounds( );
		for ( int i = 0; i < grounds.size( ); i++ ) {
			if ( grounds[ i ]->pos.x <= alive_player->getPos( ).x &&
				 grounds[ i ]->pos.x + grounds[ i ]->actual_size.x >= alive_player->getPos( ).x + alive_player->getSize( ).x &&
				 grounds[ i ]->pos.y >= alive_player->getPos( ).y + alive_player->getSize( ).y ) {
				if ( grounds[ i ]->pos.y < pos_y ) {
					pos_y = ( int )grounds[ i ]->pos.y;
				}
			}
		}

		std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
		for ( int i = 0; i < gimmicks.size( ); i++ ) {
			if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::RESPAWN && gimmicks[ i ]->getType( ) == Gimmick::TYPE::GOAL ) {
				continue;
			}
			if ( gimmicks[ i ]->getPos( ).x <= alive_player->getPos( ).x + alive_player->getSize( ).x &&
				 gimmicks[ i ]->getPos( ).x + gimmicks[ i ]->getSize( ).x >= alive_player->getPos( ).x &&
				 gimmicks[ i ]->getPos( ).y >= alive_player->getPos( ).y + alive_player->getSize( ).y ) {
				if ( gimmicks[ i ]->getPos( ).y < pos_y ) {
					pos_y = ( int )gimmicks[ i ]->getPos( ).y;
				}
			}
		}

		for ( int i = 0; i < players.size( ); i++ ) {
			if ( players[ i ]->getPos( ).x <= alive_player->getPos( ).x + alive_player->getSize( ).x &&
				 players[ i ]->getPos( ).x + players[ i ]->getSize( ).x >= alive_player->getPos( ).x &&
				 players[ i ]->getPos( ).y >= alive_player->getPos( ).y + alive_player->getSize( ).y &&
				 players[ i ] != alive_player ) {
				if ( players[ i ]->getPos( ).y < pos_y ) {
					pos_y = ( int )players[ i ]->getPos( ).y;
				}
			}
		}

		_shadow->draw( ( int )alive_player->getPos( ).x, pos_y - 10 );
	}
}

void Viewer::drawBetweenObject( ) const {
	Vector2 camera_pos = _camera->getPos( );
	DrawerPtr drawer = Task::get< Drawer >( );
	std::vector< MapData::BetweenObject* > between_objects = _map_data->getBetweenObjects( );
	for ( int i = 0; i < between_objects.size( ); i++ ) {
		MapData::BetweenObject* between_object = between_objects[ i ];
		for ( int i = 0; i < between_object->size.x; i += GROUND_IMAGE_SIZE ) {
			for ( int j = 0; j < between_object->size.y; j += GROUND_IMAGE_SIZE ) {
				int image_num = i / GROUND_IMAGE_SIZE + j / GROUND_IMAGE_SIZE;
				Vector2 adjust_pos = between_object->pos + Vector2( i, j ) - camera_pos;
				if ( j == 0 ) {
					if ( between_object->size.x - i < GROUND_IMAGE_SIZE && between_object->size.y - j < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, ( int )between_object->size.x - i, ( int )between_object->size.y - j );
					} else if ( between_object->size.x - i < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, ( int )between_object->size.x - i, GROUND_IMAGE_SIZE );
					} else if ( between_object->size.y - j < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, GROUND_IMAGE_SIZE, ( int )between_object->size.y - j );
					} else {
						_between_wall[ image_num % _between_wall.size( ) ]->draw( adjust_pos );
					}
				} else {
					if ( between_object->size.x - i < GROUND_IMAGE_SIZE && between_object->size.y - j < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, ( int )between_object->size.x - i, ( int )between_object->size.y - j );
					} else if ( between_object->size.x - i < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, ( int )between_object->size.x - i, GROUND_IMAGE_SIZE );
					} else if ( between_object->size.y - j < GROUND_IMAGE_SIZE ) {
						_between_wall[ image_num % _between_wall.size( ) ]->drawRect( adjust_pos, 0, 0, GROUND_IMAGE_SIZE, ( int )between_object->size.y - j );
					} else {
						_between_wall[ image_num % _between_wall.size( ) ]->draw( adjust_pos );
					}
				}

			}
		}
	}
}

void Viewer::animeHarmfulEnemyLeft( Vector2 pos ) {
	SystemPtr system = Task::get< System >( );
	if ( !system->isClear( ) ) {
		if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
			_enemy_wait_time++;
			_enemy_wait_time %= 5;
			if ( _enemy_wait_time == 0 ) {
				_enemy_image_num++;
			}
			if ( _enemy_image_num == 20 ) {
				_enemy_image_num = 0;
			}
		}
	}
	_harmful_enemy_move_left[ _enemy_image_num ]->draw( pos );
}

void Viewer::animeHarmfulEnemyRight( Vector2 pos ) {
	SystemPtr system = Task::get< System >( );
	if ( !system->isClear( ) ) {
		if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
			_enemy_wait_time++;
			_enemy_wait_time %= 5;
			if ( _enemy_wait_time == 0 ) {
				_enemy_image_num++;
			}
			if ( _enemy_image_num == 20 ) {
				_enemy_image_num = 0;
			}
		}
	}
	_harmful_enemy_move_right[ _enemy_image_num ]->draw( pos );
}

void Viewer::animeDeactivatedEnemyLeft( Vector2 pos ) {
	SystemPtr system = Task::get< System >( );
	if ( !system->isClear( ) ) {
		if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
			_enemy_wait_time++;
			_enemy_wait_time %= 5;
			if ( _enemy_wait_time == 0 ) {
				_enemy_image_num++;
			}
			if ( _enemy_image_num == 20 ) {
				_enemy_image_num = 0;
			}
		}
	}
	_deactivated_enemy_move_left[ _enemy_image_num ]->draw( pos );
}

void Viewer::animeDeactivatedEnemyRight( Vector2 pos ) {
	SystemPtr system = Task::get< System >( );
	if ( !system->isClear( ) ) {
		if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
			_enemy_wait_time++;
			_enemy_wait_time %= 5;
			if ( _enemy_wait_time == 0 ) {
				_enemy_image_num++;
			}
			if ( _enemy_image_num == 20 ) {
				_enemy_image_num = 0;
			}
		}
	}
	_deactivated_enemy_move_right[ _enemy_image_num ]->draw( pos );
}

void Viewer::animeEnemyModeChangeLeft( Vector2 pos, int enemy_size ) {
	SystemPtr system = Task::get< System >( );
	if ( !system->isClear( ) ) {
		if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
			_mode_change_wait_time++;
			_mode_change_wait_time %= 3;
			if ( _mode_change_wait_time == 0 ) {
				_mode_change_image_num[ enemy_size ]++;
			}
			if ( _mode_change_image_num[ enemy_size ] >= 9 ) {
				_mode_change_image_num[ enemy_size ] = 9;
			}
		}
	}
	_mode_change_left[ _mode_change_image_num[ enemy_size ] ]->draw( pos );
}

void Viewer::animeEnemyModeChangeRight( Vector2 pos, int enemy_size ) {
	SystemPtr system = Task::get< System >( );
	if ( !system->isClear( ) ) {
		if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
			_mode_change_wait_time++;
			_mode_change_wait_time %= 3;
			if ( _mode_change_wait_time == 0 ) {
				_mode_change_image_num[ enemy_size ]++;
			}
			if ( _mode_change_image_num[ enemy_size ] >= 9 ) {
				_mode_change_image_num[ enemy_size ] = 9;
			}
		}
	}
	_mode_change_right[ _mode_change_image_num[ enemy_size ] ]->draw( pos );
}

void Viewer::drawMenu( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );
	std::vector< PlayerPtr > players = _map_data->getPlayers( );
	Vector2 camera_pos = _camera->getPos( );

	int width = app->getScreenWidth( );
	int height = app->getScreenHeight( );
	int interval = 190;
	Vector2 retire_extend = Vector2( 60, 20 );//3:1
	Vector2 menu_extend = Vector2( 120, 20 );//6:1
	Vector2 operate_text_extend = Vector2( 98, 20 );//49:10
	if ( _menu->isMenu( ) ) {
		_ui_menu_base->draw( 0, 0 );
		_ui_menu->draw( width / 2 - 466, 100 );
		if ( _menu->isRetireMode( ) ) {
			_blue_window->drawExtend( width / 2 - 410, height / 2 - 200 + 50, width / 2 + 410, height / 2 - 80 + 50 );
			_ui_retire_confirm->draw( width / 2 - 350, height / 2 - 180 + 30 );
			if ( _menu->isRetireConfirm( ) ) {
				_blue_window->drawExtend( width / 4 - 410 / 2 - ( int )retire_extend.x, height / 2 + 100 - ( int )retire_extend.y,
										  width / 4 + 410 / 2 + ( int )retire_extend.x, height / 2 + 100 + 120 + ( int )retire_extend.y );
				_ui_yes->drawExtend( width / 4 - 360 / 2 - ( int )retire_extend.x, height / 2 + 100 - ( int )retire_extend.y,
									 width / 4 + 360 / 2 + ( int )retire_extend.x, height / 2 + 100 + 120 + ( int )retire_extend.y );
				_blue_window->drawExtend( width * 3 / 4 - 410 / 2, height / 2 + 100,
										  width * 3 / 4 + 410 / 2, height / 2 + 220 );
				_ui_no->draw( width * 3 / 4 - 360 / 2, height / 2 + 100 );
			} else {
				_blue_window->drawExtend( width / 4 - 410 / 2, height / 2 + 100,
										  width / 4 + 410 / 2, height / 2 + 220 );
				_ui_yes->draw( width / 4 - 360 / 2, height / 2 + 100 );
				_blue_window->drawExtend( width * 3 / 4 - 410 / 2 - ( int )retire_extend.x, height / 2 + 100 - ( int )retire_extend.y,
										  width * 3 / 4 + 410 / 2 + ( int )retire_extend.x, height / 2 + 100 + 120 + ( int )retire_extend.y );
				_ui_no->drawExtend( width * 3 / 4 - 360 / 2 - ( int )retire_extend.x, height / 2 + 100 - ( int )retire_extend.y,
									width * 3 / 4 + 360 / 2 + ( int )retire_extend.x, height / 2 + 100 + 120 + ( int )retire_extend.y );
			}
		} else {
			int add_pos_y = -180;
			if ( _menu->getSelectingButton( ) == 0 ) {
				_blue_window->drawExtend( width / 2 - 320 - menu_extend.x, height / 2 - 20 + interval * 0 - menu_extend.y + add_pos_y,
										  width / 2 + 320 + menu_extend.x, height / 2 + 100 + 20 + interval * 0 + menu_extend.y + add_pos_y );
				_ui_return_game->drawExtend( width / 2 - 300 - menu_extend.x, height / 2 + interval * 0 - menu_extend.y + add_pos_y,
											 width / 2 + 300 + menu_extend.x, height / 2 + 100 + interval * 0 + menu_extend.y + add_pos_y );
			} else {
				_blue_window->draw( width / 2 - 320, height / 2 + interval * 0 - 20 + add_pos_y );
				_ui_return_game->draw( width / 2 - 300, height / 2 + interval * 0 + add_pos_y );
			}
			if ( _menu->getSelectingButton( ) == 1 ) {
				_blue_window->drawExtend( width / 2 - 320 - menu_extend.x, height / 2 - 20 + interval * 1 - menu_extend.y + add_pos_y,
										  width / 2 + 320 + menu_extend.x, height / 2 + 100 + 20 + interval * 1 + menu_extend.y + add_pos_y );
				_ui_retire->drawExtend( width / 2 - 300 - menu_extend.x, height / 2 + interval * 1 - menu_extend.y + add_pos_y,
										width / 2 + 300 + menu_extend.x, height / 2 + 100 + interval * 1 + menu_extend.y + add_pos_y );
			} else {
				_blue_window->draw( width / 2 - 320, height / 2 + interval * 1 - 20 + add_pos_y );
				_ui_retire->draw( width / 2 - 300, height / 2 + interval * 1 + add_pos_y );
			}
			if ( _menu->getSelectingButton( ) == 2 ) {
				_blue_window->drawExtend( width / 2 - 320 - menu_extend.x, height / 2 - 20 + interval * 2 - menu_extend.y + add_pos_y,
										  width / 2 + 320 + menu_extend.x, height / 2 + 100 + 20 + interval * 2 + menu_extend.y + add_pos_y );
				_ui_operation->drawExtend( width / 2 - 300 - menu_extend.x, height / 2 + interval * 2 - menu_extend.y + add_pos_y,
										   width / 2 + 300 + menu_extend.x, height / 2 + 100 + interval * 2 + menu_extend.y + add_pos_y );
			} else {
				_blue_window->draw( width / 2 - 320, height / 2 + interval * 2 - 20 + add_pos_y );
				_ui_operation->draw( width / 2 - 300, height / 2 + interval * 2 + add_pos_y );
			}
			if ( _menu->getSelectingButton( ) == 3 ) {
				_blue_window->drawExtend( width / 2 - 320 - menu_extend.x, height / 2 - 20 + interval * 3 - menu_extend.y + add_pos_y,
										  width / 2 + 320 + menu_extend.x, height / 2 + 100 + 20 + interval * 3 + menu_extend.y + add_pos_y );
				_ui_credit->drawExtend( width / 2 - 300 - menu_extend.x, height / 2 + interval * 3 - menu_extend.y + add_pos_y,
										width / 2 + 300 + menu_extend.x, height / 2 + 100 + interval * 3 + menu_extend.y + add_pos_y );
			} else {
				_blue_window->draw( width / 2 - 320, height / 2 + interval * 3 - 20 + add_pos_y );
				_ui_credit->draw( width / 2 - 300, height / 2 + interval * 3 + add_pos_y );
			}
		}
		if ( _menu->isRetire( ) ) {
			_menu->menuClose( );
			if ( !_transition->isTransition( ) ) {
				_transition->setFadeOut( FAST_FADE_SPEED, Transition::COLOR::BLACK );
			}
		}
	}
}

void Viewer::drawTutorial( ) {
	StageManagerPtr stage_manager = Task::get< StageManager >( );
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	const Vector2 IMAGE_SIZE = Vector2( 768, 432 );
	int adjust_window = 20;
	if ( stage_manager->getStageNum( ) == 0 ) {
		_tutorial_scale -= 0.1;
		if ( _tutorial_scale < 0.0 ) {
			_tutorial_scale = 0.0;
		}
		int scale = ( IMAGE_SIZE.x / 2 ) * _tutorial_scale;
		drawer->setBlend( scope, Drawer::BLEND::ALPHA, 0.7 );
		_tutorial_window->drawExtend( 150 + scale - adjust_window, 70 + scale - adjust_window,
									  150 + IMAGE_SIZE.x - scale + adjust_window, 70 + IMAGE_SIZE.y - scale + adjust_window );
		drawer->setBlend( scope, Drawer::BLEND::ALPHA, 1 );
		_operation_method->drawExtend( 170 + scale, 70 + scale,
									   170 + IMAGE_SIZE.x - scale - 20, 70 + IMAGE_SIZE.y - scale );
	}

	const Vector2 IMAGE_SIZE2 = Vector2( 609, 82 );
	if ( stage_manager->getStageNum( ) == 2 ) {
		_tutorial_scale -= 0.1;
		if ( _tutorial_scale < 0.0 ) {
			_tutorial_scale = 0.0;
		}
		Vector2 adjust_window2 = Vector2( 60, 30 );

		int scale = ( IMAGE_SIZE2.x / 2 ) * _tutorial_scale;

		drawer->setBlend( scope, Drawer::BLEND::ALPHA, 0.7 );
		_tutorial_window->drawExtend( 170 + scale - adjust_window2.x, 400 + scale - adjust_window2.y,
									  170 + IMAGE_SIZE2.x - scale + adjust_window2.x, 400 + IMAGE_SIZE2.y - scale + adjust_window2.y );
		drawer->setBlend( scope, Drawer::BLEND::ALPHA, 1 );
		_operation_method2->drawExtend( 170 + scale, 400 + scale, 170 + IMAGE_SIZE2.x - scale, 400 + IMAGE_SIZE2.y - scale );
	}
}

void Viewer::drawEnding( ) const {
	StageManagerPtr stageManager = Task::get< StageManager >( );
	SaveDataPtr save_data = Task::get< SaveData >( );

	if ( _map_data->getStageName( stageManager->getStageNum( ) ) == "ending_stage" ) {
		_sky->draw( 0 - _camera->getPos( ).x, 0 - _camera->getPos( ).y );
		_ending_bg->draw( Vector2( 0, 0 ) - _camera->getPos( ) );
		_light->draw( 1920 - 540 - _camera->getPos( ).x, 576 - 270 - _camera->getPos( ).y );
		if ( save_data->isComplete( ) ) {
			_dead_mountain->drawRect( 2000 - _camera->getPos( ).x, 720 - _camera->getPos( ).y, 800, 0, 1000, 500 );
		} else {
			_dead_mountain->drawRect( 2000 - _camera->getPos( ).x, 1500 - _camera->getPos( ).y, 800, 0, 1000, 500 );
		}
		std::vector< PlayerPtr > players = _map_data->getPlayers( );
		for ( int i = 0; i < players.size( ); i++ ) {
			if ( players[ i ]->getState( ) == Player::STATE::ALIVE && players[ i ]->getPos( ).x == 500 ) {
				_exclamation->drawRotation( players[ i ]->getPos( ).x + players[ i ]->getSize( ).x + 50, players[ i ]->getPos( ).y, 0.2 );
			}
		}
	}
}

void Viewer::drawMovieLine( ) {
	StageManagerPtr stageManager = Task::get< StageManager >( );
	if ( _map_data->getStageName( stageManager->getStageNum( ) ) == "ending_stage" ) {
		DrawerPtr drawer = Task::get< Drawer >( );
		ApplicationPtr app = Application::getInstance( );
		drawer->drawBox( 0, 0, _movie_line_count, 100, Color( 0.0, 0.0, 0.0 ), true );
		drawer->drawBox( app->getScreenWidth( ) - _movie_line_count, app->getScreenHeight( ) - 100, app->getScreenWidth( ), app->getScreenHeight( ), Color( 0.0, 0.0, 0.0 ), true );
		_movie_line_count += 25;
	}
}

void Viewer::drawEndingText( ) {
	StageManagerPtr stageManager = Task::get< StageManager >( );
	ApplicationPtr app = Application::getInstance( );
	SystemPtr system = Task::get< System >( );
	SaveDataPtr save_data = Task::get<SaveData>( );

	if ( _map_data->getStageName( stageManager->getStageNum( ) ) == "ending_stage" ) {
		if ( !_transition->isScaling( ) ) {
			std::vector< PlayerPtr > players = _map_data->getPlayers( );
			for ( int i = 0; i < players.size( ); i++ ) {
				if ( players[ i ]->getState( ) == Player::STATE::DEAD ) {
					_ending_wait_count++;
					if ( system->isEndingMovieEnded( ) && _ending_wait_count >= 20 + 5 ) {
						if ( save_data->isComplete( ) ) {
							_end_text->draw( app->getScreenWidth( ) / 2 - 765 / 2, app->getScreenHeight( ) / 2 - 150 );
						} else {
							_continue_text->draw( app->getScreenWidth( ) / 2 - 1600 / 2, app->getScreenHeight( ) / 2 - 150 );
						}
						_switch_button_count++;
						if ( _switch_button_count >= 120 ) {
							_switch_count++;
							_switch_count %= 30;
							if ( _switch_count == 0 ) {
								_switch_image_num++;
							}
							if ( _switch_image_num > 1 ) {
								_switch_image_num = 0;
							}
							_button_switch[ _switch_image_num ]->draw( app->getScreenWidth( ) - 125 - 100, app->getScreenHeight( ) - 125 - 100 );
						}
					}
				}
			}
		}
	}
}

void Viewer::drawEndingGirl( ) {
	SaveDataPtr save_data = Task::get< SaveData >( );
	std::vector< PlayerPtr > players = _map_data->getPlayers( );
	for ( int i = 0; i < players.size( ); i++ ) {
		if ( players[ i ]->getState( ) == Player::STATE::DEAD && save_data->isComplete( ) ) {
			_true->draw( Vector2( 2050, 426 ) - _camera->getPos( ) - Vector2( 12, 0 ) );
		}
	}
}

void Viewer::animePlayerRight( Vector2 pos, bool move, bool jump, bool stand ) {
	if ( jump || !stand ) {
		animePlayerJumpRight( pos - Vector2( 0, 15 ) );
	} else {
		_jump_image_num = 0;
		if ( move ) {
			animePlayerWalkRight( pos );
		} else {
			animePlayerIdleRight( pos );
		}
	}
}

void Viewer::animePlayerLeft( Vector2 pos, bool move, bool jump, bool stand ) {
	if ( jump || !stand ) {
		animePlayerJumpLeft( pos - Vector2( 0, 15 ) );
	} else {
		_jump_image_num = 0;
		if ( move ) {
			animePlayerWalkLeft( pos );
		} else {
			animePlayerIdleLeft( pos );
		}
	}
}



void Viewer::drawWind( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::FAN ) {
			Collider::Wind* wind = _collider->getWind( std::dynamic_pointer_cast< Fan >( gimmicks[ i ] ) );
			if ( !std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->isOn( ) &&
				 std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->getOffTime( ) -
				 std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->getCount( ) > 40 ) {
				continue;
			}
			if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
				_wind_wait_time++;
				_wind_wait_time %= 8;
				if ( _wind_wait_time == 0 ) {
					_wind_image_num++;
				}
				if ( _wind_image_num == 24 ) {
					_wind_image_num = 0;
				}
			}
			Drawer::Scope scope = drawer->createScope( );
			drawer->setBlend( scope, Drawer::BLEND::ALPHA, 0.7 );
			if ( std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->getOffTime( ) -
				 std::dynamic_pointer_cast< Fan >( gimmicks[ i ] )->getCount( ) > 40 ) {
				switch ( gimmicks[ i ]->getDir( ) ) {
				case Gimmick::DIR::UP:
					_wind_up[ _wind_image_num ]->drawExtend( wind->edge1_stop_pos.x - _camera->getPos( ).x,
															 wind->edge1_stop_pos.y + 10 - _camera->getPos( ).y -
															 ( wind->center_start_pos.y - wind->edge1_stop_pos.y ) / 2,
															 wind->center_start_pos.x - _camera->getPos( ).x,
															 wind->center_start_pos.y - _camera->getPos( ).y );
					_wind_up[ _wind_image_num ]->drawExtend( wind->center_stop_pos.x - _camera->getPos( ).x,
															 wind->center_stop_pos.y + 10 - _camera->getPos( ).y -
															 ( wind->edge2_start_pos.y - wind->center_stop_pos.y ) / 2,
															 wind->edge2_start_pos.x - _camera->getPos( ).x,
															 wind->edge2_start_pos.y - _camera->getPos( ).y );
					break;
				case Gimmick::DIR::DOWN:
					_wind_down[ _wind_image_num ]->drawExtend( wind->center_start_pos.x - _camera->getPos( ).x,
															   wind->center_start_pos.y + 10 - _camera->getPos( ).y,
															   wind->edge1_stop_pos.x - _camera->getPos( ).x,
															   wind->edge1_stop_pos.y - _camera->getPos( ).y -
															   ( wind->edge1_stop_pos.y - wind->center_start_pos.y ) / 2 );
					_wind_down[ _wind_image_num ]->drawExtend( wind->center_start_pos.x - _camera->getPos( ).x,
															   wind->center_start_pos.y + 10 - _camera->getPos( ).y,
															   wind->edge2_stop_pos.x - _camera->getPos( ).x,
															   wind->edge2_stop_pos.y - _camera->getPos( ).y -
															   ( wind->edge2_stop_pos.y - wind->center_start_pos.y ) / 2 );
					break;
				case Gimmick::DIR::RIGHT:
					_wind_right[ _wind_image_num ]->drawExtend( wind->edge1_stop_pos.x - 10 - _camera->getPos( ).x -
																( wind->center_start_pos.x - wind->edge1_stop_pos.x ) / 2,
																wind->edge1_stop_pos.y + 10 - _camera->getPos( ).y,
																wind->center_start_pos.x - _camera->getPos( ).x,
																wind->center_start_pos.y - _camera->getPos( ).y );
					_wind_right[ _wind_image_num ]->drawExtend( wind->center_stop_pos.x - 10 - _camera->getPos( ).x -
																( wind->edge2_start_pos.x - wind->center_stop_pos.x ) / 2,
																wind->center_stop_pos.y + 10 - _camera->getPos( ).y,
																wind->edge2_start_pos.x - _camera->getPos( ).x,
																wind->edge2_start_pos.y - _camera->getPos( ).y );
					break;
				case Gimmick::DIR::LEFT:
					_wind_left[ _wind_image_num ]->drawExtend( wind->center_stop_pos.x - _camera->getPos( ).x -
															   ( wind->edge1_start_pos.x - wind->center_stop_pos.x ) / 2,
															   wind->center_stop_pos.y + 10 - _camera->getPos( ).y,
															   wind->edge1_start_pos.x - _camera->getPos( ).x,
															   wind->edge1_start_pos.y - _camera->getPos( ).y );
					_wind_left[ _wind_image_num ]->drawExtend( wind->center_stop_pos.x - _camera->getPos( ).x -
															   ( wind->edge2_start_pos.x - wind->center_stop_pos.x ) / 2,
															   wind->center_stop_pos.y + 10 - _camera->getPos( ).y,
															   wind->edge2_start_pos.x - _camera->getPos( ).x,
															   wind->edge2_start_pos.y - _camera->getPos( ).y );
					break;
				}
			} else {
				switch ( gimmicks[ i ]->getDir( ) ) {
				case Gimmick::DIR::UP:
					_wind_up[ _wind_image_num ]->drawExtend( wind->edge1_stop_pos.x - _camera->getPos( ).x,
															 wind->edge1_stop_pos.y + 10 - _camera->getPos( ).y,
															 wind->center_start_pos.x - _camera->getPos( ).x,
															 wind->center_start_pos.y - _camera->getPos( ).y );
					_wind_up[ _wind_image_num ]->drawExtend( wind->center_stop_pos.x - _camera->getPos( ).x,
															 wind->center_stop_pos.y + 10 - _camera->getPos( ).y,
															 wind->edge2_start_pos.x - _camera->getPos( ).x,
															 wind->edge2_start_pos.y - _camera->getPos( ).y );
					break;
				case Gimmick::DIR::DOWN:
					_wind_down[ _wind_image_num ]->drawExtend( wind->edge1_start_pos.x - _camera->getPos( ).x,
															   wind->edge1_start_pos.y + 10 - _camera->getPos( ).y,
															   wind->center_stop_pos.x - _camera->getPos( ).x,
															   wind->center_stop_pos.y - _camera->getPos( ).y );
					_wind_down[ _wind_image_num ]->drawExtend( wind->center_start_pos.x - _camera->getPos( ).x,
															   wind->center_start_pos.y + 10 - _camera->getPos( ).y,
															   wind->edge2_stop_pos.x - _camera->getPos( ).x,
															   wind->edge2_stop_pos.y - _camera->getPos( ).y );
					break;
				case Gimmick::DIR::RIGHT:
					_wind_right[ _wind_image_num ]->drawExtend( wind->edge1_stop_pos.x - 10 - _camera->getPos( ).x,
																wind->edge1_stop_pos.y + 10 - _camera->getPos( ).y,
																wind->center_start_pos.x - _camera->getPos( ).x,
																wind->center_start_pos.y - _camera->getPos( ).y );
					_wind_right[ _wind_image_num ]->drawExtend( wind->center_stop_pos.x - 10 - _camera->getPos( ).x,
																wind->center_stop_pos.y + 10 - _camera->getPos( ).y,
																wind->edge2_start_pos.x - _camera->getPos( ).x,
																wind->edge2_start_pos.y - _camera->getPos( ).y );
					break;
				case Gimmick::DIR::LEFT:
					_wind_left[ _wind_image_num ]->drawExtend( wind->center_stop_pos.x - _camera->getPos( ).x,
															   wind->center_stop_pos.y + 10 - _camera->getPos( ).y,
															   wind->edge1_start_pos.x - _camera->getPos( ).x,
															   wind->edge1_start_pos.y - _camera->getPos( ).y );
					_wind_left[ _wind_image_num ]->drawExtend( wind->center_stop_pos.x - _camera->getPos( ).x,
															   wind->center_stop_pos.y + 10 - _camera->getPos( ).y,
															   wind->edge2_start_pos.x - _camera->getPos( ).x,
															   wind->edge2_start_pos.y - _camera->getPos( ).y );
					break;
				}
			}
		}
	}
}

void Viewer::skipClear( ) {
	if ( _view_clear_count > CLEAR_BASE_COUNT[ 0 ] ) {
		_view_clear_count += 180;
	}
}

void Viewer::skipEndingButton( ) {
	ApplicationPtr app = Application::getInstance( );
	DrawerPtr drawer = Task::get< Drawer >( );
	SaveDataPtr save_data = Task::get<SaveData>( );
	SystemPtr system = Task::get< System >( );
	Drawer::Scope scope = drawer->createScope( );
	int text_width = 250;
	int button_size = 64;
	int blank = 64;
	int space = 3;
	int pos_x = app->getScreenWidth( ) - blank;
	int pos_y = app->getScreenHeight( ) - blank - space * 4;

	if ( !_controller->getPushA( ) && !_pushed_a ) {
		if ( _movie_line_count > app->getScreenWidth( ) / 4 ) {
			if ( save_data->isSecondCleared( STAGE_NUM - 1 ) && !system->isEndingMovieEnded( ) &&
				 !save_data->isComplete( ) ) {
				if ( _ending_wait_count <= 20 + 5 ) {
					_button_a_64px->draw( pos_x - ( text_width + button_size + space ), pos_y );
					_skip_text->draw( pos_x - ( text_width + space ), pos_y );
				}
			}
		}
	} else {
		_pushed_a = true;
	}
}

int Viewer::getViewClearCount( ) const {
	return _view_clear_count;
}

int Viewer::getEndingWaitCount( ) const {
	return _ending_wait_count;
}

void Viewer::drawGoal( Vector2 pos, Vector2 player_pos, Player::STATE player_state, Vector2 goal_size ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	SystemPtr system = Task::get< System >( );
	int open_pos_left_x = pos.x + ( goal_size.x / 2 ) - OPEN_X;
	int open_pos_right_x = pos.x + ( goal_size.x / 2 ) + OPEN_X;
	int open_pos_up = pos.y + ( goal_size.y / 2 ) - OPEN_Y;
	int open_pos_down = pos.y + ( goal_size.y / 2 ) + OPEN_Y;
	if ( !system->isClear( ) ) {
		//open
		if ( player_state == Player::STATE::ALIVE ) {
			if ( open_pos_left_x < player_pos.x &&
				 open_pos_right_x > player_pos.x &&
				 open_pos_up < player_pos.y &&
				 open_pos_down > player_pos.y ) {

				if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
					_goal_wait_time++;
					_goal_wait_time %= 4;
					if ( _goal_wait_time == 0 ) {
						_goal_image_num++;
					}
				}
				if ( _goal_image_num > 7 ) {
					_goal_image_num = 7;
				}
			} else {
				if ( _goal_image_num > 0 ) {
					_goal_wait_time++;
					_goal_wait_time %= 4;
					if ( _goal_wait_time == 0 ) {
						_goal_image_num--;
					}
				}
			}
		}
	}
	_goal_anime[ _goal_image_num ]->draw( pos );
}

void Viewer::drawBubbles( Vector2 pos, int acid_num, double acid_size, int amount_of_acid ) {
	switch ( _random_bubble_type[ acid_num ] ) {
	case 0:
		drawSpreadBubble( pos, acid_num, acid_size, amount_of_acid );
		break;
	case 1:
		drawWideBubble( pos, acid_num, acid_size, amount_of_acid );
		break;
	}
}

void Viewer::drawSpreadBubble( Vector2 pos, int acid_num, double acid_size, int amount_of_acid ) {
	if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
		_spread_bubble_wait_time++;
		_spread_bubble_wait_time %= 4 * amount_of_acid / 2;
		if ( _spread_bubble_wait_time == 0 ) {
			_spread_bubble_image_num++;
		}
	}
	if ( _spread_bubble_image_num == 13 ) {
		_random_bubble_type[ acid_num ] = Tool::getRand( BUBBLE_TYPE );
		_random_pos[ acid_num ] = Tool::getRand( acid_size - BUBBLE_SIZE_X );
		if ( _random_pos[ acid_num ] < 7 ) {
			_random_pos[ acid_num ] += 7;
		}
		_spread_bubble_image_num = 0;
	}
	_spread_bubble[ _spread_bubble_image_num ]->draw( pos + Vector2( _random_pos[ acid_num ], -50 ) );
}

void Viewer::drawWideBubble( Vector2 pos, int acid_num, double acid_size, int amount_of_acid ) {
	if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
		_wide_bubble_wait_time++;
		_wide_bubble_wait_time %= 8 * amount_of_acid / 2;
		if ( _wide_bubble_wait_time == 0 ) {
			_wide_bubble_image_num++;
		}
	}
	if ( _wide_bubble_image_num == 8 ) {
		_random_bubble_type[ acid_num ] = Tool::getRand( BUBBLE_TYPE );
		_random_pos[ acid_num ] = Tool::getRand( acid_size - BUBBLE_SIZE_X );
		if ( _random_pos[ acid_num ] < 7 ) {
			_random_pos[ acid_num ] += 7;
		}
		_wide_bubble_image_num = 0;
	}
	_wide_bubble[ _wide_bubble_image_num ]->draw( pos + Vector2( ( __int64 )_random_pos[ acid_num ] + 15, 0 ) );
}

void Viewer::drawRespawn( ) {
	SystemPtr system = Task::get< System >( );
	Vector2 camera_pos = _camera->getPos( );
	std::vector< GimmickPtr > gimmicks = _map_data->getGimmicks( );
	for ( int i = 0; i < gimmicks.size( ); i++ ) {
		if ( gimmicks[ i ]->getType( ) == Gimmick::TYPE::RESPAWN ) {
			if ( !system->isClear( ) ) {
				if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
					_respawn_wait_time++;
					_respawn_wait_time %= 2;
					if ( _respawn_wait_time == 0 ) {
						_respawn_num++;
					}
					if ( _respawn_num > 7 ) {
						_respawn_num = 7;
					}
					if ( _map_data->isSpawned( ) ) {
						if ( _respawn_num > 0 ) {
							_respawn_num--;

						}
					}
				}
			}
			_procession_dark->draw( gimmicks[ i ]->getPos( ) - camera_pos );
			_procession_player->draw( gimmicks[ i ]->getPos( ) - camera_pos );
			_respawn_anime[ _respawn_num ]->draw( gimmicks[ i ]->getPos( ) - camera_pos );
#if _DEBUG
			DrawerPtr drawer = Task::get< Drawer >( );
			drawer->drawBox( gimmicks[ i ]->getPos( ).x - camera_pos.x, gimmicks[ i ]->getPos( ).y - camera_pos.y,
							 gimmicks[ i ]->getPos( ).x + gimmicks[ i ]->getSize( ).x - camera_pos.x, gimmicks[ i ]->getPos( ).y + gimmicks[ i ]->getSize( ).y - camera_pos.y,
							 Color( 1.0, 0.0, 0.0 ), false );
#endif
		}
	}
}

void Viewer::animePlayerWalkRight( Vector2 pos ) {
	StageManagerPtr stage_manager = Task::get< StageManager >( );
	SystemPtr system = Task::get< System >( );
	if ( !system->isClear( ) ) {
		if ( ( !_menu->isMenu( ) && !_transition->isTransition( ) ) ||
			 _map_data->getStageName( stage_manager->getStageNum( ) ) == "ending_stage" ) {
			_player_walk_right_wait_time++;
			_player_walk_right_wait_time %= 2;
			if ( _player_walk_right_wait_time == 0 ) {
				_player_walk_right_num++;
			}
		}
		if ( _player_walk_right_num == 22 ) {
			_player_walk_right_num = 0;
		}
	}
	_player_walk_right[ _player_walk_right_num ]->draw( pos );
}

void Viewer::animePlayerWalkLeft( Vector2 pos ) {
	SystemPtr system = Task::get< System >( );
	if ( !system->isClear( ) ) {
		if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
			_player_walk_left_wait_time++;
			_player_walk_left_wait_time %= 2;
			if ( _player_walk_left_wait_time == 0 ) {
				_player_walk_left_num++;
			}
		}
		if ( _player_walk_left_num == 22 ) {
			_player_walk_left_num = 0;
		}
	}
	_player_walk_left[ _player_walk_left_num ]->draw( pos );
}

void Viewer::animePlayerIdleRight( Vector2 pos ) {
	if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
		_player_idle_wait_time++;
		_player_idle_wait_time %= 10;
		if ( _player_idle_wait_time == 0 ) {
			_player_idle_num++;
		}
	}
	if ( _player_idle_num == 15 ) {
		_player_idle_num = 0;
	}
	_player_idle_right[ _player_idle_num ]->draw( pos );
}

void Viewer::animePlayerIdleLeft( Vector2 pos ) {
	if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
		_player_idle_wait_time++;
		_player_idle_wait_time %= 10;
		if ( _player_idle_wait_time == 0 ) {
			_player_idle_num++;
		}
	}
	if ( _player_idle_num == 15 ) {
		_player_idle_num = 0;
	}
	_player_idle_left[ _player_idle_num ]->draw( pos );

}

void Viewer::animePlayerJumpRight( Vector2 pos ) {
	if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
		_jump_wait_time++;
		_jump_wait_time %= 4;
		if ( _jump_wait_time == 0 ) {
			_jump_image_num++;
		}
	}
	if ( _jump_image_num > 9 ) {
		_jump_image_num = 9;
	}
	_player_jump_right[ _jump_image_num ]->draw( pos );
}

void Viewer::animePlayerJumpLeft( Vector2 pos ) {
	if ( !_menu->isMenu( ) && !_transition->isTransition( ) ) {
		_jump_wait_time++;
		_jump_wait_time %= 4;
		if ( _jump_wait_time == 0 ) {
			_jump_image_num++;
		}
	}
	if ( _jump_image_num > 9 ) {
		_jump_image_num = 9;
	}
	_player_jump_left[ _jump_image_num ]->draw( pos );
}

