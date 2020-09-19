#pragma once

// 메모리 누수 체크
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// 에러 메시지
#ifdef _AFX
#define ERR_MSG(Message) AfxMessageBox(Message)
#else
#define ERR_MSG(Message) MessageBox(nullptr,Message,L"System_Error",MB_OK)
#endif

// 창 사이즈
#define WINCX	1600//1400//1920//1400	//800 
#define WINCY	900//900//1080//900		//600

// 폼 사이즈
#define FORMCX	300
#define FORMCY	WINCY

// 타일 사이즈 최소 단위
//#define TILECX ??		// 타일 하나의 width
//#define TILECY ??		// 타일 하나의 height
//#define TILEX	??		// 월드에서 가로로 놓을 최대 타일의 수
//#define TILEY ??		// 월드에서 세로로 놓을 최대 타일의 수

// 매크로 싱글톤 선언문 
#define DECLARE_SINGLETON(ClassName)							\
public:															\
static ClassName* GetInstance()									\
{																\
	if (nullptr == m_pInstance)									\
		m_pInstance = new ClassName;							\
	return m_pInstance;											\
}																\
static void DestroyInstance()									\
{																\
	if (m_pInstance)											\
	{															\
		delete m_pInstance;										\
		m_pInstance = nullptr;									\
	}															\
}																\
private:														\
	static ClassName* m_pInstance;				

// 매크로 싱글톤 초기화문
#define INITIALIZE_SINGLETON(ClassName)							\
ClassName* ClassName::m_pInstance = nullptr;

// 오브젝트 유효성 체크
#define IS_VALID_OBJ(_pObj)				((_pObj) && (_pObj->IsValid()))
#define DO_IF_IS_NOT_VALID_OBJ(_pObj)	if(!IS_VALID_OBJ(_pObj))
#define DO_IF_IS_VALID_OBJ(_pObj)		if(IS_VALID_OBJ(_pObj))
#define ELSE else

// 수학 관련
#define PI acos(-1.f) //== acos(cos(pi)) == pi
#define TWO_PI (2.f * PI)

#define TO_RADIAN(_fDegree) ((_fDegree) * TWO_PI / 360.f)
#define TO_DEGREE(_fRadian) ((_fRadian) * 360.f / TWO_PI)

#define DIR_X_DEGREE(_fDegree) cosf(TO_RADIAN((_fDegree)))
#define DIR_Y_DEGREE(_fDegree) sinf(TO_RADIAN((_fDegree)))

#define DIR_X_RADIAN(_fRadian) cosf((_fRadian))
#define DIR_Y_RADIAN(_fRadian) sinf((_fRadian))

#define CLAMP_RADIAN(_fRadian) ((_fRadian) -static_cast<int>((_fRadian) / TWO_PI) * TWO_PI)
#define CLAMP_DEGREE(_fDegree) ((_fDegree) - static_cast<int>((_fDegree) / 360.f) * 360.f)

// 비트마스크 플래그 관련
#define IS_PRESS_(_Key) (GetAsyncKeyState(_Key) & 0x8000)
#define ALL_FLAG(_State, _StateSize) (flags = (1 << (size * 8)) - 1)
#define EMPTY_FLAG(_State) (_State = 0)
#define ON_FLAG(_State, _Mask) (_State |= _Mask) 
#define OFF_FLAG(_State, _Mask) (_State &= ~_Mask)
#define TOGGLE_FLAG(_State, _Mask) (_State ^= _Mask)
#define IS_ON_FLAG(_State, _Mask) ((bool)((_State & _Mask) != 0))

// 키입력 관련
#define	KM_LEFT		0x00000001
#define	KM_RIGHT	0x00000002 
#define KM_UP		0x00000004
#define KM_DOWN		0x00000008

#define	KM_LBUTTON	0x00000010
#define	KM_RBUTTON	0x00000020 
#define KM_Q		0x00000040
#define KM_E		0x00000080

#define KM_W		0x00000100
#define KM_A		0x00000200
#define KM_S		0x00000400
#define KM_D		0x00000800

#define KM_LSHIFT	0x00001000
#define KM_P		0x00002000
#define KM_G		0x00004000
#define KM_SPACE	0x00008000

#define KM_R		0x00010000
#define KM_TAB		0x00020000
#define KM_M		0x00040000			// 미니맵 관련
#define KM_F		0x00080000			// 아이템과 상호작용

#define KM_Z		0x00100000			
#define KM_X		0x00200000

// 파일 입출력 관련


#ifdef _AFX

#define F_WRITE_DATA(_valueName, _fpOut)	WriteFile(_hfIn, &_valueName, sizeof(_valueName), nullptr, nullptr)
#define F_READ_DATA(_valueName, _fpIn)		ReadFile(_hfIn, &_valueName, sizeof(_valueName), nullptr, nullptr)
#define F_WRITE_HEAD(_valueName, _fpOut)	WriteFile(_hfIn, &_valueName, sizeof(_valueName), nullptr, nullptr)
#define F_READ_HEAD(_valueName, _fpIn)		ReadFile(_hfIn, &_valueName, sizeof(_valueName), nullptr, nullptr)

#else

#define F_WRITE_DATA(_valueName, _fpOut)	fwrite(&(_valueName), sizeof(_valueName), 1, _fpOut)
#define F_READ_DATA(_valueName, _fpIn)		fread(&(_valueName), sizeof(_valueName), 1, _fpIn)
#define F_WRITE_HEAD(_valueName, _fpOut)	fwrite(&(_valueName), sizeof(_valueName), 1, _fpOut)
#define F_READ_HEAD(_valueName, _fpIn)		fread(&(_valueName), sizeof(_valueName), 1, _fpIn)

#endif