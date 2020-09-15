#pragma once
#include "CScene.h"

class CSpriteObj;
class CCommander;
class CMapLoader;
class CUI_InGameUI;
class CPlayScene :
	public CScene
{
public:
	CPlayScene(CGameWorld& _rGameWorld);
	virtual ~CPlayScene();

	// CScene��(��) ���� ��ӵ�
	virtual void ResetScene(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

private:
	//CSpriteObj* m_pMap = nullptr;
	CCommander* m_pCommander[4] = { nullptr, };
	CUI_InGameUI* m_pInGameUI = nullptr;
};
