#include "stdafx.h"
#include "CComDepObj.h"
#include "CCommander.h"
#include "CTextureMgr.h"
#include "CGameWorld.h"
#include "CMapLoader.h"



CComDepObj::CComDepObj(CGameWorld & _rGameWorld, CCommander* _pCommander, float _fX, float _fY, size_t _iWidth, size_t _iHeight, float _fToX, float _fToY, float _fSpeed, int _iID)
	:
	CSpriteObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, _fToX, _fToY, _fSpeed),
	m_pCommander(_pCommander),
	m_iID(_iID)
{
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

	// �Ƹ� ���̺� �˰����� ����� ��,,, ������ �ܼ��� ��ǥ �������� �̵��� �ϴ� �ڵ�
	D3DXVECTOR3 vToTarget = m_vTargetPos - GetXY();
	D3DXVec3Normalize(&vToTarget, &vToTarget);
	SetToXY(vToTarget);
	UpdateSpriteDir();
	MoveByDeltaTime(_fDeltaTime);

	return true;
}

void CComDepObj::DetectEnemyAround()
{
	m_pTargetEnemy = nullptr;
	
	CComDepObj* pTargetEnemy = nullptr;
	CCommander* pTargetCommander = nullptr;
	float fMinLength = 987654321.f * BASE_SCALE;
	float fLength = 0.f;
	for (auto& pObj : GetGameWorld().GetListObjs()) {
		pTargetEnemy = dynamic_cast<CComDepObj*>(pObj);
		DO_IF_IS_NOT_VALID_OBJ(pTargetEnemy) continue;
		
		pTargetCommander = pTargetEnemy->GetCommander();
		// 1) Ÿ�� Ŀ����� ��ȿ�� �����̴�.
		// 2) Ÿ�� Ŀ����� �ڽ��� Ŀ����� �ٸ���. => ���̴�.
		// 3) ���� Ŀ����� �ƴϴ�. => ����� �ƴ� �Ϲ� �����̴�.
		if (pTargetCommander && pTargetCommander != GetCommander() && pTargetCommander != pTargetEnemy) {
			// Ŀ����� �ش� ��ü�� Ŀ����� �ٸ��� ������ ���� �����ϴ�.
			fLength = D3DXVec3Length(&(pTargetEnemy->GetXY() - GetXY()));
			if (fLength <= m_fDetectionRange && fLength < fMinLength) {
				// ���� ������ �����ϸ�,
				// �ڱ�κ��� ���� �����̿� �ִ� ���� Ÿ�� ������ ��´�.
				fMinLength = fLength;
				m_pTargetEnemy = pTargetEnemy;
			}
		}
	}
}

bool CComDepObj::CanAttackTargetEnemy()
{
	if (!m_pTargetEnemy) return false;

	float fLength = D3DXVec3Length(&(m_pTargetEnemy->GetXY() - GetXY()));

	// �������� ���� �ִٸ� ������ �� �ִ�.
	return fLength <= m_fDetectionRange;
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
