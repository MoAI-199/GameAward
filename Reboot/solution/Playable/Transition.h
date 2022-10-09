#pragma once
#include "smart_ptr.h"

PTR( Image );

class Transition {
public:
	Transition( );
	virtual ~Transition( );
public:
	void initialize( );
	void update( );
public:
	enum class TYPE {
		IRIS,
		FADE
	};

	enum class IO {
		IN,
		OUT
	};

	enum class COLOR {
		NONE,
		BLACK,
		WHITE
	};

	bool isTransition( );
	bool isScaling( );
	bool isStop( );
	void setIrisIn( int sx, int sy, int camera_x, int camera_y, double speed, int stop_time, int circle_ratio );
	void setIrisOut( int sx, int sy, int camera_x, int camera_y, double speed, int stop_time, int circle_ratio );
	void setFadeIn( double speed, COLOR color );
	void setFadeOut( double speed, COLOR color );

private:
	void drawIris( );
	void drawFade( );
	void drawFinishedTransition( );
private:
	ImagePtr _iris;
	bool _transition;
	bool _scaling;
	bool _finished;
	bool _stop;
	int _target_sx;
	int _target_sy;
	int _cx;
	int _cy;
	double _ratio;
	double _scale;
	IO _io;
	COLOR _color;
	TYPE _type;
	double _speed;
	int _stop_time;
	int _circle_ratio;
};

