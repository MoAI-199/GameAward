#pragma once
#include "mathematics.h"
#include "smart_ptr.h"

PTR( Collision );
PTR( SheepsManager );
PTR( CloudsManager );
PTR( PropellerManager );
PTR( SheepFactory );
PTR( FundamentalGameSystem );


class SheepEvent {
public:
	SheepEvent( CollisionPtr collision,
				SheepsManagerPtr players_manager,
				CloudsManagerPtr clouds_manager,
				SheepFactoryPtr sheep_factory,
				FundamentalGameSystemPtr allsystem_game_system,
				PropellerManagerPtr propeller_manager );
	~SheepEvent( );
	void update( );
	void setDeployInterval( int interval );
private:
	void updateExecutionLevelUp( );
	void updateCollision( );
	void updateMove( );
	void reflectiveToCloud( int player_idx, int cloud_idx );
	void reflectiveToPlayer( int player1_idx, int player2_idx );
	void reflectiveToWing1( int player_idx, int wing_idx );
	void reflectiveToWing2( int player_idx, int wing_idx );
	void reflectiveToWing3( int player_idx, int wing_idx );
	void reflectiveToShaft( int player_idx );
	void deployRandomPos( );
	void initCreate( );
	bool isInto( Vector2 vec1, Vector2 vec2 );
	bool isDebugMode( );
private:
	CollisionPtr _collision;
	SheepsManagerPtr _sheeps_manager;
	CloudsManagerPtr _clouds_manager;
	PropellerManagerPtr _propeller_manager;
	SheepFactoryPtr _sheep_factory;
	FundamentalGameSystemPtr _game_system;
private:
	int _deploy_interval;
	int _before_deploy_time;
};
