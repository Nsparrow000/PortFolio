//*****************************************************************************
// エフェクトの諸々設定(control.cpp)
// Author : 佐藤秀亮
//*****************************************************************************
#include "manager.h"
#include "control.h"
#include "keyboard.h"
#include "IntermediateSave.h"

#include "seteffect.h"
#include "Butten.h"
#include <assert.h>
//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_SERECT_COLOR (5)	//最大選択色
#define MAX_COLOR (255)			//最大色値

#define MAX_CHANGECOL (30)		//最大増減色値

#define RAND_COLOR (float(rand()% 255) + 5)	//ランダムカラー

#define BUTTEN_SIZE (18)	//ボタンサイズ

#define SAVE_TEXT "data/SaveText.txt"	//セーブするテキストファイル

//*****************************************************************************
//静的メンバ変数宣言
//*****************************************************************************
CKeyboard *CControl::m_pKeyboard = NULL;

bool CControl::m_bMouseCursor = false;
bool CControl::m_bPlayerFollow = false;

bool CControl::m_bTap = false;
D3DCOLORVALUE CControl::m_ControlColor = {};
int CControl::m_SerectColor = 1;
D3DCOLORVALUE CControl::m_ControlChangeCol = {};

float CControl::m_ControlSize = 0;
float CControl::m_ControlChangeSize = 0;
int CControl::m_nLife = 0;
int CControl::m_nTex = 0;

int CControl::m_nDensity = 0;
int CControl::m_nPattern = 1;
int CControl::m_nRandMove1 = 1;

int CControl::m_ModelNum = 0;
int CControl::m_Diffusion = 1;
int CControl::m_ModelTrajecNum = 0;

int CControl::m_nUninitVectl = 300;

int CControl::nSynthetic = 0;
float CControl::ParticleSize = 0.0f;
int CControl::ParticleTime = 1;
float CControl::fDistance = 1.0f;

float CControl::fMaxSize = 100.0f;

int CControl::nActiveTime = 10;
float CControl::fParticleAddSize = 0;
float CControl::ActiveAddSize = 0.0f;


D3DXVECTOR2 CControl::m_move = D3DXVECTOR2(1.0f,1.0f);
D3DXVECTOR2 CControl::m_Addmove = {};

bool CControl::m_RandMoveX = false;
bool CControl::m_RandMoveY = false;

bool CControl::m_ColorR = false;
bool CControl::m_ColorG = false;
bool CControl::m_ColorB = false;

bool CControl::m_bPlayerMode = false;
float CControl::m_fAddRotate = 0;

D3DXVECTOR3 CControl::m_move3d = D3DXVECTOR3(1.0f, 1.0f,1.0f);

D3DCOLORVALUE CControl::m_TrajectColor = {};
D3DCOLORVALUE CControl::m_TrajectChangeCol = {};

D3DCOLORVALUE CControl::m_ParticleColor = {};
D3DCOLORVALUE CControl::m_ParticleAddCol = {};
int CControl::nParticleSynthetic = 0;
int CControl::m_nSecondTime = 0;

int CControl::m_nVtx = 4;
int CControl::m_nType = 0;
D3DXVECTOR2 CControl::m_TexMove = D3DXVECTOR2(0.0f, 0.0f);
D3DXVECTOR2 CControl::m_TexNum = D3DXVECTOR2(1.0f, 1.0f);
int CControl::m_SecndType = 0;

D3DXVECTOR2 CControl::m_TexPattern = D3DXVECTOR2(1.0f, 1.0f);
int CControl::m_nAnimCont = -1;
D3DXVECTOR2 CControl::m_nSplit = D3DXVECTOR2(1.0f, 1.0f);
float CControl::m_fHigth = 30.0f;
D3DXVECTOR3 CControl::m_ContorolBezier = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
int CControl::m_SecondTex = 1;
int CControl::m_AnimPatternType = 0;


