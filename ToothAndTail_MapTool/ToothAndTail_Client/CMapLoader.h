#pragma once
class CGameWorld;
class CSpriteObj;
class CMapLoader final
{
public:
	explicit CMapLoader(CGameWorld& _rGameWorld, const wstring& wstrMapFilePath);
	virtual ~CMapLoader();
	
public:
	void Release(void);

public:
	const vector<CSpriteObj*>& GetDecoObjs(void) { return m_vecDecoObjs; }

private:
	CGameWorld& m_rGameWorld;
	vector<CSpriteObj*> m_vecDecoObjs;
};

