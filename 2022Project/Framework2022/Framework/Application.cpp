#include "Application.h"
#include "Binary.h"
#include "DxLib.h"
#include "Exception.h"
#include "TaskManager.h"
#include "Introduction.h"
#include "Log.h"
#include "Scene.h"
#include <assert.h>

//#pragma warning(disable:4099)

const int PARTICLE = 2000; //Effekseer�̍ő�p�[�e�B�N����
const int COLOR_BIT = 32;
const int COLOR_BIT_DEPTH = 32;
const char* WINDOW_NAME = "Framework";
static const int REFRESH_COUNT = 60;	//���ς����T���v����
const int DEFAULT_FPS = 30;

ApplicationPtr Application::_instance;

void Application::createInstance( ) {
	if ( !_instance ) {
		_instance = ApplicationPtr( new Application );
	}
}

void Application::destroyInstance( ) {
	if ( _instance ) {
		_instance.reset( );
	}
}

Application::Application( ) :
_scene( ) {
}

Application::~Application( ) {
}

bool Application::initialize( ) {
	SetOutApplicationLogValidFlag( FALSE );

	int sx;
	int sy;
	int bit;
	GetDefaultState( &sx, &sy, &bit );
	_screen_width = sx;
	_screen_height = sy;
	_window_width = sx;
	_window_height = sy;
	_skip_flipping = false;
	SetGraphMode( _screen_width, _screen_height, COLOR_BIT_DEPTH );
	SetWindowText( WINDOW_NAME );

#	if FULLSCREEN
	_windowed = false;
	ChangeWindowMode( FALSE );
#	else
	_windowed = true;
	ChangeWindowMode( TRUE );
	int w = sx * 3 / 4;
	int h = sy * 3 / 4;
	_window_width = w;
	_window_height = h;
	SetWindowSize( w, h );
	SetWindowPosition( ( sx - w ) / 2, ( sy - h ) / 2 );
#	endif

#	if _DEBUG
	SetDoubleStartValidFlag( TRUE ); // ���d�N��
#	endif
	
	SetEnableXAudioFlag( TRUE );

	// �g�p���� Direct3D �̃o�[�W������ 9EX �ɐݒ�
	_using_d3d9 = false;
#if USED3D9
	SetUseDirect3DVersion( DX_DIRECT3D_9EX );
	_using_d3d9 = true;
#endif

	if ( DxLib_Init( ) == -1 ) {
		return false;
	}
	
	initSettings( );

	_terminating = false;
	_disabled_escape_to_terminating = false;

	_global_task_mgr = TaskManagerPtr( new TaskManager );
	_scene_task_mgr = TaskManagerPtr( new TaskManager );
	_introduction = IntroductionPtr( new Introduction );

	_enable_scene_task_mgr = false;
	_flipping_refresh_count = REFRESH_COUNT;
	_flipping_start_time = GetNowCount( );
	_fps = DEFAULT_FPS;
	_fps_start_time = GetNowCount( );
	_fps_count = 0;

	return true;
}

void Application::finalize( ) {
	_scene.reset( );

	_scene_task_mgr->finalize();
	_scene_task_mgr->setStatus( TaskManager::STATUS::INVALID );
	_scene_task_mgr.reset( );

	_global_task_mgr->finalize();
	_global_task_mgr->setStatus( TaskManager::STATUS::INVALID );
	_global_task_mgr.reset( );

	DxLib_End( );
}

ApplicationPtr Application::getInstance( ) {
	return _instance;
}

void Application::initSettings( ) {
	SetUseLighting( FALSE );
	SetLightEnable( FALSE );
	SetUseZBuffer3D( TRUE );
	SetWriteZBuffer3D( TRUE );
	SetDrawScreen( DX_SCREEN_BACK );
	SetAlwaysRunFlag( TRUE );
	SetUseBackCulling( TRUE );
	SetTextureAddressModeUV( DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP );
	SetUsePremulAlphaConvertLoad( TRUE );
	SetDrawMode( DX_DRAWMODE_BILINEAR );
	SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA, 255 ) ;

	SetLightAmbColor( GetColorF( 0.5f, 0.5f, 0.5f, 0.0f ) ) ;
	SetLightDifColor( GetColorF( 0.5f, 0.5f, 0.5f, 0.0f ) ) ;

	MATERIALPARAM MatParam ;
	MatParam.Diffuse = GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) ;	// �f�B�t���[�Y�J���[�͔�
	MatParam.Ambient = GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) ;	// �A���r�G���g�J���[�͔�( ���C�g�̃A���r�G���g�J���[�����̂܂ܔ��f���� )
	MatParam.Specular = GetColorF( 0.0f, 0.0f, 0.0f, 0.0f ) ;	// �X�y�L�����J���[�͖���
	MatParam.Emissive = GetColorF( 0.0f, 0.0f, 0.0f, 0.0f ) ;	// �G�~�b�V�u�J���[( ���Ȕ��� )���Ȃ�
	MatParam.Power = 0.0f ;						// �X�y�L�����͂Ȃ��̂łO
	SetMaterialParam( MatParam ) ;

	//initCameraSetting( );
}

