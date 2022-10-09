#include "Score.h"
#include "Define.h"
#include "SaveData.h"

Score::Score() {
	//ƒfƒX”‚É‰‚¶‚½Šl“¾‚Å‚«‚é¯‚Ì”‚ğ•\¦
	initCriterion( 0, 15, 7, 0 );
	initCriterion( 1, 15, 6, 3 );
	initCriterion( 2, 18, 8, 4 );
	initCriterion( 3, 18, 8, 4 );
	initCriterion( 4, 18, 8, 3 );
	initCriterion( 5, 30, 12, 8 );
	initCriterion( 6, 17, 9, 5 );
	initCriterion( 7, 23, 12, 7 );
	initCriterion( 8, 22, 10, 6 );
	initCriterion( 9, 18, 10, 6 );
	initCriterion( 10, 15, 8, 4 );
	initCriterion( 11, 15, 7, 3 );
	initCriterion( 12, 16, 7, 2 );
	initCriterion( 13, 15, 8, 3 );
	initCriterion( 14, 18, 9, 6 );
	initCriterion( 15, 17, 8, 3 );
	initCriterion( 16, 16, 8, 3 );
	initCriterion( STAGE_NUM, 0, 0, 0 );//STAGE_NUM
}

Score::~Score( ) {
}

int Score::getBestScore( int stage_num ) {
	SaveDataPtr save_data = Task::get< SaveData >( );
	int dead_player_num = save_data->getDeadPlayerNum( stage_num );
	int score = 0;
	if ( dead_player_num != -1 ) {
		if ( _criterions[ stage_num ]->first_criterion >= dead_player_num ) {
			score = 1;
		}
		if ( _criterions[ stage_num ]->second_criterion >= dead_player_num ) {
			score = 2;
		}
		if ( _criterions[ stage_num ]->third_criterion >= dead_player_num ) {
			score = 3;
		}
	}
	return score;
}

int Score::getScore( int stage_num, int dead_player_num ) {
	int score = 0;
	if ( dead_player_num != -1 ) {
		if ( _criterions[ stage_num ]->first_criterion >= dead_player_num ) {
			score = 1;
		}
		if ( _criterions[ stage_num ]->second_criterion >= dead_player_num ) {
			score = 2;
		}
		if ( _criterions[ stage_num ]->third_criterion >= dead_player_num ) {
			score = 3;
		}
	}
	return score;
}

void Score::initCriterion( int stage_num, int first_criterion, int second_criterion, int third_criterion ) {
	Criterion* criterion = new Criterion( );
	criterion->first_criterion = first_criterion;
	criterion->second_criterion = second_criterion;
	criterion->third_criterion = third_criterion;
	_criterions[ stage_num ] = criterion;
}
