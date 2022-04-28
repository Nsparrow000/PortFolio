//*****************************************************************************
// エフェクト設置処理 [SetEffect.cpp]
// Author : 佐藤秀亮
//*****************************************************************************
//*****************************************************************************
// インクルードファイル
//***************************************************************************** 
#include "PresetSetEffect.h"

#include "MouseTracking.h"
#include "Movement.h"
#include "Rotate.h"

#include "Trajectory.h"
#include "straight3d.h"
#include "FieldEffect.h"

#include <assert.h>

//*****************************************************************************
// マクロ
//***************************************************************************** 
#define RAND_COLOR ((float(rand()% 10) + 1) / 10)		//ランダムカラー


//*****************************************************************************
// 静的
//***************************************************************************** 
int CPresetEffect::m_nEffectPattern = 0;
CPresetEffect::EFFECT_STATE2D CPresetEffect::m_EffectState2D[MAX_EFFECTPATTERN_2D] = {};
CPresetEffect::EFFECT_STATE3D CPresetEffect::m_EffectState3D[MAX_EFFECTPATTERN_3D] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CPresetEffect::CPresetEffect()
{
	m_EffectState2D[MAX_EFFECTPATTERN_2D] = {};
	m_EffectState3D[MAX_EFFECTPATTERN_3D] = {};

}

//=============================================================================
// デストラクタ
//=============================================================================
CPresetEffect::~CPresetEffect()
{
	m_nEffectPattern = 0;
}

//=============================================================================
// 変数にぶち込む関数 2D用
//=============================================================================
void CPresetEffect::SetEffectState2D(int nPattern,
	D3DXVECTOR3 pos,
	float fRotate,
	D3DXVECTOR2 move,
	D3DXVECTOR2 Addmove,
	int Diffusion,
	int Destroyvec,
	float fSize,
	float fAddSize,
	D3DCOLORVALUE col,
	D3DCOLORVALUE Changecolor,
	int nLife,
	int nDensity,
	bool bColorRandR,
	bool bColorRandG,
	bool bColorRandB,
	bool bMousePos,
	int Synthetic,
	int Texture)
{
	m_EffectState2D[m_nEffectPattern].m_nPattern = nPattern;
	m_EffectState2D[m_nEffectPattern].m_pos = pos;
	m_EffectState2D[m_nEffectPattern].m_fRotate = fRotate;
	m_EffectState2D[m_nEffectPattern].m_move = move;
	m_EffectState2D[m_nEffectPattern].m_Addmove = Addmove;
	m_EffectState2D[m_nEffectPattern].m_nDiffusion = Diffusion;
	m_EffectState2D[m_nEffectPattern].m_nDestroyvec = Destroyvec;
	m_EffectState2D[m_nEffectPattern].m_fSize = fSize;
	m_EffectState2D[m_nEffectPattern].m_fAddSize = fAddSize;
	m_EffectState2D[m_nEffectPattern].m_Col = col;
	m_EffectState2D[m_nEffectPattern].m_Changecolor = Changecolor;
	m_EffectState2D[m_nEffectPattern].m_nLife = nLife;
	m_EffectState2D[m_nEffectPattern].m_nDensity = nDensity;
	m_EffectState2D[m_nEffectPattern].m_bColorRandR = bColorRandR;
	m_EffectState2D[m_nEffectPattern].m_bColorRandG = bColorRandG;
	m_EffectState2D[m_nEffectPattern].m_bColorRandB = bColorRandB;
	m_EffectState2D[m_nEffectPattern].m_bMousePos = bMousePos;
	m_EffectState2D[m_nEffectPattern].Synthetic = Synthetic;
	m_EffectState2D[m_nEffectPattern].nTexture = Texture;

	m_nEffectPattern++;
}

