//ーーーーーーーーーーーーーーーーーーーーーーーーー
//エフェクトの処理(Effect.cpp)
//ーーーーーーーーーーーーーーーーーーーーーーーーー

#include "main.h"
#include "input.h"
#include "Effect.h"
#include "camera.h"

#define MOVE_Y (3.0f)

#define SAVEFILENAME "data/EffectSave.txt"


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect[64] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//頂点バッファへのポインタ

Effect g_Effect[MAX_EFFECT];

Effectstate g_Effectstate;

D3DXVECTOR3 rot;
D3DXVECTOR3 rot2;

int g_EffectTexture;
int g_EffectMaxTexture;

//初期化処理
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
	int nCntEffect;
	pDevice = GetDevice();//デバイスの取得

	g_EffectTexture = 0;
	g_EffectMaxTexture = 0;

	//エフェクトの初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_Effect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_Effect[nCntEffect].colorR = 0;
		g_Effect[nCntEffect].colorG = 0;
		g_Effect[nCntEffect].colorB = 0;
		g_Effect[nCntEffect].colorA = 0;

		g_Effect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Effect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_Effect[nCntEffect].fHight = 0;
		g_Effect[nCntEffect].fWhidth = 0;

		g_Effect[nCntEffect].bUse = false;
		g_Effect[nCntEffect].nlife = 0;
		g_Effect[nCntEffect].ntype = 0;
	}

	g_Effectstate.colorR = 255;
	g_Effectstate.colorG = 255;
	g_Effectstate.colorB = 255;
	g_Effectstate.colorA = 255;

	g_Effectstate.MinColorR = 0;
	g_Effectstate.MinColorG = 0;
	g_Effectstate.MinColorB = 0;
	g_Effectstate.MinColorA = 0;

	g_Effectstate.bTap = false;
	g_Effectstate.SerectColor = 0;

	g_Effectstate.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_Effectstate.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Effectstate.nLifeSerect = 30;
	g_Effectstate.SerectType = 0;
	g_Effectstate.nRadius = 50;
	g_Effectstate.nDensity = 10;

	g_Effectstate.fWhidth = 10;
	g_Effectstate.fHight = 10;
	g_Effectstate.fMinWhidth = 0;
	g_Effectstate.fMinHight = 0;

	char aFile[256];

	FILE *pFile = fopen(LOADFILENAME, "r");

	if (pFile != NULL)//一度ファイルを読み込んで必要な情報を抜く
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "NUM_TEXTURE") == 0)	//NUM_TEXTUREの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &g_EffectMaxTexture);
			}
			if (strcmp(&aFile[0], "TEXTURE_FILENAME") == 0) //TEXTURE_FILENAMEの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXCreateTextureFromFile(pDevice, &aFile[0], &g_pTextureEffect[g_EffectTexture]);
				g_EffectTexture++;
			}

			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
			{
				break;
			}
		}
	}
	fclose(pFile);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

}

//終了処理
void UninitEffect(void)//追加の可能性があるため項目は作っとく
{
	for (int nCntEffect = 0; nCntEffect < g_EffectMaxTexture; nCntEffect++)
	{
		//テクスチャの破棄
		if (g_pTextureEffect != NULL)
		{
			g_pTextureEffect[nCntEffect]->Release();
			g_pTextureEffect[nCntEffect] = NULL;
		}

		//頂点バッファも破棄
		if (g_pVtxBuffEffect != NULL)
		{
			g_pVtxBuffEffect->Release();
			g_pVtxBuffEffect = NULL;
		}
	}
}

//更新処理
void UpdateEffect(void)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ
	Camera * pCamera = GetCamera();


	float fAngle = 0;
	float fSpeed = 3;
	
	//キーボード操作
