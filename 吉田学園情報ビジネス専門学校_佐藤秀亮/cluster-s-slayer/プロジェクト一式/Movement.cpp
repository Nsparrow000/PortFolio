//*****************************************************************************
// (移動する)エフェクト処理 [Movement.cpp]
// Author : 佐藤秀亮
//*****************************************************************************

#include "Movement.h"
#include "scene2D.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CMovement::CMovement(OBJTYPE nPriority) : CEffect2d(nPriority)
{
	m_move = {};
	m_Addpos = {};
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CMovement::~CMovement()
{

}

//*****************************************************************************
//	初期化
//*****************************************************************************
HRESULT CMovement::Init(D3DXVECTOR3 pos,
	D3DXVECTOR3 move, 
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	D3DXVECTOR2 Size,
	D3DXVECTOR2 MinSize,
	int nLife, int nType,
	D3DXVECTOR3 AddMovement,
	int Synthetic)
{
	CEffect2d::Init(pos, color, Mincolor, Size, MinSize, nLife, nType, Synthetic);

	m_Addpos = AddMovement;
	m_move = move;
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CMovement::Uninit()
{
	CEffect2d::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CMovement::Update()
{
	D3DXVECTOR3 pos = GetPos();

	m_move += m_Addpos;	//移動値に慣性を加算

	pos += D3DXVECTOR3(m_move.x, m_move.y, 0.0f);

	m_Size += m_MinSize;

	//適応
	CScene2D::SetPos(pos, D3DXVECTOR3(m_Size.x, m_Size.y, 0.0f));
	CEffect2d::Update();
}

//*****************************************************************************
//描画
//*****************************************************************************
void CMovement::Draw()
{
	CEffect2d::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CMovement *CMovement::Create(D3DXVECTOR3 pos,
	D3DXVECTOR3 move,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	D3DXVECTOR2 Size,
	D3DXVECTOR2 MinSize,
	int nLife, int nType,
	D3DXVECTOR3 AddMovement,
	int Synthetic,
	const int& nPriorty)
{
	CMovement *pMovement = NULL;
	pMovement = new CMovement((CScene::OBJTYPE)nPriorty);		//メモリ確保

	//NULLチェック
	if (pMovement != NULL)
	{
		pMovement->Init(pos, move, color, Mincolor, Size, MinSize, nLife, nType, AddMovement, Synthetic);
	}

	return pMovement;
}
