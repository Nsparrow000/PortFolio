//--------------------------------------
//２Dポリゴンの縁取りの処理
//--------------------------------------
#include "frame.h"
#include "Polygon.h"
#include "input.h"
#include "keyboard.h"
#include "manager.h"
#include "mouse.h"
//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CFrame::CFrame(OBJTYPE nPriority) :CScene(nPriority)
{
	pPolygonFrame = nullptr;
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CFrame::~CFrame()
{
}
//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CFrame::Init(void)
{
	
	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CFrame::Uninit(void)
{
	if (pPolygonFrame)
	{
		pPolygonFrame->Uninit();
		pPolygonFrame = nullptr;
	}

	Release();
}
//--------------------------------------------
//更新
//--------------------------------------------
void CFrame::Update(void)
{
	//終了判定がオンなら
	if (m_bEnd)
	{
		//終了処理を呼ぶ
		Uninit();
	}
}

//--------------------------------------------
//描画
//--------------------------------------------
void CFrame::Draw()
{
}
//-----------------------------------------------
//インスタンス生成
//---------------------------------------------
CFrame *CFrame::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col)
{
	CFrame *pFrame = new CFrame(OBJTYPE_FRAME);
	if (pFrame)
	{
		if (!pFrame->pPolygonFrame)
		{
			//枠線用の画像生成
			pFrame->pPolygonFrame = CPolygon::Create(pos, {size.x + 10.0f,size.y + 10.0f,0.0f}, CTexture::None, col, OBJTYPE_FRAME);
		}
		pFrame->Init();
	}

	return pFrame;
}
