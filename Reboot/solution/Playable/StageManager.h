#pragma once
#include "Task.h"
#include "smart_ptr.h"

PTR( StageManager );

class StageManager : public Task {
public:
	StageManager( );
	virtual ~StageManager( );
public:
	void setStageNum( int stage_num );
public:
	int getStageNum( ) const;
private:
	int _stage_num;
};