//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CControl::CControl()
{
	m_bMouseCursor = false;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CControl::~CControl()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CControl::Init()
{
	m_pKeyboard = CManager::GetKeyboard();
	m_SerectColor = 1;

	m_ControlColor.r = MAX_COLOR;
	m_ControlColor.g = MAX_COLOR;
	m_ControlColor.b = MAX_COLOR;
	m_ControlColor.a = MAX_COLOR;

	m_ControlChangeCol.r = 0;
	m_ControlChangeCol.g = 0;
	m_ControlChangeCol.b = 0;
	m_ControlChangeCol.a = 0;

	m_TrajectColor.r = MAX_COLOR;
	m_TrajectColor.g = MAX_COLOR;
	m_TrajectColor.b = MAX_COLOR;
	m_TrajectColor.a = MAX_COLOR;

	m_TrajectChangeCol.r = 0;
	m_TrajectChangeCol.g = 0;
	m_TrajectChangeCol.b = 0;
	m_TrajectChangeCol.a = 0;

	m_ParticleColor.r = MAX_COLOR;
	m_ParticleColor.g = MAX_COLOR;
	m_ParticleColor.b = MAX_COLOR;
	m_ParticleColor.a = MAX_COLOR;

	m_ParticleAddCol.r = 0;
	m_ParticleAddCol.g = 0;
	m_ParticleAddCol.b = 0;
	m_ParticleAddCol.a = 0;


	m_ControlSize = 20;
	m_ControlChangeSize = 0;
	m_nLife = 100;
	m_nTex = 1;
	m_nDensity = 1;
	m_nPattern = 0;

	m_move = D3DXVECTOR2(1.0f, 1.0f);
	m_Addmove = D3DXVECTOR2(0.0f, 0.0f);
	m_move3d = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_ColorR = false;
	m_ColorG = false;
	m_ColorB = false;

	m_bMouseCursor = false;
	m_bTap = false;

	m_bPlayerMode = false;

	nSynthetic = 0;
	m_AnimPatternType = 0;
	return S_OK;
}

//*****************************************************************************
//更新
//*****************************************************************************
void CControl::Update()
{
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_2D)
	{
		Control();
	}
	else if (mode == CManager::MODE_3D)
	{
		//タップON　OFF
		if (m_pKeyboard->GetKey(DIK_F5) == true)
		{
			if (m_bPlayerMode == false)
			{
				m_bPlayerMode = true;
			}
			else if (m_bPlayerMode == true)
			{
				m_bPlayerMode = false;
			}
		}

		if (m_bPlayerMode == false)
		{
			Control();
		}
	}
}

//*****************************************************************************
//操作
//*****************************************************************************
void CControl::Control()
{
	CManager::MODE mode = CManager::GetMode();

	if (m_pKeyboard != NULL)
	{
		//タップON　OFF
		if (m_pKeyboard->GetKey(DIK_N) == true)
		{
			if (m_bTap == false)
			{
				m_bTap = true;
			}
			else if (m_bTap == true)
			{
				m_bTap = false;
			}
		}
		//上限
		if (m_SerectColor <= 0)
		{
			m_SerectColor = MAX_SERECT_COLOR - 1;
		}
		else if (m_SerectColor >= MAX_SERECT_COLOR)
		{
			m_SerectColor = 1;
		}


		if (m_nTex <= 1)
		{
			m_nTex = 1;
		}


		if (m_ColorR == true)
		{
			m_ControlColor.r = RAND_COLOR;
		}
		if (m_ColorG == true)
		{
			m_ControlColor.g = RAND_COLOR;
		}
		if (m_ColorB == true)
		{
			m_ControlColor.b = RAND_COLOR;
		}
		if (mode == CManager::MODE_2D)
		{
			CControl::Control2D();
		}
		else if (mode == CManager::MODE_3D)
		{
			CControl::Control3D();
		}
	}
}
//*****************************************************************************
//2D専用操作
//*****************************************************************************
void CControl::Control2D()
{
	if (m_pKeyboard != NULL)
	{
		//カーソル追従ON　OFF
		if (m_pKeyboard->GetKey(DIK_M) == true)
		{
			if (m_bMouseCursor == false)
			{
				m_bMouseCursor = true;
			}
			else if (m_bMouseCursor == true)
			{
				m_bMouseCursor = false;
			}
		}
	}
}

//*****************************************************************************
//3D専用操作
//*****************************************************************************
void CControl::Control3D()
{
	if (m_pKeyboard != NULL)
	{
		//プレイヤー追従ON　OFF
		if (m_pKeyboard->GetKey(DIK_M ) == true)
		{
			if (m_bPlayerFollow == false)
			{
				m_bPlayerFollow = true;
			}
			else if (m_bPlayerFollow == true)
			{
				m_bPlayerFollow = false;
			}
		}
	}
}

//*****************************************************************************
//ボタンの生成
//*****************************************************************************
void CControl::SetButten(int nPattern)
{
	if (CManager::GetMode() == CManager::MODE_2D)
	{
		//ボタン配置
		CButten::SetButten(nPattern);
	}
	else if (CManager::GetMode() == CManager::MODE_3D)
	{
			//ボタン配置
			CButten::SetButten(nPattern);
	}
}

