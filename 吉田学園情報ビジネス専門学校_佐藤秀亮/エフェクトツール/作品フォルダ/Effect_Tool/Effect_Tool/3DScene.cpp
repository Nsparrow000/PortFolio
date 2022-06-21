//=============================================================================
// 3D側エフェクト管理[3DScene.h]
// Author : 佐藤秀亮
//=============================================================================
//*****************************************************************************
//インクルード
//*****************************************************************************
#if 1
#include "3DScene.h"
#include "Fade.h"
#include "load.h"
#include "control.h"
#include "manager.h"
#include "camera.h"

#include "keyboard.h"
#include "renderer.h"

#include "SetEffect3d.h"

#include "LoadEffect.h"
#include "PresetSetEffect.h"
#include "Butten.h"

#include "Player.h"

#endif

//*****************************************************************************
//静的
//*****************************************************************************
int C3DScene::m_nDirey = 0;
bool C3DScene::m_bPresetStart = false;
int C3DScene::m_nPattarn = 0;

//*****************************************************************************
//マクロ
//*****************************************************************************
#define FILE3D "data/3DMode.txt"
#define SET_TEXT_3D "data/3DSetTex.txt"

#define LOAD_PRESET_TEXT "data/Preset.txt"

#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
C3DScene::C3DScene()
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
C3DScene::~C3DScene()
{
}
#endif

//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT C3DScene::Init(D3DXVECTOR3 /*pos*/)
{
	m_pKeyboard = CManager::GetKeyboard();

	CLoad::Load(FILE3D);
	CLoad::LoadButten(SET_TEXT_3D);

	CLoadEffect::EffectOrder(LOAD_PRESET_TEXT);
	CLoadEffect::EffectStateLoad(LOAD_PRESET_TEXT);

	CControl::SetPlayerMode(true);
	//フェード
	CFade::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	m_pCamera = CManager::GetRenderer()->GetCamera();

	m_bPresetStart = false;

	m_nDirey = 0;
	//エフェクト出すやつ
	CSetEffect3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, +30.0f, 0.0f),
		D3DXVECTOR2(0.0f, 0.0f));

	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void C3DScene::Uninit()
{
	CScene::Release();
}

//*****************************************************************************
// kousinn
//***************************************************************************** 
void C3DScene::Update()
{
	D3DXVECTOR3 pos;

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

	if (m_pKeyboard != NULL)
	{
		if (m_pKeyboard->GetKey(DIK_RETURN) == true)
		{
			CControl::SetPlayerMode(true);
			CFade::SetFade(CManager::MODE_2D);
			CControl::SetPattern(0);
			CButten::ResetPatten();
		}

		if (m_pKeyboard->GetKey(DIK_F1) == true)
		{
			CPresetEffect::SetEffect3D(m_nPattarn, D3DXVECTOR3(0.0f, 0.0f, 0.0f), {}, {});
		}

		if (m_pKeyboard->GetKey(DIK_F2) == true)
		{
			for (int nCnt = 0; nCnt < CLoadEffect::GetPresetTotal(); nCnt++)
			{

				CPresetEffect::SetEffect3D(nCnt, D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), {});
			}
		}
		if (m_pKeyboard->GetKey(DIK_F3) == true)
		{
			CPresetEffect::CallOrder3D(0, D3DXVECTOR3(0.0f, 50.0f, 0.0f), {});
		}
		if (m_pKeyboard->GetKey(DIK_F4) == true)
		{
			CPresetEffect::SetEffect2D(0, D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(1000.0f, 1000.0f, 0.0f), D3DXVECTOR3(pos.x, pos.z, {}), D3DXVECTOR3(0.0f, m_pCamera->GetRotY(), 0.0f));

		}


	}

}

#if 1
//*****************************************************************************
//描画
//***************************************************************************** 
void C3DScene::Draw()
{

}

//*****************************************************************************
//描画
//***************************************************************************** 
C3DScene *C3DScene::Create()
{
	C3DScene *p2DScene = NULL;
	p2DScene = new C3DScene;		//メモリ確保
									//NULLチェック
	if (p2DScene != NULL)
	{
		p2DScene->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return p2DScene;
}
#endif

#if 1
#endif
