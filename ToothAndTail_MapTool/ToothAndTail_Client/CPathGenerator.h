#pragma once

//class CTile;
class CGameWorld;
class CPathGenerator final
{
public:
	CPathGenerator(CGameWorld& _rGameWorld);
	~CPathGenerator();
public:
	bool GeneratePath(const D3DXVECTOR3& _vStartPos, const D3DXVECTOR3& _vGoalPos);
	bool GeneratePath(const int& _iStartLineIndex, const int& _iGoalLineIndex);
	list<D3DXVECTOR3>& GetPath() { return m_listPath; }
	void Release();

private:
	bool FindPath(int _iStartLineIndex, int _iGoalLineIndex);
	void MakePath(int _iStartLineIndex, int _iGoalLineIndex);
	bool CheckOpenList(int _iLineIndex);
	bool CheckCloseList(int _iLineIndex);

private:
	CGameWorld& m_rGameWorld;

	int m_iStartLineIndex = -1;
	int m_iGoalLineIndex = -1;
	list<int> m_listOpens;
	list<int> m_listCloses;
	list<D3DXVECTOR3> m_listPath;
};

