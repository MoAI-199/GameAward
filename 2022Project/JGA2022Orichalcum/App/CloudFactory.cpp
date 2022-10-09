#include "CloudFactory.h"
#include "define.h"
#include "Cloud.h"
#include "CloudsManager.h"

CloudFactory::CloudFactory( CloudsManagerPtr clouds_controller ) :
	_manager( clouds_controller ) {
}

CloudFactory::~CloudFactory( ) {
}

CloudPtr CloudFactory::create( Vector2 pos, double scale ) {
	return CloudPtr( new Cloud( pos, scale ) );
}

void CloudFactory::add( CloudPtr cloud ) {
	_manager->addList( cloud );
}

void CloudFactory::addDeploy( ) {
	//‰E’[
	int end_idx = _manager->getMaxNum( ) - 1;
	double pos_x;
	if ( end_idx < 0 ) {
		pos_x = 0.0;
	} else {
		pos_x = _manager->getPos( end_idx ).x + ( double )CLOUD_RADIUS_SIZE * 2;
	}

	CloudPtr cloud = create( Vector2( pos_x, 300.0 ), 1.0 );
	add( cloud );
}
