#include "stdafx.h"
#include "CTunnel.h"
#include "CUnitGenerator.h"
#include "CTextureMgr.h"


CTunnel::CTunnel(CGameWorld& _rGameWorld, float _fX, float _fY, CTunnel::E_SIZE _eSize, CUnitGenerator* _pUnitGenerator, CCommander* _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY),
	m_eSize(_eSize),
	m_pUnitGenerator(_pUnitGenerator)	// 외부에서 할당받아서 내부에서 해제한다.
{
	// 자식들은 각자의 빌딩 애니메이션 정보를 세팅한다.
	SetRenderLayer(6);
	SetScaleXY(BASE_SCALE, BASE_SCALE);

	switch (_eSize) {
	case CTunnel::SIZE_SMALL: {
		PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TUNNEL_SMALL"));
		SetSize(TUNNEL_SMALL_WIDTH, TUNNEL_SMALL_HEIGHT);
		GenerateIdentificationTintObj(TUNNEL_SMALL_WIDTH, TUNNEL_SMALL_HEIGHT, L"TUNNEL_SMALL_TINT");
		SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 1, false));
	}
		break;
	case CTunnel::SIZE_MIDDLE: {
		PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TUNNEL_MIDDLE"));
		SetSize(TUNNEL_MIDDLE_WIDTH, TUNNEL_MIDDLE_HEIGHT);
		GenerateIdentificationTintObj(TUNNEL_MIDDLE_WIDTH, TUNNEL_MIDDLE_HEIGHT, L"TUNNEL_MIDDLE_TINT");
		SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 1, false));
	}
		break;
	case CTunnel::SIZE_BIG: {
		PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TUNNEL_BIG"));
		SetSize(TUNNEL_BIG_WIDTH, TUNNEL_BIG_HEIGHT);
		GenerateIdentificationTintObj(TUNNEL_BIG_WIDTH, TUNNEL_BIG_HEIGHT, L"TUNNEL_BIG_TINT");
		SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 1, false));
	}
		break;
	}
}

CTunnel::~CTunnel()
{
	Release();
}

int CTunnel::Update(float _fDeltaTime)
{
	switch (m_eSize)
	{
	case CTunnel::SIZE_SMALL: {
		switch (m_eState)
		{
		case CTunnel::STATE_BUILDING: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				if ((m_fElapsedTime += _fDeltaTime) >= TUNNEL_SMALL_BUILD_SEC) {
					SetNewAnimInfo(AnimInfo(0, 8, 4, 11, 1.f, 1, false));
					m_eState = CTunnel::STATE_COMPLETED;
					m_fElapsedTime = 0.f;
				}
				else {
					// TODO : UI 갱신 해주기 등...
				}
			}
			break;		
		}
		case CTunnel::STATE_COMPLETED: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				SetNewAnimInfo(AnimInfo(0, 8, 14, 1, 0.f, 0, false));
				m_eState = CTunnel::STATE_GENERATE_UNIT;
			}
			break;
		}
		case CTunnel::STATE_GENERATE_UNIT: {
			if(m_pUnitGenerator) m_pUnitGenerator->Update(_fDeltaTime);
			break;
		}
		}
		break;
	}
	case CTunnel::SIZE_MIDDLE: {
		switch (m_eState)
		{
		case CTunnel::STATE_BUILDING: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				if ((m_fElapsedTime += _fDeltaTime) >= TUNNEL_MIDDLE_BUILD_SEC) {
					SetNewAnimInfo(AnimInfo(0, 8, 4, 18, 1.f, 1, false));
					m_eState = CTunnel::STATE_COMPLETED;
					m_fElapsedTime = 0.f;
				}
				else {
					// TODO : UI 갱신 해주기 등...
				}
			}
			break;
		}
		case CTunnel::STATE_COMPLETED: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				SetNewAnimInfo(AnimInfo(0, 8, 21, 1, 0.f, 0, false));
				m_eState = CTunnel::STATE_GENERATE_UNIT;
			}
			break;
		}
		case CTunnel::STATE_GENERATE_UNIT: {
			if (m_pUnitGenerator) m_pUnitGenerator->Update(_fDeltaTime);
			break;
		}
		}
		break;
	}
	case CTunnel::SIZE_BIG: {
		switch (m_eState)
		{
		case CTunnel::STATE_BUILDING: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				if ((m_fElapsedTime += _fDeltaTime) >= TUNNEL_BIG_BUILD_SEC) {
					SetNewAnimInfo(AnimInfo(0, 8, 4, 34, 1.f, 1, false));
					m_eState = CTunnel::STATE_COMPLETED;
					m_fElapsedTime = 0.f;
				}
				else {
					// TODO : UI 갱신 해주기 등...
				}
			}
			break;
		}
		case CTunnel::STATE_COMPLETED: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				SetNewAnimInfo(AnimInfo(0, 8, 37, 1, 0.f, 0, false));
				m_eState = CTunnel::STATE_GENERATE_UNIT;
			}
			break;
		}
		case CTunnel::STATE_GENERATE_UNIT: {
			if (m_pUnitGenerator) m_pUnitGenerator->Update(_fDeltaTime);
			break;
		}
		}
		break;
	}
	}

	return 0;
}

void CTunnel::Release(void)
{
	SafelyDeleteObj(m_pUnitGenerator);
}
