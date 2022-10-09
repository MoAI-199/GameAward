#pragma once
#include "Task.h"
#include <array>
#include "smart_ptr.h"
#include "Define.h"

PTR( SaveData );

class SaveData : public Task {
public:
	SaveData( );
	virtual ~SaveData( );
public:
	void save( );
	void load( );
	void reset( );
public:
	int getDegreeOfProgress( ) const;
	int getLastClearStage( ) const;
	int getLastPlayStage( ) const;
	int getDeadPlayerNum( int stage_num ) const;
	bool isSecondCleared( int stage_num ) const;
	void setAll( int stage_num, int dead_player_num );
	void setDataNum( int data_num );
	void setDegreeOfProgress( int stage_num );
	void setLastClearStage( int stage_num );
	void setLastPlayStage( int stage_num );
	void setDeadPlayerNum( int stage_num, int dead_player_num );
	bool isComplete( ) const;
private:
	int _degree_of_progress;
	int _last_clear_stage;
	int _last_play_stage;
	int _data_num;
	std::array< int, STAGE_NUM > _dead_player_num;
	std::array< bool, STAGE_NUM > _second_cleared;
};

