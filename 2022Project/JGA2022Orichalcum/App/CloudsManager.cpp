#include "Exception.h"
#include "CloudsManager.h"
#include "Cloud.h"
#include "define.h"
#include "Log.h"

CloudsManager::CloudsManager( ) {
}

CloudsManager::~CloudsManager( ) {
}

void CloudsManager::update( ) {
#if debug
	LogPtr log = Task::get< Log >( );
	log->status( "cloud_num:%d\n", getMaxNum( ) );
#endif
	for ( int i = 0; i < getMaxNum( ); i++ ) {
		_list[ i ]->update( );
	}
}

void CloudsManager::addList( CloudPtr cloud ) {
	_list.push_back( cloud );
}

void CloudsManager::deleteCloud( int idx ) {
	_list.erase( _list.begin( ) + idx );
}

void CloudsManager::clearAll( ) {
	_list.clear( );
}

int CloudsManager::getMaxNum( ) const {
	return  ( int )_list.size( );
}

Vector2 CloudsManager::getPos( int idx ) const {
	return _list[ idx ]->getPos( );
}

double CloudsManager::getScale( int idx ) const {
	if ( idx < 0 ) {
		return 1.0;
	}
	return  _list[ idx ]->getScale( );
}

Vector2 CloudsManager::getVec( int idx ) const {
	Vector2 vec;
	Vector2 now_pos = getPos( idx );
	Vector2 next_pos;
	int next_idx;
	switch ( _now_dir_rotate ) {
	case CloudsManager::DIR_ROTATE::LEFT:
		next_idx = idx == 0 ? getMaxNum( ) - 1: idx - 1;
		next_pos = getPos( next_idx );
		break;
	case CloudsManager::DIR_ROTATE::RIGHT:
		next_idx = idx == getMaxNum( ) - 1 ? 0 : idx + 1;
		next_pos = getPos( next_idx );
		break;
	case CloudsManager::DIR_ROTATE::STOP:
		return Vector2( 0.0, 0.0 );
		break;
	}

	return ( getPos( idx ) - next_pos ).multiply( 0.5 );
}

CloudsManager::DIR_ROTATE CloudsManager::getDir( ) const {
	return _now_dir_rotate;
}

Vector2 CloudsManager::getBaseLine( int idx ) const {
	return _list[ idx ]->geBaseLdine( );
}

bool CloudsManager::getValid( int idx ) const {
	return 	_list.at( idx )->getValid( );
}

void CloudsManager::setPos( int idx, Vector2 pos ) {
	_list.at( idx )->setPos( pos );
}

void CloudsManager::setVec( int idx, Vector2 vec ) {
	_list.at( idx )->setVec( vec );
}

void CloudsManager::setValid( int idx, bool valid ) {
	_list.at( idx )->setValid( valid );
}

void CloudsManager::setRotateDir( DIR_ROTATE dir_rotate ) {
	_now_dir_rotate = dir_rotate;
}

