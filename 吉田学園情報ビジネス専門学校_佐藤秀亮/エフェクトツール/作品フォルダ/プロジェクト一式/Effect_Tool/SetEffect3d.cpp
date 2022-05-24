//*****************************************************************************
// 3Dエフェクト設置処理(SetEffect3d.h)
// Author : 佐藤秀亮
//*****************************************************************************
#include "SetEffect3d.h"
#include "control.h"
#include "keyboard.h"

#include "straight3d.h"
#include "FieldEffect.h"
#include "ActiveBillboard.h"
#include "Rotate3d.h"
#include "SphereEffect.h"

#include <assert.h>
//*****************************************************************************
//静的
//*****************************************************************************
bool CSetEffect3D::bOne = false;


//*****************************************************************************
//マクロ
//*****************************************************************************
#define CIRCLE (float(rand() % 324) / 100.0f - float(rand() % 324) / 100.0f) //円のマクロ

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CSetEffect3D::CSetEffect3D(int nPriority) : CPlane(nPriority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSetEffect3D::~CSetEffect3D()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CSetEffect3D::Init(D3DXVECTOR3 Size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex)
{
	CPlane::Init(Size, pos, Tex);
	m_pKeyboard = CManager::GetKeyboard();

	bOne = false;
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CSetEffect3D::Uninit()
{
	CPlane::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CSetEffect3D::Update()
{
	D3DXVECTOR3 pos = GetPos();

	//プレイヤー追従がON
	if (CControl::GetPlayerFollow() == true)
	{
		CScene *pScene = GetScene(CManager::PRIORITY_SET);
		while (pScene)
		{
			CScene *pSceneNext;
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJECTTYPE_PLAYER)
			{
				pos = pScene->GetPos();
			}
			pScene = pSceneNext;
		}
	}

	float RandAngle;

	if (m_pKeyboard != NULL)
	{

		for (int nCnt = 0; nCnt < CControl::GetDensity(); nCnt++)
		{
			switch (CControl::GetPattern())
			{
			case(0):
				bOne = false;

				break;
			case(1):
				if (m_bOne == false)
				{
					m_ParticleTime = CControl::GetParticleTime();
					m_bOne = true;
				}
				else if (m_bOne == true)
				{
					m_ParticleTime--;
					if (m_ParticleTime < 0)
					{
						CStraight3D::Create(pos,
							D3DXVECTOR3(CControl::GetSize(), CControl::GetSize(), 0.0f),
							D3DXVECTOR3(CControl::GetChangeSize(), CControl::GetChangeSize(), 0.0f),
							CControl::Getmove3d(),
							D3DXCOLOR((float)CControl::GetControlCoror(1), (float)CControl::GetControlCoror(2), (float)CControl::GetControlCoror(3), (float)CControl::GetControlCoror(4)),
							D3DXCOLOR((float)CControl::GetChangeCol(1), (float)CControl::GetChangeCol(2), (float)CControl::GetChangeCol(3), (float)CControl::GetChangeCol(4)),
							CControl::GetTex(), CControl::GetLife(), CStraight3D::STRAIGHT, {}, CControl::GetSynthetic(),
							CControl::GetDistance(),
							(CStraight3D::RAND_PATTEN)CControl::GetType(),
							(CStraight3D::POS_PATTERN)CControl::GetSecondType(),
							D3DXVECTOR2(CControl::GetTexMoveU(), CControl::GetTexMoveV()),
							CControl::GetTexNum());
						bOne = false;
						m_bOne = false;
					}
				}

				break;
			case(2):
				bOne = false;
				break;
			case(3):
				if (m_pKeyboard->GetKey(DIK_SPACE) == true)
				{
					CFieldEffect::Create(D3DXVECTOR3(CControl::GetSize(), 0.0f, CControl::GetSize()),
						D3DXVECTOR3(0.0f, 0.5f, 0.0f),
						CControl::GetColor(),
						CControl::GetChangeColor(),
						CControl::GetRotate(),
						CControl::GetActiveTime(),
						CControl::GetMaxSize(),
						CControl::GetChangeSize() + 0.1,
						CControl::GetSynthetic(), CControl::GetDensity(), CControl::GetDistance(),
						D3DXCOLOR((float)CControl::GetParticleColor(1), (float)CControl::GetParticleColor(2), (float)CControl::GetParticleColor(3), (float)CControl::GetParticleColor(4)),
						D3DXCOLOR((float)CControl::GetParticleAddCol(1), (float)CControl::GetParticleAddCol(2), (float)CControl::GetParticleAddCol(3), (float)CControl::GetParticleAddCol(4)),
						CControl::GetTex(), CControl::GetLife(),
						CControl::GetParticleSynthetic(),
						CControl::Getmove3d().x,
						CControl::GetParticleAddSize(),
						CControl::GetParticleSize(),
						CControl::GetParticleTime(),
						CControl::GetAvctiveAddSize(),0,(bool)0,0);
				}
				break;
			case(4):
				if (m_pKeyboard->GetKey(DIK_SPACE) == true)
				{
					CActiveBillBoard::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f),
						D3DXVECTOR3(CControl::GetSize(), 0.0f, CControl::GetSize()),
						D3DXVECTOR3(CControl::GetChangeSize(), CControl::GetChangeSize(), 0.0f),
						CControl::Getmove3d(),
						D3DXCOLOR((float)CControl::GetControlCoror(1), (float)CControl::GetControlCoror(2), (float)CControl::GetControlCoror(3), (float)CControl::GetControlCoror(4)),
						D3DXCOLOR((float)CControl::GetChangeCol(1), (float)CControl::GetChangeCol(2), (float)CControl::GetChangeCol(3), (float)CControl::GetChangeCol(4)),
						CControl::GetTex(), 700,
						CControl::GetSynthetic(), CControl::GetActiveTime(),
						CControl::GetMaxSize(), CControl::GetChangeSize(), 0, 0,
						D3DXVECTOR2(CControl::GetTexMoveU(), CControl::GetTexMoveV()),
						CControl::GetTexNum());
				}
				break;
			case(5):
				if (m_pKeyboard->GetKey(DIK_SPACE) == true)
				{
					RandAngle = CIRCLE;

					CRotate3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f,50.0f,0.0f), {},
						D3DXVECTOR3(CControl::GetSize(), CControl::GetSize(),0.0f),
						D3DXVECTOR3(CControl::GetChangeSize(), CControl::GetChangeSize(), 0.0f),
						D3DXCOLOR((float)CControl::GetControlCoror(1), (float)CControl::GetControlCoror(2), (float)CControl::GetControlCoror(3), (float)CControl::GetControlCoror(4)),
						D3DXCOLOR((float)CControl::GetChangeCol(1), (float)CControl::GetChangeCol(2), (float)CControl::GetChangeCol(3), (float)CControl::GetChangeCol(4)),
						CControl::GetDistance(),
						CControl::Getmove3d().x, RandAngle,CControl::GetRotate(),
						CControl::GetTex(),CControl::GetSynthetic(),CControl::GetLife(),
						CControl::GetParticleTime(),
						CControl::GetSecondTime(),
						CControl::GetMaxSize());
				}

				break;
			case(6):
				if (m_pKeyboard->GetKey(DIK_SPACE) == true)
				{
					CSphereEffect::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CControl::GetRotate(),
						CControl::GetSize(), CControl::GetTex(),
						D3DXCOLOR((float)CControl::GetControlCoror(1), (float)CControl::GetControlCoror(2), (float)CControl::GetControlCoror(3), (float)CControl::GetControlCoror(4)),
						D3DXCOLOR((float)CControl::GetChangeCol(1), (float)CControl::GetChangeCol(2), (float)CControl::GetChangeCol(3), (float)CControl::GetChangeCol(4)),
						CControl::GetLife(), CControl::GetSynthetic(), CControl::GetChangeSize(),
						CControl::GetVtx(),(CSphereEffect::SPHERE_TYPE)CControl::GetType(),
						D3DXVECTOR2(CControl::GetTexMoveU(),CControl::GetTexMoveV()),CControl::GetTexNum(),
						CControl::GetAnimCont(),
						D3DXVECTOR2(CControl::GetSplitU(), CControl::GetSplitV()));
				}
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}

//*****************************************************************************
//作成
//*****************************************************************************
CSetEffect3D *CSetEffect3D::Create(D3DXVECTOR3 Size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex)
{
	CSetEffect3D * pSetEffect3D = NULL;
	pSetEffect3D = new CSetEffect3D(CManager::PRIORITY_EFFECT);
	if (pSetEffect3D != NULL)
	{
		pSetEffect3D->Init(Size, pos, Tex);
	}
	return pSetEffect3D;
}