//=============================================================================
// 変数にぶち込む関数 3D用
//=============================================================================
void CPresetEffect::SetEffectState3D(int nPattern,
	float m_fRotate,
	float m_move,
	float m_Addmove,
	int m_nDiffusion,
	float m_fSize,
	float m_fAddSize,
	float m_MaxSize,
	float m_fParticleSize,
	float m_fParticleAddSize,
	int m_Active,
	D3DCOLORVALUE m_Col,
	D3DCOLORVALUE m_Changecolor,
	D3DCOLORVALUE m_SecondCol,
	D3DCOLORVALUE m_SecondChangecolor,
	int m_ParticleSynthetic,
	int m_nLife,
	int m_nDensity,
	int TrajectTop,
	int TrajectCur,
	D3DXVECTOR3 move3d,
	int RandMove,
	bool bColorRandR,
	bool bColorRandG,
	bool bColorRandB,
	int Synthetic,
	int Texture,
	int nDistance,
	int ParticleTime,
	D3DXVECTOR3 pos,
	float m_fActiveAddSize,
	int m_FieldTime,
	bool m_fieldCreate,
	int mCreatePreset)
{
	m_EffectState3D[m_nEffectPattern].m_nPattern = nPattern;
	m_EffectState3D[m_nEffectPattern].m_fRotate = m_fRotate;
	m_EffectState3D[m_nEffectPattern].m_move = m_move;
	m_EffectState3D[m_nEffectPattern].m_Addmove = m_Addmove;
	m_EffectState3D[m_nEffectPattern].m_nDiffusion = m_nDiffusion;
	m_EffectState3D[m_nEffectPattern].m_fSize = m_fSize;
	m_EffectState3D[m_nEffectPattern].m_fAddSize = m_fAddSize;
	m_EffectState3D[m_nEffectPattern].m_MaxSize = m_MaxSize;
	m_EffectState3D[m_nEffectPattern].m_fParticleSize = m_fParticleSize;
	m_EffectState3D[m_nEffectPattern].m_fParticleAddSize = m_fParticleAddSize;
	m_EffectState3D[m_nEffectPattern].m_Col = m_Col;
	m_EffectState3D[m_nEffectPattern].m_Changecolor = m_Changecolor;
	m_EffectState3D[m_nEffectPattern].m_SecondCol = m_SecondCol;
	m_EffectState3D[m_nEffectPattern].m_SecondChangecolor = m_SecondChangecolor;
	m_EffectState3D[m_nEffectPattern].m_ParticleSynthetic = m_ParticleSynthetic;
	m_EffectState3D[m_nEffectPattern].m_nLife = m_nLife;
	m_EffectState3D[m_nEffectPattern].m_nDensity = m_nDensity;
	m_EffectState3D[m_nEffectPattern].TrajectTop = TrajectTop;
	m_EffectState3D[m_nEffectPattern].TrajectCur = TrajectCur;
	m_EffectState3D[m_nEffectPattern].move3d = move3d;
	m_EffectState3D[m_nEffectPattern].RandMove = RandMove;
	m_EffectState3D[m_nEffectPattern].m_bColorRandR = bColorRandR;
	m_EffectState3D[m_nEffectPattern].m_bColorRandG = bColorRandG;
	m_EffectState3D[m_nEffectPattern].m_bColorRandB = bColorRandB;
	m_EffectState3D[m_nEffectPattern].Synthetic = Synthetic;
	m_EffectState3D[m_nEffectPattern].nTexture = Texture;
	m_EffectState3D[m_nEffectPattern].m_Active = m_Active;
	m_EffectState3D[m_nEffectPattern].m_nDistance = nDistance;
	m_EffectState3D[m_nEffectPattern].ParticleTime = ParticleTime;
	m_EffectState3D[m_nEffectPattern].pos = pos;
	m_EffectState3D[m_nEffectPattern].m_fActiveAddSize = m_fActiveAddSize;
	m_EffectState3D[m_nEffectPattern].m_FieldTime = m_FieldTime;
	m_EffectState3D[m_nEffectPattern].m_fieldCreate = m_fieldCreate;
	m_EffectState3D[m_nEffectPattern].mCreatePreset = mCreatePreset;



	m_nEffectPattern++;
}


