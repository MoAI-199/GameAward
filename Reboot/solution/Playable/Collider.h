#pragma once
#include "smart_ptr.h"
#include "MapData.h"
#include <map>
#include <vector>

PTR( Collider );
PTR( MapData );
PTR( Player );
PTR( Fan );
PTR( System );
PTR( Menu );

class Collider {
public:
	enum class COLLIDE_DIR {
		NONE,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	
	struct Wind {
		Vector2 edge1_start_pos;
		Vector2 edge1_end_pos;
		Vector2 edge1_stop_pos;
		Vector2 edge2_start_pos;
		Vector2 edge2_end_pos;
		Vector2 edge2_stop_pos;
		Vector2 center_start_pos;
		Vector2 center_end_pos;
		Vector2 center_stop_pos;
	};
public:
	Collider( MapDataPtr map_data, MenuPtr menu );
	virtual ~Collider( );
public:
	void update( );
	Wind* getWind( FanPtr fan );
private:
	void collidePlayers( );

	void collidePlayerWithGrounds( std::vector< PlayerPtr > players );
	void collidePlayerWithGround( PlayerPtr player, const MapData::Ground* ground );
	void collideCornerWithGround( PlayerPtr player, const MapData::Ground* ground );
	bool collidePlayerWithGroundFromLeft( PlayerPtr player, const MapData::Ground* ground );
	bool collidePlayerWithGroundFromRight( PlayerPtr player, const MapData::Ground* ground );
	bool collidePlayerWithGroundFromUp( PlayerPtr player, const MapData::Ground* ground );
	bool collidePlayerWithGroundFromDown( PlayerPtr player, const MapData::Ground* ground );

	void collidePlayerWithOtherPlayers( std::vector< PlayerPtr > players );
	void collidePlayerWithOtherPlayer( PlayerPtr src_player, PlayerPtr dst_player );
	void collideCornerWithOtherPlayer( PlayerPtr src_player, PlayerPtr dst_player );
	bool collidePlayerWithOtherPlayerFromLeft( PlayerPtr src_player, PlayerPtr dst_player );
	bool collidePlayerWithOtherPlayerFromRight( PlayerPtr src_player, PlayerPtr dst_player );
	bool collidePlayerWithOtherPlayerFromUp( PlayerPtr src_player, PlayerPtr dst_player );
	bool collidePlayerWithOtherPlayerFromDown( PlayerPtr src_player, PlayerPtr dst_player );

	void collidePlayerWithGimmicks( std::vector< PlayerPtr > players );
	void collidePlayerWithGimmickSortType( PlayerPtr player, GimmickPtr gimmick );
	void collidePlayerWithGimmick( PlayerPtr player, GimmickPtr gimmick );
	void collideCornerWithGimmick( PlayerPtr player, GimmickPtr gimmick );
	bool collidePlayerWithGimmickFromLeft( PlayerPtr player, GimmickPtr gimmick );
	bool collidePlayerWithGimmickFromRight( PlayerPtr player, GimmickPtr gimmick );
	bool collidePlayerWithGimmickFromUp( PlayerPtr player, GimmickPtr gimmick );
	bool collidePlayerWithGimmickFromDown( PlayerPtr player, GimmickPtr gimmick );
	bool collidePlayerWithGimmickInside( PlayerPtr player, GimmickPtr gimmick );

	void collidePlayerWithAcid( PlayerPtr player, GimmickPtr acid );
	void collidePlayerWithDecelerateNet( PlayerPtr player, GimmickPtr decelerate_net );
	void collidePlayerWithStickyNet( PlayerPtr player, GimmickPtr stickey_net );
	void collidePlayerWithFan( PlayerPtr player, GimmickPtr fan );
	void setupWind( FanPtr fan );
	void collideWithWinds( std::vector< PlayerPtr > players );
	void collideWithWind( PlayerPtr player, FanPtr fan );
	void interceptWindWithGrounds( Gimmick::DIR dir, Vector2 start_pos, Vector2& end_pos );
	void interceptWindWithGimmicks( Gimmick::DIR dir, Vector2 start_pos, Vector2& end_pos );
	void collidePlayerWithNeedle( PlayerPtr player, GimmickPtr needle );
	void collidePlayerWithGoal( PlayerPtr player, GimmickPtr goal );
	void collidePlayerWithRespawn( PlayerPtr player, GimmickPtr respawn );

	void collidePlayerWithEnemies( std::vector< PlayerPtr > players );
	void collidePlayerWithEnemy( PlayerPtr player, EnemyPtr enemy );
	bool collidePlayerWithEnemyFromLeft( PlayerPtr player, EnemyPtr enemy );
	bool collidePlayerWithEnemyFromRight( PlayerPtr player, EnemyPtr enemy );
	bool collidePlayerWithEnemyFromUp( PlayerPtr player, EnemyPtr enemy );
	bool collidePlayerWithEnemyFromDown( PlayerPtr player, EnemyPtr enemy );

private:
	bool isCrossingOtherLineSegment( Vector2 line_segment1_start_pos, Vector2 line_segment1_end_pos,
									 Vector2 line_segment2_start_pos, Vector2 line_segment2_end_pos, bool just_consider );
	bool isStraddlingOtherLineSegment( Vector2 src_line_segment_start_pos, Vector2 src_line_segment_end_pos,
									   Vector2 dst_line_segment_start_pos, Vector2 dst_line_segment_end_pos, bool just_consider );
	double getCrossProduct( Vector2 vec1, Vector2 vec2 ) const;
	void collidePlayerWhenDie( PlayerPtr player );
	Gimmick::DIR convertColliderDir( ) const;
private:
	MapDataPtr _map_data;
	MenuPtr _menu;
	COLLIDE_DIR _dir;
	std::map< FanPtr, Wind* > _winds;
	bool _collide_up;
	bool _collide_down;
	bool _collide_left;
	bool _collide_right;
};