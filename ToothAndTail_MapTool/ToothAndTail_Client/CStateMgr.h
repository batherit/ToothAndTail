#pragma once
#include "CState.h"

template<typename T>
class CStateMgr final
{
public:
	CStateMgr(CGameWorld& _rGameWorld, T& _rOwner);
	~CStateMgr();

public:
	bool ConfirmValidState(void);				// ���� ���°� �����Ѵٸ�, �� �������� �����Ѵ�.
	void SetNextState(CState<T>* _pNextScene, bool _bIsForced = false);
	void Update(float _fDeltaTime);
	void LateUpdate(void);
	void Release(void);

private:
	CGameWorld& m_rGameWorld;
	T& m_rOwner;
	CState<T>* m_pCurState = nullptr;
	CState<T>* m_pNextState = nullptr;
	bool m_bIsConfirmed = true;
};

template<typename T>
CStateMgr<T>::CStateMgr(CGameWorld & _rGameWorld, T& _rOwner)
	:
	m_rGameWorld(_rGameWorld),
	m_rOwner(_rOwner),
	m_bIsConfirmed(true)		// ��.��.��. �ʱ⿡�� true�� �������־�� ��.
{
}

template<typename T>
inline CStateMgr<T>::~CStateMgr()
{
	Release();
}

template<typename T>
bool CStateMgr<T>::ConfirmValidState(void)
{
	if (!m_bIsConfirmed) {				// ���� ���� ��û�� ������ ���
		if (m_pCurState) m_pCurState->OnExited();		// ���ֱ� �� ���¸� �����Ѵ�.
		SafelyDeleteObj(m_pCurState);		// ���� ���¸� �����. 
		m_pCurState = m_pNextState;		// ���� ���¸� ���ο� ���·� ��ü�Ѵ�.
		if (m_pCurState) m_pCurState->OnLoaded();		// ���� �غ�
		m_pNextState = nullptr;			// ���ο� ���� ���� ��ȿȭ
		m_bIsConfirmed = true;			// ���� ���� �Ϸ�!
	}
	if (!m_pCurState) return false;		// �׷����� ���� ��ȿ�� ���� ���ٸ� false�� ��ȯ
	return true;						// ��� ���μ����� ����
}

template<typename T>
void CStateMgr<T>::SetNextState(CState<T> * _pNextScene, bool _bIsForced)
{
	// ���� ���� �̹� ���õ� ���, �ش� �Լ��� �����Ѵ�.
	// => ���� ���� ������� ���ο� ���·� ������ �� ����.
	// for ���¸� ����ȭ���ֱ� ����.
	if (!m_bIsConfirmed && !_bIsForced) {
		SafelyDeleteObj(_pNextScene);	// ���� ���´� �����Ѵ�.
		return;						// ������ ���� ������ ��û�� �������� ���� ������� ���� ���
	}

	m_pNextState = _pNextScene;		// ������ ���� ���� ��û�� ���� ���.
	m_bIsConfirmed = false;			// ���� ��û�� ����.
}

template<typename T>
void CStateMgr<T>::Update(float _fDeltaTime)
{
	if (m_pCurState) m_pCurState->Update(_fDeltaTime);
}

template<typename T>
void CStateMgr<T>::LateUpdate(void)
{
	if (m_pCurState) m_pCurState->LateUpdate();
}

template<typename T>
void CStateMgr<T>::Release(void)
{
	SafelyDeleteObj(m_pCurState);
	SafelyDeleteObj(m_pNextState);
	m_bIsConfirmed = true;
}

