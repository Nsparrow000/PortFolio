//--------------------------------------
//ファイル選択表示用の処理
//--------------------------------------
#include "titleselectbutton.h"
#include "Polygon.h"
#include "input.h"
#include "keyboard.h"
#include "manager.h"
#include "mouse.h"
#include "frame.h"
#include "skillselect.h"
#include "player.h"
static const float IconSize = 50.0f;//アイコンの大きさ
static const D3DXVECTOR3 IconPos = { 10.0f,-50.0f,0.0f };//アイコンの位置

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CTitleSelectButton::CTitleSelectButton(OBJTYPE nPriority) :CScene(nPriority)
{
	pSelectBottom = nullptr;
	m_bPopSelectBottom = true;
	m_bIsSelect = false;
	pFrame = nullptr;
	m_bCanAlphaDown = false;
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CTitleSelectButton::~CTitleSelectButton()
{
}
//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CTitleSelectButton::Init(void)
{
	return S_OK;
}
//--------------------------------------------
//終了
//--------------------------------------------
void CTitleSelectButton::Uninit(void)
{
	if (pSelectBottom)
	{
		pSelectBottom->Uninit();
		pSelectBottom = nullptr;
	}
	if (pFrame)
	{
		pFrame->Uninit();
		pFrame = nullptr;
	}
	Release();
}
//--------------------------------------------
//更新
//--------------------------------------------
void CTitleSelectButton::Update(void)
{
	if (m_bPopSelectBottom)
	{
		//セレクトウィンドウの出現
		PopSelectBottom();
	}
	else
	{
		SelectBottom();
	}
	if (m_bCanAlphaDown)
	{
		AlphaDowntBottom();
	}

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
void CTitleSelectButton::Draw()
{

}
//-----------------------------------------------
//インスタンス生成
//---------------------------------------------
CTitleSelectButton *CTitleSelectButton::Create(const D3DXVECTOR3& pos,
	const D3DXVECTOR3& size, const int& tex, const bool& bAlphaDown)
{
	CTitleSelectButton *pSkillSelect = new CTitleSelectButton(OBJTYPE_UI);
	if (pSkillSelect)
	{
		if (!pSkillSelect->pSelectBottom)
		{
			//スキル選択画面画像の生成
			pSkillSelect->pSelectBottom = CPolygon::Create(pos, size, (CTexture::Type)tex, { 1.0,1.0,1.0,1.0f });
		}
		pSkillSelect->m_bCanAlphaDown = bAlphaDown;
		if (pSkillSelect->m_bCanAlphaDown)
		{
			pSkillSelect->m_bPopSelectBottom = false;
		}
		pSkillSelect->Init();
	}

	return pSkillSelect;
}
//-----------------------------------------------
//セレクトウィンドウの出現
//---------------------------------------------
void CTitleSelectButton::PopSelectBottom()
{

	if (pSelectBottom)
	{
		D3DXCOLOR col = pSelectBottom->GetColor();
		D3DXVECTOR3 pos = pSelectBottom->GetPos();
		//透明から不透明になる処理
		if (col.a <= 1.0f)
		{
			col.a += 0.05f;
			pSelectBottom->SetCol(col);
		}
		else
		{
			m_bPopSelectBottom = false;
		}
	}
}
//-----------------------------------------------
//ボタンのカラー値を下げる
//---------------------------------------------
void CTitleSelectButton::AlphaDowntBottom()
{

	if (pSelectBottom)
	{
		D3DXCOLOR col = pSelectBottom->GetColor();
		D3DXVECTOR3 pos = pSelectBottom->GetPos();
		//不透明から半透明になる処理
		if (col.r >= 0.25f)
		{
			col.r -= 0.05f;
			col.g -= 0.05f;
			col.b -= 0.05f;
			pSelectBottom->SetCol(col);
		}
		else
		{
			m_bCanAlphaDown = false;
		}
	}

}
//-----------------------------------------------
//スキル選択ボタンの選択処理
//---------------------------------------------
void CTitleSelectButton::SelectBottom()
{
	m_bIsDecision = false;

	//マウス情報の取得
	CMouse *pMouse = CManager::GetMouse();

	POINT po;
	//マウスのスクリーン座標の位置の取得
	GetCursorPos(&po);
	D3DXVECTOR3 pos = pSelectBottom->GetPos();
	D3DXVECTOR3 scale = pSelectBottom->GetScale();
	HWND hWnd = GetForegroundWindow();

	//スクリーン座標からクライアント座標の変換
	ScreenToClient(hWnd, &po);

	float MousePos_X = (float)po.x;
	float MousePos_Y = (float)po.y;
	//配置してあるポリゴンの範囲にカーソルが入ったら
	if (MousePos_X > pos.x - scale.x&&
		MousePos_X < pos.x + scale.x&&
		MousePos_Y > pos.y - scale.y&&
		MousePos_Y < pos.y + scale.y)
	{
		//選択状態にする
		m_bIsSelect = true;
		//選択されたポリゴンに枠を表示
		if (!pFrame)
		{
			pFrame = CFrame::Create(pos, scale, { 1.0,1.0,0.0,1.0 });
		}
		//選択している状態で左クリックをすると決定状態にする
		if (pMouse->GetTrigger(CMouse::MOUSE_LEFT) == true)
		{
			m_bIsDecision = true;
		}
	}
	else
	{
		//選択状態を解除
		m_bIsSelect = false;

		if (pFrame)
		{
			pFrame->Uninit();
			pFrame = nullptr;
		}
	}
}