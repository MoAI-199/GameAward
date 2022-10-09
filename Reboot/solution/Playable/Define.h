#pragma once
//player
static const int PLAYER_SIZE_X = 80;
static const int PLAYER_SIZE_Y = 150;
//static const int PLAYER_LIE_SIZE_X = 150;
//static const int PLAYER_LIE_SIZE_Y = 80;
static const int PLAYER_LIE_SIZE_X = 140;
static const int PLAYER_LIE_SIZE_Y = 78;
static const int PLAYER_HP = 300;
static const int PLAYER_MINIMAM_SPEED = 2;
static const int RESPAWN_TIME = 60;
static const int DEAD_DOWNWARD_HEIGHT = 225;
static const int SUICIDE_MAX_COUNT = 60;
//enemy
static const int ENEMY_SPEED_X = 0;
static const int ENEMY_SPEED_Y = 0;
static const int ENEMY_DELETE_POS = -500;
static const Vector2 ENEMY_RIGING = Vector2( 0.0, -10.0 );

//image_size
static const Vector2 GOAL_SIZE = Vector2( 200.0, 256.0 );
static const Vector2 RESPAWN_SIZE = Vector2( 207, 200 );
static const Vector2 NEEDLE_SIZE = Vector2( 64.0, 80.0 );
static const Vector2 FAN_SIZE = Vector2( 160.0, 80.0 );
static const Vector2 ENEMY_SIZE = Vector2( 130.0, 100.0 );
static const Vector2 DECELERATE_NET_SIZE = Vector2( 80.0, 80.0 );
static const Vector2 STICKY_NET_SIZE = Vector2( 100.0, 100.0 );
static const int NUMBER_IMAGE_SIZE = 128;
//scene_change
static const int IRIS_SCALE_LIMIT = 384;
static const int DEFAULT_IRIS_SPEED = 10;
static const double DEFAULT_FADE_SPEED = 0.01;
static const double FAST_FADE_SPEED = 0.05;
//stage_data
static const int NUMBER_OF_STAGES_PER_STRATA = 4;
static const int STAGE_NUM = 16;
//acid
static const int BUBBLE_TYPE = 2;
static const int BUBBLE_SIZE_X = 152;
//bg
static const double BG_BRIGHTNESS = 1.0;
//
static const int ADJUST_GROUND_Y = 22;

static const int OPEN_X = 400;
static const int OPEN_Y = 300;

static const int PLAYER_DIE_EFFECT_IMAGE_NUM = 14;//1:12,2:11,3:14

static const int SELECT_ICON_DOOR_WAIT_TIME = 2;

static const int MAX_SUICIDER_NUM = 8;

static const int NEEDLE_COLLIDE_AJDUST = 10;

const int CLEAR_BASE_COUNT[ 4 ] = { 15, 40, 55, 150 };

