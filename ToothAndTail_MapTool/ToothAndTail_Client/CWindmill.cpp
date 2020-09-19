#include "stdafx.h"
#include "CWindmill.h"
#include "CTextureMgr.h"
#include "CCommander.h"
#include "CWindmillBase.h"
#include "CTurbine.h"
#include "CGameWorld.h"
#include "CMapLoader.h"
#include "CTile.h"
#include "CFarmland.h"
#include "CBurst.h"
#include "CUI_UnitHP.h"
#include "CCamera.h"
#include "CTAT_World.h"
#include "CBullet.h"


//CWindmill::CWindmill(CGameWorld & _rGameWorld, float _fX, float _fY, CCommander* _pCommander)
//	:
//	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, WINDMILL_WIDTH, WINDMILL_HEIGHT)
//{
//	// 안쓰이는 생성자,,,
//	SetScaleXY(BASE_SCALE, BASE_SCALE);
//
//	m_pGround = new CSpriteObj(_rGameWorld, 0.f, 0.f, WINDMILL_GROUND_WIDTH, WINDMILL_GROUND_HEIGHT);
//	m_pGround->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_GROUND"));
//	m_pGround->SetParent(this);
//	m_pGround->SetRenderLayer(0);
//
//	/*m_eState = CWindmill::STATE_UNOCCUPIED;
//	if (_pCommander) m_eState = CWindmill::STATE_OCCUPIED;*/
//
//	switch (m_eState)
//	{
//	case CWindmill::STATE_UNOCCUPIED:
//	{
//		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, CWindmillBase::STATE_UNOCCUPIED, _pCommander);
//		D3DXVECTOR3 vFarmlandPos;
//		CFarmland* pFarmland = nullptr;
//		for (int i = 0; i < 3; ++i) {
//			for (int j = 0; j < 3; ++j) {
//				if (i == 1 && j == 1) continue;
//				vFarmlandPos.x = -1.f * (TILE_WIDTH << 1) + (i + j) * (TILE_WIDTH >> 1);
//				vFarmlandPos.y = (i - j) * (TILE_HEIGHT >> 1);
//				vFarmlandPos.z = 0.f;
//				pFarmland = new CFarmland(GetGameWorld(), vFarmlandPos.x, vFarmlandPos.y, CFarmland::STATE_UNOCCUPIED, GetCommander());
//				m_vecFarmlands.emplace_back(pFarmland);
//			}
//		}
//		break;
//	}
//	case CWindmill::STATE_BUILDING: 
//		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, CWindmillBase::STATE_BUILDING, _pCommander);
//		break;
//	case CWindmill::STATE_OCCUPIED:
//		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, CWindmillBase::STATE_OCCUPIED, _pCommander);
//		break;
//	default:
//		break;
//	}
//
//	if (m_pWindmillBase) m_pWindmillBase->SetParent(this);
//}

