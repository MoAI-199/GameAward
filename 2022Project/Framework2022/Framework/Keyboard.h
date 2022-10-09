#pragma once

#include "task.h"
#include <string>
#include <map>

PTR( Keyboard );

class Keyboard : public Task {
public:
	/// <summary>
	/// �e�L�[�̐��l�𕶎���֑��
	/// </summary>
	Keyboard( );
	virtual ~Keyboard( );
public:
	///<summry>
	///GetHitKeyStateAll�ŕ����̃L�[�̓��͂��擾�ł���悤�ɂ��Ă���
	///</summry>
	virtual void update( );
public:
	/// <summary> �w�肵���L�[�������ꂽ�u�Ԃ̌��m </summary> <param name="key"> ���m����L�[ </param> <returns> �w�肵���L�[��true </returns>
	bool isPushKey( std::string key );
	/// <summary>
	/// ����̃L�[��������Ă���Ԃ̌��m
	/// </summary>
	/// <param name="key"> ���m����L�[ </param>
	/// <returns> �w�肵���L�[��true </returns>
	bool isHoldKey( std::string key );
	///<summary>
	///�������̓o�b�t�@�ɗ��܂��������f�[�^����P�����擾����
	///</summary>
	char getInputChar( );
private:
	struct KEYS {
		char buffer[ 256 ];
	};
private:
	KEYS _now_keys;
	KEYS _before_keys;
	std::map< std::string, int > _convert;
};
