#pragma once
#include "Task.h"
#include "mathematics.h"
#include "Color.h"
#include <string>
#include <array>

PTR( Drawer );
PTR( Image );
PTR( SoftImage );
PTR( Movie );
PTR( ImageTarget );
PTR( Billboard );
PTR( Sprites );
PTR( PixelShader );
PTR( VertexShader );
PTR( Primitive );
PTR( Primitive2DShader );
PTR( Primitive3DShader );
PTR( Graph );

class Drawer : public Task {
public:
	Drawer( );
	virtual ~Drawer( );
public:
	enum class BLEND {
		NONE,
		ALPHA,
		ADD,
		SUB,
		INV,
		MUL,
	};
	class Scope {
		friend class Drawer;
	private:
		Scope( );
		void onBlend( );
		void onBright( );
		void onTarget( );
		void onWriteZBuffer( );
	public:
		~Scope( );
	private:
		bool _on_blend;
		bool _on_bright;
		bool _on_target;
		bool _on_write_z_buffer;
		int _draw_screen;
	};
public: //make
	/// <summary>
	/// �摜���쐬
	/// </summary>
	/// <param name="filename">�t�@�C���̖��O</param>
	/// <returns>true or false</returns>
	ImagePtr createImage( const char* filename ) const;
	/// <summary> �\�t�g�摜�f�[�^�̕ۑ��N���X�쐬 </summary>
	/// <param name="filename"> �t�@�C���A�h���X</param>
	/// <returns> �\�t�g�摜�f�[�^�ۑ��N���X </returns>
	SoftImagePtr createSoftImage( const char* filename ) const;
	ImagePtr createImageFromSoftImage( SoftImagePtr soft_image ) const;
	/// <summary>
	/// �摜�̖ڕW���쐬
	/// </summary>
	/// <param name="width">����</param>
	/// <param name="height">����</param>
	/// <returns></returns>
	ImageTargetPtr createImageTarget( int width, int height ) const;
	/// <summary> �f���f�[�^�̕ۑ��N���X�쐬 </summary>
	/// <param name="filename"> �t�@�C���A�h���X </param>
	/// <param name="alpha"> true or false </param>
	/// <returns> �f���f�[�^�ۑ��N���X </returns>
	MoviePtr createMovie( const char* filename, bool alpha = false ) const;
	/// <summary> �r���{�[�h�f�[�^�̕ۑ��N���X�쐬 </summary>
	/// <param name="filename"> �t�@�C���A�h���X </param>
	/// <returns> �r���{�[�h�f�[�^�ۑ��N���X </returns>
	BillboardPtr createBillboard( const char* filename ) const;
	BillboardPtr createBillboard( ) const;
	/// <summary> �X�v���C�g�f�[�^�̕ۑ��N���X�쐬 </summary>
	/// <param name="count"> �J�E���g </param>
	/// <param name="filename"> �t�@�C���A�h���X </param>
	/// <param name="num"> NUM </param>
	/// <returns> �X�v���C�g�f�[�^�ۑ��N���X </returns>
	SpritesPtr createSprites( int count, const char* filename, int num ) const;

