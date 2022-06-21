//--------------------------------------------
//スキルのレベルごとのステータス情報を保持するクラス
//作成者：林
//--------------------------------------------
#include "skill_leveldata.h"

CBlackHole::State CSkill_LevelData::m_BlackHoleState[CBlackHole::Level_MAX + 1] = {};
CAssistCrystal::State CSkill_LevelData::m_AssistCrystalState[CAssistCrystal::Level_MAX + 1] = {};
CRushAttack::State CSkill_LevelData::m_RushAttackState[CRushAttack::Level_MAX + 1] = {};
CGuard::State CSkill_LevelData::m_GuardState[CGuard::Level_MAX + 1] = {};
CMissile::State CSkill_LevelData::m_MissileState[CMissile::Level_MAX + 1] = {};

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CSkill_LevelData::CSkill_LevelData()
{

}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CSkill_LevelData::~CSkill_LevelData()
{
}
//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CSkill_LevelData::Init(void)
{
	//ファイル読み込み
	BlackHoleLoad("data/TEXT/SKILL_LEVEL/BlackHole.txt");
	AssistCrystalLoad("data/TEXT/SKILL_LEVEL/Beam.txt");
	RushAttackLoad("data/TEXT/SKILL_LEVEL/RushAttack.txt");
	GuardLoad("data/TEXT/SKILL_LEVEL/Sheild.txt");
	MissileLoad("data/TEXT/SKILL_LEVEL/Rocket.txt");

	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CSkill_LevelData::Uninit(void)
{

}

//----------------------------------------------
//インスタンス生成
//----------------------------------------------
CSkill_LevelData *CSkill_LevelData::Create()
{
	CSkill_LevelData *pCharacterPartsData = NULL;
	pCharacterPartsData = new CSkill_LevelData;
	pCharacterPartsData->Init();
	return pCharacterPartsData;
}

//----------------------------------------------
//ロード
//----------------------------------------------
void CSkill_LevelData::BlackHoleLoad(const char * sFileName)
{
	char sString[6][255];	// 読み込み用の変数

	int nLevel = 0;
	// ファイル読み込み
	FILE *pFile = fopen(sFileName, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		// END_SCRIPTが呼ばれるまでループする
		while (1)
		{
			// １単語を読み込む
			fscanf(pFile, "%s", &sString[0]);
			//壁の読み込み
			while (strcmp(sString[0], "BLACKHOLE_LEVEL1") == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "HOLEEXPANSIONTIME") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_BlackHoleState[CBlackHole::Level_1].m_HoleExpansionTime);
				}
				if (strcmp(sString[1], "AREASIZE") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%f", &m_BlackHoleState[CBlackHole::Level_1].m_fArea);

				}
				if (strcmp(sString[1], "END_BLACKHOLE_LEVEL1") == 0)
				{
					break;
				}
			}
			while (strcmp(sString[0], "BLACKHOLE_LEVEL2") == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "HOLEEXPANSIONTIME") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_BlackHoleState[CBlackHole::Level_2].m_HoleExpansionTime);
				}
				if (strcmp(sString[1], "AREASIZE") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%f", &m_BlackHoleState[CBlackHole::Level_2].m_fArea);
				}
				if (strcmp(sString[1], "END_BLACKHOLE_LEVEL2") == 0)
				{
					break;
				}
			}
			while (strcmp(sString[0], "BLACKHOLE_LEVEL3") == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "HOLEEXPANSIONTIME") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_BlackHoleState[CBlackHole::Level_3].m_HoleExpansionTime);
				}
				if (strcmp(sString[1], "AREASIZE") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%f", &m_BlackHoleState[CBlackHole::Level_3].m_fArea);
				}
				if (strcmp(sString[1], "END_BLACKHOLE_LEVEL3") == 0)
				{
					break;
				}
			}
			while (strcmp(sString[0], "BLACKHOLE_LEVEL4") == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "HOLEEXPANSIONTIME") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_BlackHoleState[CBlackHole::Level_4].m_HoleExpansionTime);
				}
				if (strcmp(sString[1], "AREASIZE") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%f", &m_BlackHoleState[CBlackHole::Level_4].m_fArea);
				}
				if (strcmp(sString[1], "END_BLACKHOLE_LEVEL4") == 0)
				{
					break;
				}
			}
			while (strcmp(sString[0], "BLACKHOLE_LEVELMAX") == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "HOLEEXPANSIONTIME") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_BlackHoleState[CBlackHole::Level_MAX].m_HoleExpansionTime);
				}
				if (strcmp(sString[1], "AREASIZE") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%f", &m_BlackHoleState[CBlackHole::Level_MAX].m_fArea);
				}
				if (strcmp(sString[1], "END_BLACKHOLE_LEVELMAX") == 0)
				{
					break;
				}
			}

			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

}
void CSkill_LevelData::GuardLoad(const char * sFileName)
{
	char sString[6][255];	// 読み込み用の変数
	char sStrLevel[255];
	char sEndStrLevel[255];

	int nLevel = 0;
	// ファイル読み込み
	FILE *pFile = fopen(sFileName, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		// END_SCRIPTが呼ばれるまでループする
		while (1)
		{
			// １単語を読み込む
			fscanf(pFile, "%s", &sString[0]);
			if (nLevel < CGuard::Level_MAX)
			{
				wsprintf(&sStrLevel[0], "GUARD_LEVEL%d", nLevel + 1);
				wsprintf(&sEndStrLevel[0], "END_GUARD_LEVEL%d", nLevel + 1);
			}
			else
			{
				wsprintf(&sStrLevel[0], "GUARD_LEVELMAX");
				wsprintf(&sEndStrLevel[0], "END_GUARD_LEVELMAX");
			}
			//壁の読み込み
			while (strcmp(sString[0], &sStrLevel[0]) == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "QUANTITY") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_GuardState[nLevel].m_nMaxGuard);
				}
				if (strcmp(sString[1], &sEndStrLevel[0]) == 0)
				{
					nLevel++;
					break;
				}
			}

			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

}
void CSkill_LevelData::MissileLoad(const char * sFileName)
{
	char sString[6][255];	// 読み込み用の変数
	char sStrLevel[255];
	char sEndStrLevel[255];

	int nLevel = 0;
	// ファイル読み込み
	FILE *pFile = fopen(sFileName, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		// END_SCRIPTが呼ばれるまでループする
		while (1)
		{
			// １単語を読み込む
			fscanf(pFile, "%s", &sString[0]);
			if (nLevel < CGuard::Level_MAX)
			{
				wsprintf(&sStrLevel[0], "MISSILE_LEVEL%d", nLevel + 1);
				wsprintf(&sEndStrLevel[0], "END_MISSILE_LEVEL%d", nLevel + 1);
			}
			else
			{
				wsprintf(&sStrLevel[0], "MISSILE_LEVELMAX");
				wsprintf(&sEndStrLevel[0], "END_MISSILE_LEVELMAX");
			}
			//壁の読み込み
			while (strcmp(sString[0], &sStrLevel[0]) == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "SEARCHRANGE") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%f", &m_MissileState[nLevel].m_SearchRange);
				}
				if (strcmp(sString[1], "POWER") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_MissileState[nLevel].m_nPower);
				}
				if (strcmp(sString[1], "SEARCHMAX") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_MissileState[nLevel].m_nCntSearchMax);
				}
				if (strcmp(sString[1], &sEndStrLevel[0]) == 0)
				{
					nLevel++;
					break;
				}
			}

			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

}
//----------------------------------------------
//ロード
//----------------------------------------------
void CSkill_LevelData::AssistCrystalLoad(const char * sFileName)
{
	char sString[6][255];	// 読み込み用の変数
	char sStrLevel[255];
	char sEndStrLevel[255];

	int nLevel = 0;
	// ファイル読み込み
	FILE *pFile = fopen(sFileName, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		// END_SCRIPTが呼ばれるまでループする
		while (1)
		{
			// １単語を読み込む
			fscanf(pFile, "%s", &sString[0]);
			if (nLevel < CAssistCrystal::Level_MAX)
			{
				wsprintf(&sStrLevel[0], "ASSISTCRYSTAL_LEVEL%d", nLevel+1);
				wsprintf(&sEndStrLevel[0], "END_ASSISTCRYSTAL_LEVEL%d", nLevel+1);
			}
			else
			{
				wsprintf(&sStrLevel[0], "ASSISTCRYSTAL_LEVELMAX");
				wsprintf(&sEndStrLevel[0], "END_ASSISTCRYSTAL_LEVELMAX");
			}
			//壁の読み込み
			while (strcmp(sString[0], &sStrLevel[0]) == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "POWER") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_AssistCrystalState[nLevel].m_nPower);
				}
				if (strcmp(sString[1], "INTERVAL") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_AssistCrystalState[nLevel].m_nLaunchInterval);
				}
				if (strcmp(sString[1], "MOVESPEED") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%f", &m_AssistCrystalState[nLevel].m_fMoveSpeed);
				}
				if (strcmp(sString[1], &sEndStrLevel[0]) == 0)
				{
					nLevel++;
					break;
				}
			}

			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

}

