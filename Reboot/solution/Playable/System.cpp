#include "System.h"
#include "Controller.h"

System::System( ) {
	_clear = false;
	_retired = false;
	_loaded = false;
	_ending_movie_ended = false;
	_time = 0;
}

System::~System( ) {

}

void System::setClear( ) {
	_clear = true;
}

bool System::isClear( ) const {
	return _clear;
}

void System::resetClear( ) {
	_clear = false;
}

void System::setRetired( ) {
	_retired = true;
}

bool System::isRetired( ) const {
	return _retired;
}

void System::resetRetired( ) {
	_retired = false;
}

bool System::isLoaded( ) const {
	return _loaded;
}

void System::setLoaded( ) {
	_loaded = true;
}

void System::resetLoaded( ) {
	_loaded = false;
}

bool System::isEndingMovieEnded( ) const {
	return _ending_movie_ended;
}

void System::setEndingMovieEnded( ) {
	_ending_movie_ended = true;
}

void System::resetEndingMovieEnded( ) {
	_ending_movie_ended = false;
}

bool System::waitTime( int time ) {
	if ( _time == 0 ) {
		_time = time;
	}
	_time--;
	if ( _time <= 0 ) {
		_time = 0;
		return true;
	}
	return false;
}
