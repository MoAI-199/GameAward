#pragma once
#include "mathematics.h"
#include <map>

static const double MAX_SPEED = 7;


class Sheep {
public:
	Sheep( );
	virtual ~Sheep( );
public:
	enum class COLOR_TYPE {
		RED,
		BULE,
		GREEN,
		YELLOW,
		ORANGE,
		PURPLE,
		SECRET,
		MAX,
		NONE,
	};

	struct SheepStatus {
		Vector2 pos;
		Vector2 vec;
		double scale = 1.0;
		double angle = 0.0;
		double view_size = 0.0;
		COLOR_TYPE color_type = COLOR_TYPE::NONE;
		std::map< COLOR_TYPE, bool > color_storage;
	};

public:
	void update( );
	void setStatus( SheepStatus status );
public:
	Vector2 getPos( ) const;
	Vector2 getVec( ) const;
	double getSize( ) const;
	double getAngle( ) const;
	double getViewSize( ) const;
	double getScale( ) const;
	COLOR_TYPE getColorType( ) const;
	bool isColorStrage( COLOR_TYPE type ) const;

	void setScale( double scale );
	void setPos( Vector2 pos );
	void setVec( Vector2 vec );
	void setColorType( COLOR_TYPE type );
	void setViewSize( double size );
private:
	SheepStatus _status;
};