#if 1
	//色選択の変更
	if (GetKeyboardTrigger(DIK_RIGHT) == TRUE)
	{
		g_Effectstate.SerectColor += 1;
		if (g_Effectstate.SerectColor > 3)
		{
			g_Effectstate.SerectColor = 0;
		}
	}
	else if (GetKeyboardTrigger(DIK_LEFT) == TRUE)
	{
		g_Effectstate.SerectColor -= 1;
		if (g_Effectstate.SerectColor < 0)
		{
			g_Effectstate.SerectColor = 3;
		}
	}

	//エフェクトカラー赤の変更
	if (GetKeyboardPress(DIK_Y) == TRUE && g_Effectstate.SerectColor == 0)
	{
		g_Effectstate.colorR += 1;
		if (g_Effectstate.colorR >= 255)
		{
			g_Effectstate.colorR = 255;
		}
	}
	else if (GetKeyboardPress(DIK_H) == TRUE && g_Effectstate.SerectColor == 0)
	{
		g_Effectstate.colorR -= 1;
		if (g_Effectstate.colorR < 0)
		{
			g_Effectstate.colorR = 0;
		}
	}

	//エフェクトカラー緑の変更
	if (GetKeyboardPress(DIK_Y) == TRUE && g_Effectstate.SerectColor == 1)
	{
		g_Effectstate.colorG += 1;
		if (g_Effectstate.colorG > 255)
		{
			g_Effectstate.colorG = 255;
		}
	}
	else if (GetKeyboardPress(DIK_H) == TRUE && g_Effectstate.SerectColor == 1)
	{
		g_Effectstate.colorG -= 1;
		if (g_Effectstate.colorG < 0)
		{
			g_Effectstate.colorG = 0;
		}
	}

	//エフェクトカラー青の変更
	if (GetKeyboardPress(DIK_Y) == TRUE && g_Effectstate.SerectColor == 2)
	{
		g_Effectstate.colorB += 1;
		if (g_Effectstate.colorB > 255)
		{
			g_Effectstate.colorB = 255;
		}
	}
	else if (GetKeyboardPress(DIK_H) == TRUE && g_Effectstate.SerectColor == 2)
	{
		g_Effectstate.colorB -= 1;
		if (g_Effectstate.colorB < 0)
		{
			g_Effectstate.colorB = 0;
		}
	}
	//エフェクトカラーα値の変更
	if (GetKeyboardPress(DIK_Y) == TRUE && g_Effectstate.SerectColor == 3)
	{
		g_Effectstate.colorA += 1;
		if (g_Effectstate.colorA > 255)
		{
			g_Effectstate.colorA = 255;
		}
	}
	else if (GetKeyboardPress(DIK_H) == TRUE && g_Effectstate.SerectColor == 3)
	{
		g_Effectstate.colorA -= 1;
		if (g_Effectstate.colorA < 0)
		{
			g_Effectstate.colorA = 0;
		}
	}


	//赤の減衰値の変更
	if (GetKeyboardPress(DIK_U) == TRUE && g_Effectstate.SerectColor == 0)
	{
		g_Effectstate.MinColorR += 1;
		if (g_Effectstate.MinColorR > 20)
		{
			g_Effectstate.MinColorR = 20;
		}
	}
	else if (GetKeyboardPress(DIK_J) == TRUE && g_Effectstate.SerectColor == 0)
	{
		g_Effectstate.MinColorR -= 1;
		if (g_Effectstate.MinColorR < 0)
		{
			g_Effectstate.MinColorR = 0;
		}
	}

	//緑の減衰値の変更
	if (GetKeyboardPress(DIK_U) == TRUE && g_Effectstate.SerectColor == 1)
	{
		g_Effectstate.MinColorG += 1;
		if (g_Effectstate.MinColorG > 20)
		{
			g_Effectstate.MinColorG = 20;
		}
	}
	else if (GetKeyboardPress(DIK_J) == TRUE && g_Effectstate.SerectColor == 1)
	{
		g_Effectstate.MinColorG -= 1;
		if (g_Effectstate.MinColorG < 0)
		{
			g_Effectstate.MinColorG = 0;
		}
	}

	//青の減衰値の変更
	if (GetKeyboardPress(DIK_U) == TRUE && g_Effectstate.SerectColor == 2)
	{
		g_Effectstate.MinColorB += 1;
		if (g_Effectstate.MinColorB > 20)
		{
			g_Effectstate.MinColorB = 20;
		}
	}
	else if (GetKeyboardPress(DIK_J) == TRUE && g_Effectstate.SerectColor == 2)
	{
		g_Effectstate.MinColorB -= 1;
		if (g_Effectstate.MinColorB < 0)
		{
			g_Effectstate.MinColorB = 0;
		}
	}

	//α値の減衰値の変更
	if (GetKeyboardPress(DIK_U) == TRUE && g_Effectstate.SerectColor == 3)
	{
		g_Effectstate.MinColorA += 1;
		if (g_Effectstate.MinColorA > 20)
		{
			g_Effectstate.MinColorA = 20;
		}
	}
	else if (GetKeyboardPress(DIK_J) == TRUE && g_Effectstate.SerectColor == 3)
	{
		g_Effectstate.MinColorA -= 1;
		if (g_Effectstate.MinColorA < 0)
		{
			g_Effectstate.MinColorA = 0;
		}
	}


	//選択エフェクトタイプの変更と色、減衰値リセット
	if (GetKeyboardTrigger(DIK_UP) == TRUE)
	{
		//g_Effectstate.colorR = 255;
		//g_Effectstate.colorG = 255;
		//g_Effectstate.colorB = 255;
		//g_Effectstate.colorA = 255;

		g_Effectstate.MinColorR = 0;
		g_Effectstate.MinColorG = 0;
		g_Effectstate.MinColorB = 0;
		g_Effectstate.MinColorA = 0;

		g_Effectstate.SerectType += 1;
		if (g_Effectstate.SerectType > EFFECTTYPE_MAX - 1)
		{
			g_Effectstate.SerectType = 0;
		}
	}
	else if (GetKeyboardTrigger(DIK_DOWN) == TRUE)
	{
		//g_Effectstate.colorR = 255;
		//g_Effectstate.colorG = 255;
		//g_Effectstate.colorB = 255;
		//g_Effectstate.colorA = 255;

		g_Effectstate.MinColorR = 0;
		g_Effectstate.MinColorG = 0;
		g_Effectstate.MinColorB = 0;
		g_Effectstate.MinColorA = 0;

		g_Effectstate.SerectType -= 1;
		if (g_Effectstate.SerectType < 0)
		{
			g_Effectstate.SerectType = EFFECTTYPE_MAX - 1;
		}
	}

	//エフェクトライフの変更
	if (GetKeyboardPress(DIK_O) == TRUE)
	{
		g_Effectstate.nLifeSerect += 1;
		if (g_Effectstate.nLifeSerect > 75)
		{
			g_Effectstate.nLifeSerect = 75;
		}
	}
	else if (GetKeyboardPress(DIK_L) == TRUE)
	{
		g_Effectstate.nLifeSerect -= 1;
		if (g_Effectstate.nLifeSerect < 10)
		{
			g_Effectstate.nLifeSerect = 10;
		}
	}

	//密度の変更
	if (GetKeyboardPress(DIK_I) == TRUE)
	{
		g_Effectstate.nDensity += 1;
		if (g_Effectstate.nDensity > 75)
		{
			g_Effectstate.nDensity = 75;
		}
	}
	else if (GetKeyboardPress(DIK_K) == TRUE)
	{
		g_Effectstate.nDensity -= 1;
		if (g_Effectstate.nDensity < 1)
		{
			g_Effectstate.nDensity = 1;
		}
	}

	//大きさ(XY)の変更
	if (GetKeyboardTrigger(DIK_Z) == TRUE)
	{
		g_Effectstate.fWhidth += 1;
		g_Effectstate.fHight += 1;
		if (g_Effectstate.fWhidth > 20 || 
			g_Effectstate.fHight > 20)
		{
			g_Effectstate.fHight = 20;
			g_Effectstate.fWhidth = 20;
		}
	}
	else if (GetKeyboardTrigger(DIK_X) == TRUE)
	{
		g_Effectstate.fWhidth -= 1;
		g_Effectstate.fHight -= 1;
		if (g_Effectstate.fWhidth < 5 || 
			g_Effectstate.fHight < 5)
		{
			g_Effectstate.fHight = 5;
			g_Effectstate.fWhidth = 5;
		}
	}


	//大きさ減衰(XY)の変更
	if (GetKeyboardTrigger(DIK_C) == TRUE)
	{
		g_Effectstate.fMinWhidth += 0.1;
		g_Effectstate.fMinHight += 0.1;
		if (g_Effectstate.fMinWhidth > 1 ||
			g_Effectstate.fMinHight > 1)
		{
			g_Effectstate.fMinHight = 1;
			g_Effectstate.fMinWhidth = 1;
		}
	}
	else if (GetKeyboardTrigger(DIK_V) == TRUE)
	{
		g_Effectstate.fMinWhidth -= 0.1;
		g_Effectstate.fMinHight -= 0.1;
		if (g_Effectstate.fMinWhidth < 0 ||
			g_Effectstate.fMinHight < 0)
		{
			g_Effectstate.fMinHight = 0;
			g_Effectstate.fMinWhidth = 0;
		}
	}


	//出現位置の半径の変更
	if (GetKeyboardPress(DIK_P) == TRUE)
	{
		g_Effectstate.nRadius += 2;
		if (g_Effectstate.nRadius > 200)
		{
			g_Effectstate.nRadius = 0;
		}
	}

	if (GetKeyboardTrigger(DIK_F9))
	{
		SaveEffect();
	}

	//出現位置の半径の変更
	if (GetKeyboardTrigger(DIK_1) == TRUE && g_Effectstate.bTap == false)
	{
		g_Effectstate.bTap = true;
	}

	//出現位置の半径の変更
	else if (GetKeyboardTrigger(DIK_1) == TRUE && g_Effectstate.bTap == true)
	{
		g_Effectstate.bTap = false;
	}

