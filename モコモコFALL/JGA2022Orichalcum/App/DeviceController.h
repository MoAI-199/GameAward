#pragma once
class DeviceController {
public:
	DeviceController( );
	virtual ~DeviceController( );
public:
	bool isHoldArrowUp( ) const;
	bool isHoldArrowDown( ) const;
	bool isHoldArrowLeft( ) const;
	bool isHoldArrowRight( ) const;

	bool isLeftStickRightDown( )const;
	bool isLeftStickLeftDown( )const;
	bool isLeftStickUpperDown( )const;
	bool isLeftStickLowerDown( )const;

	bool isRightStickRightDown( )const;
	bool isRightStickLeftDown( )const;
	bool isRightStickUpperDown( )const;
	bool isRightStickLowerDown( )const;

	bool isPressedA( ) const;
	bool isPressedB( ) const;
	bool isPressedC( ) const;
	bool isPressedD( ) const;
};

