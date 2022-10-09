#pragma once
#include "smart_ptr.h"
#include "mathematics.h"

PTR( Effect );

class Effect {
public:
	Effect( int resource_handle );
	virtual ~Effect( );

public:
	/// <summary>
	/// 3D�G�t�F�N�g�̕`����~����
	/// </summary>
	void stop( ) const;
	/// <summary> �G�t�F�N�g�̒l�ݒ� </summary>
	/// <param name="pos"> ���W </param>
	/// <param name="size"> �������i1.0�j </param>
	/// <param name="rotate"> ��] </param>
	void setTransform( const Vector& pos, const double size = 1.0, const Vector& rotate = Vector( ) );

private:
	int _play_handle;
};

