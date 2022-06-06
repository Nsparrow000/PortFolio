//=============================================================================
// レンダリング処理 [renderer.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "main.h"

class CInputKeyboard;
class CCamera;

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }
	CCamera *GetCamera() { return m_pCamera; }

	void DrawFPS();
	void DrawText();
	void DrawTextLeft();


	static HWND GetWnd() { return m_wnd; }

	D3DXVECTOR3 GetMousePos() { return GetMouse(); }

private:
	LPDIRECT3D9				m_pD3D = NULL;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
	LPD3DXFONT				m_pFont = NULL;			// フォントへのポインタ
	static CCamera *m_pCamera;

	static HWND m_wnd;
};

#endif