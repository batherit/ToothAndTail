#include "stdafx.h"
#include "Function.h"

// 해당 HWND에 대한 커서 포인터 얻기
POINT GetClientCursorPoint(const HWND& _hWND)
{
	POINT pt;
	GetCursorPos(&pt);				// 데스크탑 기준 좌표
	ScreenToClient(_hWND, &pt);	// 윈도우창 기준 좌표

	return pt;
}

// 오브젝트 충돌
bool IsCollided(const RECT& _rRect1, const RECT& _rRect2) {
	if (_rRect1.left > _rRect2.right) return false;
	if (_rRect1.right < _rRect2.left) return false;
	if (_rRect1.top > _rRect2.bottom) return false;
	if (_rRect1.bottom < _rRect2.top) return false;
	return true;
}

bool IsCollided(const CObj* _pObj1, const CObj* _pObj2) {
	if (!IS_VALID_OBJ(_pObj1) || !IS_VALID_OBJ(_pObj2)) return false;
	if (_pObj1->GetLeft() > _pObj2->GetRight()) return false;
	if (_pObj1->GetRight() < _pObj2->GetLeft()) return false;
	if (_pObj1->GetTop() > _pObj2->GetBottom()) return false;
	if (_pObj1->GetBottom() < _pObj2->GetTop()) return false;
	return true;
}
bool IsCollided(const CObj* _pObj1, const CObj* _pObj2, RECT& _rCollidedPoint) {
	if (!IS_VALID_OBJ(_pObj1) || !IS_VALID_OBJ(_pObj2)) return false;
	return IntersectRect(&_rCollidedPoint, &(_pObj1->GetRect()), &(_pObj2->GetRect()));
}

bool IsPointInRect(const RECT & _rRect, const POINT & _rPoint)
{
	return (_rRect.left <= _rPoint.x && _rPoint.x <= _rRect.right) && (_rRect.top <= _rPoint.y && _rPoint.y <= _rRect.bottom);
}

void PushObjectInRect(CObj& _pObj, const RECT& _rRect)
{
	if (_pObj.GetLeft() <= _rRect.left)		_pObj.SetX(static_cast<float>(_rRect.left + (_pObj.GetWidth() >> 1)));
	if (_pObj.GetTop() <= _rRect.top)		_pObj.SetY(static_cast<float>(_rRect.top + (_pObj.GetHeight() >> 1)));
	if (_pObj.GetRight() >= _rRect.right)	_pObj.SetX(static_cast<float>(_rRect.right - (_pObj.GetWidth() >> 1)));
	if (_pObj.GetBottom() >= _rRect.bottom)	_pObj.SetY(static_cast<float>(_rRect.bottom - (_pObj.GetHeight() >> 1)));
}

bool MyIntersectRect(const RECT & _rRect1, const RECT & _rRect2, RECT* _pCollidedPoint)
{
	if (!IsCollided(_rRect1, _rRect2)) return false;

	if (_rRect1.left <= _rRect2.right) {
		if (_rRect1.left <= _rRect2.left) {
			if (_pCollidedPoint)
				_pCollidedPoint->left = _rRect2.left;
		}
		else {
			if (_pCollidedPoint)
				_pCollidedPoint->left = _rRect1.left;
		}
	}
	if (_rRect1.right >= _rRect2.left) {
		if (_rRect1.right >= _rRect2.right) {
			if (_pCollidedPoint)
				_pCollidedPoint->right = _rRect2.right;
		}
		else {
			if (_pCollidedPoint)
				_pCollidedPoint->right = _rRect1.right;
		}
	}
	if (_rRect1.top <= _rRect2.bottom) {
		if (_rRect1.top <= _rRect2.top) {
			if (_pCollidedPoint)
				_pCollidedPoint->top = _rRect2.top;
		}
		else {
			if (_pCollidedPoint)
				_pCollidedPoint->top = _rRect1.top;
		}
	}
	if (_rRect1.bottom >= _rRect2.top) {
		if (_rRect1.bottom >= _rRect2.bottom) {
			if (_pCollidedPoint)
				_pCollidedPoint->bottom = _rRect2.bottom;
		}
		else {
			if (_pCollidedPoint)
				_pCollidedPoint->bottom = _rRect1.bottom;
		}
	}

	return true;
}

// 랜덤 유틸
float GetRandomFloat(void)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int GetNumberMinBetweenMax(int _iMin, int _iMax) {
	if (_iMin > _iMax) abort();

	return rand() % (_iMax - _iMin + 1) + _iMin;
}


float GetNumberMinBetweenMax(float _fMin, float _fMax) {
	if (_fMin > _fMax) abort();

	float fT = GetRandomFloat();
	return _fMin * (1.f - fT) + _fMax * fT;
}



// 가중치
float GetWeightByDegree(float _fDegree)
{
	return cosf(TO_RADIAN(_fDegree)) * 0.5f + 0.5f;
}

// 벡터
void NormalizeVector(float & _fToX, float & _fToY)
{
	float fVectorLength = sqrtf(_fToX *_fToX + _fToY * _fToY);
	if (fVectorLength == 0) {
		_fToX = 0;
		_fToY = 0;
		return;
	}
	_fToX /= fVectorLength;
	_fToY /= fVectorLength;
}

float GetVectorLength(const float & _fToX, const float & _fToY)
{
	return sqrtf(_fToX * _fToX + _fToY * _fToY);
}

// 각
float GetRadianByVector(float _fToX, float _fToY)
{
	float fRadian = acosf(_fToX / GetVectorLength(_fToX, _fToY));
	if (_fToY < 0) fRadian *= -1.f;

	return fRadian;
}