//*****************************************************************************
//セーブ
//*****************************************************************************
void CControl::SaveEffect(CManager::MODE mode, int nPattern)
{
	FILE *pFile = fopen(SAVE_TEXT, "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "#========================================================================\n");
		fprintf(pFile, "#エフェクトの情報をこのテキストファイルに書き出します\n");
		fprintf(pFile, "#読み込むテキストファイルにコピペするなりしてご自由に\n");
		fprintf(pFile, "#========================================================================\n\n\n");

		fprintf(pFile, "#========================================================================\n");
		fprintf(pFile, "#エフェクトパターン ()番目\n");
		fprintf(pFile, "#メモ：(このエフェクトの使い道等、書いてください)\n");
		fprintf(pFile, "#========================================================================\n");


		if (mode == CManager::MODE_2D)
		{
			fprintf(pFile, "EFFECTSTATE2D\n");
			fprintf(pFile, "	PATTERN = %d				//動きのパターン\n", nPattern);


			fprintf(pFile, "	POS = %.1f %.1f				//座標\n", CSetEffect::GetSetPos().x, CSetEffect::GetSetPos().y);

			switch (nPattern)
			{
			case(0):
				fprintf(pFile, "	MOVE = %.1f %.1f					//動き\n", m_move.x, m_move.y);
				fprintf(pFile, "	ADDMOVE = %.1f %.1f				//動き加算値\n", m_Addmove.x, m_Addmove.y);
				break;
			case(1):
				fprintf(pFile, "	MOVE = %.1f					//スピード\n", m_move.x);
				fprintf(pFile, "	DIFFUSION = %d					//拡散率\n", m_Diffusion);
				fprintf(pFile, "	DESTROYVEC = %d					//消えるベクトル\n", m_nUninitVectl);
				fprintf(pFile, "	DISTANCE = %.1f					//距離\n", CControl::GetDistance());
				break;

			case(2):
				fprintf(pFile, "	ROTATE = %.2f					//回転\n", m_fAddRotate);
				break;
			default:
				assert(false);
				break;
			}
			fprintf(pFile, "	SIZE = %.1f						//大きさ\n", m_ControlSize);
			fprintf(pFile, "	ADDSIZE = %.1f					//大きさ変動\n", m_ControlChangeSize);
			fprintf(pFile, "	COLOR = %d %d %d %d			//カラー\n", (int)m_ControlColor.r, (int)m_ControlColor.g, (int)m_ControlColor.b, (int)m_ControlColor.a);
			fprintf(pFile, "	CHANGECOLOR = %d %d %d %d			//カラー変動値\n", (int)m_ControlChangeCol.r, (int)m_ControlChangeCol.g, (int)m_ControlChangeCol.b, (int)m_ControlChangeCol.a);
			fprintf(pFile, "	LIFE = %d						//ライフ\n", m_nLife);
			fprintf(pFile, "	DENSITY = %d						//密度\n", m_nDensity);

			fprintf(pFile, "	COLORRANDR = %d						//カラーランダム赤 1だと有効\n", m_ColorR);
			fprintf(pFile, "	COLORRANDG = %d						//カラーランダム緑 1だと有効\n", m_ColorG);
			fprintf(pFile, "	COLORRANDB = %d						//カラーランダム青 1だと有効\n", m_ColorB);

			fprintf(pFile, "	MOVERANDX = %d						//移動ランダムX 1だと有効\n", m_RandMoveX);

			if (m_nPattern != 2)
			{
				fprintf(pFile, "	MOVERANDY = %d						//移動ランダムY 1だと有効\n", m_RandMoveY);
			}

			fprintf(pFile, "	MOUSEPOS = %d						//マウス追従ON 1だと有効\n", m_bMouseCursor);
			fprintf(pFile, "	TEXTURE = %d						//テクスチャ\n", CControl::GetTex());
			fprintf(pFile, "	SYNTHETIC = %d						//合成\n", CControl::GetSynthetic());


			fprintf(pFile, "END_EFFECTSTATE2D\n\n");
		}
		else if (mode == CManager::MODE_3D)	//3Dモード
		{
			fprintf(pFile, "EFFECTSTATE3D\n");
			fprintf(pFile, "	PATTERN = %d				//動きのパターン\n", nPattern);

			switch (nPattern)
			{
			case(0):
				fprintf(pFile, "	SECONDCOLOR = %d %d %d %d			//先端カラー\n", (int)m_TrajectColor.r, (int)m_TrajectColor.g, (int)m_TrajectColor.b, (int)m_TrajectColor.a);
				fprintf(pFile, "	SECONDCHANGECOLOR = %d %d %d %d			//先端カラー変動値\n", (int)m_TrajectChangeCol.r, (int)m_TrajectChangeCol.g, (int)m_TrajectChangeCol.b, (int)m_TrajectChangeCol.a);

				fprintf(pFile, "	MODEL = %d						//軌跡手元モデル\n", m_ModelNum);
				fprintf(pFile, "	TRAJECTMODEL = %d				//軌跡先端モデル\n", m_ModelTrajecNum);
				break;
			case(1):
				fprintf(pFile, "	SIZE = %.1f						//大きさ\n", CControl::GetSize());
				fprintf(pFile, "	ADDSIZE = %.1f					//大きさ変動\n", CControl::GetChangeSize());
				fprintf(pFile, "	DENSITY = %d						//密度\n", CControl::GetDensity());

				fprintf(pFile, "	MOVE3D = %.1f %.1f %.1f					//移動\n", CControl::Getmove3d().x, CControl::Getmove3d().y, CControl::Getmove3d().z);

				fprintf(pFile, "	DISTANCE = %.1f						//発生距離\n", CControl::GetDistance());
				fprintf(pFile, "	TYPE = %d					//出現位置のパターン\n", CControl::GetType());
				fprintf(pFile, "	SECONDTYPE = %d					//ランダム移動のパターン\n", CControl::GetSecondType());
				fprintf(pFile, "	PARTICLETIME = %d						//粒発生間隔\n", CControl::GetParticleTime());

				break;
			case(2):
				fprintf(pFile, "	SIZE = %.1f						//大きさ\n", CControl::GetSize());
				fprintf(pFile, "	ADDSIZE = %.1f					//大きさ変動\n", CControl::GetChangeSize());
				fprintf(pFile, "	DENSITY = %d						//密度\n", CControl::GetDensity());

				fprintf(pFile, "	RANDMOVE = %d				//移動値\n", CControl::GetRandMove1());
				fprintf(pFile, "	TRAJECTTOP = %d				//発生モデル１\n", CControl::GetModelNum());
				fprintf(pFile, "	TRAJECTCUR = %d				//発生モデル２\n", CControl::GetTrajectModelNum());

				break;
			case(3):
				fprintf(pFile, "	ROTATE = %.2f						//回転\n", CControl::GetRotate());
				fprintf(pFile, "	MOVE = %.1f						//移動\n", CControl::Getmove3d().x);
				fprintf(pFile, "	SIZE = %.1f						//大きさ\n", CControl::GetSize());
				fprintf(pFile, "	ADDSIZE = %.1f					//大きさ変動\n", CControl::GetChangeSize());
				fprintf(pFile, "	DENSITY = %d						//粒密度\n", CControl::GetDensity());

				fprintf(pFile, "	PARTICLESIZE = %.1f						//粒サイズ\n", CControl::GetParticleSize());
				fprintf(pFile, "	PARTICLETIME = %d						//粒発生間隔\n", CControl::GetParticleTime());
				fprintf(pFile, "	DISTANCE = %.1f						//粒の発生距離\n", CControl::GetDistance());

				fprintf(pFile, "	ACTIVE = %d						//稼働時間\n", CControl::GetActiveTime());
				fprintf(pFile, "	PARTICLEADDSIZE = %.1f						//粒サイズ変更\n", CControl::GetParticleAddSize());
				fprintf(pFile, "	MAXSIZE = %.1f						//フィールド最大サイズ\n", CControl::GetMaxSize());

				fprintf(pFile, "	SECONDCOLOR = %d %d %d %d			//パーティクルカラー\n", (int)CControl::GetParticleColor(1), (int)CControl::GetParticleColor(2), (int)CControl::GetParticleColor(3), (int)CControl::GetParticleColor(4));
				fprintf(pFile, "	SECONDADDCOLOR = %d %d %d %d			//パーティクルカラー増減\n", (int)CControl::GetParticleAddCol(1), (int)CControl::GetParticleAddCol(2), (int)CControl::GetParticleAddCol(3), (int)CControl::GetParticleAddCol(4));
				fprintf(pFile, "	SECONDSYNTHETIC = %d			//パーティクル合成\n", (int)CControl::GetParticleSynthetic());
				fprintf(pFile, "	ACTIVEADDSIZE = %.1f					//稼働中の稼働時間\n", CControl::GetAvctiveAddSize());

				fprintf(pFile, "	FIELDTIME = 10			//フィールド生成間隔\n");
				fprintf(pFile, "	FIELDCREATE = 0			//フィールド生成するか\n");
				fprintf(pFile, "	CREATEPRESET = 0					//生成プリセット\n");
				break;
			case(4):

				fprintf(pFile, "	MOVE3D = %.1f %.1f %.1f					//移動\n", CControl::Getmove3d().x, CControl::Getmove3d().y, CControl::Getmove3d().z);
				fprintf(pFile, "	ACTIVE = %d						//稼働時間\n", CControl::GetActiveTime());
				fprintf(pFile, "	MAXSIZE = %.1f						//フィールド最大サイズ\n", CControl::GetMaxSize());
				fprintf(pFile, "	DENSITY = %d						//粒密度\n", CControl::GetDensity());

				break;
			case(5):
				fprintf(pFile, "	SIZE = %.1f						//大きさ\n", CControl::GetSize());
				fprintf(pFile, "	ADDSIZE = %.1f					//大きさ変動\n", CControl::GetChangeSize());
				fprintf(pFile, "	DENSITY = %d						//粒密度\n", CControl::GetDensity());
				fprintf(pFile, "	ROTATE = %.2f						//回転\n", CControl::GetRotate());
				fprintf(pFile, "	DISTANCE = %.1f						//粒の発生距離\n", CControl::GetDistance());
				fprintf(pFile, "	PARTICLETIME = %d						//粒発生間隔\n", CControl::GetParticleTime());
				fprintf(pFile, "	SECONDTIME = %d						//近づくまでの時間\n", CControl::GetSecondTime());
				fprintf(pFile, "	MAXSIZE = %.1f						//アクティブサイズ\n", CControl::GetMaxSize());
				fprintf(pFile, "	MOVE = %.1f						//距離加算\n", CControl::Getmove3d().x);
				fprintf(pFile, "	TYPE = %d					//パーティクル(0)or軌跡(1)\n", CControl::GetType());
				fprintf(pFile, "	SECONDTYPE = %d					//移動ランダム\n", CControl::GetSecondType());

				break;
			case(6):
				fprintf(pFile, "	SIZE = %.1f						//大きさ\n", CControl::GetSize());
				fprintf(pFile, "	ADDSIZE = %.1f					//大きさ変動\n", CControl::GetChangeSize());

				fprintf(pFile, "	ROTATE = %.2f						//回転\n", CControl::GetRotate());
				fprintf(pFile, "	VTX = %d						//頂点数\n", CControl::GetVtx());
				fprintf(pFile, "	TYPE = %d					//球(0)or半球(1)\n", CControl::GetType());


				break;
			case(7):
				fprintf(pFile, "	SIZE = %.1f						//大きさ\n", CControl::GetSize());
				fprintf(pFile, "	ADDSIZE = %.1f					//大きさ変動\n", CControl::GetChangeSize());

				fprintf(pFile, "	DISTANCE = %.1f						//上発生距離\n", CControl::GetDistance());
				fprintf(pFile, "	HIGTH = %.1f						//上距離\n", CControl::GetHigth());
				fprintf(pFile, "	PARTICLESIZE = %.1f						//下部分の幅\n", CControl::GetParticleSize());
				break;
			case(8):
				fprintf(pFile, "	SIZE = %.1f						//大きさ\n", CControl::GetSize());
				fprintf(pFile, "	ADDSIZE = %.1f					//大きさ変動\n", CControl::GetChangeSize());
				fprintf(pFile, "	DENSITY = %d						//密度\n", CControl::GetDensity());

				fprintf(pFile, "	CONTROLBEZIER = %.1f %.1f %.1f						//ベジェ制御点\n", CControl::GetContorolBezierX(), CControl::GetContorolBezierY(), CControl::GetContorolBezierZ());
				fprintf(pFile, "	MOVE = %.1f						//移動（制御点の数）\n", CControl::Getmove3d().x);

				fprintf(pFile, "	SECONDCOLOR = %d %d %d %d			//軌跡の色１\n", (int)CControl::GetParticleColor(1), (int)CControl::GetParticleColor(2), (int)CControl::GetParticleColor(3), (int)CControl::GetParticleColor(4));
				fprintf(pFile, "	SECONDADDCOLOR = %d %d %d %d			//軌跡の色１増減\n", (int)CControl::GetParticleAddCol(1), (int)CControl::GetParticleAddCol(2), (int)CControl::GetParticleAddCol(3), (int)CControl::GetParticleAddCol(4));

				fprintf(pFile, "	THERDCOLOR = %d %d %d %d				//軌跡の色２\n", (int)CControl::GetTrajectColor(1), (int)CControl::GetTrajectColor(2), (int)CControl::GetTrajectColor(3), (int)CControl::GetTrajectColor(4));
				fprintf(pFile, "	THERDADDCOLOR = %d %d %d %d				//軌跡の色２増減\n", (int)CControl::GetTrajectCol(1), (int)CControl::GetTrajectCol(2), (int)CControl::GetTrajectCol(3), (int)CControl::GetTrajectCol(4));

				fprintf(pFile, "	MAXSIZE = %.1f						//軌跡サイズ\n", CControl::GetMaxSize());
				fprintf(pFile, "	SECONDTEX = %d						//軌跡テクスチャ\n", CControl::GetSecondTex());
				fprintf(pFile, "	PARTICLETIME = %d						//軌跡寿命\n", CControl::GetParticleTime());

				fprintf(pFile, "	DISTANCE = %.1f						//ターゲットからのランダム距離\n", CControl::GetDistance());
				fprintf(pFile, "	SECONDSYNTHETIC = %d			//軌跡合成\n", (int)CControl::GetParticleSynthetic());
				break;
			case(9):
				break;
			default:
				assert(false);
				break;
			}
			fprintf(pFile, "	POS = 0.0 0.0 0.0			//出現位置\n");

			fprintf(pFile, "	COLOR = %d %d %d %d			//カラー\n", (int)CControl::GetControlCoror(1), (int)CControl::GetControlCoror(2), (int)CControl::GetControlCoror(3), (int)CControl::GetControlCoror(4));
			fprintf(pFile, "	CHANGECOLOR = %d %d %d %d			//カラー変動値\n", (int)CControl::GetChangeCol(1), (int)CControl::GetChangeCol(2), (int)CControl::GetChangeCol(3), (int)CControl::GetChangeCol(4));
			fprintf(pFile, "	LIFE = %d						//ライフ\n", CControl::GetLife());
			fprintf(pFile, "	TEXTURE = %d						//テクスチャ\n", CControl::GetTex());
			fprintf(pFile, "	SYNTHETIC = %d						//合成\n", CControl::GetSynthetic());
			fprintf(pFile, "	TEXMOVE = %.3f %.3f					//テクスチャ移動\n", CControl::GetTexMoveU(), CControl::GetTexMoveV());
			fprintf(pFile, "	TEXNUM = %.1f %.1f					//テクスチャ枚数\n", CControl::GetTexNum().x, CControl::GetTexNum().y);

			fprintf(pFile, "	TEXANIMCOUNT = %d					//テクスチャアニメーションカウント\n", CControl::GetAnimCont());
			fprintf(pFile, "	TEXSPLIT = %.0f %.0f					//テクスチャ分割数\n", CControl::GetSplitU(), CControl::GetSplitV());
			fprintf(pFile, "	ANIMPATTERNTYPE = %d					//アニメーションパターンタイプ\n", CControl::GetAnimPatternType());

			fprintf(pFile, "END_EFFECTSTATE3D\n\n");
		}
		fclose(pFile);
	}

}

