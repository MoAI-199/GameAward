#pragma once

#include "smart_ptr.h"
#include "mathematics.h"
#include "Color.h"
#include <string>

PTR( Primitive );
PTR( PrimitiveImpl );
PTR( Image );

class Primitive {
	friend class Drawer;
public:
	/// <summary>
	/// class ModelImpl�Ő������ꂽ�ϐ��̏���������
	/// </summary>
	/// <summary>
	/// _vertex���Ăяo����Ă�����Ȃ��̔z����폜����
	/// </summary>
	virtual ~Primitive( );
public:
	struct VERTEX {
		Vector pos;
		double u;
		double v;
		Vector norm;
		Color color;
		VERTEX( ) { }
		VERTEX(
			Vector pos_,
			double u_,
			double v_,
			Vector norm_ = Vector( ),
			Color color_ = Color( ) ) {
				pos = pos_;
				u = u_;
				v = v_;
				color = color_;
				norm = norm_;
		}
	};
public:
	/// <summary>
	/// 3D���f���`��
	/// </summary>
	void draw( );
	/// <summary>
	/// 3D���f���̍��W�ݒ�
	/// </summary>
	/// <param name="mat">�񎟌��z��̗v�f</param>
	void draw( const Matrix& mat );
	/// <summary>
	/// �摜�̃n���h�����擾
	/// </summary>
	/// <param name="image">�摜�̃n���h��</param>
	void setImage( ImageConstPtr image );
	/// <summary>
	/// �l��z������
	/// </summary>
	/// <param name="polygon_num">�ύX����Ȃ��l</param>
	void alloc( const int& polygon_num );
	/// <summary>
	/// �l��_polygon_num�ɃZ�b�g
	/// </summary>
	/// <param name="num">�ύX����Ȃ��l</param>
	void setNum( const int& num );
	/// <summary>
	/// _polygon_num�Ɏw�肳�ꂽ�l��Ԃ�
	/// </summary>
	/// <returns></returns>
	int  getNum( ) const;
	/// <summary>
	/// 3D�|���S���ɗv�f���Z�b�g
	/// </summary>
	/// <param name="n">�z��̒l</param>
	/// <param name="vertex">�\���̂̒��_�z��</param>
	void set( const int& n, const VERTEX& vertex ); // n * 3
	/// <summary>
	/// ���[�h
	/// </summary>
	/// <param name="filename">�ǂݍ��ݐ�̃t�@�C����</param>
	/// <returns></returns>
	bool load( const std::string& filename );
	/// <summary>
	/// �Z�[�u
	/// </summary>
	/// <param name="filename">�ǂݍ��ݐ�̃t�@�C����</param>
	void save( const std::string& filename );
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void reset( );
	/// <summary>
	/// �|���S���ʒu�́~�v�Z
	/// </summary>
	/// <param name="mat">�|���S���̍��W</param>
	void multiply( const Matrix& mat );
	/// <summary> �F�̐ݒ� </summary>
	/// <param name="color"> �F�̒l </param>
	void setColor( const Color& color );
		/// <summary>
	/// �F�̒l���擾
	/// </summary>
	/// <param name="vertex_idx">�z��̍\����</param>
	/// <param name="color">�F�̍\����</param>
	void setColor( const int& vertex_idx, const Color& color );
	/// <summary>
	/// ���f���̒��_���擾����
	/// </summary>
	/// <param name="idx">���f���̒��_</param>
	/// <returns></returns>
	VERTEX getVertex( const int& idx ) const;
	/// <summary>
	/// �|�C���g�̒l�擾
	/// </summary>
	/// <param name="idx">�|�C���g�̍��W�l</param>
	/// <returns></returns>
	Vector getPoint( const int& idx ) const;
	/// <summary>
	/// ���f���̃|�C���g��ݒ肷��
	/// </summary>
	/// <param name="vertex_idx">�|�C���g�̒��_</param>
	/// <param name="pos">�|�C���g�̍��W</param>
	/// <returns></returns>
	void setPoint( const int& vertex_idx, const Vector& pos );
	/// <summary>
	/// �|���S���̖@����`��
	/// </summary>
	/// <param name="vertex_idx">�|���S���̖@���̒l</param>
	/// <param name="norm">�|���S���̃x�N�g���̍��W</param>
	void setNorm( const int& vertex_idx, const Vector& norm );

	void setTrans( bool trans );

	ImageConstPtr getImage( );
protected:
	Primitive( bool buffer );
private:
	/// <summary>
	/// ���f���̒l������
	/// </summary>
	/// <returns>_impl�̒l</returns>
	PrimitiveImplConstPtr getModelImpl( ) const;
private:
	PrimitiveImplPtr _impl;
	ImageConstPtr _image;
	bool _buffer;
	bool _buffer_changed;
	bool _trans;
};

