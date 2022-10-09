#pragma once
#include "Gimmick.h"

class ColorBall : public Gimmick {
public:
	enum class COLOR_TYPE {
		RED,
		BULE,
		GREEN,
		YELLO,
		ORANGE,
		PURPLE,
		NONE,
	};
public:
	ColorBall( );
	~ColorBall( );
public:
	void setColorType( COLOR_TYPE type );
	COLOR_TYPE getColorType( ) const;
private:
	COLOR_TYPE _color_type;
};