CWindmill::CWindmill(CGameWorld & _rGameWorld, int _iLineIndex, CCommander * _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, 0.f, 0.f, WINDMILL_WIDTH, WINDMILL_HEIGHT)
{
	// 카메라 세팅
	SetPrivateCamera(new CCamera(_rGameWorld, this));
	GetPrivateCamera()->SetZoomMultiple(0.6f);

	InitHP(WINDMILL_MAX_HP);
	GetUIUnitHP()->SetY(-20.f);
	SetCollisionRadius(WINDMILL_RADIUS);
	SetMinimapSign(MINIMAP::SIGN_WINDMILL);

	SetTileSiteInfo(TileSiteInfo(_iLineIndex, 6, 6, 3, 3));
	CMapLoader* pMapLoader = _rGameWorld.GetMapLoader();
	pMapLoader->SetSiteType(TileSiteInfo(_iLineIndex, 6, 6, 3, 3), TILE::TYPE_NO);
	pMapLoader->SetSiteType(TileSiteInfo(_iLineIndex, 2, 2, 1, 1), TILE::TYPE_BLOCKING);
	pMapLoader->UpdateBlockingTiles();
	SetXY(pMapLoader->GetSiteCenter(TileSiteInfo(_iLineIndex, 2, 2, 1, 1)));
	/*D3DXVECTOR3 vTilePos = pMapLoader->GetTile(_iLineIndex)->GetXY();
	vTilePos.x -= (TILE_WIDTH >> 1) * BASE_SCALE;
	SetXY(vTilePos);*/
	SetScaleXY(BASE_SCALE, BASE_SCALE);

	// 그라운드 생성
	m_pGround = new CSpriteObj(_rGameWorld, 0.f, 0.f, WINDMILL_GROUND_WIDTH, WINDMILL_GROUND_HEIGHT);
	m_pGround->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_GROUND"));
	m_pGround->SetParent(this);
	m_pGround->SetRenderLayer(0);

	// 상태 설정에 따른 농장 생성
	/*m_eState = CWindmill::STATE_UNOCCUPIED;
	if (_pCommander) m_eState = CWindmill::STATE_OCCUPIED;*/

	if (_pCommander) {
		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, WINDMILL::STATE_OCCUPIED, _pCommander);
		m_pWindmillBase->SetParent(this);
		D3DXVECTOR3 vFarmlandPos;
		CFarmland* pFarmland = nullptr;
		CFarmland::E_STATE eFarmlandStates[9] = {
			CFarmland::STATE_OCCUPIED, CFarmland::STATE_OCCUPIED, CFarmland::STATE_OCCUPIED,
			CFarmland::STATE_OCCUPIED, CFarmland::STATE_END, CFarmland::STATE_OCCUPIED,
			CFarmland::STATE_OCCUPIED, CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED
		};
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				vFarmlandPos.x = -1.f * (TILE_WIDTH << 1) + (i + j) * (TILE_WIDTH);
				vFarmlandPos.y = (i - j) * (TILE_HEIGHT);
				vFarmlandPos.z = 0.f;
				pFarmland = new CFarmland(GetGameWorld(), vFarmlandPos.x, vFarmlandPos.y, eFarmlandStates[i * 3 + j], _pCommander);
				pFarmland->SetParent(this);
				m_vecFarmlands.emplace_back(pFarmland);
			}
		}
	}
	else {
		m_pWindmillBase = new CWindmillBase(_rGameWorld, 0.f, -45.f, WINDMILL::STATE_UNOCCUPIED, _pCommander);
		m_pWindmillBase->SetParent(this);
		D3DXVECTOR3 vFarmlandPos;
		CFarmland* pFarmland = nullptr;
		CFarmland::E_STATE eFarmlandStates[9] = {
			CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED,
			CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_END, CFarmland::STATE_UNOCCUPIED,
			CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED, CFarmland::STATE_UNOCCUPIED
		};
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				vFarmlandPos.x = -1.f * (TILE_WIDTH << 1) + (i + j) * (TILE_WIDTH);
				vFarmlandPos.y = (i - j) * (TILE_HEIGHT);
				vFarmlandPos.z = 0.f;
				pFarmland = new CFarmland(GetGameWorld(), vFarmlandPos.x, vFarmlandPos.y, eFarmlandStates[i * 3 + j], _pCommander);
				pFarmland->SetParent(this);
				m_vecFarmlands.emplace_back(pFarmland);
			}
		}
	}	
}

CWindmill::~CWindmill()
{
	Release();
}

void CWindmill::Ready(void)
{
}

