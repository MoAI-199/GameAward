#pragma once
#include "Image.h"
#include <string>

class ImageTarget : public Image {
	friend class Drawer;
public:
	/// <summary>
	/// �����Ȃ�
	/// </summary>
	virtual ~ImageTarget( );
public:
	/// <summary>
	/// �^�[�Q�b�g�̃Z�[�u
	/// </summary>
	/// <param name="path">�t�@�C���̃A�N�Z�X�p�X</param>
	void save( std::string path );
protected:
	/// <summary>
	/// �����Ȃ�
	/// </summary>
	ImageTarget( );
private:
	/// <summary>
	/// �X�N���[�������
	/// </summary>
	/// <param name="width">����</param>
	/// <param name="height">�c��</param>
	void create( int width, int height );
};