void CSkill_LevelData::RushAttackLoad(const char * sFileName)
{
	char sString[6][255];	// 読み込み用の変数
	char sStrLevel[255];
	char sEndStrLevel[255];

	int nLevel = 0;
	// ファイル読み込み
	FILE *pFile = fopen(sFileName, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		// END_SCRIPTが呼ばれるまでループする
		while (1)
		{
			// １単語を読み込む
			fscanf(pFile, "%s", &sString[0]);
			if (nLevel < CRushAttack::Level_MAX)
			{
				wsprintf(&sStrLevel[0], "RUSHATTACK_LEVEL%d", nLevel + 1);
				wsprintf(&sEndStrLevel[0], "END_RUSHATTACK_LEVEL%d", nLevel + 1);
			}
			else
			{
				wsprintf(&sStrLevel[0], "RUSHATTACK_LEVELMAX");
				wsprintf(&sEndStrLevel[0], "END_RUSHATTACK_LEVELMAX");
			}
			//壁の読み込み
			while (strcmp(sString[0], &sStrLevel[0]) == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "POWER") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%d", &m_RushAttackState[nLevel].m_nPower);
				}
				if (strcmp(sString[1], "SIZEDIAMETER") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[1]);
					fscanf(pFile, "%f", &m_RushAttackState[nLevel].m_fSizeDiameter);
				}

				if (strcmp(sString[1], &sEndStrLevel[0]) == 0)
				{
					nLevel++;
					break;
				}
			}

			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

}