	PixelShaderPtr createPixelShader( const char* filename ) const;
	VertexShaderPtr createVertexShader( const char* filename ) const;
	PrimitivePtr createPrimitive( bool buffer = false ) const;
	Primitive2DShaderPtr createPrimitive2DShader( );
	Primitive3DShaderPtr createPrimitive3DShader( );
	GraphPtr createGraph( );
public: //2D
	/// <summary>�~�̕`��</summary>
	///<param name="pos">���W</param>
	///<param name="radius">���a</param>
	///<param name="color">�F</param>
	///<param name="fill">�h��</param>
	void drawCircle( int x, int y, const double radius, const Color& color, bool fill ) const;
	void drawCircle( const Vector2& pos, const double radius, const Color& color, bool fill ) const;
	/// <summary> �`�悷�镶����̃t�H���g�̃T�C�Y���Z�b�g���� </summary>
	/// <param name="size"> �`�悷��t�H���g�̃T�C�Y </param>
	void setStringSize( int size ) const;
    /// <summary> �����̓��� </summary>
	/// <param name="x"> ������X���W </param>
	/// <param name="y"> ������Y���W </param>
	/// <param name="string"> ���͂��������� </param>
	/// <param name="..."> �ϒ� </param>
	void drawString( int x, int y, const char* string, ... ) const;
	void drawString( const Vector2& pos, const char* string, ... ) const;
	/// <summary> ���̕`�� </summary>
	///<param name="x1"> ���[��X���W </param> 
	///<param name="y1"> ���[��Y���W </param>
	///<param name="x2"> �t���̐��[��X���W </param>
	///<param name="y2"> �t���̐��[��Y���W </param>
	///<param name="color"> �F </param>
	void drawLine( int x1, int y1, int x2, int y2, const Color& color ) const;
	void drawLine( const Vector2& pos1, const Vector2& pos2, const Color& color ) const;
	/// <summary> �l�p�̕`�� </summary>
	///<param name="x1"> �l�p�̍���X���W </param>
	///<param name="y1"> �l�p�̍���Y���W </param>
	///<param name="x2"> �l�p�̉E��X���W </param>
	///<param name="y2"> �l�p�̉E��y���W </param>
	///<param name="color"> �l�p�̐F </param>
	///<param name="fill"> �l�p��g�������ŕ`�悷�邩�ǂ��� </param>
	void drawBox( int x1, int y1, int x2, int y2, const Color& color, bool fill ) const;
	void drawBox( const Vector2& pos1, const Vector2& pos2, const Color& color, bool fill ) const;
	///<summary>�s�N�Z����`�悷��</summary>
	///<param name="x"> �s�N�Z����X���W </param>
	///<param name="y"> �s�N�Z����Y���W </param>
	///<param name="color"> �s�N�Z���̐F </param>
	void drawPixel( int x, int y, const Color& color ) const;
	void drawPixel( const Vector2& pos, const Color& color ) const;
	/// <summary>
	/// ��`�̕`��
	/// </summary>
	/// <param name="x"> X���W </param>
	/// <param name="y"> Y���W </param>
	/// <param name="image"> �`�悷��~�̉摜 </param>
	/// <param name="ratio"> �䗦 </param>
	void drawSector( int x, int y, ImageConstPtr image, double ratio ) const;
	/// <summary> �s�N�Z���̐F�̎擾 </summary>
	/// <param name="x"> �擾������X���W </param>
	/// <param name="y"> �擾������Y���W </param>
	/// <returns> �擾�����F </returns>
	Color getPixelColor( int x, int y ) const;
public: //3D
	void drawTriangle( const Vector& pos1, const Vector& pos2, const Vector& pos3, const Color& color, bool fill ) const;
	/// <summary> ���̕`�� </summary>
	/// <param name="pos"> ���̒��S���W </param>
	/// <param name="radius"> ���̔��a </param>
	/// <param name="polygon_num"> �����`�����邽�߂̃|���S���ׂ̍��� </param>
	/// <param name="color"> ���̐F </param>
	/// <param name="fill"> �h��Ԃ�or�h��Ԃ��Ȃ� </param>
	void drawSphere( const Vector& pos, const double radius, int polygon_num, const Color& color, bool fill ) const;
	/// <summary>���̕`��</summary>
	/// <param name="pos1">�ŏ��̓_</param>
	/// <param name="pos2">�ŏI�̓_</param>
	/// <param name="color">���̐F</param>
	void drawLine( const Vector& pos1, const Vector& pos2, const Color& color ) const;
	/// <summary> �J�����̍��W���グ�� </summary>
	/// <param name="up"> �グ�鐔�l </param>
	void setCameraUp( const Vector& up );
	/// <summary> �J�����̈ʒu�A�����_�̐ݒ� </summary> 
	/// <param name="pos">�@�J�����̈ʒu���W�@</param>
	/// <param name="target"> �J�����̒���������́A���� </param>
	void setCamera( const Vector& pos, const Vector& target );
	/// <summary> �J�����̍s��̐ݒ� </summary>
	/// <param name="view"> �r���[�s�� </param>
	/// <param name="proj"> �ˉe�s�� </param>
	void setCamera( const Matrix& view, const Matrix& proj );
	/// <summary>
	/// �J�����̒��S���擾
	/// </summary>
	/// <param name="x">�J������X���W���擾</param>
	/// <param name="y">�J������Y���W���擾</param>
	void setCameraCenter( double x, double y );
	/// <summary> �J�����̎��_���Z�b�g���� </summary>
	/// <param name="fov">����̍L��</param>
	void setCameraPerspective( double fov );
	/// <summary> �J�����̌Ăяo�� </summary>
	void initCameraSetting( ) const;
	/// <summary>
	/// �����擾
	/// </summary>
	/// <param name="start">���̏��߂̍��W</param>
	/// <param name="end">���̏I���̍��W</param>
	/// <param name="red">�ԐF</param>
	/// <param name="green">�ΐF</param>
	/// <param name="blue">�F</param>
	void setFog( double start, double end, int red, int green, int blue );
	/// <summary>
	///	���C�g�̈ʒu�ƕ����̐ݒ�
	/// </summary>
	/// <param name="pos">���C�g�̈ʒu�̒l</param>
	/// <param name="dir">���C�g�̕����̒l</param>
	void setLight( const Vector& pos, const Vector& dir );
	/// <summary>�@�g�����C�g�̐ݒ� </summary>
	/// <param name="on">���C�g��ON/OFF</param>
	void setUseLight( bool on );
	/// <summary>
	/// �J�����̎�O�N���b�v�����Ɖ��N���b�v�����̐ݒ�
	/// </summary>
	/// <param name="near">�J�����̎�O�N���b�v�����̒l</param>
	/// <param name="far">�J�����̉��N���b�v�����̒l</param> 
	void setNearFar( double near, double far );
	/// <summary>
	/// �J��������ʓ��ɂ��邩�ǂ����𔻕ʂ���
	/// </summary>
	/// <param name="pos">�J�����ő����������̂̈ʒu</param>
	/// <returns>true or false</returns>
	bool isInCamera( const Vector& pos ) const;
	/// <summary>
	/// ���[���h�̍��W���擾����
	/// </summary>
	/// <param name="screen_pos"> �X�N���[�����W </param>
	/// <returns>true or false</returns>
	Vector getWorldPos( const Vector& screen_pos ) const;
	/// <summary>
	/// �X�N���[���̍��W���擾����
	/// </summary>
	Vector2 getScreenPos( const Vector& world_pos ) const;

