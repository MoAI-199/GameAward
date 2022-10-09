#include "GimmickManager.h"

GimmickManager::GimmickManager( ) {
}

GimmickManager::~GimmickManager( ) {
}

void GimmickManager::update( ) {
	int num = getMaxNum( );
	for ( int i = 0; i < num; i++ ) {
		_list[ i ]->update( );
	}
}

void GimmickManager::clearAll( ) {
	_list.clear( );
}

void GimmickManager::addGimmick( GimmickPtr gimmick ) {
	_list.push_back( gimmick );
}

void GimmickManager::deleteGimmick( int idx ) {
	_list.erase( _list.begin( ) + idx );
}

void GimmickManager::setPos( int idx, Vector2 pos ) {
	_list.at( idx )->setPos( pos );
}

Vector2 GimmickManager::getVec( int idx ) {
	return _list.at( idx )->getVec( );
}

int GimmickManager::getMaxNum( ) {
	return ( int )_list.size( );
}

Vector2 GimmickManager::getPos( int idx ) const {
	return _list[ idx ]->getPos( );
}

double GimmickManager::getSize( int idx ) const {
	return _list[ idx ]->getSize( );
}

Gimmick::TYPE GimmickManager::getType( int idx ) const {
	return _list[ idx ]->getType( );
}

ColorBall::COLOR_TYPE GimmickManager::getColorType( int idx ) const {
	return  std::dynamic_pointer_cast< ColorBall >( _list[ idx ] )->getColorType( );
}
