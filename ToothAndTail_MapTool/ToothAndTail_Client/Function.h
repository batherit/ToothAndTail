#pragma once

class CObj;

// 해당 HWND에 대한 커서 포인터 얻기
POINT GetClientCursorPoint(const HWND& _hWND);

// 오브젝트 충돌
bool IsCollided(const RECT& _rRect1, const RECT& _rRect2);
bool IsCollided(const CObj* _pObj1, const CObj* _pObj2);
bool IsCollided(const CObj* _pObj1, const CObj* _pObj2, RECT& _rCollidedPoint);
bool IsPointInRect(const RECT & _rRect, const POINT & _rPoint);
bool IsPointInRect(const RECT& _rRect, const D3DXVECTOR3& _rPoint);
void PushObjectInRect(CObj& _pObj, const RECT& _rRect);
bool MyIntersectRect(const RECT & _rRect1, const RECT & _rRect2, RECT* _pCollidedPoint);

// 랜덤 유틸
float GetRandomFloat(void);
int GetNumberMinBetweenMax(int _iMin, int _iMax);
float GetNumberMinBetweenMax(float _fMin, float _fMax);

// 가중치
float GetWeightByDegree(float _fDegree);

// 벡터
void NormalizeVector(float & _fToX, float & _fToY);
float GetVectorLength(const float & _fToX, const float & _fToY);

// 각
float GetRadianByVector(float _fToX, float _fToY);
float GetRadianByVector(const D3DXVECTOR3& _vDir);
float GetPositiveDegreeByVector(float _fToX, float _fToY);
float GetPositiveDegreeByVector(const D3DXVECTOR3& _vDir);
OBJ::E_DIRECTION GetDirByDegree(float _fDegree, float _fWidth, float _fHeight, float _fWeight);
OBJ::E_DIRECTION GetDirByDegree(float _fDegree);
OBJ::E_DIRECTION GetDirByVector(const D3DXVECTOR3& _vDir);

// 메모리 해제
template<class T>
void SafelyReleaseCOM(T& rObj) {
	if (rObj) {
		rObj->Release();
	}
}
template<typename T>
void SafelyDeleteObj(T& rObj) {
	if (rObj) {
		delete rObj;
		rObj = nullptr;
	}
}

template<typename T>
void SafelyDeleteObjs(list<T>& _list) {
	for (auto& pObj : _list) { SafelyDeleteObj(pObj); }
	_list.clear();
}

template<typename T>
void SafelyDeleteObjs(vector<T>& _vector) {
	for (auto& pObj : _vector) { SafelyDeleteObj(pObj); }
	_vector.clear();
	_vector.shrink_to_fit();
}

template<typename T>
void CollectGarbageObjs(list<T*>& _list)
{
	for (auto& pObj : _list) { DO_IF_IS_NOT_VALID_OBJ(pObj) { SafelyDeleteObj(pObj); } }
	_list.remove_if([](auto& pObj) { return pObj == nullptr; });
}

template<typename T>
void CollectGarbageObjs(vector<T*>& _vector)
{
	for (auto& pObj : _vector) { DO_IF_IS_NOT_VALID_OBJ(pObj) { SafelyDeleteObj(pObj); } }
	remove_if(_vector.begin(), _vector.end(), [](auto& pObj) { return pObj == nullptr; });
}

// 클램프
template<typename T>
void Clamp(T* _pValue, T _Min, T _Max) {
	if (*_pValue < _Min)		*_pValue = _Min;
	else if (*_pValue > _Max)	*_pValue = _Max;
}
template<typename T>
T Clamp(T _Value, T _Min, T _Max) {
	Clamp(&_Value, _Min, _Max);
	return _Value;
}

// 2차 베지어 곡선
template<typename T>
T GetQuadraticBezierCurve(float _fT, T _p1, T _p2, T _p3) {
	Clamp(&_fT, 0.f, 1.f);
	return _p1 * (1.f - _fT) * (1.f - _fT) + _p2 * 2.f * _fT * (1.f - _fT) + _p3 * _fT * _fT;
}

bool IsPointInPolygon(const D3DXVECTOR3& _vPoint, const D3DXVECTOR3 _vPolygonPointsArr[], int _iPolygonPointsNum);
bool IsPointInTile(const D3DXVECTOR3& _vPoint, const D3DXVECTOR3& vTilePos, const float& _fTileWidth, const float& _fTileHeight);

// MFC
//CString ConvertToRelativePath(const CString& strAbsolutePath);
//void AdjustHorizontalScroll(CListBox& _rListBox);