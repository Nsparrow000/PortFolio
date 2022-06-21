//--------------------------------------
//リザルト画面の処理
//--------------------------------------
#include "result.h"
#include "bg.h"
#include "texture.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "Polygon.h"
#include "XInput.h"
#include "sound.h"
#include "directinput.h"
#include "gametimer.h"
#include "resultselectbotton.h"
#include "score.h"
#include "savedata.h"
static int ClearTimeMagnification = 1000;
static int EnemyCntMagnification = 5000;
static int S_LankScore = 1000000;
static int A_LankScore = 900000;
static int B_LankScore = 700000;

CBg *CResult::m_pBg = NULL;
static int CntSpeed = 40;
//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CResult::CResult()
{
	m_bNextMode = false;
	m_Cursol = NULL;
	m_pTimer = nullptr;
	m_pEnemyCnt = nullptr;
	m_bCanResultAnnouncement = true;
	memset(m_pSelectButton, NULL, sizeof(m_pSelectButton));
	memset(m_pTotalScoreCnt, NULL, sizeof(m_pTotalScoreCnt));
	memset(pPolygon, NULL, sizeof(pPolygon));
	for (int nCnt = 0; nCnt < MAX_TOTALSCORE; nCnt++)
	{
		m_bTotalScorePoped[nCnt] = false;
	}
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CResult::~CResult()
{
}
//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CResult::Init(void)
{
	m_bNextMode = false;
	m_pBg = CBg::Create(CTexture::None, CScene::OBJTYPE_BG, false);	//背景
	//クリアパーセントの計算
	int nEnemyCntKill = CManager::GetKilledEnemyCount();
	int nClearTime = CManager::GetGameTimeSecond();

	int nTotalScore = (nClearTime*ClearTimeMagnification) + (nEnemyCntKill*EnemyCntMagnification);

	int nClearPercent = 0.0f;
	if (nTotalScore >= S_LankScore)
	{
		nClearPercent = 60;
	}
	else if (nTotalScore >= A_LankScore)
	{
		nClearPercent = 50;
	}
	else if (nTotalScore >= B_LankScore)
	{
		nClearPercent = 40;
	}

	for (int SkillNum = 0; SkillNum < CPlayer::Skill_Max; SkillNum++)
	{
		if (CSaveData::GetHaveSkill(SkillNum))
		{
			nClearPercent += 5;
		}
	}

	CSaveData::SetClearPercent(nClearPercent);
	CSaveData::SaveFile();

	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------

void CResult::Uninit(void)
{
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}
	for (int nCnt = 0; nCnt < MAX_RESULT; nCnt++)
	{
		if (pPolygon[nCnt] != NULL)
		{
			pPolygon[nCnt]->Uninit();
			pPolygon[nCnt] = NULL;
		}
	}
	if (m_Cursol != NULL)
	{
		m_Cursol->Uninit();
		m_Cursol = NULL;
	}

	for (int nCnt = 0; nCnt < MAX; nCnt++)
	{
		if (m_pSelectButton[nCnt])
		{
			m_pSelectButton[nCnt]->Uninit();
			m_pSelectButton[nCnt] = nullptr;
		}
	}

	if (m_pEnemyCnt)
	{
		m_pEnemyCnt->Uninit();
		m_pEnemyCnt = nullptr;
	}
	for (int nCnt = 0; nCnt < MAX_TOTALSCORE; nCnt++)
	{
		if (m_pTotalScore[nCnt])
		{
			m_pTotalScore[nCnt]->Uninit();
			m_pTotalScore[nCnt] = nullptr;
		}

	}
	for (int nCnt = 0; nCnt < MAX_TOTALSCORECNT; nCnt++)
	{
		if (m_pTotalScoreCnt[nCnt])
		{
			m_pTotalScoreCnt[nCnt]->Uninit();
			m_pTotalScoreCnt[nCnt] = nullptr;
		}

	}
}
//--------------------------------------------
//更新
//--------------------------------------------
void CResult::Update(void)
{
	if (!m_bNextMode)
	{
		if (m_bCanResultAnnouncement)
		{
			ScoreCalculating();
		}
		else
		{
			//if (!m_pSelectButton[GAME_MENU])
			//{
			//	m_pSelectButton[GAME_MENU] = CResultSelectButton::Create({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 80.0f, 0.0f },
			//	{ 200, 40.0f, 0.0f }, CTexture::GameBack);
			//}
			if (!m_pSelectButton[TITLE])
			{
				m_pSelectButton[TITLE] = CResultSelectButton::Create({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f },
				{ 200, 40.0f, 0.0f }, CTexture::TitleBack001);
			}
			for (int nCnt = 0; nCnt < MAX; nCnt++)
			{
				if (m_pSelectButton[nCnt])
				{
					bool bDecision = m_pSelectButton[nCnt]->GetDecision();
					if (bDecision)
					{
						switch (nCnt)
						{
						case CResult::GAME_MENU:
							// 音声を再生
							CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);
							CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENTER, 0.6f);

							m_bNextMode = true;

							// メニューシーンへ行く
							CFade::SetFade(CManager::MODE_MENU);

							break;
						case CResult::TITLE:
							// 音声を再生
							CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);
							CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENTER, 0.6f);

							m_bNextMode = true;

							// メニューシーンへ行く
							CFade::SetFade(CManager::MODE_TITLE);

							break;

						}
					}

				}
			}
		}

	}
}
//--------------------------------------------
//描画
//--------------------------------------------

