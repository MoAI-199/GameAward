#pragma once
#include "mathematics.h"
#include <vector>
#include "smart_ptr.h"

PTR( DeviceController );

class PropellerManager {
public:
	PropellerManager( DeviceControllerPtr device_controller );
	~PropellerManager( );

public:
	void update( );

public:
	struct Wing {
		Vector2 pos;
		double size = 10.0;
	};
	struct Status {
		Vector2 shaft_pos;
		double shaft_size = 20.0;
		std::vector< Wing > wing1;
		std::vector< Wing > wing2;
		std::vector< Wing > wing3;
	};

	Status getStatus( ) const;
	double getRotateAccel( ) const;
private:
	void rotateWings( );
	void move( );
private:
	int _before_pushed_time;
	double _rotate_accel;
	Status _status;
	DeviceControllerPtr _device;
};
