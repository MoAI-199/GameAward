#pragma once
#include "mathematics.h"
#include "smart_ptr.h"

PTR( CloudsManager );
PTR( Cloud );

class CloudFactory {
public:
	CloudFactory( CloudsManagerPtr manager );
	virtual ~CloudFactory( );
public:
	void addDeploy( );
private:
	CloudPtr create( Vector2 pos,  double scale );
	void add( CloudPtr cloud );
private:
	CloudsManagerPtr _manager;
};

