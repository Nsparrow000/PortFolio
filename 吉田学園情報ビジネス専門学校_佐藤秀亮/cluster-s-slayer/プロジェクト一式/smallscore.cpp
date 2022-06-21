//---------------------------------------------------
//背景の処理
//---------------------------------------------------
#include"smallscore.h"
#include "manager.h"
#include "Renderer.h"
#include "scene2D.h"
#include "number.h"
#include <math.h>
#include "Score.h"
static const int MaxTime = 20;
//--------------------------
//コンストラクト
//----------------------------
CSmallScore::CSmallScore(OBJTYPE nPriority) :CScene(nPriority)
{
	m_fMove = 0.0f;
	m_bCanAddAlpha = true;
	m_bCanDefAlpha = false;
}

//--------------------------
//デストラクト
//----------------------------
CSmallScore::~CSmallScore()
{

}

//--------------------------
//初期化
//----------------------------
HRESULT CSmallScore::Init()
{
	for (int nCnt = 0; nCnt < m_nMaxTruss; nCnt++)
	{
		//ナンバーの桁数分生成
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(((m_Scale.x * 2)* nCnt), m_pos.y, 0.0f), m_Scale,this,true);
		m_pNumber[nCnt]->SetCol(m_col);

	}
	//スコアの数値を更新し続ける
	SetSmallScore(m_nSmallScore);
	SetTrussSmallScore();

	return S_OK;

}

//--------------------------
//終了
//----------------------------
void CSmallScore::Uninit()
{
	for (int nCnt = 0; nCnt < m_nMaxTruss; nCnt++)
	{
		if (m_pNumber[nCnt])
		{
			m_pNumber[nCnt]->Uninit();
			m_pNumber[nCnt] = nullptr;
		}
	}
	Release();
}

//--------------------------
//更新
//----------------------------
void CSmallScore::Update()
{
	m_pos.y += 1.0f;
	if (m_bCanAddAlpha)
	{
		AddAlpha();
	}
	else
	{
		m_bCanDefAlpha = true;
	}
	if (m_bCanDefAlpha)
	{
		m_nCounter++;
		if (m_nCounter >= MaxTime)
		{
			DefAlpha();

		}
	}
	if (m_bUninit)
	{
		Uninit();
	}

}

//--------------------------
//描画
//----------------------------
void CSmallScore::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス

										  //各パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	pDevice->GetTransform(D3DTS_VIEW, &m_mtxView);//ビューマトリックスを取得


	//ポリゴンをカメラに対して正面に向ける
	//逆行列を求める	
	m_mtxWorld._11 = m_mtxView._11;
	m_mtxWorld._12 = m_mtxView._21;
	m_mtxWorld._13 = m_mtxView._31;
	m_mtxWorld._21 = m_mtxView._12;
	m_mtxWorld._22 = m_mtxView._22;
	m_mtxWorld._23 = m_mtxView._32;
	m_mtxWorld._31 = m_mtxView._13;
	m_mtxWorld._32 = m_mtxView._23;
	m_mtxWorld._33 = m_mtxView._33;

	//位置を設定
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	//位置を反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nMaxTruss; nCnt++)
	{
		if (m_pNumber[nCnt] != NULL)
		{
			m_pNumber[nCnt]->Draw();
		}
	}

}
//---------------------------------------------------------------
//スコアの加算
//---------------------------------------------------------------
void CSmallScore::SetColor(D3DXCOLOR col)
{
	for (int nCnt = 0; nCnt < m_nMaxTruss; nCnt++)
	{
		m_pNumber[nCnt]->SetCol(D3DXCOLOR(col));
	}
}
//---------------------------------------------------------------
//α値減算
//---------------------------------------------------------------
void CSmallScore::AddAlpha()
{
	for (int nCnt = 0; nCnt < m_nMaxTruss; nCnt++)
	{
		D3DXCOLOR col = m_pNumber[nCnt]->GetCol();
		
		if (col.a < 1.0f)
		{
			col.a += 0.1f;
		}
		else
		{
			m_bCanAddAlpha = false;
		}
		//更新した色を設定
		m_pNumber[nCnt]->SetCol(col);
	}
}
//---------------------------------------------------------------
//α値減算
//---------------------------------------------------------------
void CSmallScore::DefAlpha()
{
	for (int nCnt = 0; nCnt < m_nMaxTruss; nCnt++)
	{
		D3DXCOLOR col = m_pNumber[nCnt]->GetCol();

		col.a -= 0.1f;

		if(col.a < 0.0f)
		{
			m_bUninit = true;
		}
		//更新した色を設定
		m_pNumber[nCnt]->SetCol(col);
	}

}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CSmallScore *CSmallScore::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const int& nSmallScore)
{
	//インスタンス生成
	CSmallScore *pSmallScore = new CSmallScore(OBJTYPE_UI);
	if (pSmallScore != NULL)
	{
		pSmallScore->m_pos = pos;
		pSmallScore->m_col = col;
		pSmallScore->m_Scale = scale;
		pSmallScore->m_nSmallScore = nSmallScore;
		//桁数を数える
		pSmallScore->SetTrussSmallScore();

		pSmallScore->Init();
	}

	return pSmallScore;
}
//---------------------------------------------------------------
//スコアの設定
//---------------------------------------------------------------

void CSmallScore::SetSmallScore(int nSmallScore)
{
	int nPoly[MAXSMALLSCORE];

	for (int nCnt = 0; nCnt < m_nMaxTruss; nCnt++)
	{
		if (m_pNumber[nCnt] != NULL)
		{
			//10の累乗の計算
			int Number = (int)pow(10.0, (m_nMaxTruss - (nCnt)));
			int Number2 = (int)pow(10.0, (m_nMaxTruss - (nCnt + 1)));
			if (Number2 <= 0)
			{
				Number2 = 1;
			}
			nPoly[nCnt] = m_nSmallScore % Number / Number2;
			m_pNumber[nCnt]->SetNumber(nPoly[nCnt]);
		}
	}
}
//---------------------------------------------------------------
//スコアの加算
//---------------------------------------------------------------
void CSmallScore::AddSmallScore(int nValue)
{
	m_nSmallScore += nValue;
}

//---------------------------------------------------------------
//桁を計算
//---------------------------------------------------------------
void CSmallScore::SetTrussSmallScore()
{
	int nNumber = m_nSmallScore;
	m_nMaxTruss = 0;
	while (nNumber != 0)
	{
		nNumber = nNumber / 10;
		m_nMaxTruss++;
	}
	if (m_nMaxTruss > MAXSMALLSCORE)
	{
		m_nMaxTruss = MAXSMALLSCORE;
	}
}