//=============================================================================
// 呼ばれた物を呼び出すやつ2D
//=============================================================================
void CPresetEffect::SetEffect2D(int nPattern, D3DXVECTOR3 pos, D3DXVECTOR3 Endpos)
{
	switch (m_EffectState2D[nPattern].m_nPattern)
	{
	case(1):
		for (int nCnt = 0; nCnt < m_EffectState2D[nPattern].m_nDensity; nCnt++)
		{
			//各色のランダム化
			if (m_EffectState2D[nPattern].m_bColorRandR == true)
			{
				m_EffectState2D[nPattern].m_Col.r = RAND_COLOR;
			}
			if (m_EffectState2D[nPattern].m_bColorRandG == true)
			{
				m_EffectState2D[nPattern].m_Col.g = RAND_COLOR;
			}
			if (m_EffectState2D[nPattern].m_bColorRandB == true)
			{
				m_EffectState2D[nPattern].m_Col.b = RAND_COLOR;
			}


			//出現位置が自分の位置か
			if (m_EffectState2D[nPattern].m_bMousePos == true)
			{
				m_EffectState2D[nPattern].m_pos = pos;
			}

			CMovement::Create(m_EffectState2D[nPattern].m_pos,
				m_EffectState2D[nPattern].m_move,
				m_EffectState2D[nPattern].m_Col,
				m_EffectState2D[nPattern].m_Changecolor,
				D3DXVECTOR2(m_EffectState2D[nPattern].m_fSize, m_EffectState2D[nPattern].m_fSize),
				D3DXVECTOR2(m_EffectState2D[nPattern].m_fAddSize, m_EffectState2D[nPattern].m_fAddSize),
				m_EffectState2D[nPattern].m_nLife, m_EffectState2D[nPattern].nTexture,
				m_EffectState2D[nPattern].m_Addmove,
				m_EffectState2D[m_nEffectPattern].Synthetic);
		}
		break;
	case(2):
		for (int nCnt = 0; nCnt < m_EffectState2D[nPattern].m_nDensity; nCnt++)
		{
			//出現位置が自分の位置か
			if (m_EffectState2D[nPattern].m_bMousePos == true)
			{
				m_EffectState2D[nPattern].m_pos = pos;
			}
			//各色のランダム化
			if (m_EffectState2D[nPattern].m_bColorRandR == true)
			{
				m_EffectState2D[nPattern].m_Col.r = RAND_COLOR;
			}
			if (m_EffectState2D[nPattern].m_bColorRandG == true)
			{
				m_EffectState2D[nPattern].m_Col.g = RAND_COLOR;
			}
			if (m_EffectState2D[nPattern].m_bColorRandB == true)
			{
				m_EffectState2D[nPattern].m_Col.b = RAND_COLOR;
			}

			CMouseTracking::Create(pos,
				m_EffectState2D[nPattern].m_move,
				m_EffectState2D[nPattern].m_Col,
				m_EffectState2D[nPattern].m_Changecolor,
				D3DXVECTOR2(m_EffectState2D[nPattern].m_fSize, m_EffectState2D[nPattern].m_fSize),
				D3DXVECTOR2(m_EffectState2D[nPattern].m_fAddSize, m_EffectState2D[nPattern].m_fAddSize),
				m_EffectState2D[nPattern].m_nLife, m_EffectState2D[nPattern].nTexture,
				Endpos, m_EffectState2D[nPattern].m_nDiffusion,
				m_EffectState2D[nPattern].m_nDestroyvec,
				m_EffectState2D[nPattern].Synthetic);
		}
		break;
	case(3):
		for (int nCnt = 0; nCnt < m_EffectState2D[nPattern].m_nDensity; nCnt++)
		{

			//各色のランダム化
			if (m_EffectState2D[nPattern].m_bColorRandR == true)
			{
				m_EffectState2D[nPattern].m_Col.r = RAND_COLOR;
			}
			if (m_EffectState2D[nPattern].m_bColorRandG == true)
			{
				m_EffectState2D[nPattern].m_Col.g = RAND_COLOR;
			}
			if (m_EffectState2D[nPattern].m_bColorRandB == true)
			{
				m_EffectState2D[nPattern].m_Col.b = RAND_COLOR;
			}

			CRotate::Create(pos,
				m_EffectState2D[nPattern].m_move,
				m_EffectState2D[nPattern].m_Col,
				m_EffectState2D[nPattern].m_Changecolor,
				D3DXVECTOR2(m_EffectState2D[nPattern].m_fSize, m_EffectState2D[nPattern].m_fSize),
				D3DXVECTOR2(m_EffectState2D[nPattern].m_fAddSize, m_EffectState2D[nPattern].m_fAddSize),
				m_EffectState2D[nPattern].m_nLife,
				m_EffectState2D[nPattern].nTexture,
				m_EffectState2D[nPattern].m_fRotate,
				m_EffectState2D[nPattern].Synthetic);
		}
		break;
	case(4):
		for (int nCnt = 0; nCnt < m_EffectState2D[nPattern].m_nDensity; nCnt++)
		{

			//各色のランダム化
			if (m_EffectState2D[nPattern].m_bColorRandR == true)
			{
				m_EffectState2D[nPattern].m_Col.r = RAND_COLOR;
			}
			if (m_EffectState2D[nPattern].m_bColorRandG == true)
			{
				m_EffectState2D[nPattern].m_Col.g = RAND_COLOR;
			}
			if (m_EffectState2D[nPattern].m_bColorRandB == true)
			{
				m_EffectState2D[nPattern].m_Col.b = RAND_COLOR;
			}

			CRotate::Create(pos,
				m_EffectState2D[nPattern].m_move,
				m_EffectState2D[nPattern].m_Col,
				m_EffectState2D[nPattern].m_Changecolor,
				D3DXVECTOR2(m_EffectState2D[nPattern].m_fSize, m_EffectState2D[nPattern].m_fSize),
				D3DXVECTOR2(m_EffectState2D[nPattern].m_fAddSize, m_EffectState2D[nPattern].m_fAddSize),
				m_EffectState2D[nPattern].m_nLife,
				m_EffectState2D[nPattern].nTexture,
				m_EffectState2D[nPattern].m_fRotate,
				m_EffectState2D[nPattern].Synthetic);
		}
		break;

	default:
		assert(false);
		break;
	}
}

