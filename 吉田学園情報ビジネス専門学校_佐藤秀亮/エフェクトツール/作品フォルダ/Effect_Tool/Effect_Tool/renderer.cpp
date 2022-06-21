//=============================================================================
// レンダリング処理 [renderer.h]
// Author : 佐藤秀亮
//=============================================================================
#include "renderer.h"
#include "scene2d.h"
#include "manager.h"
#include "camera.h"

#include "control.h"
#include "seteffect.h"

#include <assert.h>

//静的メンバ変数
CCamera *CRenderer::m_pCamera = NULL;
HWND CRenderer::m_wnd = NULL;

CRenderer::CRenderer()
{

}

CRenderer::~CRenderer()
{

}

HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

																// デバイスの生成
																// ディスプレイアダプタを表すためのデバイスを作成
																// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	m_wnd = hWnd;

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングを行わない
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

																			// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

																			// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 20, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);		//文字の大きさ

	m_pCamera = CCamera::Create(D3DXVECTOR3(0.0f, 200.0f, -300.0f), 700.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	

	return S_OK;
}

void CRenderer::Uninit()
{
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
	}

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

}

void CRenderer::Update()
{
	m_pCamera->Update();

	GetMouse();
	CScene::UpdateAll();
}

void CRenderer::Draw()
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(100, 100, 100, 0), 1.0f, 0); //背景の色を変えれる

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// ポリゴンの描画処理
		CScene::DrawAll();

		// FPS表示
		DrawFPS();


		DrawText();
		DrawTextLeft();

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void CRenderer::DrawFPS()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", GetFPS());

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

