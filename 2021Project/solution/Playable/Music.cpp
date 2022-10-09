#include "Music.h"
#include "Sound.h"
#include "MapData.h"
#include "Controller.h"
#include "Menu.h"
#include <vector>
#include "StageManager.h"

Music::Music( ) {
}

Music::~Music( ) {
}

void Music::playBGM( std::string bgm_name, std::string extension, int volume ) const {
	std::string pass = "Resource/sound/BGM/";
	pass = pass + bgm_name + "." + extension;
	SoundPtr sound = Task::get< Sound >( );
	sound->playBGM( pass, true );
	sound->setVolumeBGM( ( double )volume / 100 );
}

void Music::stopBGM( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->stopBGM( );
}

void Music::decisionSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/decision_1.mp3" );
}

void Music::cursorSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/cursor_0.mp3" );
}

void Music::cancelSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/cancel_0.mp3" );
}

void Music::birdSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/bird_1.mp3" );
}

void Music::stopBirdSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->stopSE( "Resource/sound/SE/bird_1.mp3" );
}

void Music::deadSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/dead_1.mp3" );
}

void Music::openSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/open_02.mp3", false );
}

void Music::onSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/on_02.mp3", false );
}

void Music::electroSE( ) const {
	std::string path = "Resource/sound/SE/batibati_02.mp3";
	SoundPtr sound = Task::get< Sound >( );
	if ( !sound->isPlayingSE( path ) ) {
		sound->playSE( path, false );
	}
}

void Music::jumpSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/jump_36_3.mp3", false );
}

void Music::walkSE( ) {
	SoundPtr sound = Task::get< Sound >( );
	_wait_time++;
	_wait_time %= 22;
	if ( _wait_time == 5 ) {
		//sound->playSE( "Resource/sound/SE/walk_03.mp3", false );
	} else {
		_wait_time = 0;
	}
}

void Music::starGetSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/star19.mp3", false );
}

void Music::clearSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/YO_RO_KO_BI_2.mp3", false );
}

void Music::stopClearSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->stopSE( "Resource/sound/SE/YO_RO_KO_BI_2.mp3" );
}

void Music::exclamationSE( ) const {
	SoundPtr sound = Task::get< Sound >( );
	sound->playSE( "Resource/sound/SE/flashing.mp3", false );//coin01 or flashing
}