//*****************************************************************************
//選択色の取得
//*****************************************************************************
int CControl::GetControlCoror(int nSerectcolor)
{
	switch (nSerectcolor)
	{
	case(1):
		return (int)m_ControlColor.r;
		break;
	case(2):
		return (int)m_ControlColor.g;
		break;
	case(3):
		return (int)m_ControlColor.b;
		break;
	case(4):
		return (int)m_ControlColor.a;
		break;
	default:
		return 0;		//例外処理(処理不可能）
		break;
	}
}

//*****************************************************************************
//選択色がランダムかどうか
//*****************************************************************************
bool CControl::GetRandCol(int nSerectcolor)	//色ランダム化取得
{
	switch (nSerectcolor)
	{
	case(1):
		return m_ColorR;
		break;
	case(2):
		return m_ColorG;
		break;
	case(3):
		return m_ColorB;
		break;
	case(4):
		return false;
		break;
	default:
		return false;		//例外処理(処理不可能）
		break;
	}
}

//*****************************************************************************
//選択色ランダム化
//*****************************************************************************
void CControl::SetColorRandom(int nSerect, bool bAdaptation)
{
	switch (nSerect)
	{
	case(1):
		m_ColorR = bAdaptation;
		break;
	case(2):
		m_ColorG = bAdaptation;
		break;
	case(3):
		m_ColorB = bAdaptation;
		break;
	case(4):
		return;
		break;
	default:
		assert(false);
		break;
	}
}

