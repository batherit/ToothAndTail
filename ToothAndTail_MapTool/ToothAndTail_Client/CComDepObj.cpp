#include "stdafx.h"
#include "CComDepObj.h"
#include "CCommander.h"
#include "CTextureMgr.h"
#include "CGameWorld.h"
#include "CMapLoader.h"
#include "CUI_UnitHP.h"
#include "CWindmill.h"
#include "CTile.h"



CComDepObj::CComDepObj(CGameWorld & _rGameWorld, CCommander* _pCommander, float _fX, float _fY, size_t _iWidth, size_t _iHeight, float _fToX, float _fToY, float _fSpeed, int _iID)
	:
	CSpriteObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, _fToX, _fToY, _fSpeed),
	m_pCommander(_pCommander),
	m_iID(_iID),
	m_pUIUnitHP(new CUI_UnitHP(_rGameWorld, this))
{
	m_vecCollidedUnits.reserve(100);
	m_vecCollidedBlockingTiles.reserve(10);
}

CComDepObj::~CComDepObj()
{
	Release();
}

void CComDepObj::Render(CCamera * _pCamera)
{
	CSpriteObj::Render(_pCamera);
	if (m_pIdentificationTintSprite)
		m_pIdentificationTintSprite->Render(_pCamera);
}

void CComDepObj::SetNewAnimInfo(const AnimInfo & _stAnimInfo)
{
	CSpriteObj::SetNewAnimInfo(_stAnimInfo);
	if (m_pIdentificationTintSprite)
		m_pIdentificationTintSprite->SetNewAnimInfo(_stAnimInfo);
}

int CComDepObj::UpdateAnim(float _fDeltaTime)
{
	if (m_pIdentificationTintSprite)
		m_pIdentificationTintSprite->UpdateAnim(_fDeltaTime);
	return CSpriteObj::UpdateAnim(_fDeltaTime);
}

void CComDepObj::Release(void)
{
	SafelyDeleteObj(m_pIdentificationTintSprite);
	SafelyDeleteObj(m_pUIUnitHP);
	m_vecCollidedUnits.clear();
	m_vecCollidedUnits.shrink_to_fit();
	m_vecCollidedBlockingTiles.clear();
	m_vecCollidedBlockingTiles.shrink_to_fit();
}

void CComDepObj::InvalidateObj()
{
	if (m_tTilesiteInfo.iTileLineIndex != -1) {
		GetGameWorld().GetMapLoader()->SetSiteType(m_tTilesiteInfo, TILE::TYPE_NORMAL);
	}
	SetValid(false);
}

void CComDepObj::GenerateIdentificationTintObj(size_t _iWidth, size_t _iHeight, const wstring & _wstrTintKey, D3DXCOLOR _clIdentificationTint)
{
	if (m_pIdentificationTintSprite) SafelyDeleteObj(m_pIdentificationTintSprite);
	// �� ������ ��Ÿ���� ƾƮ �ؽ�ó ����
	m_pIdentificationTintSprite = new CSpriteObj(GetGameWorld(), 0.f, 0.f, _iWidth, _iHeight);
	m_pIdentificationTintSprite->SetParent(this);
	m_pIdentificationTintSprite->SetRenderOffsetXY(GetRenderOffsetXY());
	if(m_pCommander && (m_pCommander != this))
		m_pIdentificationTintSprite->SetColor(m_pCommander->GetIdentificationTint());
	else
		m_pIdentificationTintSprite->SetColor(_clIdentificationTint);
	m_pIdentificationTintSprite->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(_wstrTintKey));
}

D3DCOLOR CComDepObj::GetIdentificationTint(void) const
{
	D3DCOLOR clIdentificationTint = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (m_pIdentificationTintSprite)
		clIdentificationTint = m_pIdentificationTintSprite->GetRenderColor();
	else if (m_pCommander && m_pCommander != this) {
		clIdentificationTint = m_pCommander->GetIdentificationTint();
	}
	return clIdentificationTint;
}

void CComDepObj::SetCommander(CCommander * _pCommander, D3DXCOLOR _clIdentificationTint){
	m_pCommander = _pCommander;
	if (m_pCommander && (m_pCommander != this)) {
		if (m_pIdentificationTintSprite)
			if (m_pCommander)
				m_pIdentificationTintSprite->SetColor(_pCommander->GetIdentificationTint());
			else
				m_pIdentificationTintSprite->SetColor(_clIdentificationTint);
		return;
	}
	if (m_pIdentificationTintSprite) m_pIdentificationTintSprite->SetColor(_clIdentificationTint);
}

bool CComDepObj::IsLocatedAtTargetPoint(void) const
{
	D3DXVECTOR3 vToTarget = m_vTargetPos - GetXY();
	float fLength = D3DXVec3Length(&vToTarget);

	return fLength <= STOP_RANGE;
}

