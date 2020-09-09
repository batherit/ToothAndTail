#pragma once
#include "CSpriteObj.h"

class CCommander;
class CUI_UnitHP;
class CComDepObj :
	public CSpriteObj
{
public:
	CComDepObj(CGameWorld& _rGameWorld, CCommander* _pCommander, float _fX = 0, float _fY = 0, size_t _iWidth = 10, size_t _iHeight = 10, float _fToX = 1.f, float _fToY = 0.f, float _fSpeed = 0.f, int _iID = -1);
	virtual ~CComDepObj();

public:
	virtual void Render(CCamera* _pCamera) override;
	virtual void SetNewAnimInfo(const AnimInfo& _stAnimInfo) override;
	virtual int UpdateAnim(float _fDeltaTime);
	virtual void Release(void) override;
	virtual void CollectGarbageObjs() { DO_IF_IS_NOT_VALID_OBJ(m_pTargetEnemy) m_pTargetEnemy = nullptr; };

	// ���� ������ �������� �ʰ�, ��ȿȭ ó���Ͽ� �����ӿ�ũ�� �˾Ƽ� �����ϵ��� ��.
	// ��, GameWorld�� GameObjsList�� ���ؼ� �۵��Ѵ�.
	virtual void InvalidateObj();

public:
	void GenerateIdentificationTintObj(size_t _iWidth, size_t _iHeight, const wstring& _wstrTintKey, D3DXCOLOR _clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DCOLOR GetIdentificationTint(void) const;

	// ��� ����
	int GetID(void) const { return m_iID; }
	virtual void SetCommander(CCommander* _pCommander, D3DXCOLOR _clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255));
	CCommander* GetCommander(void) const { return m_pCommander; }
	void SetTargetPos(const D3DXVECTOR3& _vTargetPos) { m_vTargetPos = _vTargetPos; }

	// Ÿ�� ������ ��ġ�ߴ���? �̵� ���θ� ������ �� ���δ�.
	// �� �Լ��� ȣ���ϱ� ������ ��ȿ�� ��ǥ������ �����صΰ� �־�� �Ѵ�.
	bool IsLocatedAtTargetPoint(void) const;		
	// ��ǥ �������� �̵��� �� ���δ�.
	bool GoToTargetPoint(float _fDeltaTime);		
	// ���� ����
	// 1) ���� ���� �����ϱ�
	void SetDetectionRange(float _fDetectionRange) { m_fDetectionRange = _fDetectionRange; }
	// 2) �ֺ� ������ �� ����(�Ÿ���)
	void DetectEnemyAround();
	void SetTargetEnemy(CComDepObj* _pTargetEnemy) { m_pTargetEnemy = _pTargetEnemy; }
	CComDepObj* GetTargetEnemy() const { return m_pTargetEnemy; }
	bool CanAttackTargetEnemy();
	bool IsDead() const { return m_fHP <= 0.f; }
	void TakeDamage(float _fDamageAmount);
	void UpdateSpriteDir(void);
	void SetMinimapSign(MINIMAP::E_SIGN _eMinimapSign) { m_eMinimapSign = _eMinimapSign; }
	MINIMAP::E_SIGN GetMinimapSign() const { return m_eMinimapSign; }
	float GetHPRatio() const { return m_fHP / m_fMaxHP; }

protected:
	void SetID(int _iID) { m_iID = _iID; }
	void SetTileSiteInfo(const TileSiteInfo& _rTilesiteInfo) { m_tTilesiteInfo = _rTilesiteInfo; }
	void InitHP(float _fHP) { m_fMaxHP = _fHP; if (m_fMaxHP < 0.f) m_fMaxHP = 0.f; m_fHP = m_fMaxHP; }
	void SetHP(float _fHP) { m_fHP = _fHP; Clamp(&m_fHP, 0.f, m_fMaxHP); }
	float GetHP() const { return m_fHP; }
	void SetMaxHP(float _fMaxHP) { m_fMaxHP = _fMaxHP; if (m_fMaxHP < 0.f) m_fMaxHP = 0.f; Clamp(&m_fHP, 0.f, m_fMaxHP); }
	float GetMaxHP() const { return m_fMaxHP; }
	CUI_UnitHP* GetUIUnitHP() const { return m_pUIUnitHP; }
	bool IsFullHP() const { return m_fHP >= m_fMaxHP; }

private:
	int m_iID = -1;
	CCommander* m_pCommander = nullptr;
	CSpriteObj* m_pIdentificationTintSprite = nullptr; // �� ���� ���� ��ü
	D3DXVECTOR3 m_vTargetPos;

	float m_fDetectionRange = 100.f;
	CComDepObj* m_pTargetEnemy = nullptr;
	float m_fHP = 100.f;
	float m_fMaxHP = 100.f;
	CUI_UnitHP* m_pUIUnitHP = nullptr;
	
	TileSiteInfo m_tTilesiteInfo;
	MINIMAP::E_SIGN m_eMinimapSign = MINIMAP::SIGN_NONE;
};