//*****************************************************************************
//増減値取得
//*****************************************************************************
int CControl::GetChangeCol(int nSerectcolor)
{
	switch (nSerectcolor)
	{
	case(1):
		return (int)m_ControlChangeCol.r;
		break;
	case(2):
		return (int)m_ControlChangeCol.g;
		break;
	case(3):
		return (int)m_ControlChangeCol.b;
		break;
	case(4):
		return (int)m_ControlChangeCol.a;
		break;
	default:
		return 0;		//例外処理
		break;
	}
}

//*****************************************************************************
//選択色の変更
//*****************************************************************************
void CControl::AddColor(int nColor, int nSerect)
{
	switch (nSerect)
	{
	case(1):
		m_ControlColor.r += nColor;
		if (m_ControlColor.r > MAX_COLOR)
		{
			m_ControlColor.r = MAX_COLOR;
		}
		else if (m_ControlColor.r < 0)
		{
			m_ControlColor.r = 0;
		}
		break;
	case(2):
		m_ControlColor.g += nColor;
		if (m_ControlColor.g > MAX_COLOR)
		{
			m_ControlColor.g = MAX_COLOR;
		}
		else if (m_ControlColor.g < 0)
		{
			m_ControlColor.g = 0;
		}
		break;
	case(3):
		m_ControlColor.b += nColor;
		if (m_ControlColor.b > MAX_COLOR)
		{
			m_ControlColor.b = MAX_COLOR;
		}
		else if (m_ControlColor.b < 0)
		{
			m_ControlColor.b = 0;
		}
		break;
	case(4):
		m_ControlColor.a += nColor;
		if (m_ControlColor.a > MAX_COLOR)
		{
			m_ControlColor.a= MAX_COLOR;
		}
		else if (m_ControlColor.a < 0)
		{
			m_ControlColor.a = 0;
		}
		break;
	default:
		assert(false);		//例外処理
		break;
	}
}

