#pragma once
#include "smart_ptr.h"
#include "Task.h"

PTR( System );

class System : public Task {
public:
	System( );
	virtual ~System( );
public:
	void setClear( );
	bool isClear( ) const;
	void resetClear( );
	void setRetired( );
	bool isRetired( ) const;
	void resetRetired( );
	bool isLoaded( ) const;
	void setLoaded( );
	void resetLoaded( );
	bool isEndingMovieEnded( ) const;
	void setEndingMovieEnded( );
	void resetEndingMovieEnded( );
	bool waitTime( int time );
private:
	bool _clear;
	bool _retired;
	bool _loaded;
	bool _ending_movie_ended;
	int _time;
};