bool CComDepObj::GoToTargetPoint(float _fDeltaTime)
{
	if (IsLocatedAtTargetPoint()) return false;

	D3DXVECTOR3 vMainDir(0.f, 0.f, 0.f);
	D3DXVECTOR3 vResistanceDir(0.f, 0.f, 0.f);
	D3DXVECTOR3 vToTarget(0.f, 0.f, 0.f);
	// ���� �ӵ��� ���Ѵ�.
	for (auto& pCollidedUnit : m_vecCollidedUnits) {
		D3DXVec3Normalize(&vToTarget, &(GetXY() - pCollidedUnit->GetXY()));
		vResistanceDir += vToTarget;
	}

	//if (m_pCollidedWindmill) {
	//	D3DXVec3Normalize(&vToTarget, &(GetXY() - m_pCollidedWindmill->GetXY()));
	//	vResistanceDir += (vToTarget + GetToXY())/* * 5.5f*/;


	//}
	//else {
		for (auto& pBlockingTile : m_vecCollidedBlockingTiles) {
			D3DXVec3Normalize(&vToTarget, &(GetXY() - pBlockingTile->GetXY()));
			vResistanceDir += (vToTarget + GetToXY()) /** 4.5f*/;
			pBlockingTile->PushOutOfTile(this);
		}
	//}

	// ��ħ�� �������� ������ �پ���.
	if (!m_vecCollidedUnits.empty()) {
		vResistanceDir /= (m_vecCollidedUnits.size());
	}
	// Ÿ�� ���͸� ���Ѵ�.
	D3DXVec3Normalize(&vToTarget, &(m_vTargetPos - GetXY()));
	// Ÿ�� ���Ϳ� ���� �ӵ��� ���Ѵ�.
	vMainDir = vToTarget + vResistanceDir;
	// ���� ������ ���Ѵ�.
	D3DXVec3Normalize(&vMainDir, &vMainDir);
	// �ӷ��� �����Ѵ�.
	float fT = D3DXVec3Dot(&GetToXY(), &vMainDir) * 0.5f + 0.5f;
	SetSpeed(GetMaxSpeed() * fT);

	SetToXY(vMainDir);
	
	UpdateSpriteDir();
	MoveByDeltaTime(_fDeltaTime);

	return true;
}

bool CComDepObj::GoToTarget(float _fDeltaTime)
{
	if (CanAttackTargetEnemy()) return false;	// ������ �� �ִ� �Ÿ����� ������ false�� ��ȯ�Ѵ�.
	if (!m_pTargetEnemy) return false;			// Ÿ���� ���ٸ� false�� ��ȯ�Ѵ�.

	D3DXVECTOR3 vMainDir(0.f, 0.f, 0.f);
	D3DXVECTOR3 vResistanceDir(0.f, 0.f, 0.f);
	D3DXVECTOR3 vToTarget(0.f, 0.f, 0.f);
	// ���� �ӵ��� ���Ѵ�.
	for (auto& pCollidedUnit : m_vecCollidedUnits) {
		D3DXVec3Normalize(&vToTarget, &(GetXY() - pCollidedUnit->GetXY()));
		vResistanceDir += vToTarget;
	}

	//if (m_pCollidedWindmill) {
	//	D3DXVec3Normalize(&vToTarget, &(GetXY() - m_pCollidedWindmill->GetXY()));
	//	vResistanceDir += (vToTarget + GetToXY())/* * 5.5f*/;


	//}
	//else {
		for (auto& pBlockingTile : m_vecCollidedBlockingTiles) {
			D3DXVec3Normalize(&vToTarget, &(GetXY() - pBlockingTile->GetXY()));
			vResistanceDir += (vToTarget + GetToXY()) /** 4.5f*/;
			pBlockingTile->PushOutOfTile(this);
		}
	//}

	// ��ħ�� �������� ������ �پ���.
	if (!m_vecCollidedUnits.empty()) {
		vResistanceDir /= (m_vecCollidedUnits.size());
	}
	// Ÿ�� ���͸� ���Ѵ�.
	D3DXVec3Normalize(&vToTarget, &(m_pTargetEnemy->GetXY() - GetXY()));
	// Ÿ�� ���Ϳ� ���� �ӵ��� ���Ѵ�.
	vMainDir = vToTarget + vResistanceDir;
	// ���� ������ ���Ѵ�.
	D3DXVec3Normalize(&vMainDir, &vMainDir);
	// �ӷ��� �����Ѵ�.
	float fT = D3DXVec3Dot(&GetToXY(), &vMainDir) * 0.5f + 0.5f;
	SetSpeed(GetMaxSpeed() * fT);
	

	SetToXY(vMainDir);
	UpdateSpriteDir();
	MoveByDeltaTime(_fDeltaTime);

	return true;
}

