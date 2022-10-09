#include "SceneResult.h"
#include "Drawer.h"

SCENE( SceneResult );

SceneResult::SceneResult( ) : 
	SCREEN_WIDTH( Application::getInstance( )->getScreenWidth( ) ),
	SCREEN_HEIGHT( Application::getInstance( )->getScreenHeight( ) ) {
	DrawerPtr drawer = Task::get<Drawer>( );
	_gauge = drawer->createImage( "resource/gauge.png" );
	_result_board = drawer->createImage( "resource/result_board.png" );
	_word1 = drawer->createImage( "resource/word1.png" );
}

SceneResult::~SceneResult( ) {
}

void SceneResult::update( ) {
	_result_board->drawExtend( Vector2( SCREEN_WIDTH / 10, SCREEN_HEIGHT / 10 ),
							   Vector2( SCREEN_WIDTH / 1.1, SCREEN_HEIGHT / 1.1 ) );
	_gauge->drawExtend( Vector2( SCREEN_WIDTH / 5.8, SCREEN_HEIGHT - SCREEN_HEIGHT / 4 ),
						Vector2( SCREEN_WIDTH / 1.2, SCREEN_HEIGHT / 1.2 ) );
	_word1->drawExtend( Vector2( SCREEN_WIDTH / 5, SCREEN_HEIGHT / 5 ), Vector2( SCREEN_WIDTH / 1.5, SCREEN_HEIGHT / 3 ) );
}
