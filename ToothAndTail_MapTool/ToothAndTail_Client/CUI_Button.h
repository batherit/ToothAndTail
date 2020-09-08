#pragma once
#include "CObj.h"

template<typename T>
class CUI_Button :
	public CObj
{
public:
	enum BUTTON_STATE { 
		BUTTON_STATE_CLICKED,		// 클릭직후
		BUTTON_STATE_PRESSED,		// 누르고 있는 동안
		BUTTON_STATE_RELEASED,		// 누른걸 뗀 직후
		BUTTON_STATE_HOVERED,		// 누르지 않은 상태로 버튼 위에 커서 존재
		BUTTON_STATE_UNHOVERED,		// 드래그 되지 않은 상태에서 버튼 밖에 위치
		BUTTON_STATE_END };

public:
	CUI_Button(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight/*, const TCHAR* _szButtonName*/, T* _pOwner = nullptr, void (T::*_pEvent) (void*) = nullptr, void* _pArgu = nullptr);
	~CUI_Button();

public:
	virtual int Update(float _fDeltaTime) override;
	//virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;
	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override
	{
	}
	virtual void LateUpdate(void) override
	{
	}
	virtual void Release(void) override
	{
		for (int i = 0; i < BUTTON_STATE_END; i++) {
			ReleaseEvent(static_cast<BUTTON_STATE>(i));
		}
	}
	void SetEvent(BUTTON_STATE _eButtonState, void (T::*_pEvent) (void*), void* _pArgu);
	void ReleaseEvent(BUTTON_STATE _eButtonState);

private:
	T* m_pOwner;
	void(T::*m_pEvent[BUTTON_STATE_END]) (void*);
	void* m_pArgu[BUTTON_STATE_END];
	/*TCHAR m_szButtonName[64];*/
	bool m_bIsClicked = false;

	
};

template<typename T>
inline CUI_Button<T>::CUI_Button(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight/*, const TCHAR* _szButtonName*/, T* _pOwner, void (T::*_pEvent) (void*), void* _pArgu)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight),
	m_pOwner(_pOwner)
{
	ZeroMemory(m_pEvent, sizeof(m_pEvent));
	ZeroMemory(m_pArgu, sizeof(m_pArgu));
	m_pEvent[BUTTON_STATE_CLICKED] = _pEvent;
	m_pArgu[BUTTON_STATE_CLICKED] = _pArgu;
	//wcscpy_s(m_szButtonName, _szButtonName);
}

template<typename T>
inline CUI_Button<T>::~CUI_Button()
{
	Release();
}

template<typename T>
int CUI_Button<T>::Update(float _fDeltaTime)
{
	POINT pt = GetClientCursorPoint(g_hWND);

	if (IsPointInRect(GetRect(), pt))
	{
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
			// Clicked
			if (m_pOwner && m_pEvent[BUTTON_STATE_CLICKED])
				(m_pOwner->*m_pEvent[BUTTON_STATE_CLICKED])(m_pArgu[BUTTON_STATE_CLICKED]);
			m_bIsClicked = true;
			return 0;
		}
		else {
			if (!m_bIsClicked) {
				// Hovered
				if (m_pOwner && m_pEvent[BUTTON_STATE_HOVERED])
					(m_pOwner->*m_pEvent[BUTTON_STATE_HOVERED])(m_pArgu[BUTTON_STATE_HOVERED]);
				return 0;
			}
		}
	}
	else if (!m_bIsClicked) {
		// Unhovered
		if (m_pOwner && m_pEvent[BUTTON_STATE_UNHOVERED])
			(m_pOwner->*m_pEvent[BUTTON_STATE_UNHOVERED])(m_pArgu[BUTTON_STATE_UNHOVERED]);
		return 0;
	}

	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_LBUTTON)) {
		if (m_bIsClicked) {
			// Pressed
			if (m_pOwner && m_pEvent[BUTTON_STATE_PRESSED])
				(m_pOwner->*m_pEvent[BUTTON_STATE_PRESSED])(m_pArgu[BUTTON_STATE_PRESSED]);
			return 0;
		}
	}
	if (CKeyMgr::GetInstance()->IsKeyUp(KEY::KEY_LBUTTON)) {
		if (m_bIsClicked) {
			// Released 
			if (m_pOwner && m_pEvent[BUTTON_STATE_RELEASED])
				(m_pOwner->*m_pEvent[BUTTON_STATE_RELEASED])(m_pArgu[BUTTON_STATE_RELEASED]);
			m_bIsClicked = false;
			return 0;
		}
	}

	return 0;
}

//template<typename T>
//void CUI_Button<T>::Render(HDC & _hdc, CCamera * _pCamera)
//{
//	CObj::Render(_hdc);
//	TextOut(_hdc, GetLeft() + 5, GetTop() + 5, m_szButtonName, lstrlen(m_szButtonName));
//}

template<typename T>
inline void CUI_Button<T>::SetEvent(BUTTON_STATE _eButtonState, void(T::* _pEvent)(void *), void * _pArgu)
{
	ReleaseEvent(_eButtonState);
	m_pEvent[_eButtonState] = _pEvent;
	m_pArgu[_eButtonState] = _pArgu;
}

template<typename T>
inline void CUI_Button<T>::ReleaseEvent(BUTTON_STATE _eButtonState)
{
	SafelyDeleteObj(m_pArgu[_eButtonState]);
	m_pEvent[_eButtonState] = nullptr;
}