	Quaternion getScreenPosPlusW( const Vector& world_pos ) const;
	/// <summary>
	/// �J�����̍��W���擾����
	/// </summary>
	Vector getCameraPos( ) const;
	/// <summary>
	/// �J�����̒����_�̎擾
	/// </summary>
	/// <returns> �J�����̒����_�̒l��Ԃ� </returns>
	Vector getCameraTarget( ) const;
	/// <summary>
	/// �J�����̃Y�[���̒l��Ԃ�
	/// </summary>
	/// <returns>�Y�[���̒l</returns>
	Vector getCameraUp( ) const;
	/// <summary> �J�����̐ݒ� </summary>
	/// <param name="camera_pos"> ���W </param>
	/// <param name="camera_rot"> �p�x�i��]�j </param>
	/// <returns> �J�����ݒ�f�[�^�̕ۑ��N���X </returns>
	Matrix createViewMatrix( const Vector& camera_pos, const Matrix& camera_rot ) const;
	/// <summary> �ˉe�s��̍쐬 </summary>
	/// <returns> �s�� </returns>
	Matrix createProjectionMatrix( ) const;
public: // common
	/// <summary> ��ʂ������� </summary>
	void clearScreen( ) const;
public: // scope
	/// <summary>
	/// Scope�쐬
	/// </summary>
	/// <returns>�������ꂽScope</returns>
	Scope createScope( ) const;
	/// <summary>�y�o�b�t�@���g�p���邩�ǂ�����ݒ肷�� </summary>
	/// <param name="write_z_buffer">zbuffer</param>
	void disableZBufferWriting( Scope& scope, bool write_z_buffer );
	/// <summary>
	///	�`���w��
	/// </summary>
	/// <param name="target">�`��Ώ�(���L����BackScreen)</param>
	void setTarget( Scope& scope, ImageTargetPtr target = ImageTargetPtr( ) ) const;
	/// <summary>
	/// �u�����h�ݒ�
	/// </summary>
	/// <param name="scope">scope</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	/// <param name="ratio">�u�����h���V�I</param>
	void setBlend( Scope& scope, BLEND blend, double ratio ) const;
	/// <summary>
	/// �u���C�g�ݒ�
	/// </summary>
	/// <param name="scope">scope</param>
	/// <param name="r">�Ԑ������V�I</param>
	/// <param name="g">�ΐ������V�I</param>
	/// <param name="b">�������V�I</param>
	void setBright( Scope& scope, double r, double g, double b ) const;
	bool isOutterOfScreen( const Vector& pos );
	void polygon( Vector v1, Vector v2, Vector v3, int r, int g, int b );
private:
	void update( ) override;
private:
	Vector _light_pos;
	Vector _light_dir;
	bool _light;
	Vector _camera_pos;
	Vector _camera_target;
	Vector _camera_up;
	double _fov;
	double _near;
	double _far;
	int _screen_width;
	int _screen_height;
};