void CResult::Draw(void)
{

}
//--------------------------------------------
//総合的なスコアを出すクラス
//--------------------------------------------
void CResult::ScoreCalculating()
{
	//クリアタイムの演出
	if(!m_bTotalScorePoped[0])
	{
		int nClearTime = CManager::GetGameTimeSecond();
		int nCntSpeed = nClearTime / CntSpeed;
		if (nCntSpeed <= 0)
		{
			nCntSpeed = 1;
		}
		int nTime = 0;
		if (!m_pTimer)
		{
			m_pTimer = CGametimer::Create({ 300.0f,160.0f,0.0f }, { 1.0,1.0,1.0,1.0 }, false);
		}
		nTime = m_pTimer->GetSecond() + (m_pTimer->GetMinute() * 60);
		if (nTime < nClearTime)
		{
			m_pTimer->AddMinute(nCntSpeed);
		}
		else
		{
			if (!pPolygon[0])
			{
				pPolygon[0] = CPolygon::Create({ 450.0f,160.0f,0.0f }, { 20.0f,30.0f,0.0f }, CTexture::L_BUTTON);
			}
			if (!pPolygon[2])
			{
				pPolygon[2] = CPolygon::Create({ 750.0f,160.0f,0.0f }, { 20.0f,30.0f,0.0f }, CTexture::L_BUTTON);
			}
			//倍率の表示
			if (!m_pTotalScoreCnt[0])
			{
				m_pTotalScoreCnt[0] = CScore::Create({ 550.0f,160.0f,0.0f }, { 20.0f,30.0f,0.0f });
				m_pTotalScoreCnt[0]->SetFitScore(ClearTimeMagnification);
			}

			//クリアタイムの総合点の算出
			int nTotalScore = nClearTime * ClearTimeMagnification;
			int nCntSpeed = nTotalScore / CntSpeed;
			if (!m_pTotalScore[0])
			{
				m_pTotalScore[0] = CScore::Create({ 850.0f,160.0f,0.0f }, { 20.0f,30.0f,0.0f });
			}
			int nScore = m_pTotalScore[0]->GetScore();
			if (nScore < nTotalScore)
			{
				m_pTotalScore[0]->AddScore(nCntSpeed);
			}
			else
			{
				m_bTotalScorePoped[0] = true;
			}
		}
	}
	//倒した敵の数の演出
	if(!m_bTotalScorePoped[1])
	{
		int nEnemyCntKill = CManager::GetKilledEnemyCount();
		int nCntSpeed = nEnemyCntKill / CntSpeed;
		if (nCntSpeed <= 0)
		{
			nCntSpeed = 1;
		}
		if (!m_pEnemyCnt)
		{
			m_pEnemyCnt = CScore::Create({ 200.0f,250.0f,0.0f }, { 20.0f,30.0f,0.0f });
		}
		int nCnt = m_pEnemyCnt->GetScore();
		if (nCnt < nEnemyCntKill)
		{
			m_pEnemyCnt->AddScore(nCntSpeed);
		}
		else
		{
			if (!pPolygon[1])
			{
				pPolygon[1] = CPolygon::Create({ 450.0f,250.0f,0.0f }, { 20.0f,30.0f,0.0f }, CTexture::L_BUTTON);
			}
			if (!pPolygon[3])
			{
				pPolygon[3] = CPolygon::Create({ 750.0f,250.0f,0.0f }, { 20.0f,30.0f,0.0f }, CTexture::L_BUTTON);
			}
			//倍率の表示
			if (!m_pTotalScoreCnt[1])
			{
				m_pTotalScoreCnt[1] = CScore::Create({ 550.0f,250.0f,0.0f }, { 20.0f,30.0f,0.0f });
				m_pTotalScoreCnt[1]->SetFitScore(EnemyCntMagnification);
			}
			int nTotalScore = nEnemyCntKill * EnemyCntMagnification;
			int nCntSpeed = nTotalScore / CntSpeed;
			if (!m_pTotalScore[1])
			{
				m_pTotalScore[1] = CScore::Create({ 850.0f,250.0f,0.0f }, { 20.0f,30.0f,0.0f });
			}
			int nScore = m_pTotalScore[1]->GetScore();
			if (nScore < nTotalScore)
			{
				m_pTotalScore[1]->AddScore(nCntSpeed);
			}
			else if (nScore >= nTotalScore)
			{
				m_pTotalScore[1]->SetFitScore(nTotalScore);
				m_bTotalScorePoped[1] = true;
			}
		}
	}
	//上記二つが出終わったら
	if (m_bTotalScorePoped[0] && m_bTotalScorePoped[1])
	{
		int nTotalScore = m_pTotalScore[0]->GetScore()+ m_pTotalScore[1]->GetScore();
		int nCntSpeed = nTotalScore / CntSpeed;
		if (nCntSpeed < 0)
		{
			nCntSpeed = 1;
		}
		if (!m_pTotalScore[2])
		{
			m_pTotalScore[2] = CScore::Create({ 850.0f,350.0f,0.0f }, { 20.0f,30.0f,0.0f });
		}
		int nCnt = m_pTotalScore[2]->GetScore();
		if (nCnt < nTotalScore)
		{
			m_pTotalScore[2]->AddScore(nCntSpeed);
		}
		else
		{
			m_bTotalScorePoped[2] = true;
		}
	}
	if (m_bTotalScorePoped[2])
	{
		m_bCanResultAnnouncement = false;
	}
}

