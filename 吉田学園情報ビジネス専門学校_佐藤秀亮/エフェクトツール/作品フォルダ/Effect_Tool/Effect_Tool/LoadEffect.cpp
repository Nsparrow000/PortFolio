//=============================================================================
// ロードさせる処理 [Load.cpp]
// Author : 佐藤秀亮
//=============================================================================
//*****************************************************************************
// インクルードファイル
//***************************************************************************** 
#include "LoadEffect.h"
#include "PresetSetEffect.h"

//=============================================================================
// 静的
//=============================================================================
int CLoadEffect::m_Total = 0;


//=============================================================================
// コンストラクタ
//=============================================================================
CLoadEffect::CLoadEffect()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadEffect::~CLoadEffect()
{

}

//=============================================================================
// エフェクト情報の読み込み
//=============================================================================
void CLoadEffect::EffectStateLoad(const char *aFileName)
{
	FILE *pFile;
	pFile = fopen(aFileName, "r");
	char aFile[256];

	m_Total = 0;

	bool bEffectState2D = false;
	bool bEffectState3D = false;

	int nPattern = 0;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR2 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR2 Addmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int Diffusion = 1;
	int Destroyvec = 0;
	float fSize = 0;
	float fAddSize = 0;
	float fRotate = 0;
	D3DCOLORVALUE col;
	D3DCOLORVALUE ChangeColor;
	int nLife = 0;
	int Density = 1;
	int bRandColR = 0;
	int bRandColG = 0;
	int bRandColB = 0;
	int bMousePos = 0;
	int nSynthetic = 0;
	int nTexture = 0;

	float move3d = 0.0f;
	float Addmove3d = 0.0f;
	float MaxSize = 100.0f;
	float ParticleSize = 0.0f;
	float ParticleAddSize = 0.0f;
	int Active = 0;
	D3DCOLORVALUE Secondcol;
	D3DCOLORVALUE SecondChangeColor;
	int SecondSynthetic = 0;
	int TrajectTop = 0;
	int TrajectCur = 0;
	D3DXVECTOR3 Move3D = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int RandMove = 1;
	int Distance = 1;
	int ParticleTime = 1;
	float fActiveAddSize = 0.0f;
	int FieldTime = 0;
	int FieldCreate = 0;
	int CreatePreset = 0;




	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく

			if (bEffectState2D == true)
			{
				if (strcmp(&aFile[0], "PATTERN") == 0)	//動きのパターン
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nPattern);
				}
				if (strcmp(&aFile[0], "POS") == 0)	//出現位置
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROTATE") == 0)	//回転
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fRotate);
				}
				if (strcmp(&aFile[0], "MOVE") == 0)	//移動量
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &move.x, &move.y);
				}
				if (strcmp(&aFile[0], "ADDMOVE") == 0)	//動き加算
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &Addmove.x, &Addmove.y);
				}
				if (strcmp(&aFile[0], "DIFFUSION") == 0)	//拡散率
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &Diffusion);
				}
				if (strcmp(&aFile[0], "DESTROYVEC") == 0)	//消えるベクトル
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &Destroyvec);
				}
				if (strcmp(&aFile[0], "SIZE") == 0)	//大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fSize);
				}
				if (strcmp(&aFile[0], "ADDSIZE") == 0)	//大きさ加算
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fAddSize);
				}
				if (strcmp(&aFile[0], "COLOR") == 0)	//カラー
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f %f", &col.r, &col.g, &col.b, &col.a);
				}
				if (strcmp(&aFile[0], "CHANGECOLOR") == 0)	//カラー変動
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f %f", &ChangeColor.r, &ChangeColor.g, &ChangeColor.b, &ChangeColor.a);
				}
				if (strcmp(&aFile[0], "LIFE") == 0)	//寿命
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nLife);
				}
				if (strcmp(&aFile[0], "DENSITY") == 0)	//密度
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &Density);
				}
				if (strcmp(&aFile[0], "COLORRANDR") == 0)	//カラーランダム赤
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &bRandColR);
				}
				if (strcmp(&aFile[0], "COLORRANDG") == 0)	//カラーランダム緑
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &bRandColG);
				}
				if (strcmp(&aFile[0], "COLORRANDB") == 0)	//カラーランダム青
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &bRandColB);
				}
				if (strcmp(&aFile[0], "MOUSEPOS") == 0)		//マウス追従(プレイヤー追従だったり)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &bMousePos);
				}
				if (strcmp(&aFile[0], "SYNTHETIC") == 0)		//合成
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nSynthetic);
				}
				if (strcmp(&aFile[0], "TEXTURE") == 0)		//テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTexture);
				}

			}

			if (bEffectState3D == true)
			{
				if (strcmp(&aFile[0], "PATTERN") == 0)	//動きのパターン
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nPattern);
				}
				if (strcmp(&aFile[0], "POS") == 0)	//出現位置
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROTATE") == 0)	//回転
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fRotate);
				}
				if (strcmp(&aFile[0], "MOVE") == 0)	//移動量
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &move3d);
				}
				if (strcmp(&aFile[0], "ADDMOVE") == 0)	//動き加算
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &Addmove3d);
				}
				if (strcmp(&aFile[0], "DIFFUSION") == 0)	//拡散率
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &Diffusion);
				}
				if (strcmp(&aFile[0], "SIZE") == 0)	//大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fSize);
				}
				if (strcmp(&aFile[0], "ADDSIZE") == 0)	//大きさ加算
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fAddSize);
				}
				if (strcmp(&aFile[0], "MAXSIZE") == 0)	//最大の大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &MaxSize);
				}
				if (strcmp(&aFile[0], "PARTICLESIZE") == 0)	//最大の大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &ParticleSize);
				}
				if (strcmp(&aFile[0], "PARTICLEADDSIZE") == 0)	//最大の大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &ParticleAddSize);
				}
				if (strcmp(&aFile[0], "ACTIVE") == 0)	//稼働時間
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &Active);
				}
				if (strcmp(&aFile[0], "COLOR") == 0)	//カラー
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f %f", &col.r, &col.g, &col.b, &col.a);
				}
				if (strcmp(&aFile[0], "CHANGECOLOR") == 0)	//カラー変動
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f %f", &ChangeColor.r, &ChangeColor.g, &ChangeColor.b, &ChangeColor.a);
				}
				if (strcmp(&aFile[0], "SECONDCOLOR") == 0)	//２番カラー
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f %f", &Secondcol.r, &Secondcol.g, &Secondcol.b, &Secondcol.a);
				}
				if (strcmp(&aFile[0], "SECONDADDCOLOR") == 0)	//２番カラー変動
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f %f", &SecondChangeColor.r, &SecondChangeColor.g, &SecondChangeColor.b, &SecondChangeColor.a);
				}
				if (strcmp(&aFile[0], "SECONDSYNTHETIC") == 0)		//合成
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &SecondSynthetic);
				}
				if (strcmp(&aFile[0], "LIFE") == 0)	//寿命
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nLife);
				}
				if (strcmp(&aFile[0], "DENSITY") == 0)	//密度
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &Density);
				}
				if (strcmp(&aFile[0], "TRAJECTTOP") == 0)	//モデル１
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &TrajectTop);
				}
				if (strcmp(&aFile[0], "TRAJECTCUR") == 0)	//モデル２
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &TrajectTop);
				}
				if (strcmp(&aFile[0], "TRAJECTCUR") == 0)	//モデル２
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &TrajectTop);
				}
				if (strcmp(&aFile[0], "MOVE3D") == 0)	//３ｄ移動量
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &Move3D.x, &Move3D.y, &Move3D.z);
				}
				if (strcmp(&aFile[0], "RANDMOVE") == 0)	//ランダム移動
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &RandMove);
				}
				if (strcmp(&aFile[0], "COLORRANDR") == 0)	//カラーランダム赤
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &bRandColR);
				}
				if (strcmp(&aFile[0], "COLORRANDG") == 0)	//カラーランダム緑
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &bRandColG);
				}
				if (strcmp(&aFile[0], "COLORRANDB") == 0)	//カラーランダム青
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &bRandColB);
				}
				if (strcmp(&aFile[0], "SYNTHETIC") == 0)		//合成
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nSynthetic);
				}
				if (strcmp(&aFile[0], "TEXTURE") == 0)		//テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTexture);
				}
				if (strcmp(&aFile[0], "DISTANCE") == 0)		//発生距離
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &Distance);
				}
				if (strcmp(&aFile[0], "PARTICLETIME") == 0)		//発生間隔
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &ParticleTime);
				}
				if (strcmp(&aFile[0], "ACTIVEADDSIZE") == 0)	//大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fActiveAddSize);
				}
				if (strcmp(&aFile[0], "FIELDTIME") == 0)		//発生距離
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &FieldTime);
				}
				if (strcmp(&aFile[0], "FIELDCREATE") == 0)	//カラーランダム緑
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &FieldCreate);
				}
				if (strcmp(&aFile[0], "CREATEPRESET") == 0)	//大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &CreatePreset);
				}

			}
			//エフェクト情報セット
			if (strcmp(&aFile[0], "EFFECTSTATE2D") == 0)
			{
				bEffectState2D = true;
			}
			if (strcmp(&aFile[0], "END_EFFECTSTATE2D") == 0)
			{
				bEffectState2D = false;
				CPresetEffect::SetEffectState2D(nPattern, pos, fRotate, move, Addmove, Diffusion, Destroyvec, fSize, fAddSize, col, ChangeColor, nLife, Density,
					(bool)bRandColR, (bool)bRandColG, (bool)bRandColB, bMousePos, nSynthetic, nTexture);
			}

			//3Dエフェクト情報セット
			if (strcmp(&aFile[0], "EFFECTSTATE3D") == 0)
			{
				bEffectState3D = true;
			}
			if (strcmp(&aFile[0], "END_EFFECTSTATE3D") == 0)
			{
				bEffectState3D = false;
				CPresetEffect::SetEffectState3D(nPattern, fRotate, move3d, Addmove3d, Diffusion, fSize, fAddSize, MaxSize, ParticleSize,
					ParticleAddSize, Active, col, ChangeColor, Secondcol, SecondChangeColor, SecondSynthetic, nLife, Density, TrajectTop, TrajectCur, Move3D, RandMove,
					(bool)bRandColR, (bool)bRandColG, (bool)bRandColB,
					nSynthetic, nTexture, Distance, ParticleTime, pos, fActiveAddSize,
					FieldTime, (bool)FieldCreate, CreatePreset);

				m_Total++;
			}

			//終わり
			if (strcmp(&aFile[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	fclose(pFile);
	CPresetEffect::ResetPattern();
}


////=============================================================================
//// プリセットトータルの読み込み
////=============================================================================
//void CLoadEffect::PresetTotal(const char *aFileName)
//{
//	FILE *pFile;
//	pFile = fopen(aFileName, "r");
//	char aFile[256];
//
//
//	if (pFile != NULL)
//	{
//		while (true)
//		{
//			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
//
//			if (strcmp(&aFile[0], "TOTALPRESET") == 0)	//トータル
//			{
//				fscanf(pFile, "%s", &aFile[0]);
//				fscanf(pFile, "%d", &m_Total);
//			}
//
//			//終わり
//			if (strcmp(&aFile[0], "END_SCRIPT") == 0)
//			{
//				break;
//			}
//
//		}
//	}
//	fclose(pFile);
//
//}