#endif

	//エフェクトのセット
#if 1
	switch (g_Effectstate.SerectType)
	{
	case(EFFECTTYPE_CIRCLE):
		if (GetKeyboardTrigger(DIK_SPACE) == TRUE)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)/** fSpeed*/, cosf(fAngle)/* * fSpeed*/, tanf(fAngle)/** fSpeed*/);
				SetEffect(D3DXVECTOR3(sinf(fAngle) * g_Effectstate.nRadius, 10.0, cosf(fAngle) * g_Effectstate.nRadius), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), move, g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}

		if (g_Effectstate.bTap == false)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)* fSpeed, cosf(fAngle) * fSpeed, tanf(fAngle)* fSpeed);
				SetEffect(D3DXVECTOR3(sinf(fAngle) * g_Effectstate.nRadius, 10.0, cosf(fAngle) * g_Effectstate.nRadius), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), move, g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		break;
	case(EFFECTTYPE_STRIGHIT):
		if (GetKeyboardTrigger(DIK_SPACE) == TRUE)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				SetEffect(D3DXVECTOR3(0.0, 10.0, 0.0), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), D3DXVECTOR3(0.0, pCamera->rot.y, 0.0), g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		if (g_Effectstate.bTap == false)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				SetEffect(D3DXVECTOR3(0.0, 10.0, 0.0), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), D3DXVECTOR3(0.0, pCamera->rot.y, 0.0), g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		break;
	case(EFFECTTYPE_FOLL):
		if (GetKeyboardTrigger(DIK_SPACE) == TRUE)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)* fSpeed, cosf(fAngle) * fSpeed, tanf(fAngle)* fSpeed);
				SetEffect(D3DXVECTOR3(0.0, 10.0, 0.0), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), move, g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		if (g_Effectstate.bTap == false)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)* fSpeed, cosf(fAngle) * fSpeed, tanf(fAngle)* fSpeed);
				SetEffect(D3DXVECTOR3(0.0, 10.0, 0.0), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), move, g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		break;
	case(EFFECTTYPE_POP):

		if (GetKeyboardTrigger(DIK_SPACE) == TRUE)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)* fSpeed, cosf(fAngle) * fSpeed, CIRCLE * 1.5f);
				SetEffect(D3DXVECTOR3(0.0, 10.0, 0.0), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), move, g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		if (g_Effectstate.bTap == false)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)* fSpeed, cosf(fAngle) * fSpeed, CIRCLE * 1.5f);
				SetEffect(D3DXVECTOR3(0.0, 10.0, 0.0), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), move, g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		break;
	case(EFFECTTYPE_BIRD):
		if (GetKeyboardTrigger(DIK_SPACE) == TRUE)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)* fSpeed, cosf(fAngle) * fSpeed, tanf(fAngle)* fSpeed);
				SetEffect(D3DXVECTOR3(sinf(fAngle) * g_Effectstate.nRadius, 10.0, cosf(fAngle) * g_Effectstate.nRadius), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), move, g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		if (g_Effectstate.bTap == false)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)* fSpeed, cosf(fAngle) * fSpeed, tanf(fAngle)* fSpeed);
				SetEffect(D3DXVECTOR3(sinf(fAngle) * g_Effectstate.nRadius, 10.0, cosf(fAngle) * g_Effectstate.nRadius), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), move, g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		break;

	case(EFFECTTYPE_DOME):
		if (GetKeyboardTrigger(DIK_SPACE) == TRUE)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)* fSpeed, CIRCLE * 1.5f, cosf(fAngle) * fSpeed);
				SetEffect(D3DXVECTOR3(0.0, DOME_START, 0.0), D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA), move, g_Effectstate.fWhidth, g_Effectstate.fHight, g_Effectstate.SerectType, g_Effectstate.nLifeSerect);
			}
		}
		if (g_Effectstate.bTap == false)
		{
			for (int nCnt = 0; nCnt < g_Effectstate.nDensity; nCnt++)
			{
				fAngle = CIRCLE;
				D3DXVECTOR3 move = D3DXVECTOR3(sinf(fAngle)* fSpeed, cosf(fAngle) * fSpeed, CIRCLE * 1.5f);
				SetEffect(D3DXVECTOR3(0.0, DOME_START, 0.0),
					D3DXCOLOR(g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA),
					move,
					g_Effectstate.fWhidth,
					g_Effectstate.fHight,
					g_Effectstate.SerectType, 
					g_Effectstate.nLifeSerect);
			}
		}
		break;

	}
