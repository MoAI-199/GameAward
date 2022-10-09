#include "StageManager.h"

StageManager::StageManager(  ) {
	_stage_num = 0;
}

StageManager::~StageManager( ) {

}

void StageManager::setStageNum( int stage_num ) {
	_stage_num = stage_num;
}

int StageManager::getStageNum( ) const {
	return _stage_num;
}
