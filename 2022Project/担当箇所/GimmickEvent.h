#pragma once
#include "Collision.h"
#include "smart_ptr.h"

PTR( Collision );
PTR( SheepsManager );
PTR( SheepFactory );
PTR( GimmickManager );
PTR( GimmickFactory );
PTR( CloudsManager );
PTR( FundamentalGameSystem );

class GimmickEvent {
public:
	GimmickEvent( CollisionPtr collisiton,
				  SheepsManagerPtr sheeps_manager,
				  GimmickManagerPtr gimmick_manager,
				  SheepFactoryPtr player_factory,
				  GimmickFactoryPtr gimmick_factory,
				  CloudsManagerPtr clouds_manager,
				  FundamentalGameSystemPtr fundamental_game_system );
	~GimmickEvent( );
public:
	void update( );

	void setDeployInterval( int interval );
private:
	void updateExecutionLevelUp( );
	void updateMove( );
	void updateCreateGimmick( );
	void updateAction( );
	void actionGimmick( Collision::IdxList idx_list );
	void actionHeart( int gimmick_idx, int sheep_idx );
	void actionCarrot( int gimmick_idx, int sheep_idx );
	void actionColorBall( int gimmick_idx, int player_idx );
	void actionHole( int gimmick_idx, int player_idx );
	double animationFallSheep( int idx );
	void deployRandomGimmick( );
	bool isDeleteSheep( int idx );
private:
	CollisionPtr _collision;
	SheepsManagerPtr _sheeps_manager;
	GimmickManagerPtr _gimmicks_manager;
	SheepFactoryPtr _sheep_factory;
	GimmickFactoryPtr _gimmick_factory;
	CloudsManagerPtr _clouds_manager;
	FundamentalGameSystemPtr _game_system;
private:
	int _deploy_interval;
	int _before_deploy_time;
	std::vector< int > _deleted_gimmick_idx;
};
