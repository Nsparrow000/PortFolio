//=============================================================================
//
// セーブデータ関連の処理 [savedata.h]
// Author : 三橋錬
//
//=============================================================================
#ifndef _SAVEDATA_H_
#define _SAVEDATA_H_
#include "player.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSaveData
{
public:
	CSaveData();
	~CSaveData();

	HRESULT Init(void);

	static HRESULT LoadFile(string FileName);
	static HRESULT SaveFile();
	static HRESULT ResetFile(string FileName);
	static bool GetHaveSkill(int nSkillType) { return m_bHaveSkill[nSkillType]; }
	static void FetchSetHaveSkill(int nSkillType) { m_bHaveSkill[nSkillType] = true; }
	static void SetClearPercent(int nClearPercent) { m_nClearPercent = nClearPercent; }
	static int GetClearPercent() { return m_nClearPercent; }
private:
	static int m_nClearPercent;		// クリア率
	static const int m_nMaxClearPercent = 100;

	static int m_nHaveMoney;		// 所持金
	static const int m_nMaxHaveMoney = 99999;

	static bool m_bHaveSkill[CPlayer::Skill_Max];	// 所持スキル

	static string m_FileName;
};

#endif