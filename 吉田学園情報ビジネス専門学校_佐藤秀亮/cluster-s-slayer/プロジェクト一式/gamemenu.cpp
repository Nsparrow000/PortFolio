//--------------------------------------
//ゲーム開始前のメニュー画面
//--------------------------------------
#include "gamemenu.h"
#include "bg.h"
#include "texture.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "Polygon.h"
#include "XInput.h"
#include "sound.h"
#include "directinput.h"
#include "titleselectbutton.h"
#include "savedata.h"

#include "PresetSetEffect.h"
#include "mouse.h"


//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CGameMenu::CGameMenu()
{
	for (int nCnt = 0; nCnt < MENUBUTTON_MAX; nCnt++)
	{
		pButton[nCnt] = nullptr;
	}
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CGameMenu::~CGameMenu()
{

}

//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CGameMenu::Init(void)
{
	m_bNextMode = false;
	m_nSelectType = 0;
	m_nDecisionType = 0;

	CBg::Create(CTexture::GAUGEBER, CScene::OBJTYPE_BG, false);	//背景

	int line = 0;
	D3DXVECTOR3 SkillPos = D3DXVECTOR3(115.0f, SCREEN_HEIGHT / 2 - 200.0f, 0.0f);
	D3DXVECTOR3 SkillSize = D3DXVECTOR3(50.0f, 50.0f, 0.0f);

	for (int nCnt = 0; nCnt < MENUBUTTON_MAX; nCnt++)
	{
		line++;
		bool bHaveSkill = false;

		switch (nCnt)
		{
		case CPlayer::ATKup:
		case CPlayer::Heal:
		case CPlayer::OverHeal:
		case CPlayer::Sheild:
		case CPlayer::Beam:
		case CPlayer::BlackHole:
		case CPlayer::Rocket:
		case CPlayer::RushAttack:

			if (CSaveData::GetHaveSkill(nCnt))
			{
				bHaveSkill = true;
			}

			pButton[nCnt] = CTitleSelectButton::Create(SkillPos, SkillSize, CTexture::ATKup + nCnt, !bHaveSkill);

			break;

		case MENUBUTTON_GAME_START:
			pButton[nCnt] = CTitleSelectButton::Create(
				D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 250, SCREEN_HEIGHT / 2.0f - 75.0f, 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH / 5.0f, SCREEN_HEIGHT / 5.0f, 0.0f),
				CTexture::Menu_GameStart);

			break;

		case MENUBUTTON_RETURN_TO_TITLE:
			pButton[nCnt] = CTitleSelectButton::Create(
				D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT - (SCREEN_HEIGHT / 8.0f), 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH / 10.0f, SCREEN_HEIGHT / 20.0f, 0.0f),
				CTexture::Menu_Back);

			break;

		default:
			break;
		}

		SkillPos.x += 115.0f;

		if (line % 3 == 0)
		{
			SkillPos.x = 115.0f;
			SkillPos.y += 115.0f;
		}
	}

	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CGameMenu::Uninit(void)
{
	CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_TITLE);
}
//--------------------------------------------
//更新
//--------------------------------------------
void CGameMenu::Update(void)
{
	if (!m_bNextMode)
	{
		for (int nCnt = 0; nCnt < MENUBUTTON_MAX; nCnt++)
		{
			if (pButton[nCnt]->GetDecision())
			{
				m_nDecisionType = nCnt;
			}

			if (m_nDecisionType != 0)
			{
				//どれかが決定している状態なら終了
				if (m_nDecisionType == MENUBUTTON_GAME_START)
				{
					// 強化したスキルをセーブ

					// 音声を再生
					CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);
					CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENTER, 0.6f);

					m_bNextMode = true;

					// ゲームシーンへ行く
					CFade::SetFade(CManager::MODE_GAME);
				}

				else if (m_nDecisionType == MENUBUTTON_RETURN_TO_TITLE)
				{
					// 強化したスキルをセーブ

					// 音声を再生
					CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);
					CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENTER, 0.6f);

					m_bNextMode = true;

					// タイトルシーンへ行く
					CFade::SetFade(CManager::MODE_TITLE);
				}
			}
		}
	}

	//クリックエフェクト
	POINT po;
	GetCursorPos(&po);
	HWND hWnd = GetForegroundWindow();
	ScreenToClient(hWnd, &po);

	if (CManager::GetMouse()->GetTrigger(CMouse::MOUSE_LEFT))
	{
		CPresetEffect::SetEffect2D(0, D3DXVECTOR3(po.x, po.y, 0.0f), {}, CScene::OBJTYPE_UI);
	}

}
//--------------------------------------------
//描画
//--------------------------------------------
void CGameMenu::Draw(void)
{

}