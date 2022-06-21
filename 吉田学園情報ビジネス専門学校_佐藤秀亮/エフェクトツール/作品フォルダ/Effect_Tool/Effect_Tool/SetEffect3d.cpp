//*****************************************************************************
// 3Dエフェクト設置処理(SetEffect3d.h)
// Author : 佐藤秀亮
//*****************************************************************************
#include "SetEffect3d.h"
#include "control.h"
#include "keyboard.h"
#include "mouse.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"

#include "straight3d.h"
#include "FieldEffect.h"
#include "ActiveBillboard.h"
#include "Rotate3d.h"
#include "SphereEffect.h"
#include "ThunderBill.h"
#include "BulletHoll.h"

#include <assert.h>
//*****************************************************************************
//静的
//*****************************************************************************
bool CSetEffect3D::bOne = false;

bool CSetEffect3D::m_MouseButtenPush = false;
int CSetEffect3D::m_MousePushTime = 0.0f;

bool CSetEffect3D::m_PushDeley = false;
int CSetEffect3D::m_DeleyTime = 0.0f;

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
	m_pMouse = CManager::GetMouse();
	m_pCamera = CManager::GetRenderer()->GetCamera();

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


	//エフェクト発生
	if (m_pMouse->GetMouseButton(CMouse::DIM_L) == true)
	{
		SetEffect();
		m_MouseButtenPush = true;
	}
	if (m_MouseButtenPush == true)
	{
		m_MousePushTime++;

		if (m_MousePushTime >= 30)
		{
			m_PushDeley = true;
		}
		if (m_PushDeley == true)
		{
			m_DeleyTime++;
			if (m_DeleyTime > 0)
			{
				m_DeleyTime = 0;
				SetEffect();
			}
		}
	}
	if (m_pMouse->GetRelease(CMouse::DIM_L) == true)
	{
		m_PushDeley = false;
		m_MouseButtenPush = false;
		m_MousePushTime = 0;
	}

}
//*****************************************************************************
//エフェクト作成
//*****************************************************************************
void CSetEffect3D::SetEffect()
{
	float RandAngle;
	D3DXVECTOR3 pos = GetPos();

	for (int nCnt = 0; nCnt < CControl::GetDensity(); nCnt++)
	{
		switch (CControl::GetPattern())
		{
		case(0):
			bOne = false;

			break;
		case(1):
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
				CControl::GetTexNum(),
				CControl::GetAnimCont(),
				D3DXVECTOR2(CControl::GetSplitU(), CControl::GetSplitV()),
				(CBillEffect::ANIMPATTERN)CControl::GetAnimPatternType());
			bOne = false;
			break;
		case(2):
			bOne = false;
			break;
		case(3):
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
					CControl::GetAvctiveAddSize(), 0, (bool)0, 0,
					CControl::GetAnimPatternType());
			break;
		case(4):
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
					D3DXVECTOR2(CControl::GetTexNum().x, CControl::GetTexNum().y),
					CControl::GetAnimCont(),
					D3DXVECTOR2(CControl::GetSplitU(), CControl::GetSplitV()),
					(CBillEffect::ANIMPATTERN)CControl::GetAnimPatternType());
			break;
		case(5):
				RandAngle = CIRCLE;

				CRotate3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), {},
					D3DXVECTOR3(CControl::GetSize(), CControl::GetSize(), 0.0f),
					D3DXVECTOR3(CControl::GetChangeSize(), CControl::GetChangeSize(), 0.0f),
					D3DXCOLOR((float)CControl::GetControlCoror(1), (float)CControl::GetControlCoror(2), (float)CControl::GetControlCoror(3), (float)CControl::GetControlCoror(4)),
					D3DXCOLOR((float)CControl::GetChangeCol(1), (float)CControl::GetChangeCol(2), (float)CControl::GetChangeCol(3), (float)CControl::GetChangeCol(4)),
					CControl::GetDistance(),
					CControl::Getmove3d().x, RandAngle, CControl::GetRotate(),
					CControl::GetTex(), CControl::GetSynthetic(), CControl::GetLife(),
					CControl::GetParticleTime(),
					CControl::GetSecondTime(),
					CControl::GetMaxSize(),
					(CBillEffect::ANIMPATTERN)CControl::GetAnimPatternType(),
					(CRotate3D::EFFECT_TYPE)CControl::GetType(),
					(CRotate3D::MOVE_TYPE)CControl::GetSecondType());

			break;
		case(6):
				CSphereEffect::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CControl::GetRotate(),
					CControl::GetSize(), CControl::GetTex(),
					D3DXCOLOR((float)CControl::GetControlCoror(1), (float)CControl::GetControlCoror(2), (float)CControl::GetControlCoror(3), (float)CControl::GetControlCoror(4)),
					D3DXCOLOR((float)CControl::GetChangeCol(1), (float)CControl::GetChangeCol(2), (float)CControl::GetChangeCol(3), (float)CControl::GetChangeCol(4)),
					CControl::GetLife(), CControl::GetSynthetic(), CControl::GetChangeSize(),
					CControl::GetVtx(), (CSphereEffect::SPHERE_TYPE)CControl::GetType(),
					D3DXVECTOR2(CControl::GetTexMoveU(), CControl::GetTexMoveV()), CControl::GetTexNum(),
					CControl::GetAnimCont(),
					D3DXVECTOR2(CControl::GetSplitU(), CControl::GetSplitV()));
			break;
		case(7):
				CThunderBill::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(CControl::GetSize(), CControl::GetSize(), 0.0f),
					D3DXVECTOR3(CControl::GetChangeSize(), CControl::GetChangeSize(), 0.0f),
					D3DXCOLOR((float)CControl::GetControlCoror(1), (float)CControl::GetControlCoror(2), (float)CControl::GetControlCoror(3), (float)CControl::GetControlCoror(4)),
					D3DXCOLOR((float)CControl::GetChangeCol(1), (float)CControl::GetChangeCol(2), (float)CControl::GetChangeCol(3), (float)CControl::GetChangeCol(4)),
					CControl::GetTex(), CControl::GetLife(),
					CControl::GetDistance(),
					D3DXVECTOR2(CControl::GetTexMoveU(), CControl::GetTexMoveV()), CControl::GetTexNum(),
					CControl::GetAnimCont(),
					D3DXVECTOR2(CControl::GetSplitU(), CControl::GetSplitV()),
					CControl::GetHigth(),
					D3DXVECTOR3(CControl::GetParticleSize(), CControl::GetParticleSize(), {}),
					CControl::GetSynthetic(),
					(CBillEffect::ANIMPATTERN)CControl::GetAnimPatternType());
			break;
		case(8):
			break;
		case(9):
			CBulletHoll::Create(D3DXVECTOR3(CControl::GetSize(), CControl::GetHigth(), 0.0f),
				D3DXVECTOR3(CControl::GetChangeSize(), CControl::GetChangeSize(), 0.0f),
				D3DXVECTOR3(100.0f, 100.0f, 100.0f),
				D3DXCOLOR((float)CControl::GetControlCoror(1), (float)CControl::GetControlCoror(2), (float)CControl::GetControlCoror(3), (float)CControl::GetControlCoror(4)),
				D3DXCOLOR((float)CControl::GetChangeCol(1), (float)CControl::GetChangeCol(2), (float)CControl::GetChangeCol(3), (float)CControl::GetChangeCol(4)),
				CControl::GetLife(),
				CControl::GetTex(),
				D3DXVECTOR2(CControl::GetTexMoveU(), CControl::GetTexMoveV()),
				CControl::GetTexNum(),
				CControl::GetAnimCont(),
				D3DXVECTOR2(CControl::GetSplitU(), CControl::GetSplitV()),
				D3DXVECTOR3(m_pCamera->GetRotX(), m_pCamera->GetRotY(), {}),
				CControl::GetSynthetic(),
				(CBillEffect::ANIMPATTERN)CControl::GetAnimPatternType()
			);
			break;
		default:
			assert(false);
			break;
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