#endif
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		
		if (g_Effect[nCntEffect].bUse == true)
		{
			//エフェクトの種類で処理を分ける
			switch (g_Effect[nCntEffect].ntype)
			{
			case(EFFECTTYPE_CIRCLE)://出現位置が円形

				g_Effect[nCntEffect].move.y += 0.2;
				g_Effect[nCntEffect].pos.y += g_Effect[nCntEffect].move.y;
				break;

			case(EFFECTTYPE_STRIGHIT)://まっすぐ

				g_Effect[nCntEffect].pos.x += sinf(g_Effect[nCntEffect].rot.y) * 10;
				g_Effect[nCntEffect].pos.z += cosf(g_Effect[nCntEffect].rot.y) * 10;
				break;

			case(EFFECTTYPE_FOLL)://噴水
				
				g_Effect[nCntEffect].move.y -= 1;
				g_Effect[nCntEffect].pos.x += sinf(g_Effect[nCntEffect].rot.y) * 5;
				g_Effect[nCntEffect].pos.z += cosf(g_Effect[nCntEffect].rot.y) * 5;
				g_Effect[nCntEffect].pos.y += g_Effect[nCntEffect].move.y;
				break;

			case(EFFECTTYPE_POP)://弾け型

				g_Effect[nCntEffect].pos += g_Effect[nCntEffect].rot;
				break;

			case(EFFECTTYPE_BIRD)://鳥型

				g_Effect[nCntEffect].move.y -= 0.05;
				g_Effect[nCntEffect].pos.y += g_Effect[nCntEffect].move.y;
				if (g_Effect[nCntEffect].pos.x > 0)
				{
					g_Effect[nCntEffect].pos.x -= g_Effect[nCntEffect].rot.y;
					if (g_Effect[nCntEffect].pos.x < 0)
					{
						g_Effect[nCntEffect].bUse == false;
					}
				}
				else if (g_Effect[nCntEffect].pos.x < 0)
				{
					g_Effect[nCntEffect].pos.x += g_Effect[nCntEffect].rot.y;

					if (g_Effect[nCntEffect].pos.x > 0)
					{
						g_Effect[nCntEffect].bUse == false;
					}

				}

				if (g_Effect[nCntEffect].pos.z > 0)
				{
					g_Effect[nCntEffect].pos.z += g_Effect[nCntEffect].rot.y;
				}
				else if (g_Effect[nCntEffect].pos.z < 0)
				{
					g_Effect[nCntEffect].pos.z += g_Effect[nCntEffect].rot.y;

				}

				break;

			case(EFFECTTYPE_DOME)://広がりつつ上にあがる

				g_Effect[nCntEffect].move.y -= 0.05;
				g_Effect[nCntEffect].pos.x += cosf(g_Effect[nCntEffect].rot.y) * 1.5;
				g_Effect[nCntEffect].pos.z += sinf(g_Effect[nCntEffect].rot.y) * 1.5;
				g_Effect[nCntEffect].pos.y += g_Effect[nCntEffect].move.y;
				break;

			}

			//常に必要な処理
#if 1
			//サイズ変更
			g_Effect[nCntEffect].fHight -= g_Effectstate.fMinHight;
			g_Effect[nCntEffect].fWhidth -= g_Effectstate.fMinWhidth;

			//色変更
			g_Effect[nCntEffect].colorR -= g_Effectstate.MinColorR;
			g_Effect[nCntEffect].colorG -= g_Effectstate.MinColorG;
			g_Effect[nCntEffect].colorB -= g_Effectstate.MinColorB;
			g_Effect[nCntEffect].colorA -= g_Effectstate.MinColorA;

			//寿命減らし
			g_Effect[nCntEffect].nlife--;
			if (g_Effect[nCntEffect].nlife < 0)
			{
				g_Effect[nCntEffect].bUse = false;
			}
			//0より下の数値に行かないようにする
			if (g_Effect[nCntEffect].colorR < 0)
			{
				g_Effect[nCntEffect].colorR = 0;
			}

			if (g_Effect[nCntEffect].colorG < 0)
			{
				g_Effect[nCntEffect].colorG = 0;
			}

			if (g_Effect[nCntEffect].colorB < 0)
			{
				g_Effect[nCntEffect].colorB = 0;
			}

			if (g_Effect[nCntEffect].colorA < 0)
			{
				g_Effect[nCntEffect].colorA = 0;
			}

			if (g_Effect[nCntEffect].fHight < 0)
			{
				g_Effect[nCntEffect].fHight = 0;
			}
			if (g_Effect[nCntEffect].fWhidth < 0)
			{
				g_Effect[nCntEffect].fWhidth = 0;
			}
#endif
			//頂点の座標
			pVtx[0].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fWhidth / 2, +g_Effect[nCntEffect].fHight / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fWhidth / 2, +g_Effect[nCntEffect].fHight / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fWhidth / 2, -g_Effect[nCntEffect].fHight / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fWhidth / 2, -g_Effect[nCntEffect].fHight / 2, 0.0f);

			//頂点の色
			pVtx[0].col = D3DCOLOR_RGBA(g_Effect[nCntEffect].colorR,g_Effect[nCntEffect].colorG,g_Effect[nCntEffect].colorB,g_Effect[nCntEffect].colorA);
			pVtx[1].col = D3DCOLOR_RGBA(g_Effect[nCntEffect].colorR,g_Effect[nCntEffect].colorG,g_Effect[nCntEffect].colorB,g_Effect[nCntEffect].colorA);
			pVtx[2].col = D3DCOLOR_RGBA(g_Effect[nCntEffect].colorR,g_Effect[nCntEffect].colorG,g_Effect[nCntEffect].colorB,g_Effect[nCntEffect].colorA);
			pVtx[3].col = D3DCOLOR_RGBA(g_Effect[nCntEffect].colorR,g_Effect[nCntEffect].colorG,g_Effect[nCntEffect].colorB,g_Effect[nCntEffect].colorA);
			pVtx += 4;
		}

	}
	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();


	//3.14 ～ -3.14に変える処理
	if (rot.y >= D3DX_PI)
	{
		rot.y -= D3DX_PI * 2;
	}

	else if (rot.y <= -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}

	//3.14 ～ -3.14に変える処理
	if (rot2.y >= D3DX_PI)
	{
		rot2.y -= D3DX_PI * 2;
	}

	else if (rot2.y <= -D3DX_PI)
	{
		rot2.y += D3DX_PI * 2;
	}


}



