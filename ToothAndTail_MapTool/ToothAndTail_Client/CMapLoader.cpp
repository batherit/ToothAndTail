#include "stdafx.h"
#include "CMapLoader.h"


CMapLoader::CMapLoader(CGameWorld & _rGameWorld, const wstring & wstrMapFilePath)
	:
	m_rGameWorld(_rGameWorld)
{
}

CMapLoader::~CMapLoader()
{
	Release();
}

void CMapLoader::Release(void)
{
	SafelyDeleteObjs(m_vecDecoObjs);
}
