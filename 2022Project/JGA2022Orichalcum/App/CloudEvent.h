#pragma once
#include "mathematics.h"
#include "smart_ptr.h"
#include <vector>

PTR( CloudFactory );
PTR( CloudsManager );
PTR( DeviceController );
PTR( FundamentalGameSystem );

class CloudEvent {
public:
	CloudEvent( CloudFactoryPtr cloud_factory,
				CloudsManagerPtr clouds_manager,
				DeviceControllerPtr device_controller,
				FundamentalGameSystemPtr always_game_system );
	~CloudEvent( );
public:
	void update( );
private:
	enum class MOVE_GRADE {
		BEGINNER,
		NOVICE,
		DIFFICULT,
	};
	
private:
	void updateExecutionLevelUp( );
	void updateMove( );
	void updateForm( );
	void placementFirstCircle( );
	void placementFirstSquare( );
	void placementNew( );
	void makeHole( int array_size, std::vector< int > ary );
	void moveGrade( MOVE_GRADE move_grade );
	std::vector< Vector2 > placementCircle( );
	std::vector< Vector2 > placementSquare( );
	void creates( int num );

	void moveFormCircle( );
	void moveFormSquare( );
	void moveAnimation( );
	void switchFormFlag( );
	void switchForm( );
	bool isAnimationEnd( );
private:
	bool _form_switch;
private:
	int _form_count_timer;
	int _start_millisecont;
	double _speed;
	bool _animation;
	bool _form_circle;
	bool _form_square;
	Vector2 _base_pos;
	CloudFactoryPtr _cloud_factory;
	CloudsManagerPtr _clouds_manager;
	FundamentalGameSystemPtr _game_system;
	DeviceControllerPtr _device_controller;
	std::vector<Vector2>_vec;
	std::vector<Vector2> _moved_pos;
};
