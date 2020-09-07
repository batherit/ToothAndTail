#include "stdafx.h"
#include "CBurst.h"
#include "CTextureMgr.h"

CBurst::CBurst(CGameWorld & _rGameWorld, D3DXVECTOR3 _vStartPos, float _fLifeTime, float _fScale)
	:
	CSpriteObj(_rGameWorld, _vStartPos.x, _vStartPos.y, BURST_WIDTH, BURST_HEIGHT)
{
	SetRenderLayer(10);
	SetScale(_fScale);

	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"BURST"));

	SetNewAnimInfo(AnimInfo(0, 8, 0, 40, _fLifeTime, 1, false));
}

CBurst::~CBurst()
{
}

int CBurst::Update(float _fDeltaTime)
{
	if (1 == UpdateAnim(_fDeltaTime)) {
		SetValid(false);
	}

	return 0;
}
