#include "SheepFactory.h"
#include "SheepsManager.h"
#include "Sheep.h"
#include "Tool.h"

static const double PLAYER_SIZE = 50.0;
static const double VIEW_SIZE = 1.0;

SheepFactory::SheepFactory( SheepsManagerPtr manager ) {
	_manager = manager;
}

SheepFactory::~SheepFactory( ) {
}

void SheepFactory::deploy( Vector2 pos, Vector2 vec ) {
	SheepPtr new_player = createSheep( pos, vec );
	_manager->addSheep( new_player );
}

SheepPtr SheepFactory::createSheep( Vector2 pos, Vector2 vec ) {
	SheepPtr player = SheepPtr( new Sheep( ) );
	Sheep::SheepStatus status;
	status.pos = pos;
	status.view_size = VIEW_SIZE;//‰½‚¾‚±‚ê‚ÍH
	double start_vec_x = vec.x;
	double start_vec_y = vec.y;
	status.vec = Vector2( start_vec_x, start_vec_y );

	//¶¬‚É”’‚ª‘O’ñ‚Ìˆ—
	for ( int i = 0; i < ( int )Sheep::COLOR_TYPE::MAX - 1; i++ ) {
		Sheep::COLOR_TYPE color_type = ( Sheep::COLOR_TYPE )i;
		status.color_storage[ color_type ] = false;
	}
	player->setStatus( status );
	return player;
}
