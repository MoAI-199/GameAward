#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include "Define.h"
#include <array>

PTR( Score );

class Score : public Task {
public:
	struct Criterion {
		int first_criterion;
		int second_criterion;
		int third_criterion;
	};
public:
	Score( );
	virtual ~Score( );
	int getBestScore( int stage_num );
	int getScore( int stage_num, int dead_player_num );
private:
	void initCriterion( int stage_num, int first_criterion, int second_criterion, int third_criterion );
private:
	std::array< Criterion*, STAGE_NUM+1 > _criterions;
};