//*****************************************************************************
//増減値の変更
//*****************************************************************************
void CControl::AddChangeCol(int nColor, int nSerect)
{
	switch (nSerect)
	{
	case(1):
		m_ControlChangeCol.r += nColor;
		if (m_ControlChangeCol.r > MAX_CHANGECOL)
		{
			m_ControlChangeCol.r = MAX_CHANGECOL;
		}
		else if (m_ControlChangeCol.r < -MAX_CHANGECOL)
		{
			m_ControlChangeCol.r = -MAX_CHANGECOL;
		}
		break;
	case(2):
		m_ControlChangeCol.g += nColor;
		if (m_ControlChangeCol.g > MAX_CHANGECOL)
		{
			m_ControlChangeCol.g = MAX_CHANGECOL;
		}
		else if (m_ControlChangeCol.g < -MAX_CHANGECOL)
		{
			m_ControlChangeCol.g = -MAX_CHANGECOL;
		}
		break;
	case(3):
		m_ControlChangeCol.b += nColor;
		if (m_ControlChangeCol.b > MAX_CHANGECOL)
		{
			m_ControlChangeCol.b = MAX_CHANGECOL;
		}
		else if (m_ControlChangeCol.b < -MAX_CHANGECOL)
		{
			m_ControlChangeCol.b = -MAX_CHANGECOL;
		}
		break;
	case(4):
		m_ControlChangeCol.a += nColor;
		if (m_ControlChangeCol.a > MAX_CHANGECOL)
		{
			m_ControlChangeCol.a = MAX_CHANGECOL;
		}
		else if (m_ControlChangeCol.a < -MAX_CHANGECOL)
		{
			m_ControlChangeCol.a = -MAX_CHANGECOL;
		}
		break;
	default:
		assert(false);		//例外処理
		break;
	}
}

