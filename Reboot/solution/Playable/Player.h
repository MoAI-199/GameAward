#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include <array>

PTR( Player );
PTR( MapData );
PTR( Controller );
PTR( Music );

class Player {
public:
	enum class STATE {
		ALIVE,
		DEAD
	};
	enum class DIR {
		RIGHT,
		LEFT
	};
	enum class DEAD_REASON {
		NONE,
		ACID,
		ENEMY,
		NEEDLE,
		STICKY_NET,
		SUICIDE,
	};
public:
	Player( Vector2 pos,ControllerPtr controller );
	virtual ~Player( );
public:
	void update( );
	void endingUpdate( );
private:
	void updatePos( );
	void gravity( );
	void move( );
	void jump( );
	void acidDamage( );
	void suicide( );
	//ending
	void endingMove( );
public:
	Vector2 getPos( ) const;
	Vector2 getVec( ) const;
	Vector2 getSize( ) const;
	int getHP( ) const;
	int getStickUsedCount( ) const;
	int getNowSoakedCount( ) const;
	int getSuicideCount( ) const;
	double getMaximumPosY( ) const;
	double getBeforeStandPosY( ) const;
	bool isMoving( ) const;
	bool isDeleted( ) const;
	bool isJumping( ) const;
	bool isDeadLie( ) const;
	bool isStanding( ) const;
	bool isCollapse( ) const;
	bool isInherited( ) const;
	bool isDieAdjusted( ) const;
	bool isDrawnDieEffdect( ) const;
	bool isExplosive( ) const;
	bool isCannotExplosive( ) const;
	DIR getDir( )const;
	STATE getState( ) const;
	DEAD_REASON getReason( ) const;
	int getAfterSuicideExistenceCount( ) const;
public:
	double& setGravityCount( );
	void addSoakedCount( );
	void resetSoakedCount( );
	void setInherited( );
	void setStanding( bool standing );
	void setDead( bool lie );
	void setDieAdjusted( );
	void setDrawnDieEffect( );
	void setDeadReason( DEAD_REASON reason );
	void setCollapse( );
	void setDelete( );
	void setMoving( bool moving );
	void setExplosive( bool explosive );
	void speedDeceleration( );
	Vector2& setPos( );
	Vector2& setVec( );
private:
	Vector2 _pos;
	Vector2 _vec;
	Vector2 _before_pos;
	Vector2 _size;
	int _num_of_jump;
	int _stick_used_count;
	int _jump_count;
	int _soaked_count;
	int _hp;
	int _move_speed;
	int _suicide_count;
	double _before_stand_pos_y;
	double _maximum_pos_y;
	double _jump_power;
	double _gravity_count;
	bool _inherited;
	bool _delete;
	bool _standing;
	bool _jumping;
	bool _die_adjusted;
	bool _moving;
	bool _collapse;
	bool _dead_lie;
	bool _drawn_die_effect;
	bool _explosive;
	bool _cannot_explosive;
	int _ending_move_stop_count;
	int _after_suicide_existence_count;
	STATE _state;
	DIR _dir;
	DEAD_REASON _reason;
	ControllerPtr _controller;
	MusicPtr _music;
};

