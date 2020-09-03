#pragma once
#include "CScene.h"

class CSpriteObj;
class CCommander;
class CMapLoader;
class CTestScene :
	public CScene
{
public:
	CTestScene(CGameWorld& _rGameWorld);
	virtual ~CTestScene();

	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

private:
	//CSpriteObj* m_pMap = nullptr;
	CCommander* m_pCommander[4] = { nullptr, };
};

