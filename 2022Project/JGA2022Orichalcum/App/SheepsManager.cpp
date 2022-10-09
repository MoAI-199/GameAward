#include "SheepsManager.h"
#include "Sheep.h"
#include "ColorBall.h"
#include <algorithm>
#include "define.h"

static const int OUT_RANGE = 250;

SheepsManager::SheepsManager( ) {
}

SheepsManager::~SheepsManager( ) {
}

void SheepsManager::update( ) {
	int sheep_num = ( int )getMaxNum( );
	for ( int i = 0; i < sheep_num; i++ ) {
		Vector2 vec = _list[ i ]->getVec( );
		_list[ i ]->update( );
		deleteOutOfScreenSheep( i );
	}
	int delete_num = _delete_list.size( );
	for ( int i = 0; i < delete_num; i++ ) {
		int idx = _delete_list[ i ];
		deletePlayer( idx );
	}
	_delete_list.clear( );
}

Vector2 SheepsManager::getPos( int idx ) const {
	Vector2 pos;
	pos = _list[ idx ]->getPos( );
	return pos;
}

double SheepsManager::getSize( int idx ) const {
	double size = 0.0;
	if ( _list.size( ) == 0 ) {
		return size;
	}
	size = _list[ idx ]->getSize( );
	return size;
}

double SheepsManager::getAngle( int idx ) const {
	return _list[ idx ]->getAngle( );
}

void SheepsManager::addDeleteList( int idx ) {
	_delete_list.push_back( idx );
}

void SheepsManager::clearAll( ) {
	if ( _list.size( ) == 0 ) {
		return;
	}
	_list.clear( );
}

int SheepsManager::getMaxNum( ) const {
	return ( int )_list.size( );
}

int SheepsManager::getLeadPlayer( ) const {
	int player_num = getMaxNum( );
	//x座標と元のインデックスを保持する構造体
	struct Sort {
		int idx;
		double pos_x;
	};
	//プレイヤー人数分構造体を用意
	std::vector< Sort > data( player_num );
	//インデックスとx座標で初期化
	for ( int i = 0; i < player_num; i++ ) {
		data[ i ].idx = i;
		data[ i ].pos_x = getPos( i ).x;
	}
	//配列をx座標でソート
	std::sort( data.begin( ), data.end( ), [ ]( const Sort& a, const Sort& b ) { return a.pos_x < b.pos_x; } );
	int last = ( int )data.size( ) - 1;
	if ( last == -1 ) {
		return -1;
	}
	return data[ last ].idx;
}

Vector2 SheepsManager::getVec( int idx ) const {
	Vector2 vec = _list[ idx ]->getVec( );
	return vec;
}

Sheep::COLOR_TYPE SheepsManager::getColorType( int idx ) const {
	return _list[ idx ]->getColorType( );
}

double SheepsManager::getViewSize( int idx )const {
	double size = 0.0;
	size = _list[ idx ]->getViewSize( );
	return size;
}

double SheepsManager::getScale( int idx ) const {
	return _list[ idx ]->getScale( );
}

bool SheepsManager::isColorStorage( int idx, Sheep::COLOR_TYPE type ) {
	return _list[ idx ]->isColorStrage( type );
}


void SheepsManager::setColorType( int idx, Sheep::COLOR_TYPE type ) {
	_list[ idx ]->setColorType( type );
}

void SheepsManager::addSheep( SheepPtr player ) {
	_list.push_back( player );
}

void SheepsManager::deletePlayer( int idx ) {
	std::vector< SheepPtr >::iterator begin = _list.begin( );
	if ( idx > _list.size( ) - 1 ) {
		return;
	}
	_list.erase( begin + idx );
}

void SheepsManager::setPos( int idx, Vector2 pos ) {
	_list[ idx ]->setPos( pos );
}


void SheepsManager::setVec( int idx, Vector2 vec ) {
	_list[ idx ]->setVec( vec );
}

void SheepsManager::setSwapPlayersPos( int first_idx, int second_idx, Vector2 first_pos, Vector2 second_pos ) {
	_list[ first_idx ]->setPos( second_pos );
	_list[ second_idx ]->setPos( first_pos );
}

void SheepsManager::setViewSize( int idx, double size ) {
	_list[ idx ]->setViewSize( size );
}

void SheepsManager::setScale( int idx, double scale ) {
	_list[ idx ]->setScale( scale );
}

void SheepsManager::deleteOutOfScreenSheep( int idx ) {
	Vector2 pos = _list[ idx ]->getPos( );
	bool need_delete = false;
	if ( pos.x < -OUT_RANGE || pos.x > SCREEN_WIDTH + OUT_RANGE ) {
		need_delete = true;
	}
	if ( pos.y < -OUT_RANGE || pos.y > SCREEN_HEIGHT + OUT_RANGE ) {
		need_delete = true;
	}
	if ( need_delete ) {
		addDeleteList( idx );
	}
}
