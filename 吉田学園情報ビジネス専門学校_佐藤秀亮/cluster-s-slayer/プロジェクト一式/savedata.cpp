//=============================================================================
//
// セーブデータ関連の処理 [savedata.cpp]
// Author : 三橋錬
//
//=============================================================================
#include "savedata.h"

int CSaveData::m_nClearPercent = 0;
int CSaveData::m_nHaveMoney = 0;
bool CSaveData::m_bHaveSkill[] = {};
string CSaveData::m_FileName = "";

//=============================================================================
// コンストラクタ
//=============================================================================
CSaveData::CSaveData()
{
	m_nClearPercent = 0;
	m_nHaveMoney = 0;

	for (int SkillNum = 0; SkillNum < CPlayer::Skill_Max; SkillNum++)
	{
		m_bHaveSkill[SkillNum] = false;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CSaveData::~CSaveData()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSaveData::Init(void)
{
	return S_OK;
}

//=============================================================================
// ステージファイルのロード
//=============================================================================
HRESULT CSaveData::LoadFile(string FileName)
{
	m_FileName = FileName;
	m_nClearPercent = 0;
	m_nHaveMoney = 0;

	for (int SkillNum = 0; SkillNum < CPlayer::Skill_Max; SkillNum++)
	{
		m_bHaveSkill[SkillNum] = false;
	}

	// ファイルを読み込む
	ifstream File(m_FileName);

	if (!File)
	{
		return E_FAIL;
	}

	string str = "";

	//=============================================================================
	// ファイル読み込み
	//=============================================================================
	// ファイルの中身を一行ずつ読み取る
	while (getline(File, str))
	{
		if (str == "")
		{
			str = "0";
		}

		// クリア率取得
		if (str.find("ClearPercent=") == 0)
		{
			// 次のブロックのmovexを変更
			str.erase(0, strlen("ClearPercent="));
			m_nClearPercent = stoi(str);
		}

		else if (str.find("HaveMoney=") == 0)
		{
			// 次のブロックのmovexを変更
			str.erase(0, strlen("HaveMoney="));
			m_nHaveMoney = stoi(str);
		}

		else if (str.find("Skill=") == 0)
		{
			// スキル番号に応じたスキルを取得
			str.erase(0, strlen("Skill="));
			m_bHaveSkill[stoi(str)] = true;
		}
	}

	if (m_nClearPercent > m_nMaxClearPercent)	m_nClearPercent = m_nMaxClearPercent;
	if (m_nHaveMoney > m_nMaxHaveMoney)	m_nHaveMoney = m_nMaxHaveMoney;

	// データに各種変数を突っ込む(Set)

	return S_OK;
}

//=============================================================================
// ステージファイルのセーブ
//=============================================================================
HRESULT CSaveData::SaveFile()
{
	// 各種変数にデータを突っ込む(Get)

	// ファイルに書き込む
	ofstream File(m_FileName);

	if (!File)
	{
		return E_FAIL;
	}

	File << "ClearPercent=" << m_nClearPercent << "\n";	// クリア率セーブ
	File << "HaveMoney=" << m_nHaveMoney << "\n";		// 所持金セーブ

	for (int SkillNum = 0; SkillNum < CPlayer::Skill_Max; SkillNum++)
	{
		if (m_bHaveSkill[SkillNum])
		{
			File << "Skill=" << SkillNum << "\n";		// 所持スキルセーブ
		}
	}

	File << endl;

	return S_OK;
}

//=============================================================================
// ステージファイルの消去
//=============================================================================
HRESULT CSaveData::ResetFile(string FileName)
{
	m_FileName = FileName;

	// ファイルに書き込む
	ofstream File(m_FileName);

	if (!File)
	{
		return E_FAIL;
	}

	File << "ClearPercent=" << 0 << "\n";	// クリア率リセット
	File << "HaveMoney=" << 0 << "\n";		// 所持金リセット

	File << endl;

	return S_OK;
}