//*****************************************************************************
//画面テキスト表示
//*****************************************************************************
void CRenderer::DrawText()
{
	CManager::MODE mode = CManager::GetMode();

	RECT rect = { 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[1024];

	bool bMouse = CControl::GetMouseCursor();
	bool bTap = CControl::GetTap();

	int nNum = sprintf(&str[0], "選択カラー [←][→]：%d\n", CControl::GetSerectColor());

	nNum += sprintf(&str[nNum], "カラー値 [+][-]：(%d %d %d %d)\n", CControl::GetControlCoror(1), CControl::GetControlCoror(2), CControl::GetControlCoror(3), CControl::GetControlCoror(4));

	nNum += sprintf(&str[nNum], "カラー増減値 [+][-]：(%d %d %d %d)\n", CControl::GetChangeCol(1), CControl::GetChangeCol(2), CControl::GetChangeCol(3), CControl::GetChangeCol(4));

	nNum += sprintf(&str[nNum], "寿命 [+][-]：(%d)\n", CControl::GetLife());
	nNum += sprintf(&str[nNum], "\n");
	nNum += sprintf(&str[nNum], "エフェクトパターン [←][→]：(%d)\n", CControl::GetPattern());

	nNum += sprintf(&str[nNum], "\n");

	switch (mode)
	{
		//*****************************************************************************
		//2Dモード
	case(CManager::MODE_2D):

		nNum += sprintf(&str[nNum], "サイズ [+][-]：(%.1f)\n", CControl::GetSize());
		nNum += sprintf(&str[nNum], "サイズ増減 [+][-]：(%.1f)\n", CControl::GetChangeSize());
		nNum += sprintf(&str[nNum], "\n");
		nNum += sprintf(&str[nNum], "密度 [+][-]：(%d)\n", CControl::GetDensity());
		nNum += sprintf(&str[nNum], "テクスチャ [←][→]：(%d)\n", CControl::GetTex());
		nNum += sprintf(&str[nNum], "\n");
		if (bMouse == true)
		{
			nNum += sprintf(&str[nNum], "マウス追従 [M]：(ON)\n");
		}
		else
		{
			nNum += sprintf(&str[nNum], "マウス追従 [M]：(OFF)\n");
		}
		if (bTap == true)
		{
			nNum += sprintf(&str[nNum], "Tapモード [N]：(ON)\n");
		}
		else
		{
			nNum += sprintf(&str[nNum], "Tapモード [N]：(OFF)\n");
		}
		nNum += sprintf(&str[nNum], "\n");
		nNum += sprintf(&str[nNum], "テキスト出力 [　]\n");
		nNum += sprintf(&str[nNum], "中間セーブのロード [　]\n");
		nNum += sprintf(&str[nNum], "\n");


		nNum += sprintf(&str[nNum], "生成座標XY：(%.1f %.1f)\n", CSetEffect::GetSetPos().x, CSetEffect::GetSetPos().y);

		nNum += sprintf(&str[nNum], "\n");

		switch (CControl::GetPattern())
		{
		case(0):
			nNum += sprintf(&str[nNum], "移動値X [+][-] : (%.1f) \n", CControl::GetMove().x);
			nNum += sprintf(&str[nNum], "移動値Y [+][-] : (%.1f) \n", CControl::GetMove().y);
			nNum += sprintf(&str[nNum], "移動加算値 X[+][-]：(%.1f)\n", CControl::GetAddMove().x);
			nNum += sprintf(&str[nNum], "移動加算値 Y[+][-]：(%.1f)\n", CControl::GetAddMove().y);
			break;
		case(1):
			nNum += sprintf(&str[nNum], "移動値 [+][-]：(%.1f)\n", CControl::GetMove().x);
			nNum += sprintf(&str[nNum], "拡散率 [+][-]：(%.2f)\n", (float)CControl::GetDiffusion() / 100);
			nNum += sprintf(&str[nNum], "消える距離 [+][-]：(%d)\n", CControl::GetUninitVectl());
			nNum += sprintf(&str[nNum], "出現距離 [+][-]：(%.1f)\n", CControl::GetDistance());

			nNum += sprintf(&str[nNum], "\n");
			break;
		case(2):
			nNum += sprintf(&str[nNum], "移動値X [+][-] : (%.1f)\n", CControl::GetMove().x);
			nNum += sprintf(&str[nNum], "移動値Y [+][-] : (%.1f)\n", CControl::GetMove().y);

			nNum += sprintf(&str[nNum], "回転 [+][-] ：(%.2f)\n", CControl::GetRotate());
			break;
		default:
			break;
		}
		nNum += sprintf(&str[nNum], "\n");

		nNum += sprintf(&str[nNum], "\n");

		nNum += sprintf(&str[nNum], "3Dモードへ [ENTER]\n");
		nNum += sprintf(&str[nNum], "\n");

		break;

		//*****************************************************************************
		//3Dモード
	case(CManager::MODE_3D):
		nNum += sprintf(&str[nNum], "操作モード [F5]：");

		if (CControl::GetPlayerMode() == true)
		{
			nNum += sprintf(&str[nNum], "プレイヤー操作\n");
			nNum += sprintf(&str[nNum], "移動 WASD\n");
			nNum += sprintf(&str[nNum], "SPACE ジャンプ\n");
			nNum += sprintf(&str[nNum], "左クリック 攻撃(軌跡確認)\n\n");
		}
		else
		{
			nNum += sprintf(&str[nNum], "エフェクト操作\n");
			nNum += sprintf(&str[nNum], "プレイヤー一部更新停止\n");

			nNum += sprintf(&str[nNum], "\n");
			if (CControl::GetPattern() != 0)
			{
				nNum += sprintf(&str[nNum], "サイズ [+][-]：(%.1f)\n", CControl::GetSize());
				nNum += sprintf(&str[nNum], "サイズ増減 [+][-]：(%.1f)\n", CControl::GetChangeSize());
				nNum += sprintf(&str[nNum], "密度 [+][-]：(%d)\n", CControl::GetDensity());

			}
			nNum += sprintf(&str[nNum], "テクスチャ [←][→]：(%d)\n", CControl::GetTex());
			nNum += sprintf(&str[nNum], "\n");


			switch (CControl::GetPattern())
			{
			case(0)://軌跡パターン
				nNum += sprintf(&str[nNum], "左クリック 攻撃(軌跡確認) \n\n");

				nNum += sprintf(&str[nNum], "軌跡先端初期色 [+][-]：(%d %d %d %d)\n", CControl::GetTrajectColor(1), CControl::GetTrajectColor(2), CControl::GetTrajectColor(3), CControl::GetTrajectColor(4));
				nNum += sprintf(&str[nNum], "軌跡先端増減 [+][-]：(%d %d %d %d)\n", CControl::GetTrajectCol(1), CControl::GetTrajectCol(2), CControl::GetTrajectCol(3), CControl::GetTrajectCol(4));

				nNum += sprintf(&str[nNum], "軌跡手元モデル [+][-] ：(%d / 14)\n", CControl::GetModelNum());
				nNum += sprintf(&str[nNum], "軌跡先端モデル [+][-] ：(%d / 14)\n", CControl::GetTrajectModelNum());

				nNum += sprintf(&str[nNum], "\n");
				break;
			case(1)://ストレートパターン
				nNum += sprintf(&str[nNum], "移動値 X[+][-]：%.1f\n", CControl::Getmove3d().x);
				nNum += sprintf(&str[nNum], "移動値 Y[+][-]：%.1f\n", CControl::Getmove3d().y);
				nNum += sprintf(&str[nNum], "移動値 Z[+][-]：%.1f\n", CControl::Getmove3d().z);

				nNum += sprintf(&str[nNum], "出現位置の距離 [+][-]：%.1f\n", CControl::GetDistance());
				nNum += sprintf(&str[nNum], "出現位置パターン [+][-]：%d\n", CControl::GetSecondType());
				nNum += sprintf(&str[nNum], "ランダム移動パターン [+][-]：%d\n", CControl::GetType());
				nNum += sprintf(&str[nNum], "粒発生間隔 [+][-]：%d\n", CControl::GetParticleTime());

				break;
			case(2)://纏わせ
				nNum += sprintf(&str[nNum], "動く値の最大 最小[+][-] ：(%d)\n", CControl::GetRandMove1());
				nNum += sprintf(&str[nNum], "発生モデル1 [+][-] ：(%d / 14)\n", CControl::GetModelNum());
				nNum += sprintf(&str[nNum], "発生モデル2 [+][-] ：(%d / 14)\n", CControl::GetTrajectModelNum());
				break;
			case(3):
				nNum += sprintf(&str[nNum], "回転 [+][-]：%.2f\n", CControl::GetRotate());

				nNum += sprintf(&str[nNum], "粒移動値 [+][-]：%.1f\n", CControl::Getmove3d().x);
				nNum += sprintf(&str[nNum], "粒サイズ [+][-]：%.1f\n", CControl::GetParticleSize());
				nNum += sprintf(&str[nNum], "粒発生最大時間 [+][-]：%d\n", CControl::GetParticleTime());
				nNum += sprintf(&str[nNum], "粒発生地点 [+][-]：%.1f\n", CControl::GetDistance());
				nNum += sprintf(&str[nNum], "\n");
				nNum += sprintf(&str[nNum], "アクティブ時間 [+][-]：%d\n", CControl::GetActiveTime());
				nNum += sprintf(&str[nNum], "粒大きさ変更 [+][-]：%.1f\n", CControl::GetParticleAddSize());
				nNum += sprintf(&str[nNum], "最大の大きさ [+][-]：%.1f\n", CControl::GetMaxSize());
				nNum += sprintf(&str[nNum], "\n");
				nNum += sprintf(&str[nNum], "粒色 [+][-]：(%d %d %d %d)\n", CControl::GetParticleColor(1), CControl::GetParticleColor(2), CControl::GetParticleColor(3), CControl::GetParticleColor(4));
				nNum += sprintf(&str[nNum], "粒色増減 [+][-]：(%d %d %d %d)\n", CControl::GetParticleAddCol(1), CControl::GetParticleAddCol(2), CControl::GetParticleAddCol(3), CControl::GetParticleAddCol(4));
				nNum += sprintf(&str[nNum], "粒合成 [+][-]：(%d)\n", CControl::GetParticleSynthetic());
				nNum += sprintf(&str[nNum], "稼働中の大きさ変更 [+][-]：(%.1f)\n", CControl::GetAvctiveAddSize());

				break;
			case(4):
				nNum += sprintf(&str[nNum], "移動値 X[+][-]：%.1f\n", CControl::Getmove3d().x);
				nNum += sprintf(&str[nNum], "移動値 Y[+][-]：%.1f\n", CControl::Getmove3d().y);
				nNum += sprintf(&str[nNum], "移動値 Z[+][-]：%.1f\n", CControl::Getmove3d().z);

				nNum += sprintf(&str[nNum], "アクティブ時間 [+][-]：%d\n", CControl::GetActiveTime());
				nNum += sprintf(&str[nNum], "最大の大きさ [+][-]：%.1f\n", CControl::GetMaxSize());

				break;
			case(5):
				nNum += sprintf(&str[nNum], "回転 [+][-]：%.2f\n", CControl::GetRotate());
				nNum += sprintf(&str[nNum], "粒発生地点 [+][-]：%.1f\n", CControl::GetDistance());
				nNum += sprintf(&str[nNum], "粒寿命 [+][-]：%d\n", CControl::GetParticleTime());
				nNum += sprintf(&str[nNum], "近づくまでの時間 [+][-]：%d\n", CControl::GetSecondTime());
				nNum += sprintf(&str[nNum], "アクティブになるまでの大きさ [+][-]：%.1f\n", CControl::GetMaxSize());
				nNum += sprintf(&str[nNum], "移動値[+][-]：%.1f\n", CControl::Getmove3d().x);
				nNum += sprintf(&str[nNum], "エフェクトタイプ[+][-]：%d\n", CControl::GetType());
				nNum += sprintf(&str[nNum], "移動タイプ[+][-]：%d\n", CControl::GetSecondType());

				break;
			case(6):
				nNum += sprintf(&str[nNum], "回転 [+][-]：%.2f\n", CControl::GetRotate());
				nNum += sprintf(&str[nNum], "頂点数 [+][-]：%d\n", CControl::GetVtx());
				nNum += sprintf(&str[nNum], "生成タイプ [+][-]：%d\n", CControl::GetType());
				break;
			case(7):
				nNum += sprintf(&str[nNum], "距離 [+][-]：%.1f\n", CControl::GetDistance());
				nNum += sprintf(&str[nNum], "高さ [+][-]：%.1f\n", CControl::GetHigth());
				nNum += sprintf(&str[nNum], "下部分幅 [+][-]：%.1f\n", CControl::GetParticleSize());
				break;
			case(8):
				nNum += sprintf(&str[nNum], "制御点 X[+][-]：%.1f\n", CControl::GetContorolBezierX());
				nNum += sprintf(&str[nNum], "制御点 Y[+][-]：%.1f\n", CControl::GetContorolBezierY());
				nNum += sprintf(&str[nNum], "制御点 Z[+][-]：%.1f\n", CControl::GetContorolBezierZ());
				nNum += sprintf(&str[nNum], "移動(通過点の数) [+][-]：%.0f\n", CControl::Getmove3d().x);

				nNum += sprintf(&str[nNum], "軌跡１色 [+][-]：(%d %d %d %d)\n", CControl::GetParticleColor(1), CControl::GetParticleColor(2), CControl::GetParticleColor(3), CControl::GetParticleColor(4));
				nNum += sprintf(&str[nNum], "軌跡１増減 [+][-]：(%d %d %d %d)\n", CControl::GetParticleAddCol(1), CControl::GetParticleAddCol(2), CControl::GetParticleAddCol(3), CControl::GetParticleAddCol(4));

				nNum += sprintf(&str[nNum], "軌跡２色 [+][-]：(%d %d %d %d)\n", CControl::GetTrajectColor(1), CControl::GetTrajectColor(2), CControl::GetTrajectColor(3), CControl::GetTrajectColor(4));
				nNum += sprintf(&str[nNum], "軌跡２増減 [+][-]：(%d %d %d %d)\n", CControl::GetTrajectCol(1), CControl::GetTrajectCol(2), CControl::GetTrajectCol(3), CControl::GetTrajectCol(4));
				nNum += sprintf(&str[nNum], "軌跡サイズ [+][-]：%.1f\n", CControl::GetMaxSize());
				nNum += sprintf(&str[nNum], "軌跡テクスチャ [+][-]：%d\n", CControl::GetSecondTex());
				nNum += sprintf(&str[nNum], "軌跡寿命 [+][-]：%d\n", CControl::GetParticleTime());
				nNum += sprintf(&str[nNum], "ターゲットランダム距離 [+][-]：%.1f\n", CControl::GetDistance());
				nNum += sprintf(&str[nNum], "軌跡合成 [+][-]：%d\n", CControl::GetParticleSynthetic());
				break;
			case(9):
				break;
			default:
				break;
			}
			nNum += sprintf(&str[nNum], "\n");

			nNum += sprintf(&str[nNum], "テキスト出力 [　]\n");
			nNum += sprintf(&str[nNum], "中間ロード [　]\n");
			nNum += sprintf(&str[nNum], "プリセットロード [　]\n");

		}
		nNum += sprintf(&str[nNum], "2Dモードへ [ENTER]\n");
		break;

	default:
		assert(false);
		break;
	}
	nNum += sprintf(&str[nNum], "合成 [←][→]：(%d)\n", CControl::GetSynthetic());

	nNum += sprintf(&str[nNum], "%.0f %.0f\n", GetMousePos().x, GetMousePos().y);

	// テキストの描画
	m_pFont->DrawText(
		NULL,
		&str[0],
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, 255));

}