//*****************************************************************************
//選択色のリセット
//*****************************************************************************
void CControl::SetControlCoror(int nSerectcolor)
{
	switch (nSerectcolor)
	{
	case(1):
		m_ControlColor.r = 255;
		break;
	case(2):
		m_ControlColor.g = 255;
		break;
	case(3):
		m_ControlColor.b = 255;
		break;
	case(4):
		m_ControlColor.a;
		break;
	default:
		return ;
		break;
	}
}

//*****************************************************************************
//メッシュ初期色の変更
//*****************************************************************************
void CControl::TrajectColor(int nColor, int nSerect)
{
	switch (nSerect)
	{
	case(1):
		m_TrajectColor.r += nColor;
		if (m_TrajectColor.r > MAX_COLOR)
		{
			m_TrajectColor.r = MAX_COLOR;
		}
		else if (m_TrajectColor.r < 0)
		{
			m_TrajectColor.r = 0;
		}
		break;
	case(2):
		m_TrajectColor.g += nColor;
		if (m_TrajectColor.g > MAX_COLOR)
		{
			m_TrajectColor.g = MAX_COLOR;
		}
		else if (m_TrajectColor.g < 0)
		{
			m_TrajectColor.g = 0;
		}
		break;
	case(3):
		m_TrajectColor.b += nColor;
		if (m_TrajectColor.b > MAX_COLOR)
		{
			m_TrajectColor.b = MAX_COLOR;
		}
		else if (m_TrajectColor.b < 0)
		{
			m_TrajectColor.b = 0;
		}
		break;
	case(4):
		m_TrajectColor.a += nColor;
		if (m_TrajectColor.a > MAX_COLOR)
		{
			m_TrajectColor.a = MAX_COLOR;
		}
		else if (m_TrajectColor.a < 0)
		{
			m_TrajectColor.a = 0;
		}
		break;
	default:
		assert(false);		//例外処理
		break;
	}
}

//*****************************************************************************
//メッシュ初期色の取得
//*****************************************************************************
int CControl::GetTrajectColor(int nSerectcolor)
{
	switch (nSerectcolor)
	{
	case(1):
		return (int)m_TrajectColor.r;
		break;
	case(2):
		return (int)m_TrajectColor.g;
		break;
	case(3):
		return (int)m_TrajectColor.b;
		break;
	case(4):
		return (int)m_TrajectColor.a;
		break;
	default:
		return 0;		//例外処理(処理不可能）
		break;
	}
}

//*****************************************************************************
//メッシュ増減色の変更
//*****************************************************************************
void CControl::AddTrajectCol(int nColor, int nSerect)
{
	switch (nSerect)
	{
	case(1):
		m_TrajectChangeCol.r += nColor;
		if (m_TrajectChangeCol.r > MAX_CHANGECOL)
		{
			m_TrajectChangeCol.r = MAX_CHANGECOL;
		}
		else if (m_TrajectChangeCol.r < -MAX_CHANGECOL)
		{
			m_TrajectChangeCol.r = -MAX_CHANGECOL;
		}
		break;
	case(2):
		m_TrajectChangeCol.g += nColor;
		if (m_TrajectChangeCol.g > MAX_CHANGECOL)
		{
			m_TrajectChangeCol.g = MAX_CHANGECOL;
		}
		else if (m_TrajectChangeCol.g < -MAX_CHANGECOL)
		{
			m_TrajectChangeCol.g = -MAX_CHANGECOL;
		}
		break;
	case(3):
		m_TrajectChangeCol.b += nColor;
		if (m_TrajectChangeCol.b > MAX_CHANGECOL)
		{
			m_TrajectChangeCol.b = MAX_CHANGECOL;
		}
		else if (m_TrajectChangeCol.b < -MAX_CHANGECOL)
		{
			m_TrajectChangeCol.b = -MAX_CHANGECOL;
		}
		break;
	case(4):
		m_TrajectChangeCol.a += nColor;
		if (m_TrajectChangeCol.a > MAX_CHANGECOL)
		{
			m_TrajectChangeCol.a = MAX_CHANGECOL;
		}
		else if (m_TrajectChangeCol.a < -MAX_CHANGECOL)
		{
			m_TrajectChangeCol.a = -MAX_CHANGECOL;
		}
		break;
	default:
		assert(false);		//例外処理(処理不可能）
		break;
	}
}

