#include "GimmickFactory.h"
#include "GimmickManager.h"
#include "Coin.h"
#include "ColorBall.h"
#include "Carrot.h"
#include "Gimmick.h"
#include "Exception.h"
#include "Hole.h"


GimmickFactory::GimmickFactory( GimmickManagerPtr manager ) :
	_manager( manager ) {
}

GimmickFactory::~GimmickFactory( ) {
}

void GimmickFactory::deploy( Vector2 pos, Gimmick::TYPE type ) {
	EXCEPTION( type == Gimmick::TYPE::COLOR_BALL, "gimmick deploy type error" );
	GimmickPtr gimmick;
	switch ( type ) {
	case Gimmick::TYPE::COIN:
		gimmick = createCoin( pos );
		break;
	case Gimmick::TYPE::HOLE:
		gimmick = createHole( pos );
		break;

	case Gimmick::TYPE::CARROT:
		gimmick = createCarrot( pos );
		break;
	}
	_manager->addGimmick( gimmick );
}

void GimmickFactory::deploy( Vector2 pos, Gimmick::TYPE type, ColorBall::COLOR_TYPE color_type ) {
	GimmickPtr gimmick;
	switch ( type ) {
	case Gimmick::TYPE::COLOR_BALL:
		gimmick = createColor( pos, color_type );
		break;
	}
	_manager->addGimmick( gimmick );
}

GimmickPtr GimmickFactory::createCoin( Vector2 pos ) const {
	GimmickPtr coin = CoinPtr( new Coin );
	coin->setType( Gimmick::TYPE::COIN );
	coin->setPos( pos );
	coin->setSize( 50.0 );
	return coin;
}

GimmickPtr GimmickFactory::createColor( Vector2 pos, ColorBall::COLOR_TYPE type ) const {
	GimmickPtr color_ball = ColorBallPtr( new ColorBall );
	color_ball->setType( Gimmick::TYPE::COLOR_BALL );
	color_ball->setPos( pos );
	color_ball->setSize( 50.0 );
	std::dynamic_pointer_cast< ColorBall >( color_ball )->setColorType( type );
	return color_ball;
}

GimmickPtr GimmickFactory::createHole( Vector2 pos ) const {
	GimmickPtr hole = HolePtr( new Hole );
	hole->setType( Gimmick::TYPE::HOLE );
	hole->setPos( pos );
	hole->setSize( 60.0 );
	return hole;
}

GimmickPtr GimmickFactory::createCarrot( Vector2 pos ) const {
	GimmickPtr carrot = CarrotPtr( new Carrot );
	carrot->setType( Gimmick::TYPE::CARROT );
	carrot->setPos( pos );
	carrot->setSize( 30 );
	return carrot;
}