int CWindmill::Update(float _fDeltaTime)
{
	if (GetState() == WINDMILL::STATE_DESTROYED) return 0;
	/*if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_G)) {
		InvalidateObj();
	}*/

	GetUIUnitHP()->Update(_fDeltaTime);
	if (m_pWindmillBase) m_pWindmillBase->Update(_fDeltaTime);
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->Update(_fDeltaTime);
	}
	
	if (!GetCommander()) return 0;
	int iTotalCropAmount = 0;
	if ((m_fCroppingTickTime += _fDeltaTime) >= PIG_CROP_TIME) {
		for (auto& pFarmland : m_vecFarmlands) {
			iTotalCropAmount += pFarmland->Cropped();
		}
		GetCommander()->IncreaseMoney(iTotalCropAmount);
		m_fCroppingTickTime = 0.f;
	}

	if (m_bIsAttackedRecently) {
		if ((m_fAttackedTickTime -= _fDeltaTime) <= 0.f) {
			m_bIsAttackedRecently = false;
			m_fAttackedTickTime = 0.f;
		}
	}

	return 0;
}

void CWindmill::LateUpdate(void)
{
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->LateUpdate();
	}

	// 이 제분소가 점령되었거나, 점령중인지?
	if (GetState() != WINDMILL::STATE_UNOCCUPIED && GetState() != WINDMILL::STATE_DESTROYED) return;

	// 제분소 주변에 기수가 있는지를 확인한다.
	CCommander* pCommander = nullptr;
	for (auto& pObj : GetGameWorld().GetListObjs()) {
		pCommander = dynamic_cast<CCommander*>(pObj);
		// 제분소 중앙은 2*2 타일을 점령하고 있기 때문에 2를 곱하고 0.5배만큼의 오프셋을 두었다.
		if (pCommander 
			&& pCommander->IsActivating() 
			&& pCommander->GetMoney() >= WINDMILL_COST
			&& IsPointInTile(pCommander->GetXY(), GetXY(), TILE_WIDTH * BASE_SCALE * 4.0f, TILE_HEIGHT * BASE_SCALE * 4.f)) {
			pCommander->DecreseMoney(WINDMILL_COST);
			Occupied(pCommander);
			SetHP(GetMaxHP());
			break;
		}
	}
}

void CWindmill::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	// TODO : 자신을 그리고 싶으면 CObj::RegisterToRenderList(_vecRenderList)를 추가합니다.
	if (m_pGround) m_pGround->RegisterToRenderList(_vecRenderList);
	if (m_pWindmillBase) m_pWindmillBase->RegisterToRenderList(_vecRenderList);
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->RegisterToRenderList(_vecRenderList);
	}
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}

void CWindmill::Release(void)
{
	SafelyDeleteObj(m_pWindmillBase);
	SafelyDeleteObj(m_pGround);
	SafelyDeleteObjs(m_vecFarmlands);
}

void CWindmill::InvalidateObj()
{
	// 일종의 무효화처리(무효화된 상태이므로 더이상 무효화하지 마라.)
	if (GetState() == WINDMILL::STATE_DESTROYED) return;
	// 자기 자신을 무효화하면 인게임에서 제거되는 것이니 제분소 무효화는 하지 않는다.
	//CComDepObj::InvalidateObj();

	GetPrivateCamera()->Shake(0.6f, 10.f, 100);
	dynamic_cast<CTAT_World&>(GetGameWorld()).SetAnotherCameraTemporarily(GetPrivateCamera(), 2.0f);

	GetUIUnitHP()->CloseHP();

	SetCommander(nullptr);
	D3DXVECTOR3 vBurstPos = GetXY();
	vBurstPos.y -= 35.f * BASE_SCALE;
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), vBurstPos, 1.2f, 3.f));

	if (m_pWindmillBase) {
		m_pWindmillBase->SetCommander(nullptr);
		m_pWindmillBase->SetWindmillBaseState(WINDMILL::STATE_DESTROYED);
	}
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->InvalidateObj();
	}
	m_fCroppingTickTime = 0.f;

	m_bIsAttackedRecently = false;
	m_fAttackedTickTime = 0.f;

	GetGameWorld().JudgetResult();

	CSoundMgr::GetInstance()->PlaySound(L"Destroy_HQ.wav", CSoundMgr::PLAYER);
}

