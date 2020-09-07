#pragma once

class CObj;
class CComDepObj;
// �ؽ�ó ����
typedef struct _texture_info
{
	LPDIRECT3DTEXTURE9 pTexture;	// �̹��� ������ �����ϱ� ���� �İ�ü. 
	D3DXIMAGE_INFO tImageInfo;		//�̹����� ������ ��Ƶ� ����ü. 
}TextureInfo;

// �ִϸ��̼� ����
typedef struct _anim_info_line {
	int iState = -1;				// ���¸� ��Ÿ���� �������� 0�� �⺻������ Idle
	int iStartFrameIndex = 0;			// �ִϸ��̼� ���� �ε���
	int iFrameCount = 0;				// ���� �ε����������� ����� �� ������.
	float fTotalTime = 0.f;			// �� �ִϸ��̼� �� ���� �ð�
	int iCountToRepeat = 0;			// �� �ִϸ��̼��� �� �� ����? (0 - ����, 1 - 1ȸ�ϰ� ��(�� �� ������ IDLE�� ��ȯ))
	bool bIsReversePlay = false;
}AnimInfoLine;

typedef struct _anim_info {
public:
	_anim_info(int _iAnimIndex = 0, int _iCol = 1, int _iStartFrameIndex = 0, int _iFrameCount = 0, float _fTotalTime = 0.f, int _iCountToRepeat = 0, bool _bIsReversePlay = false) :
		iAnimIndex(_iAnimIndex),
		iCol(_iCol),
		iStartFrameIndex(_iStartFrameIndex),
		iFrameCount(_iFrameCount),
		fTotalTime(_fTotalTime),
		iCountToRepeat(_iCountToRepeat),
		bIsReversePlay(_bIsReversePlay)
	{}

public:
	int iAnimIndex = 0;				// ������ �ִ� �ε���
	int iCol = 1;					// �ִ� ��Ʋ�� �̹������� ���� �� (������ �������� ��� ���� ���ϴµ� ����.)
	int iStartFrameIndex = 0;		// �ִϸ��̼� ���� �ε���
	int iFrameCount = 1;			// ���� �ε����������� ����� �� �Ƽ�
	float fTotalTime = 0.f;			// �ִ� �� ����ð�
	int iCountToRepeat = 0;			// �ִ� �ݺ� Ƚ�� (0 - ����)
	bool bIsReversePlay = false;	// �Ųٷ� ����� ������?
}AnimInfo;

// �ִ� ��� ó�� ����ü
typedef struct _anim_processing_info {
	float fAnimElapsedTime = 0.f;
	int iCurrentIndex = 0;
	int iRepeatedCount = 0;
}AnimProcessingInfo;

// Ÿ�� ���� ����ü
typedef struct _tile_site_info {
public:
	_tile_site_info(int _iTileLineIndex = -1, int _iCoveredRow = 1, int _iCoveredCol = 1, int _iPivotRow = 0, int _iPivotCol = 0)
		:
		iTileLineIndex(_iTileLineIndex),
		iCoveredRow(_iCoveredRow),
		iCoveredCol(_iCoveredCol),
		iPivotRow(_iPivotRow),
		iPivotCol(_iPivotCol)
	{}

public:
	int iTileLineIndex = -1;
	int iCoveredRow = 1;
	int iCoveredCol = 1;
	int iPivotRow = 0;
	int iPivotCol = 0;
}TileSiteInfo;

// Ŀ��� ��� ����ü
typedef struct _command_info {
	COMMANDER::E_COMMAND eCommand = COMMANDER::COMMAND_NOTHING;
	int iUnitID = -1;	// ����� iUnitID�� -1�̶�� ��ü ����
	//bool bIgnoreEnemy = false;	// ���� �� �߰��� ������ ���� ������ ������?
	CComDepObj* pTarget = nullptr;
	D3DXVECTOR3 vTargetPos; // �̵� ��ǥ ����
}CommandInfo;