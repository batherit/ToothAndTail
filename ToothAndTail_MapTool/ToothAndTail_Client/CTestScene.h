#pragma once
#include "CScene.h"
class CTestScene :
	public CScene
{
public:
	CTestScene(CGameWorld& _rGameWorld);
	virtual ~CTestScene();

	// CScene��(��) ���� ��ӵ�
	virtual void ResetScene(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
};

