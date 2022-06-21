//--------------------------------------
//スキル選択画面の処理
//--------------------------------------
#include "skillselect.h"
#include "Polygon.h"
#include "input.h"
#include "keyboard.h"
#include "manager.h"
#include "selectbottom.h"
#include "player.h"
#include "game.h"
#include "mouse.h"
#include "PresetSetEffect.h"
#include "sound.h"

static const D3DXVECTOR3 SelectWindowSize = { 500.0f,300.f,0.0f };
static const D3DXVECTOR3 SelectBottomSize = { 150.0f,250.f,0.0f };
static const D3DXVECTOR3 SelectBottomPos = { 300.0f,300.f,0.0f };
static const float SelectBottomPosInterval = 200.0f;//アクションスキルの乱数の最小値

static const int ActionSkillMin = 4;//アクションスキルの乱数の最小値
static const int ActionSkillMax = 8;//アクションスキルの乱数の最大値
static const int PassiveSkillMin = 0;//パッシブスキルの乱数の最小値
static const int PassiveSkillMax = 4;//パッシブスキルの乱数の最大値

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CSkillSelect::CSkillSelect(OBJTYPE nPriority):CScene(nPriority)
{
	pSelectWindow = nullptr;
	m_bPopSelectWindow = true;
	pSelectWindowBg = nullptr;
	m_nSelectType = 0;
	m_nDecisionType = -1;
	for (int nCnt = 0; nCnt < max_Bottom; nCnt++)
	{
		pSkillBottom[nCnt] = nullptr;
	}
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CSkillSelect::~CSkillSelect()
{
}
//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CSkillSelect::Init(void)
{
	if (!pSelectWindowBg)
	{
		//スキル選択画面の背景の生成
		pSelectWindowBg = CPolygon::Create({ SCREEN_WIDTH / 2.0f, SCREEN_WIDTH / 2.0f, 0.0f },
		{ SCREEN_WIDTH / 2.0f, SCREEN_WIDTH / 2.0f, 0.0f }, CTexture::FADE, { 1.0f,1.0f,1.0f,0.0f }, OBJTYPE_TIME);

	}
	if (!pSelectWindow)
	{
		//スキル選択画面画像の生成
		pSelectWindow = CPolygon::Create({ SCREEN_WIDTH / 2.0f,320.0f,0.0f }, SelectWindowSize, CTexture::FADE, {1.0,1.0,1.0,0.0}, OBJTYPE_TIME);
	}
	ShowCursor(true);

	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CSkillSelect::Uninit(void)
{
	//カーソルをみえないようにする
	ShowCursor(false);

	if (pSelectWindow)
	{
		pSelectWindow->Uninit();
		pSelectWindow = nullptr;
	}
	if (pSelectWindowBg)
	{
		pSelectWindowBg->Uninit();
		pSelectWindowBg = nullptr;
	}
	Release();
}
//--------------------------------------------
//更新
//--------------------------------------------
void CSkillSelect::Update(void)
{
	POINT po;
	//マウスのスクリーン座標の位置の取得
	GetCursorPos(&po);
	HWND hWnd = GetForegroundWindow();

	//スクリーン座標からクライアント座標の変換
	ScreenToClient(hWnd, &po);

	//マウス情報の取得
	CMouse *pMouse = CManager::GetMouse();

	//選択している状態で左クリックをすると決定状態にする
	if (pMouse->GetTrigger(CMouse::MOUSE_LEFT) == true)
	{
		CPresetEffect::SetEffect2D(0, D3DXVECTOR3(po.x, po.y, 0.0f), {},OBJTYPE_UI);
	}

	if (m_bPopSelectWindow)
	{
		//セレクトウィンドウの出現
		PopSelectWindow();
	}
	else
	{

		CInputKeyBoard *Key = CManager::GetInputKeyboard();
		for (int nCnt = 0; nCnt < max_Bottom; nCnt++)
		{
			if (pSkillBottom[nCnt])
			{
				bool bDecision = pSkillBottom[nCnt]->GetDecision();
				//どれかが決定している状態なら終了
				if (bDecision)
				{
					m_nDecisionType = nCnt;
					//どのスキルを選択したかを取得
					int nSkillType = pSkillBottom[nCnt]->GetSkillNo();
					CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
					//選択したスキルの反映
					pPlayer->LevelUp(nSkillType);

					m_bEnd = true;
				}
			}
		}
	}
	//終了判定がオンなら
	if (m_bEnd)
	{
		//ゲームを止める
		CManager::SetPause(false, false);
		//スキル選択ボタンも消す
		for (int nCnt = 0; nCnt < max_Bottom; nCnt++)
		{
			if (pSkillBottom[nCnt])
			{
				pSkillBottom[nCnt]->SetEnd(true);

			}
		}
		Uninit();
	}
}

//--------------------------------------------
//描画
//--------------------------------------------
void CSkillSelect::Draw()
{
}
//-----------------------------------------------
//インスタンス生成
//---------------------------------------------
CSkillSelect *CSkillSelect::Create()
{
	CSkillSelect *pSkillSelect = new CSkillSelect;
	if (pSkillSelect)
	{
		pSkillSelect->Init();
	}

	return pSkillSelect;
}
//-----------------------------------------------
//セレクトウィンドウの出現
//---------------------------------------------
void CSkillSelect::PopSelectWindow()
{
	if (pSelectWindowBg)
	{
		D3DXCOLOR col = pSelectWindowBg->GetColor();
		if (col.a <= 0.2f)
		{
			col.a += 0.01f;
			pSelectWindowBg->SetCol(col);
		}
	}
	if (pSelectWindow)
	{
		D3DXVECTOR3 pos = pSelectWindow->GetPos();
		if (pos.y <= SCREEN_HEIGHT / 2.0f)
		{
			pos.y += 2.0f;
			pSelectWindow->SetPos(pos);
		}
		else
		{
			static std::random_device random;	// 非決定的な乱数生成器
			std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
			std::uniform_real_distribution<> randPassive(PassiveSkillMin, PassiveSkillMax);
			std::uniform_real_distribution<> randAction(ActionSkillMin, ActionSkillMax);
			//パッシブスキル1の抽選
			int nPassive01 = (int)randPassive(mt);
			//パッシブスキル2の抽選
			int nPassive02 = (int)randPassive(mt);
			//パッシブスキル2が１と同じなら再抽選
			while (nPassive01== nPassive02)
			{
				nPassive02 = (int)randPassive(mt);
			}
			//アクションスキルの抽選
			int nAction = (int)randAction(mt);

			//スキルボタンの生成
			if (!pSkillBottom[0])
			{
				pSkillBottom[0] = CSkillSelectBottom::Create(
				{ SelectBottomPos.x + ((SelectBottomSize.x+ SelectBottomPosInterval)*0),SelectBottomPos.y,0.0f },
					SelectBottomSize, CTexture::FADE, nPassive01);
			}
			if (!pSkillBottom[1])
			{
				pSkillBottom[1] = CSkillSelectBottom::Create(
				{ SelectBottomPos.x + ((SelectBottomSize.x + SelectBottomPosInterval) * 1),SelectBottomPos.y,0.0f },
					SelectBottomSize, CTexture::FADE, nPassive02);
			}
			if (!pSkillBottom[2])
			{
				pSkillBottom[2] = CSkillSelectBottom::Create(
				{ SelectBottomPos.x + ((SelectBottomSize.x + SelectBottomPosInterval) * 2),SelectBottomPos.y,0.0f },
					SelectBottomSize, CTexture::FADE, nAction);
			}
			m_bPopSelectWindow = false;
		}
	}
}