//描画処理
void DrawEffect(void)
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ

	D3DXMATRIX mtxTrans;
	pDevice = GetDevice();//デバイスを取得する

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_Effect[nCntEffect].bUse == true)
		{

			//Zテスト関係
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//アルファテスト関係
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);


			//加算合成関係
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//マトリックス初期化
			D3DXMatrixIdentity(&g_Effect[nCntEffect].mtxWorld);

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &g_Effect[nCntEffect].mtxView);

			//ポリゴンをカメラに対して正面に
			D3DXMatrixInverse(&g_Effect[nCntEffect].mtxWorld, NULL, &g_Effect[nCntEffect].mtxView);

			g_Effect[nCntEffect].mtxWorld._41 = 0.0f;
			g_Effect[nCntEffect].mtxWorld._42 = 0.0f;
			g_Effect[nCntEffect].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Effect[nCntEffect].pos.x, g_Effect[nCntEffect].pos.y, g_Effect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_Effect[nCntEffect].mtxWorld, &g_Effect[nCntEffect].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Effect[nCntEffect].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			pDevice->SetFVF(FVF_VERTEX_3D);//頂点フォーマットの設定

			pDevice->SetTexture(0, g_pTextureEffect[g_Effect[nCntEffect].ntype]);//テクスチャの設定

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

			//Zテスト関係
			
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//通常合成に戻す(加算合成)
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//アルファテスト関係
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
		}

	}

}