//*****************************************************************************
//画面テキスト表示左側
//*****************************************************************************
void CRenderer::DrawTextLeft()
{

	RECT rect = { 900, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[1024];

	int nNum = sprintf(&str[0], "\n");

	if (CManager::MODE mode = CManager::GetMode())
	{
		nNum += sprintf(&str[nNum], "テクスチャ移動 U [+][-]：%.3f\n", CControl::GetTexMoveU());
		nNum += sprintf(&str[nNum], "テクスチャ移動 V [+][-]：%.3f\n", CControl::GetTexMoveV());
		nNum += sprintf(&str[nNum], "テクスチャ枚数 U [+][-]：%.1f\n", CControl::GetTexNum().x);
		nNum += sprintf(&str[nNum], "テクスチャ枚数 V [+][-]：%.1f\n", CControl::GetTexNum().y);

		nNum += sprintf(&str[nNum], "テクスチャ分割数 U [+][-]：%.0f\n", CControl::GetSplitU());
		nNum += sprintf(&str[nNum], "テクスチャ分割数 V [+][-]：%.0f\n", CControl::GetSplitV());
		nNum += sprintf(&str[nNum], "パターンカウント  [+][-]：%d\n", CControl::GetAnimCont());
		nNum += sprintf(&str[nNum], "ランダム化  [+][-]：%d\n", CControl::GetAnimPatternType());


		nNum += sprintf(&str[nNum], "プリセット再生[F1]\n");
		nNum += sprintf(&str[nNum], "全てのプリセット再生[F2]\n");
		nNum += sprintf(&str[nNum], "時間差付きでプリセット再生[F3]\n");

		switch (CControl::GetPattern())
		{

		case(1):
			break;
		case(2):
			break;
		case(3):
		case(4):
			nNum += sprintf(&str[nNum], "再生[SPACE]\n");

			break;
		default:
			break;
		}
	}

	// テキストの描画
	m_pFont->DrawText(
		NULL,
		&str[0],
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, 255));

}