//=============================================================================
// 呼ばれた物を呼び出すやつ3D
//=============================================================================
void CPresetEffect::SetEffect3D(int nPattern, D3DXVECTOR3 pos, D3DXVECTOR3 Endpos)
{
	D3DXVECTOR3 Vectl;
	float fA;
	float fAY;
	int Vectlx;
	int Vectly;
	int Vectlz;
	float Rx;
	float Ry;
	float Rz;
	float y;
	float z;
	float x;
	bool bRx = false;
	bool bRy = false;
	bool bRz = false;

	D3DXVECTOR3 move = m_EffectState3D[nPattern].move3d;

	switch (m_EffectState3D[nPattern].m_nPattern)
	{
	case(0):	//軌跡

		break;
	case(1):	//パーティクル
		move.x = (float(rand() % (int)m_EffectState3D[nPattern].move3d.x)) - (float(rand() % (int)m_EffectState3D[nPattern].move3d.x));
		move.x /= 10;
		move.y = (float(rand() % (int)m_EffectState3D[nPattern].move3d.y)) - (float(rand() % (int)m_EffectState3D[nPattern].move3d.y));
		move.y /= 10;
		move.z = (float(rand() % (int)m_EffectState3D[nPattern].move3d.z)) - (float(rand() % (int)m_EffectState3D[nPattern].move3d.z));
		move.z /= 10;


		CStraight3D::Create(pos,
			D3DXVECTOR3(m_EffectState3D[nPattern].m_fSize, m_EffectState3D[nPattern].m_fSize, 0.0f),
			D3DXVECTOR3(m_EffectState3D[nPattern].m_fAddSize, m_EffectState3D[nPattern].m_fAddSize, 0.0f),
			move,
			m_EffectState3D[nPattern].m_Col,
			m_EffectState3D[nPattern].m_Changecolor,
			m_EffectState3D[nPattern].nTexture,
			m_EffectState3D[nPattern].m_nLife,
			CStraight3D::STRAIGHT, {},
			m_EffectState3D[nPattern].Synthetic);
		break;
	case(2):	//纏わせ
		Vectl = pos - Endpos;

		fA = (float)atan2(Vectl.x, Vectl.z);
		fAY = (float)atan2(Vectl.x, Vectl.y);

		Vectlx = (int)Vectl.x;
		Vectly = (int)Vectl.y;
		Vectlz = (int)Vectl.z;

		//ピッタリ0だとrandで死ぬので
		if (Vectlx == 0)
		{
			Vectlx = 1;
		}
		if (Vectly == 0)
		{
			Vectly = 1;
		}
		if (Vectlz == 0)
		{
			Vectlz = 1;
		}

		if (Vectlx <= 0)
		{
			Vectlx *= -1;
			bRx = true;
		}
		if (Vectly <= 0)
		{
			Vectly *= -1;
			bRy = true;
		}
		if (Vectlz <= 0)
		{
			Vectlz *= -1;
			bRz = true;
		}
		for (int nCnt = 0; nCnt < m_EffectState3D[nPattern].m_nDensity; nCnt++)
		{
			Rx = float(rand() % Vectlx);
			Ry = float(rand() % Vectly);
			Rz = float(rand() % Vectlz);

			if (bRx == true)
			{
				Rx *= -1;
			}
			if (bRy == true)
			{
				Ry *= -1;
			}
			if (bRz == true)
			{
				Rz *= -1;
			}

			//発生した地点から少し動かす
			x = float(rand() % m_EffectState3D[nPattern].RandMove) + 1;
			y = float(rand() % m_EffectState3D[nPattern].RandMove) + 1;
			z = float(rand() % m_EffectState3D[nPattern].RandMove) + 1;

			//大きすぎる値を〇.〇〇の値へ
			x /= 100;
			y /= 100;
			z /= 100;

			//粒の発生
			CStraight3D::Create(D3DXVECTOR3(Endpos.x + Rx, Endpos.y + Ry, Endpos.z + Rz),
				D3DXVECTOR3(m_EffectState3D[nPattern].m_fSize, m_EffectState3D[nPattern].m_fAddSize, 0.0f),
				D3DXVECTOR3(m_EffectState3D[nPattern].m_fAddSize, m_EffectState3D[nPattern].m_fAddSize, 0.0f),
				D3DXVECTOR3(sinf(fA) * x, cosf(fAY) * y, cosf(fA) * z),
				m_EffectState3D[nPattern].m_Col,
				m_EffectState3D[nPattern].m_Changecolor,
				m_EffectState3D[nPattern].nTexture, m_EffectState3D[nPattern].m_nLife, CStraight3D::STRAIGHT, {}, m_EffectState3D[nPattern].Synthetic);
		}
		break;
	case(3):	//フィールド
		CFieldEffect::Create(D3DXVECTOR3(m_EffectState3D[nPattern].m_fSize, 0.0f, m_EffectState3D[nPattern].m_fSize),
			D3DXVECTOR3(pos.x, m_EffectState3D[nPattern].pos.y, pos.z),
			m_EffectState3D[nPattern].m_Col,
			m_EffectState3D[nPattern].m_Changecolor,
			m_EffectState3D[nPattern].m_fRotate,
			m_EffectState3D[nPattern].m_Active,
			m_EffectState3D[nPattern].m_MaxSize,
			m_EffectState3D[nPattern].m_fAddSize + 0.1,
			m_EffectState3D[nPattern].Synthetic,
			m_EffectState3D[nPattern].m_nDensity,
			m_EffectState3D[nPattern].m_nDistance,
			m_EffectState3D[nPattern].m_SecondCol,
			m_EffectState3D[nPattern].m_SecondChangecolor,
			m_EffectState3D[nPattern].nTexture,
			m_EffectState3D[nPattern].m_nLife,
			m_EffectState3D[nPattern].m_ParticleSynthetic,
			m_EffectState3D[nPattern].m_move,
			m_EffectState3D[nPattern].m_fParticleAddSize,
			m_EffectState3D[nPattern].m_fParticleSize,
			m_EffectState3D[nPattern].ParticleTime,
			m_EffectState3D[nPattern].m_fActiveAddSize,
			m_EffectState3D[nPattern].m_FieldTime,
			m_EffectState3D[nPattern].m_fieldCreate,
			m_EffectState3D[nPattern].mCreatePreset);

		break;
	}
}