void Application::setNextSceneName( const std::string& name ) {
	_next_scene_name = name;
}

void Application::updateScene( ) {
	if (_scene) {
		_scene->update();

		LogPtr log = Task::get< Log >();
		if (log) {
			log->draw();
		}

		if ( !_skip_flipping ) {
			flip();
		}
		_skip_flipping = false;
	}

	if ( _next_scene_name.empty( ) ) {
		return;
	}

	if ( _scene ) {
		_scene->finalize( );
	}

	_scene.reset( );
	_scene_task_mgr->finalize( );
	_scene_task_mgr->setStatus( TaskManager::STATUS::INVALID );
	_scene_task_mgr.reset( );
	_scene_task_mgr = TaskManagerPtr( new TaskManager );

	_scene = _introduction->generateScene( _next_scene_name );

	_scene_task_mgr->setStatus( TaskManager::STATUS::CREATED );
	_scene->initialize( );

	_scene_task_mgr->setStatus( TaskManager::STATUS::USED );
	_scene_task_mgr->initialize( );

	_next_scene_name.clear( );
}

void Application::waitForFlipping( ) {
	// wait�Ȃ�
	if ( _fps == 0 ) {
		return;
	}

	// fps�擾
	if ( _fps_count >= _fps ) {
		int now = GetNowCount( );
		//0�t���[���ڂ���̎���
		int frame_time_sum = now - _fps_start_time;
		//���ς̎���
		double frame_time_avg = ( double )frame_time_sum / _fps;
		//fps�̌v�Z
		if ( frame_time_avg != 0.0 ) {
			_actual_fps = 1000 / frame_time_avg;
		}
		_fps_start_time = now;
		_fps_count = 0;
	} else {
		_fps_count++;
	}

	//���݂̎���
	int now_time = GetNowCount( );
	//60�t���[���ڂȂ畽�ς��v�Z����
	if ( _flipping_refresh_count >= REFRESH_COUNT ) {
		//�ēx0�t���[���ڂ���
		_flipping_refresh_count = 0;
		_flipping_start_time = now_time;
	}

	//0�t���[���ڂ���̎���
	int took_time = now_time - _flipping_start_time;

	//0�t���[���ڂ���̗\�z����鎞��
	int game_time = _flipping_refresh_count * 1000 / _fps;

	//�҂ׂ�����
	int wait_time = game_time - took_time;

	if ( wait_time > 0 ) {
		//�ҋ@
		Sleep( wait_time );
	}

	_flipping_refresh_count++;
}

void Application::flip( ) {
	waitForFlipping( );
	ScreenFlip( );
	ClearDrawScreen( );
}

void Application::run( ) {
	if ( !initialize( ) ) {
		return;
	}

	_global_task_mgr->setStatus( TaskManager::STATUS::CREATED );
	_introduction->procedure( );

	_global_task_mgr->setStatus( TaskManager::STATUS::USED );
	_global_task_mgr->initialize( );

	_enable_scene_task_mgr = true;

	// ���C�����[�v
	while ( true ) {
		if ( _terminating ) {
			break;
		}
		if ( ProcessMessage( ) != 0 ) {
			break;
		}
		if ( !_disabled_escape_to_terminating &&
			 CheckHitKey( KEY_INPUT_ESCAPE ) ) {
			break;
		}

		// task
		_global_task_mgr->update( );
		_scene_task_mgr->update( );

		//scene
		updateScene( );
	}

	finalize( );
}


void Application::addTask( const std::string& tag, TaskPtr task ) {
	if ( _enable_scene_task_mgr ) {
		_scene_task_mgr->addTask( tag, task );
	} else {
		_global_task_mgr->addTask( tag, task );
	}
}

