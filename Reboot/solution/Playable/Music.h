#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>

PTR( Music );
PTR( Player );
PTR( Enemy );
PTR( Gimmick );
PTR( Sound );

class Music : public Task {
public:
	Music( );
	virtual ~Music( );
public:
	void jumpSE( ) const;
	void walkSE( );
	void stopBGM( ) const;
	void decisionSE( ) const;
	void cursorSE( ) const;
	void cancelSE( ) const;
	void birdSE( ) const;
	void stopBirdSE( ) const;
	void deadSE( ) const;
	void openSE( ) const;
	void onSE( ) const;
	void electroSE( ) const;
	void playBGM( std::string bgm_name, std::string extension, int volume ) const;
	void starGetSE( ) const;
	void clearSE( ) const;
	void stopClearSE( ) const;
	void exclamationSE( ) const;
private:
	int _wait_time;
	SoundPtr _bgm;
};

