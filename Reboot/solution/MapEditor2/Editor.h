#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include <string>
#include <vector>
#include "Define.h"
#include "GameData.h"

PTR( GameData );
PTR( Camera );

class Editor {
public:
	enum class MODE {
		SINGLE,
		MULTIPLE
	};
public:
	Editor( GameDataPtr data, CameraPtr camera );
	virtual ~Editor( );
public:
	void update( );
public:
	Vector2 getTempPos1( ) const;
	Vector2 getTempPos2( ) const;
	int getSelect( ) const;
	int getSelectType( ) const;
	MODE getMode( ) const;
	std::vector< int > getMultipleSelects( ) const;
private:
	void createGroundToMouse( );
	void createGroundToNumber( );
	void moveSelectedGround( );
	void tweakSelectedGround( );
	void deleteSelectedGround( );
private:
	void createGimmickToMouse( );
	void moveSelectedGimmick( );
	void tweakSelectedGimmick( );
	void deleteSelectedGimmick( );
	void changeSelectedGimmickDir( );
	void changeSelectedFanData( );
	void changeSelectedFanVec( GameData::Fan* fan );
	void changeSelectedFanStartState( GameData::Fan* fan );
	void changeSelectedFanAutoSwitching( GameData::Fan* fan );
	void changeSelectedFanOntime( GameData::Fan* fan );
	void changeSelectedFanOffTime( GameData::Fan* fan );
private:
	void createEnemyToMouse( );
	void moveSelectedEnemy( );
	void tweakSelectedEnemy( );
	void deleteSelectedEnemy( );
	void changeSelectedEnemyMovePattern( );
	void changeSelectedEnemyVecX( );
	void changeSelectedEnemyVecY( );
private:
	void moveSelectedMultiple( );
	void tweakSelectedMultiple( );
	void changeSelectedMultipleData( );
	void changeSelectedMultipleFanVec( );
	void changeSelectedMultipleFanStartState( );
	void changeSelectedMultipleFanAutoSwitching( );
	void changeSelectedMultipleFanOnTime( );
	void changeSelectedMultipleFanOffTime( );
	void changeSelectedMultipleEnemyMovePattern( );
	void changeSelectedMultipleEnemyVecX( );
	void changeSelectedMultipleEnemyVecY( );
	void changeSelectedMultipleDir( );
private:
	void createBetweenObjectToMouse( );
	void moveSelectedBetweenObject( );
	void tweakSelectedBetweenObject( );
	void deleteSelectedBetweenObject( );
private:
	void selectSingle( );
	void selectMultiple( );
	void changeWidth( );
	void changeHeight( );
	void selectCreateType( );
	Vector2 getGimmickSize( int gimmick_type );
private:
	int convertFromStringToNumber( std::string str );
	void save( );
	void load( );
	void oldLoad( );
private:
	GameDataPtr _data;
	CameraPtr _camera;
	Vector2 _temp_pos1;
	Vector2 _temp_pos2;
	bool _mouse_hold_left;
	bool _mouse_hold_right;
	int _select;
	MODE _mode;
	std::vector< int > _multiple_selects;
	Vector2 _save_distance;
	int _select_type;
};

