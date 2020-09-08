// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
#include <d3d9.h>
#include <d3dx9.h>

#include <ctime>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
using namespace std;


#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Const.h"
#include "Extern.h"
#include "Function.h"

#include "MacroValue.h"
#include "CGraphicDevice.h"
#include "CKeyMgr.h"

#include "CObj.h"
#include "CSpriteObj.h"
#include "CUI_Button.h"