#pragma once
#include "smart_ptr.h"

PTR( SheepsManager );
PTR( SheepFactory );
PTR( CloudsManager );

class FundamentalGameSystem {
public:
	FundamentalGameSystem( SheepsManagerPtr sheeps_manager,
					  CloudsManagerPtr clouds_manager,
					  SheepFactoryPtr sheep_factory );
	~FundamentalGameSystem( );
public:
	void update( );
	bool isLevelUp( ) const;
	bool isInterval( ) const;
	int getLevel( ) const;
	int getMokoRate( ) const;
private:
	void updateInterval( ); 
	void updateMokoRate( );
	void setMokoRate( double rate );
	void upLevel( );
private:
	SheepsManagerPtr _sheeps_manager;
	CloudsManagerPtr _clouds_manager;
	SheepFactoryPtr _sheep_factory;
	int _now_millisecond;
	int _now_level;
	int _moko_rate;
	bool _levelup;
	bool _interval;
};
