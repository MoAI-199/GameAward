#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>
#include <map>

PTR( Effector );
PTR( Effect );

class Effector : public Task {
public:
	/// <summary> Effector�̃R���X�g���N�^ </summary>
	/// <param name="directory"> �t�@�C���� </param>
	Effector( const std::string& directory );
	/// <summary>
	/// Effector�̃f�X�g���N�^
	/// </summary>
	virtual ~Effector( );
public:
	/// <summary>
	/// �G�t�F�N�g���쐬����
	/// </summary>
	EffectPtr createEffect( const std::string& filename ); // ���[�h�����G�t�F�N�g��id��Ԃ�
	/// <summary>
	/// �G�t�F�N�g��`�悷��
	/// </summary>
	void drawEffect( ) const;
private:
	/// <summary> �t�@�C���ۑ��p�ϐ� </summary>
	std::string _directory;
	/// <summary>
	/// ���\�[�X�������ϐ�
	/// </summary>
	std::map< std::string, int > _resource_handles;
	/// <summary> �R���X�g���N�^����-1��������ϐ� </summary>
	int _effekseer_graph;
};