//*****************************************************************************
//メッシュ増減色の取得
//*****************************************************************************
int CControl::GetTrajectCol(int nSerectcolor)
{
	switch (nSerectcolor)
	{
	case(0):
		break;
	case(1):
		return (int)m_TrajectChangeCol.r;
		break;
	case(2):
		return (int)m_TrajectChangeCol.g;
		break;
	case(3):
		return (int)m_TrajectChangeCol.b;
		break;
	case(4):
		return (int)m_TrajectChangeCol.a;
		break;
	default:
		assert(false);
		break;
	}
	return 0;
}

//*****************************************************************************
//３D移動の変化
//*****************************************************************************
void CControl::Addmove3d(float Move, int nNum)
{
	switch (nNum)
	{
	case(0):
		m_move3d.x += Move;
		break;
	case(1):
		m_move3d.y += Move;
		break;
	case(2):
		m_move3d.z += Move;

		break;
	default:
		assert(false);
		break;
	}
}

//*****************************************************************************
//パーティクル色の取得
//*****************************************************************************
int CControl::GetParticleColor(int nSerectcolor)
{
	switch (nSerectcolor)
	{
	case(1):
		return (int)m_ParticleColor.r;
		break;
	case(2):
		return (int)m_ParticleColor.g;
		break;
	case(3):
		return (int)m_ParticleColor.b;
		break;
	case(4):
		return (int)m_ParticleColor.a;
		break;
	default:
		return 0;		//例外処理(処理不可能）
		break;
	}
}

//*****************************************************************************
//パーティクル増減色の取得
//*****************************************************************************
int CControl::GetParticleAddCol(int nSerectcolor)
{
	switch (nSerectcolor)
	{
	case(1):
		return (int)m_ParticleAddCol.r;
		break;
	case(2):
		return (int)m_ParticleAddCol.g;
		break;
	case(3):
		return (int)m_ParticleAddCol.b;
		break;
	case(4):
		return (int)m_ParticleAddCol.a;
		break;
	default:
		return 0;		//例外処理(処理不可能）
		break;
	}
}

//*****************************************************************************
//パーティクル色の変更
//*****************************************************************************
void CControl::AddParticleColor(int nColor, int nSerect)
{
	switch (nSerect)
	{
	case(1):
		m_ParticleColor.r += nColor;
		if (m_ParticleColor.r > MAX_COLOR)
		{
			m_ParticleColor.r = MAX_COLOR;
		}
		else if (m_ParticleColor.r < -MAX_COLOR)
		{
			m_ParticleColor.r = -MAX_COLOR;
		}
		break;
	case(2):
		m_ParticleColor.g += nColor;
		if (m_ParticleColor.g > MAX_COLOR)
		{
			m_ParticleColor.g = MAX_COLOR;
		}
		else if (m_ParticleColor.g < -MAX_COLOR)
		{
			m_ParticleColor.g = -MAX_COLOR;
		}
		break;
	case(3):
		m_ParticleColor.b += nColor;
		if (m_ParticleColor.b > MAX_COLOR)
		{
			m_ParticleColor.b = MAX_COLOR;
		}
		else if (m_ParticleColor.b < -MAX_COLOR)
		{
			m_ParticleColor.b = -MAX_COLOR;
		}
		break;
	case(4):
		m_ParticleColor.a += nColor;
		if (m_ParticleColor.a > MAX_COLOR)
		{
			m_ParticleColor.a = MAX_COLOR;
		}
		else if (m_ParticleColor.a < -MAX_COLOR)
		{
			m_ParticleColor.a = -MAX_COLOR;
		}
		break;
	default:
		assert(false);		//例外処理
		break;
	}
}

//*****************************************************************************
//パーティクル増減色の変更
//*****************************************************************************
void CControl::AddParticleAddCol(int nColor, int nSerect)
{
	switch (nSerect)
	{
	case(1):
		m_ParticleAddCol.r += nColor;
		if (m_ParticleAddCol.r > MAX_COLOR)
		{
			m_ParticleAddCol.r = MAX_COLOR;
		}
		else if (m_ParticleAddCol.r < -MAX_COLOR)
		{
			m_ParticleAddCol.r = -MAX_COLOR;
		}
		break;
	case(2):
		m_ParticleAddCol.g += nColor;
		if (m_ParticleAddCol.g > MAX_COLOR)
		{
			m_ParticleAddCol.g = MAX_COLOR;
		}
		else if (m_ParticleAddCol.g < -MAX_COLOR)
		{
			m_ParticleAddCol.g = -MAX_COLOR;
		}
		break;
	case(3):
		m_ParticleAddCol.b += nColor;
		if (m_ParticleAddCol.b > MAX_COLOR)
		{
			m_ParticleAddCol.b = MAX_COLOR;
		}
		else if (m_ParticleAddCol.b < -MAX_COLOR)
		{
			m_ParticleAddCol.b = -MAX_COLOR;
		}
		break;
	case(4):
		m_ParticleAddCol.a += nColor;
		if (m_ParticleAddCol.a > MAX_COLOR)
		{
			m_ParticleAddCol.a = MAX_COLOR;
		}
		else if (m_ParticleAddCol.a < -MAX_COLOR)
		{
			m_ParticleAddCol.a = -MAX_COLOR;
		}
		break;
	default:
		assert(false);		//例外処理
		break;
	}
}