void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXVECTOR3 rot, float fWhidth, float fHight, int ntype, int nLife)
{
	int nCntEffect;
	int frand = 0;
	frand = POSRAND;
	VERTEX_3D*pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_Effect[nCntEffect].bUse == false)
		{
			g_Effect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_Effect[nCntEffect].fWhidth = fWhidth;
			g_Effect[nCntEffect].fHight = fHight;
			g_Effect[nCntEffect].ntype = ntype;
			g_Effect[nCntEffect].pos = pos;
			g_Effect[nCntEffect].colorR = color.r;
			g_Effect[nCntEffect].colorG = color.g; 
			g_Effect[nCntEffect].colorB = color.b; 
			g_Effect[nCntEffect].colorA = color.a;
			g_Effect[nCntEffect].rot = rot;


			////頂点の座標
			//pVtx[0].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fWhidth / 2, +g_Effect[nCntEffect].fHight, 0.0f);
			//pVtx[1].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fWhidth / 2, +g_Effect[nCntEffect].fHight, 0.0f);
			//pVtx[2].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fWhidth / 2, 0.0f, 0.0f);
			//pVtx[3].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fWhidth / 2, 0.0f, 0.0f);

			//ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//テクスチャのUV座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


			g_Effect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (g_Effect[nCntEffect].ntype == EFFECTTYPE_FOLL)
			{
				g_Effect[nCntEffect].move.y = frand;
			}
			else if (g_Effect[nCntEffect].ntype == EFFECTTYPE_POP)
			{
				g_Effect[nCntEffect].move.y = rand() % 5 - 1 - rand() % -5 + 1;
			}
			else if (g_Effect[nCntEffect].ntype == EFFECTTYPE_BIRD)
			{
				g_Effect[nCntEffect].move.y = 3;
			}

			g_Effect[nCntEffect].nlife = nLife;
			g_Effect[nCntEffect].bUse = true;


			break;
		}
		pVtx += 4;

	}
	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

