#pragma once
#include "smart_ptr.h"

PTR( Controller );
PTR( StageManager );
PTR( Music );

class Menu {
public:
	Menu( ControllerPtr controller );
	virtual ~Menu( );
public:
	void update( );
	bool isMenu( )const;
private:
	void changeMenu( );
	void choose( );
	void choiseChange( );
	void confirmRetire( );
	void operationChange( );
	void creditChange( );
public:
	void menuClose( );
public:
	int getSelectingButton( );
	bool isRetire( ) const;
	bool isRetireMode( ) const;
	bool isRetireConfirm( ) const;
	bool isOperationMode( ) const;
	bool isCreditMode( ) const;
private:
	int _select;
	bool _menu;
	bool _operation_mode;
	bool _retire_mode;
	bool _credit_mode;
	bool _confirm_select;
	bool _retire;
	ControllerPtr _controller;
};

