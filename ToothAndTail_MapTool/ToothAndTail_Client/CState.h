#pragma once

template<typename T>
class CState abstract
{
public:
	CState(CGameWorld& _rGameWorld, T& _rOwner);
	virtual ~CState();

public:
	virtual void OnLoaded(void) = 0;
	virtual int Update(float _fDeltaTime) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void OnExited(void) = 0;
	virtual int AlwaysUpdate(float _fDeltaTime) { return 0; }

protected:
	CGameWorld& m_rGameWorld;
	T& m_rOwner;
};

template<typename T>
inline CState<T>::CState(CGameWorld& _rGameWorld, T& _rOwner)
	:
	m_rGameWorld(_rGameWorld),
	m_rOwner(_rOwner)
{

}

template<typename T>
inline CState<T>::~CState()
{
}