void CWindmill::Occupied(CCommander* _pCommander)
{
	/*if (m_eState != CWindmill::STATE_UNOCCUPIED && m_eState != CWindmill::STATE_DESTROYED) return;
	m_eState = CWindmill::STATE_BUILDING;*/
	SetCommander(_pCommander);
	// TODO : 점령 과정을 작성한다.
	if (m_pWindmillBase) {
		m_pWindmillBase->SetCommander(_pCommander);
		m_pWindmillBase->SetWindmillBaseState(WINDMILL::STATE_BUILDING);
	}
	for (auto& pFarmland : m_vecFarmlands) {
		pFarmland->SetCommander(_pCommander);
	}
	InitHP(WINDMILL_MAX_HP);
}

void CWindmill::TakeDamage(float _fDamageAmount, CObj* _pAttacker)
{
	CComDepObj::TakeDamage(_fDamageAmount);
	m_vAttackerPos = dynamic_cast<CBullet*>(_pAttacker)->GetStartPos();
	m_bIsAttackedRecently = true;
	m_fAttackedTickTime = ATTACKED_KEEP_TIME;
}

WINDMILL::E_STATE CWindmill::GetState() const
{
	return m_pWindmillBase->GetState();
}

bool CWindmill::DetectEmptyLot(D3DXVECTOR3 & _rEmptyLotPos)
{
	// 너비 우선 탐색으로 점점 확대해가면서 갈 곳을 찾아본다.
	bool bVisited[100][100] = { false, };
	queue<POINT> qVisited;	// 방문한 노드를 집어넣는다.
	D3DXVECTOR3 vStartPos = GetXY();
	vStartPos.x += (TILE_WIDTH >> 1) * BASE_SCALE;
	CMapLoader* pMapLoader = GetGameWorld().GetMapLoader();
	POINT ptRowColIndexes = pMapLoader->GetRowColIndex(vStartPos);
	qVisited.push(ptRowColIndexes);
	bVisited[ptRowColIndexes.y][ptRowColIndexes.x] = true;

	POINT ptHere;
	POINT ptThere;
	CTile* pTile = nullptr;
	int iSignI = 1;
	int iSignJ = 1;
	while (!qVisited.empty()) {
		ptHere = qVisited.front();
		qVisited.pop();
		iSignI = ((rand() % 2 == 0) ? 1 : -1);
		iSignJ = ((rand() % 2 == 0) ? 1 : -1);
		for (int i = -iSignI; i != 2 * iSignI; i += iSignI) {
			for (int j = -iSignJ; j != 2 * iSignJ; j += iSignJ) {
				if (0 == i && 0 == j) continue; // 자기 자신을 가리키므로 다음을 진행한다.
				ptThere.y = ptHere.y + i;
				ptThere.x = ptHere.x + j;
				if (!pMapLoader->IsTileInRange(ptThere))
					continue;

				if (pMapLoader->IsEmptyLot(ptThere, 2, 2, 1, 1)) {
					//공터라면, 이것을 목표 지점으로 둔다.
					pTile = pMapLoader->GetTile(ptThere.y, ptThere.x);
					_rEmptyLotPos = pTile->GetXY();
					// x값 조정
					_rEmptyLotPos.x -= (TILE_WIDTH >> 1) * BASE_SCALE;
					return true;
				}
				// 공터를 찾지 못했다면, 이곳을 방문한 곳으로 처리하고
				// 다음 노드를 살펴본다.
				if (!bVisited[ptThere.y][ptThere.x]) {
					qVisited.push(ptThere);
					bVisited[ptThere.y][ptThere.x] = true;
				}
			}
		}
	}

	_rEmptyLotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	return false;
}

//void CWindmill::CollectGarbageObjs()
//{
//	for (auto& pFarmland : m_vecFarmlands) {
//		pFarmland->CollectGarbageObjs();
//	}
//}