float GetPositiveDegreeByVector(float _fToX, float _fToY) {
	float fDegree = TO_DEGREE(GetRadianByVector(_fToX, _fToY));
	if (fDegree < 0.f) fDegree += 360.f;
	return fDegree;
}

OBJ::E_DIRECTION GetDirByDegree(float _fDegree, float _fWidth, float _fHeight, float _fWeight) {
	Clamp(&_fWeight, 0.f, 1.f);

	float fWidthDegree = 90.f * (_fWidth / (_fWidth + _fHeight));
	float fHeightDegree = 90.f - fWidthDegree;

	if (_fWidth > _fHeight) {
		fWidthDegree = 90.f * (_fWidth * (1.f - _fWeight) + (_fWidth + _fHeight) * _fWeight) / (_fWidth + _fHeight);
		fHeightDegree = 90.f - fWidthDegree;
	}
	else if (_fWidth < _fHeight) {
		fHeightDegree = 90.f * (_fHeight * (1.f - _fWeight) + (_fWidth + _fHeight) * _fWeight) / (_fWidth + _fHeight);
		fWidthDegree = 90.f - fHeightDegree;
	}


	if (fHeightDegree <= _fDegree && _fDegree < fHeightDegree + 2.f * fWidthDegree) {
		return OBJ::DIR_DOWN;
	}
	else if (fHeightDegree + 2.f * fWidthDegree <= _fDegree && _fDegree < 3.f * fHeightDegree + 2.f * fWidthDegree) {
		return OBJ::DIR_LEFT;
	}
	else if (3.f * fHeightDegree + 2.f * fWidthDegree <= _fDegree && _fDegree < 3.f * fHeightDegree + 4.f * fWidthDegree) {
		return OBJ::DIR_UP;
	}
	else return OBJ::DIR_RIGHT;
}

bool IsPointInPolygon(const D3DXVECTOR3 & _vPoint, const D3DXVECTOR3 _vPolygonPointsArr[], int _iPolygonPointsNum)
{
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vNormal;
	float fDot = 0.f;
	for (int i = 0; i < _iPolygonPointsNum; i++) {
		// 방향 벡터를 구한다. (법선 벡터를 구하기 위함이다.)
		vDir = _vPolygonPointsArr[(i + 1) % 4] - _vPolygonPointsArr[i % 4];
		D3DXVec3Normalize(&vDir, &vDir);
		// 법선 벡터를 구한다. (2차원에서만 가능, 3차원은 외적을 통해 구한다.)
		vNormal = D3DXVECTOR3(vDir.y, -vDir.x, 0.f); // root (vDir.y^2 + vDir.x^2)이므로 단위벡터
		// 꼭짓점에서 커서 좌표로의 방향 벡터를 구한다.
		vDir = _vPoint - _vPolygonPointsArr[i % 4];
		D3DXVec3Normalize(&vDir, &vDir);
		// 내적을 구한다.
		fDot = D3DXVec3Dot(&vDir, &vNormal);
		// 내적이 양수이면 마름모 밖에 점이 존재한다는 의미이다. 즉, false를 반환한다.
		if (fDot > 0.f) return false;
	}
	return true;
}

bool IsPointInTile(const D3DXVECTOR3 & _vPoint, const D3DXVECTOR3 & vTilePos, const float & _fTileWidth, const float & _fTileHeight)
{
	D3DXVECTOR3 vTilePointsArr[4] = {
		D3DXVECTOR3(vTilePos.x, vTilePos.y - _fTileHeight * 0.5f, 0.f),
		D3DXVECTOR3(vTilePos.x + _fTileWidth * 0.5f, vTilePos.y, 0.f),
		D3DXVECTOR3(vTilePos.x, vTilePos.y + _fTileHeight * 0.5f, 0.f),
		D3DXVECTOR3(vTilePos.x - _fTileWidth * 0.5f, vTilePos.y, 0.f)
	};

	return IsPointInPolygon(_vPoint, vTilePointsArr, 4);
}

CString ConvertToRelativePath(const CString & strAbsolutePath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szCurDirectory[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurDirectory);
	PathRelativePathTo(szRelativePath, szCurDirectory, FILE_ATTRIBUTE_DIRECTORY, strAbsolutePath, FILE_ATTRIBUTE_DIRECTORY);

	//절대 경로 : 어떤 지점까지 경로 다나옴
	//상대 경로 : 현재 경로에서 어떤 지점까지 경로 다나옴
	//현재 경로 : 현재 플젝있는 곳까지 경로 다나옴
	return CString(szRelativePath);
}

void AdjustHorizontalScroll(CListBox & _rListBox)
{
	CString strName;
	CSize tSize;
	int iCX = 0;

	CDC* pDC = _rListBox.GetDC();
	for (int i = 0; i < _rListBox.GetCount(); ++i)
	{
		// 리스트박스로부터 i번째 인덱스의 문자열 얻기
		_rListBox.GetText(i, strName);

		// 문자열의 가로 픽셀 길이 얻기
		tSize = pDC->GetTextExtent(strName);

		// 가장 긴 가로 픽셀 길이(iCX) 구하기
		if (tSize.cx > iCX)
			iCX = tSize.cx;
	}
	_rListBox.ReleaseDC(pDC);

	// 가로 스크롤 사이즈 조정
	if (_rListBox.GetHorizontalExtent() < iCX)
		_rListBox.SetHorizontalExtent(iCX);
}