TaskPtr Application::getTask( const std::string& tag ) {
	TaskPtr task = _global_task_mgr->getTask( tag );
	if ( !task && _enable_scene_task_mgr ) {
		task = _scene_task_mgr->getTask( tag );
	}
	return task;
}

int Application::getWindowWidth( ) const {
	return _window_width;
}

int Application::getWindowHeight( ) const {
	return _window_height;
}

int Application::getScreenWidth( ) const {
	return _screen_width;
}

int Application::getScreenHeight( ) const {
	return _screen_height;
}

bool Application::isWindowed( ) const {
	return _windowed;
}

bool Application::isEnabledD3D9( ) const {
	return _using_d3d9;
}

bool Application::isEnabledShaderVersion( const int vnum ) const {
	if ( GetValidShaderVersion( ) >= vnum * 100 ) {
		return true;
	}
	return false;
}

int Application::getNowCount( ) const {
	int now_count = GetNowCount( );
	return now_count;
}

bool Application::loadBinary( std::string  filename, BinaryPtr binary ) {
	int size = ( int )FileRead_size( filename.c_str( ) );
	if ( size == -1 ) {
		filename = "../" + filename;
		size = ( int )FileRead_size( filename.c_str( ) );
		if ( size == -1 ) {
			return false;
		}
	}

	int handle = FileRead_open( filename.c_str( ) );
	if ( handle == -1 ) {
		return false;
	}

	binary->ensure( size );

	FileRead_read( binary->getPtr( ), size, handle );

	FileRead_close( handle );

	return true;
}


void Application::saveBinary( std::string filename, BinaryPtr binary ) {

	int size = binary->getSize( );
	if ( size <= 0 ) {
		return;
	}

	FILE *fp;
	errno_t err = fopen_s( &fp, filename.c_str( ), "wb" );
	if ( err != 0 ) {
		return;
	}

	fwrite( binary->getPtr( ), size, 1, fp );
	fclose( fp );
}

std::string Application::inputString( int sx, int sy ) {
	char buf[ 2048 ];
	std::string str;
	if ( KeyInputString( sx, sy, 2048, buf, TRUE ) == TRUE ) {
		str = buf;
	}
	return str;
}

void Application::terminate( ) {
	_terminating = true;
}

void Application::changeWindow( bool windowed ) {
	_windowed = windowed;
	ChangeWindowMode( _windowed ? TRUE : FALSE );
	initSettings( );
}

void Application::setWindowSize( int width, int height ) {
	_window_width  = width;
	_window_height = height;
	SetWindowSize( _window_width, _window_height );
}

void Application::setScreenSize( int width, int height ) {
	_screen_width = width;
	_screen_height = height;
	SetGraphMode( _screen_width, _screen_height, COLOR_BIT_DEPTH );
	setWindowSize( _window_width, _window_height );
	initSettings( );
}

void Application::setFPS( unsigned int fps ) {
	_fps = fps;
}


unsigned int Application::getFPS( ) const {
	return _fps;
}


double Application::getActualFPS( ) const {
	return _actual_fps;
}

Vector Application::getCursor( int mouse_x, int mouse_y ) const {
	VECTOR sp = ConvScreenPosToWorldPos( VGet( ( float )mouse_x, ( float )mouse_y, 0.0f ) );
	VECTOR ep = ConvScreenPosToWorldPos( VGet( ( float )mouse_x, ( float )mouse_y, 1.0f ) );
	Vector s( sp.x, sp.y, sp.z );
	Vector e( ep.x, ep.y, ep.z );
	double ratio = abs( s.y ) / abs( ( e - s ).y );
	return ( e - s ) * ratio + s;
}



void Application::fetchFilename( const char* format, std::vector< std::string >& files ) {
	HANDLE hSearch;
	WIN32_FIND_DATA fd;
	hSearch = FindFirstFile( format, &fd );
	if ( hSearch != INVALID_HANDLE_VALUE ) {
		files.push_back( fd.cFileName );
		while ( TRUE ) {
			if ( !FindNextFile( hSearch, &fd ) ) {
				break;
			}
			files.push_back( fd.cFileName );
		}
	}
	FindClose( hSearch );
}

void Application::skipFlipping( ) {
	_skip_flipping = true;
}

void Application::disableEscapeToTerminating( ) {
	_disabled_escape_to_terminating = true;
}

