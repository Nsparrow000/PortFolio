//========================================
// ステージの塊を作成する
// Author:林
//========================================
#include "stage_preset_data.h"
#include "model.h"
#include "wall.h"
#include "field.h"
#include "texture.h"
#include "model_spawner.h"
std::vector<CStage_Preset_Data::PRESETDATA> CStage_Preset_Data::m_PresetData;
//------------------------------------
// コンストラクタ
//------------------------------------
CStage_Preset_Data::CStage_Preset_Data()
{
	m_PresetData.clear();
}

//------------------------------------
// デストラクタ
//------------------------------------
CStage_Preset_Data::~CStage_Preset_Data()
{

}
//------------------------------------
// sステージプリセットファイルの読み込み
//------------------------------------
void CStage_Preset_Data::StagePresetLoad(const char * cFileName)
{
	char sString[2][255];	// 読み込み用の変数
	char sFileName[255];	// 読み込み用の変数

	int nType = 0;
	// ファイル読み込み
	FILE *pFile = fopen(cFileName, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		// END_SCRIPTが呼ばれるまでループする
		while (1)
		{
			// １単語を読み込む
			fscanf(pFile, "%s", &sString[0]);
			//壁の読み込み
			if (strcmp(sString[0], "STAGE_PRESET_FILE") == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[0]);
				// １単語を読み込む
				fscanf(pFile, "%s", &sFileName);
				PRESETDATA hoge = {};
				m_PresetData.push_back(hoge);
				//ステージプリセット情報を読み込む
				StagePresetDataLoad(sFileName, nType);
				//ステージプリセット数を加算
				nType++;
			}
			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

}
//------------------------------------
// ステージプリセットの情報を読み込み
//------------------------------------
void CStage_Preset_Data::StagePresetDataLoad(const char *cFileName, const int& nType)
{
	char sString[6][255];	// 読み込み用の変数
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nBlockX = 0;
	int nBlockY = 0;
	int nModelType = 0;
	int nWallCnt = 0;
	int nFieldCnt = 0;
	int nModelCnt = 0;

	// ファイル読み込み
	FILE *pFile = fopen(cFileName, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		// END_SCRIPTが呼ばれるまでループする
		while (1)
		{
			// １単語を読み込む
			fscanf(pFile, "%s", &sString[0]);
			//壁の読み込み
			while (strcmp(sString[0], "WALLSET") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(sString[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
				}
				if (strcmp(sString[1], "BLOCK") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%d %d", &nBlockX, &nBlockY);
				}
				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f", &size.x, &size.y);
				}
				if (strcmp(sString[1], "END_WALLSET") == 0)
				{
					WALLDATA hoge = {};
					m_PresetData[nType].WallData.push_back(hoge);
					//上で読み込んだ壁の情報を代入
					m_PresetData[nType].WallData[nWallCnt].pos = pos;
					m_PresetData[nType].WallData[nWallCnt].size = size;
					m_PresetData[nType].WallData[nWallCnt].rot = rot;
					m_PresetData[nType].WallData[nWallCnt].block = { (float)nBlockX ,(float)nBlockY };
					pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					nBlockX = 0;
					nBlockY = 0;

					//壁の個数を加算
					nWallCnt++;
					break;
				}
			}

			// メッシュフィールドの読み込み
			while (strcmp(sString[0], "FIELDSET") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(sString[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
				}
				if (strcmp(sString[1], "BLOCK") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%d %d", &nBlockX, &nBlockY);
				}
				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f", &size.x, &size.z);
				}
				if (strcmp(sString[1], "END_FIELDSET") == 0)
				{
					FIELDDATA hoge = {};
					m_PresetData[nType].FieldData.push_back(hoge);
					//上で読み込んだ床の情報を代入
					m_PresetData[nType].FieldData[nFieldCnt].pos = pos;
					m_PresetData[nType].FieldData[nFieldCnt].size = size;
					m_PresetData[nType].FieldData[nFieldCnt].rot = rot;
					m_PresetData[nType].FieldData[nFieldCnt].block = { (float)nBlockX ,(float)nBlockY };
					pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					nBlockX = 0;
					nBlockY = 0;

					//床の個数を加算
					nFieldCnt++;
					break;
				}
			}
			// モデルの読み込み
			while (strcmp(sString[0], "MODELSET") == 0)
			{
				fscanf(pFile, "%s", &sString[1]);
				if (strcmp(sString[1], "TYPE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%d", &nModelType);
				}
				if (strcmp(sString[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(sString[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
				}
				if (strcmp(sString[1], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);	// イコールを噛ませる
					fscanf(pFile, "%f %f", &size.x, &size.z);
				}
				if (strcmp(sString[1], "END_MODELSET") == 0)
				{
					MODELDATA hoge = {};
					m_PresetData[nType].ModelData.push_back(hoge);
					//上で読み込んだ壁の情報を代入
					m_PresetData[nType].ModelData[nModelCnt].pos = pos;
					m_PresetData[nType].ModelData[nModelCnt].size = size;
					m_PresetData[nType].ModelData[nModelCnt].rot = rot;
					m_PresetData[nType].ModelData[nModelCnt].nType = nModelType;
					pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					nBlockX = 0;
					nBlockY = 0;

					//壁の個数を加算
					nModelCnt++;
					break;
				}
			}
			// 敵の読み込み
			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
	}

	// ファイルを閉じる
	fclose(pFile);


}

