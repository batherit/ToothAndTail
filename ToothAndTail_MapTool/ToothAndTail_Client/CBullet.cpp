#include "stdafx.h"
#include "CBullet.h"
#include "CTextureMgr.h"
#include "CComDepObj.h"

CBullet::CBullet(CGameWorld & _rGameWorld, CComDepObj * _pTarget, UNIT::E_TYPE _eUnitType, D3DXVECTOR3 _vStartPos)
	:
	CSpriteObj(_rGameWorld, _vStartPos.x, _vStartPos.y),
	m_pTarget(_pTarget),
	m_vTargetPos(_pTarget->GetXY())
{
	SetRenderLayer(10);

	D3DXVECTOR3 vToTarget = _pTarget->GetXY() - _vStartPos;
	D3DXVec3Normalize(&vToTarget, &vToTarget);
	SetToXY(vToTarget);

	const TextureInfo* pTextureInfo = nullptr;
	switch (_eUnitType) {
	case UNIT::TYPE_PIG:
		pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"PIG_BULLET");
		SetSpeed(PIG_BULLET_SPEED);
		m_fDamage = PIG_BULLET_DAMAGE;
		break;
	case UNIT::TYPE_SQUIRREL:	
		pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"SQUIRREL_BULLET");
		SetSpeed(SQUIRREL_BULLET_SPEED);
		m_fDamage = SQUIRREL_BULLET_DAMAGE;
		break;
	case UNIT::TYPE_LIZARD:
		pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"LIZARD_BULLET");
		SetSpeed(LIZARD_BULLET_SPEED);
		m_fDamage = LIZARD_BULLET_DAMAGE;
		break;
	case UNIT::TYPE_MOLE:
		// źȯ ������ �� ����.
		SetSpeed(SKUNK_BULLET_SPEED);
		m_fDamage = SKUNK_BULLET_DAMAGE;
		break;
	case UNIT::TYPE_SKUNK:
		pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"SKUNK_BULLET");
		SetSpeed(SKUNK_BULLET_SPEED);
		m_fDamage = SKUNK_BULLET_DAMAGE;
		break;
	case UNIT::TYPE_BADGER:
		pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"BADGER_BULLET");
		SetSpeed(BADGER_BULLET_SPEED);
		m_fDamage = BADGER_BULLET_DAMAGE;
		break;
	case UNIT::TYPE_FOX:
		pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"FOX_BULLET");
		SetSpeed(FOX_BULLET_SPEED);
		m_fDamage = FOX_BULLET_DAMAGE;
		break;
	}

	if (pTextureInfo) {
		PushTexture(pTextureInfo);
		SetSize(pTextureInfo->tImageInfo.Width, pTextureInfo->tImageInfo.Height);
	}
}

CBullet::~CBullet()
{
}

int CBullet::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;

	D3DXVECTOR3 vToTarget = m_vTargetPos - GetXY();
	float fLength = D3DXVec3Length(&(vToTarget));
	D3DXVec3Normalize(&vToTarget, &vToTarget);
	
	if (fLength <= 10.f || D3DXVec3Dot(&GetToXY(), &vToTarget) <= 0.f) {
		// źȯ�� Ÿ�� �������κ��� 10���� ���̷� �ְų�, ��հ� ������ ���
		if(IS_VALID_OBJ(m_pTarget) && !m_pTarget->IsDead()) {
			m_pTarget->TakeDamage(m_fDamage);
			if (m_pTarget->IsDead())
				// Ÿ���� �׾��ٸ� ��ȿȭ�Ѵ�.
				m_pTarget->InvalidateObj();
		}
		// �����ߴٸ�, �Ѿ� ��ȿȭ ó��
		SetValid(false);
		return 1;
	}

	MoveByDeltaTime(_fDeltaTime);

	return 0;
}

void CBullet::LateUpdate(void)
{
}

void CBullet::CollectGarbageObjs()
{
	DO_IF_IS_NOT_VALID_OBJ(m_pTarget) m_pTarget = nullptr;
}
