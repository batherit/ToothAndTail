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
	// 색 정보를 나타내는 틴트 텍스처 장착
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

	// 아마 레이븐 알고리즘이 적용될 곳,,, 지금은 단순히 목표 지점으로 이동만 하는 코드
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
		// 1) 타겟 커멘더가 유효한 상태이다.
		// 2) 타겟 커멘더는 자신의 커멘더와 다르다. => 적이다.
		// 3) 적은 커멘더가 아니다. => 기수가 아닌 일반 유닛이다.
		if (pTargetCommander && pTargetCommander != GetCommander() && pTargetCommander != pTargetEnemy) {
			// 커멘더가 해당 객체의 커멘더와 다르면 적으로 간주 가능하다.
			fLength = D3DXVec3Length(&(pTargetEnemy->GetXY() - GetXY()));
			if (fLength <= m_fDetectionRange && fLength < fMinLength) {
				// 감지 범위에 존재하며,
				// 자기로부터 가장 가까이에 있는 적을 타겟 적으로 삼는다.
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

	// 감지범위 내에 있다면 공격할 수 있다.
	return fLength <= m_fDetectionRange;
}

void CComDepObj::UpdateSpriteDir(void)
{
	D3DXVECTOR3 vDir = GetToXY();
	// 방향 전환
	if (vDir.y < 0.f) SetScaleX(fabs(GetScaleX()));
	if (vDir.y > 0.f) SetScaleX(-fabs(GetScaleX()));
	if (vDir.x > 0.f) SetScaleX(fabs(GetScaleX()));
	if (vDir.x < 0.f) SetScaleX(-fabs(GetScaleX()));
}
