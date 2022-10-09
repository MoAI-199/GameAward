#include "SaveData.h"
#include <direct.h>
#include "Application.h"
#include "Binary.h"
#include "Exception.h"
#include "Score.h"

const std::string DIRECTORY_DATA_SAVE = "Resource/SaveData/";
const std::string EXTENSION = ".dat";

SaveData::SaveData( ) {
	for ( int i = 0; i < _dead_player_num.size( ); i++ ) {
		_dead_player_num[ i ] = -1;
		_second_cleared[ i ] = false;
	}
	_degree_of_progress = -1;
	//_degree_of_progress = 20;
	_last_clear_stage = 0;
	_last_play_stage = 0;
	_data_num = 0;
}

SaveData::~SaveData( ) {
}

void SaveData::save( ) {
	int error = _mkdir( DIRECTORY_DATA_SAVE.c_str( ) );
	std::string file_name = "save_data";
	switch ( _data_num ) {
	case 0:
		file_name += "00";
		break;
	case 1:
		file_name += "01";
		break;
	case 2:
		file_name += "02";
		break;
	default:
		file_name += "00";
		break;
	}
	file_name = DIRECTORY_DATA_SAVE + file_name;
	file_name += EXTENSION;
	BinaryPtr binary( new Binary );
	int size = ( int )( sizeof( int ) * 3 + ( sizeof( int ) + sizeof( bool ) ) * _dead_player_num.size( ) );
	binary->ensure( size );
	binary->write( ( void* )&size, sizeof( int ) );
	binary->write( ( void* )&_degree_of_progress, sizeof( int ) );
	binary->write( ( void* )&_last_clear_stage, sizeof( int ) );
	for ( int i = 0; i < _dead_player_num.size( ); i++ ) {
		binary->write( ( void* )&_dead_player_num[ i ], sizeof( int ) );
		binary->write( ( void* )&_second_cleared[ i ], sizeof( bool ) );
	}
	ApplicationPtr app( Application::getInstance( ) );
	app->saveBinary( file_name, binary );

	struct stat buf;
	if ( stat( file_name.c_str( ), &buf ) != 0 ) {
#ifdef _DEBUG
		EXCEPTION( true, "ERROR[save_failed] fail saved file" );
#endif
	}
}

void SaveData::load( ) {
	std::string file_name = "save_data";
	switch ( _data_num ) {
	case 0:
		file_name += "00";
		break;
	case 1:
		file_name += "01";
		break;
	case 2:
		file_name += "02";
		break;
	default:
		file_name += "00";
		break;
	}

	file_name = DIRECTORY_DATA_SAVE + file_name;
	file_name += EXTENSION;

	BinaryPtr binary( new Binary );

	ApplicationPtr app( Application::getInstance( ) );
	if ( !app->loadBinary( file_name, binary ) ) {
#ifdef _DEBUG
		//EXCEPTION( !binary, "ERROR[file::load] fail loaded file" );
#endif
		return;
	}

	int size = 0;
	binary->read( ( void* )&size, sizeof( int ) );

	binary->read( ( void* )&_degree_of_progress, sizeof( int ) );
	binary->read( ( void* )&_last_clear_stage, sizeof( int ) );
	for ( int i = 0; i < ( size - 3 * sizeof( int ) ) / ( sizeof( int ) + sizeof( bool ) ); i++ ) {
		if ( i >= _dead_player_num.size( ) ) {
			break;
		}
		binary->read( ( void* )&_dead_player_num[ i ], sizeof( int ) );
		binary->read( ( void* )&_second_cleared[ i ], sizeof( bool ) );
	}
	_last_play_stage = _last_clear_stage;
}

void SaveData::reset( ) {
	for ( int i = 0; i < _dead_player_num.size( ); i++ ) {
		_dead_player_num[ i ] = -1;
		_second_cleared[ i ] = false;
	}
	_degree_of_progress = -1;
	_last_clear_stage = 0;
	_last_play_stage = 0;
}

int SaveData::getDegreeOfProgress( ) const {
	return _degree_of_progress;
}

int SaveData::getLastClearStage( ) const {
	return _last_clear_stage;
}

int SaveData::getLastPlayStage( ) const {
	return _last_play_stage;
}

int SaveData::getDeadPlayerNum( int stage_num ) const {
	if ( stage_num >= _dead_player_num.size( ) ) {
		return -1;
	}
	return _dead_player_num[ stage_num ];
}

bool SaveData::isSecondCleared( int stage_num ) const {
	return _second_cleared[ stage_num ];
}

void SaveData::setAll( int stage_num, int dead_player_num ) {
	if ( _dead_player_num[ stage_num ] != -1 ) {
		_second_cleared[ stage_num ] = true;
	}
	setDegreeOfProgress( stage_num );
	setLastClearStage( stage_num );
	setDeadPlayerNum( stage_num, dead_player_num );
}

void SaveData::setDataNum( int data_num ) {
	_data_num = data_num;
}

void SaveData::setDegreeOfProgress( int stage_num ) {
	if ( _degree_of_progress < stage_num ) {
		_degree_of_progress = stage_num;
	}
}

void SaveData::setLastClearStage( int stage_num ) {
	_last_clear_stage = stage_num;
	_last_play_stage = _last_clear_stage;
}

void SaveData::setLastPlayStage( int stage_num ) {
	_last_play_stage = stage_num;
}

void SaveData::setDeadPlayerNum( int stage_num, int dead_player_num ) {
	if ( _dead_player_num[ stage_num ] > dead_player_num || _dead_player_num[ stage_num ] == -1 ) {
		_dead_player_num[ stage_num ] = dead_player_num;
	}
}

bool SaveData::isComplete( ) const {
	ScorePtr score = Task::get< Score >( );
	bool complete = true;
	for ( int i = 0; i < STAGE_NUM; i++ ) {
		if (  score->getBestScore( i ) != 3 ) {
			complete = false;
		}
	}
	return complete;
}