void CComDepObj::DetectUnitsAround()
{
	m_pTargetEnemy = nullptr;
	m_vecCollidedUnits.clear();
	m_pCollidedWindmill = nullptr;
	m_vecCollidedBlockingTiles.clear();
	
	CComDepObj* pUnit= nullptr;
	CCommander* pCommander = nullptr;
	CWindmill* pWindmill = nullptr;
	float fMinLength = 987654321.f * BASE_SCALE;
	float fLength = 0.f;
	for (auto& pObj : GetGameWorld().GetListObjs()) {
		pUnit = dynamic_cast<CComDepObj*>(pObj);
		DO_IF_IS_NOT_VALID_OBJ(pUnit) continue;
		
		fLength = D3DXVec3Length(&(pUnit->GetXY() - GetXY()));
		if (fLength <= m_fDetectionRange && fLength < fMinLength) {
			// ���� ������ �����ϸ�, �ڱ�κ��� ���� �����̿� �ִ� ���� Ÿ�� ������ ��´�.
			
			// 1) Ÿ�� Ŀ����� ��ȿ�� �����̴�.
			// 2) Ÿ�� Ŀ����� �ڽ��� Ŀ����� �ٸ���. => ���̴�.
			// 3) ���� Ŀ����� �ƴϴ�. => ����� �ƴ� �Ϲ� �����̴�. // Ŀ����� ���� ����� ���� ���Ѵ�.
			// => ���̴�.
			pCommander = pUnit->GetCommander();
			if (pCommander && pCommander != GetCommander() && pCommander != pUnit) {
				// Ŀ����� �ش� ��ü�� Ŀ����� �ٸ��� ������ ���� �����ϴ�.
				fMinLength = fLength;
				m_pTargetEnemy = pUnit;
			}

			pWindmill = dynamic_cast<CWindmill*>(pUnit);
			if (!m_pCollidedWindmill && pWindmill) {
				// ������ ���мҿ��ٸ�,
				if (!IsCollided(pWindmill->GetXY(), pWindmill->GetCollisionRadius(), GetXY(), GetCollisionRadius()))
					continue;
				m_pCollidedWindmill = pWindmill;
			}
			else {
				// �׳� �Ϲ� �����̾��ٸ�,
				if (!IsCollided(pUnit->GetXY(), pUnit->GetCollisionRadius(), GetXY(), GetCollisionRadius()))
					continue;
				m_vecCollidedUnits.emplace_back(pUnit);
			}
		}
	}

	//if (pWindmill) return;
	for (auto& pBlockingTile : GetGameWorld().GetMapLoader()->GetBlockingTiles()) {
		if (!IsCollided(pBlockingTile->GetXY(), TILE_RADIUS, GetXY(), GetCollisionRadius()))
			continue;
		m_vecCollidedBlockingTiles.emplace_back(pBlockingTile);
	}
	// TODO : �浹�� ���ŷ Ÿ���� �����Ѵ�.
}

void CComDepObj::AdjustPosition(float _fDeltaTime, float _fSmooth)
{
	D3DXVECTOR3 vMainDir(0.f, 0.f, 0.f);
	D3DXVECTOR3 vToTarget(0.f, 0.f, 0.f);
	for (auto& pCollidedUnit : m_vecCollidedUnits) {
		D3DXVec3Normalize(&vToTarget, &(GetXY() - pCollidedUnit->GetXY()));
		vMainDir += vToTarget;
	}
	/*if (m_pCollidedWindmill) {
		D3DXVec3Normalize(&vToTarget, &(GetXY() - m_pCollidedWindmill->GetXY()));
		vMainDir += vToTarget;
	}
	else {*/
		for (auto& pBlockingTile : m_vecCollidedBlockingTiles) {
			D3DXVec3Normalize(&vToTarget, &(GetXY() - pBlockingTile->GetXY()));
			vMainDir += (GetToXY() + vToTarget) * 5.f;
			pBlockingTile->PushOutOfTile(this);
		}
	//}
	D3DXVec3Normalize(&vMainDir, &vMainDir);
	SetToXY(vMainDir);
	SetSpeed(_fSmooth * BASE_SCALE * m_vecCollidedUnits.size());
	//UpdateSpriteDir();1
	MoveByDeltaTime(_fDeltaTime);
}

bool CComDepObj::CanAttackTargetEnemy()
{
	if (!m_pTargetEnemy) return false;

	float fLength = D3DXVec3Length(&(m_pTargetEnemy->GetXY() - GetXY()));

	// �������� ���� �ִٸ� ������ �� �ִ�.
	return fLength <= m_fAttackRange;
}

void CComDepObj::TakeDamage(float _fDamageAmount)
{
	m_fHP -= _fDamageAmount; 
	if (m_fHP < 0.f)
		m_fHP = 0.f; 
	m_pUIUnitHP->DisplayHP();
}

void CComDepObj::UpdateSpriteDir(void)
{
	D3DXVECTOR3 vDir = GetToXY();
	// ���� ��ȯ
	if (vDir.y < 0.f) SetScaleX(fabs(GetScaleX()));
	if (vDir.y > 0.f) SetScaleX(-fabs(GetScaleX()));
	if (vDir.x > 0.f) SetScaleX(fabs(GetScaleX()));
	if (vDir.x < 0.f) SetScaleX(-fabs(GetScaleX()));
}
