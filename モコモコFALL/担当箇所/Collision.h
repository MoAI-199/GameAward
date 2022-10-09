#pragma once
#include "mathematics.h"
#include "smart_ptr.h"
#include <vector>

PTR( SheepsManager );
PTR( CloudsManager );
PTR( GimmickManager );
PTR( PropellerManager );
PTR( FundamentalGameSystem );

class Collision {
public:
	enum HIT_BAR {
		NONE = -1,
		TOP = 0,
		BOTTOM = 1,
		LEFT = 2,
		RIGHT = 3,
	};
	struct IdxList {
		int sheep1 = -1;
		int sheep2 = -1;
		int cloud = -1;
		int gimmick = -1;
		int wing1 = -1;
		int wing2 = -1;
		int wing3 = -1;
		bool shaft = false;
	};
public:
	Collision( SheepsManagerPtr sheep_manager,
			   CloudsManagerPtr cloud_manager,
			   GimmickManagerPtr gimmick_manager,
			   FundamentalGameSystemPtr fundamental_fame_system,
			   PropellerManagerPtr propeller_manager );
	virtual ~Collision( );

public:
	void update( );
	IdxList getIdxList( int idx ) const;
	int getIdxListMaxSize( ) const;
private:
	
	bool isHitCircleToCircle( Vector2 pos1, Vector2 pos2 ) const;
	bool isHitCircleToCircle( Vector2 pos1, Vector2 pos2, double size ) const;
	bool isOverlapCircleToCircle( Vector2 pos1, Vector2 pos2, double size ) const;
	bool isHitCircleToSquare( Vector2 circle_pos,
							  int circle_range,
							  Vector2 square_pos2,
							  int square_width,
							  int square_height ) const;
	void updateExecutionLevelUp( );
	void updatePlayerToPlayer( int idx );
	void updatePlayerToCloud( int idx );
	void updatePlayerToGimmick( int idx );
	void updatePlayerToPropeller( int idx );
	void resetIdxList( );

private:
	CloudsManagerPtr _clouds_manager;
	SheepsManagerPtr _sheeps_manager;
	GimmickManagerPtr _gimmick_manager;
	PropellerManagerPtr _propeller_manager;
	FundamentalGameSystemPtr _fundamental_game_system;
	
	std::vector< IdxList >_idx_list;

};
