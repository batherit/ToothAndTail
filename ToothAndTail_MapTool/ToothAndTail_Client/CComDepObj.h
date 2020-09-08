#pragma once
#include "CSpriteObj.h"

class CCommander;
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
	bool IsDead() const { return m_iHP <= 0; }
	void TakeDamage(int _iDamageAmount) { m_iHP -= _iDamageAmount; if (m_iHP < 0) m_iHP = 0; }
	void UpdateSpriteDir(void);
	void SetMinimapSign(MINIMAP::E_SIGN _eMinimapSign) { m_eMinimapSign = _eMinimapSign; }
	MINIMAP::E_SIGN GetMinimapSign() const { return m_eMinimapSign; }

protected:
	void SetID(int _iID) { m_iID = _iID; }
	void SetTileSiteInfo(const TileSiteInfo& _rTilesiteInfo) { m_tTilesiteInfo = _rTilesiteInfo; }

private:
	int m_iID = -1;
	CCommander* m_pCommander = nullptr;
	CSpriteObj* m_pIdentificationTintSprite = nullptr; // �� ���� ���� ��ü
	D3DXVECTOR3 m_vTargetPos;

	float m_fDetectionRange = 100.f;
	CComDepObj* m_pTargetEnemy = nullptr;
	int m_iHP = 100;
	
	TileSiteInfo m_tTilesiteInfo;
	MINIMAP::E_SIGN m_eMinimapSign = MINIMAP::SIGN_NONE;
};

