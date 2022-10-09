#include "Effector.h"
#include "Application.h"
#include "DxLib.h"
#include "Effect.h"
#include "Exception.h"


#ifndef EFFEKSEER

Effector::Effector( const std::string& directory ) :
_directory( directory ),
_resource_handles( ),
_effekseer_graph( -1 ) {
}

Effector::~Effector( ) {
}

EffectPtr Effector::createEffect( const std::string& directory ) { return EffectPtr( ); }

void Effector::drawEffect( ) const { }

#else

#include "EffekseerForDXLib.h"

#include <assert.h>

const int PARTICLE = 3000; //Effector�̍ő�p�[�e�B�N����

Effector::Effector( const std::string& directory ) :
_directory( directory ),
_resource_handles( ),
_effekseer_graph( -1 ) {
	// DirectX9���g�p����悤�ɂ���B
	// Effector���g�p����ɂ͕K���ݒ肷��B
	//SetUseDirect3DVersion(DX_DIRECT3D_9);


	// Effector������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if ( Effkseer_Init( PARTICLE ) == -1 ) {
		return;
	}


	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effector���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag( FALSE );

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions( );
}

Effector::~Effector( ) {
	Effkseer_End( );
}


EffectPtr Effector::createEffect( const std::string& filename ) {
	if ( _resource_handles.count( filename ) == 0 ) {
		std::string path = _directory + "/" + filename;
		int handle = LoadEffekseerEffect( path.c_str( ) );
		if ( handle < 0 ) {
			path = ( std::string )"../" + path;
			handle = LoadEffekseerEffect( path.c_str( ) );
			EXCEPTION( handle < 0, filename );
		}
		_resource_handles[ filename ] = handle;
	}

	EffectPtr result( new Effect( _resource_handles[ filename ] ) );
	return result;
}

void Effector::drawEffect( ) const {
	CHECKDRAWING
	//DrawGraph( 0, 0, _effekseer_graph, TRUE );
	RenderVertex( );

	// Effector��DxLib�̃J�����Ɠ���������
	Effekseer_Sync3DSetting( );

	// Effector�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D( );

	// Effector�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D( );
}

#endif