#include "main.h"
#include "manager.h"
#include "keyboard.h"
#include "scene.h"
#include "Scene2D.h"
#include "score.h"
#include "texture.h"
#include "2Dnumber.h"
CScore::CScore(OBJTYPE nPriority) : CScene(nPriority)
{
	m_nMaxTruss = 0;
}

CScore::~CScore()
{

}

CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScore *pTime = NULL;
	pTime = new CScore;
	pTime->m_pos = pos;
	pTime->m_size = size;
	pTime->Init();
	return pTime;
}

HRESULT CScore::Init(D3DXVECTOR3 pos, float fsize)
{

	m_nScore = 300;
	return S_OK;
}

HRESULT CScore::Init(void)
{
	//桁数の更新
	SetTrussSmallScore();

	//現在のスコアをポリゴンに反映
	SetScore();

	return S_OK;
}

void CScore::Uninit()
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}
}

void CScore::Update()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{
		AddScore(10);
	}
	//現在のスコアをポリゴンに反映
	SetScore();
	//桁数の更新
	SetTrussSmallScore();
}

void CScore::Draw()
{
	for (int nCntTime = 0; nCntTime < m_nMaxTruss; nCntTime++)
	{
		if (m_apNumber[nCntTime])
		{
			m_apNumber[nCntTime]->Draw();
		}
	}
}

void CScore::SetScore()
{
	int nPoly[MAX_SCORE];

	for (int nCnt = 0; nCnt < m_nMaxTruss; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{
			//10の累乗の計算
			int Number = (int)pow(10.0, (m_nMaxTruss - (nCnt)));
			int Number2 = (int)pow(10.0, (m_nMaxTruss - (nCnt + 1)));
			if (Number2 <= 0)
			{
				Number2 = 1;
			}
			nPoly[nCnt] = m_nScore % Number / Number2;
			m_apNumber[nCnt]->SetNumber(nPoly[nCnt]);
		}
	}

}

void CScore::CreateNumber(const int & nTruss)
{
	if (nTruss != m_nMaxTruss)
	{
		for (int nCntTime = 0; nCntTime < m_nMaxTruss; nCntTime++)
		{
			if (m_apNumber[nCntTime])
			{
				m_apNumber[nCntTime]->Uninit();
				m_apNumber[nCntTime] = nullptr;
			}
		}
		for (int nCnt = 0; nCnt < nTruss; nCnt++)
		{
			if (!m_apNumber[nCnt])
			{
				m_apNumber[nCnt] = C2DNumber::Create(D3DXVECTOR3(m_pos.x + (nCnt * (m_size.x * 2.0f)) , m_pos.y, m_pos.z), m_size);
				m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		//桁数を保存
		m_nMaxTruss = nTruss;

	}


}

void CScore::SetTrussSmallScore()
{
	int nCrateTruss = 0;
	int nNumber = m_nScore;

	while (nNumber != 0)
	{
		nNumber = nNumber / 10;
		nCrateTruss++;
	}
	if (nCrateTruss > MAX_SCORE)
	{
		nCrateTruss = MAX_SCORE;
	}
	if (nCrateTruss <= 0)
	{
		nCrateTruss = 1;
	}
	CreateNumber(nCrateTruss);

}