void SaveEffect(void)
{
	FILE *pFile = fopen(SAVEFILENAME, "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "#========================================================================\n");
		fprintf(pFile, "#セットしたエフェクトのステータスをこのテキストファイルに書き出します\n");
		fprintf(pFile, "#読み込むテキストファイルにコピペするなりして下さい\n");
		fprintf(pFile, "#========================================================================\n\n\n");

		fprintf(pFile, "EFFECTTYPE()\n");
		fprintf(pFile, "	EFFECTSTATE\n");
		fprintf(pFile, "		DENSITY = %d\n", g_Effectstate.nDensity);
		fprintf(pFile, "		TYPE = %d\n",g_Effectstate.SerectType);
		fprintf(pFile, "		COLOR = %d %d %d %d\n", g_Effectstate.colorR, g_Effectstate.colorG, g_Effectstate.colorB, g_Effectstate.colorA);
		fprintf(pFile, "		COLORMIN = %d %d %d %d\n", g_Effectstate.MinColorR, g_Effectstate.MinColorG, g_Effectstate.MinColorB, g_Effectstate.MinColorA);
		fprintf(pFile, "		LIFE = %d\n", g_Effectstate.nLifeSerect);
		fprintf(pFile, "		SIZE = %.1f %.1f\n", g_Effectstate.fWhidth, g_Effectstate.fHight);
		fprintf(pFile, "		SIZEMIN = %.1f %.1f\n", g_Effectstate.fMinWhidth, g_Effectstate.fMinHight);

		if (g_Effectstate.SerectType == EFFECTTYPE_CIRCLE)
		{
			fprintf(pFile, "		RADIUS = %d\n", g_Effectstate.nRadius);
		}

		fprintf(pFile, "	EFFECTSTATE_END\n");
		fprintf(pFile, "EFFECTTYPE()_END\n");

	}
	fclose(pFile);
}


Effect *GetEffect(void)
{
	return &g_Effect[0];
}


Effectstate * GetEffectstate(void)
{
	return &g_Effectstate;
}
