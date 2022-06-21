//----------------------------------
//ナンバーの処理
//-----------------------------------
#include "number.h"
#include "main.h"
#include "scene3D.h"
#include "Renderer.h"
#include "manager.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CNumber::CNumber()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.0f,0.0f,0.0f };
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_rot = { 0.0f,0.0f,0.0f };
	m_bCanParent = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CNumber *CNumber::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale,CSmallScore *pParent, const bool& bCanParent)
{
	//インスタンス生成
	CNumber *pNumber = new CNumber;
	if (pNumber != NULL)
	{
		pNumber->m_pos = pos;
		pNumber->m_Scale = scale;
		pNumber->m_pParent = pParent;
		pNumber->m_bCanParent = bCanParent;
		//テクスチャ画像の設定
		pNumber->m_pTexture = CManager::GetTexture()->GetTexture(CTexture::TIME);

		pNumber->Init();

	}

	return pNumber;
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CNumber::Init(void)
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

																	 // 頂点情報を設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);



	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(-m_Scale.x, m_Scale.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Scale.x, m_Scale.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_Scale.x, -m_Scale.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Scale.x, -m_Scale.y, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2(1.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 1.0);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_pVtxBuff->Unlock();


	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CNumber::Update(void)
{

	if (m_bUninit == true)
	{
		Uninit();
	}
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス
	D3DXMATRIX mtxParent;//親のマトリックス

								//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//ライト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//上から書き込むことをしないようにする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	////向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ペアレント設定が有効ならペアレントする
	if (m_bCanParent)
	{
		if (m_pParent)
		{
			mtxParent = m_pParent->GetMatrix();
		}
		else
		{
			//最新のマトリックスを取得
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}
		//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&mtxParent);

	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをでバスのでーたすとりーむに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	pDevice->SetFVF(FVF_VERTEX_3D);//頂点フォーマットの設定

	pDevice->SetTexture(0, m_pTexture);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
	//αテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//元に戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}
//-----------------------------------
//番号をセット
//-----------------------------------
void CNumber::SetNumber(int nNumber)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の更新
	pVtx[0].tex = D3DXVECTOR2(0.1*nNumber, 0.0);
	pVtx[1].tex = D3DXVECTOR2(0.1 + 0.1*nNumber, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.1*nNumber, 1.0);
	pVtx[3].tex = D3DXVECTOR2(0.1 + 0.1*nNumber, 1.0);

	m_pVtxBuff->Unlock();

}


//=============================================================================
//カラー変更
//=============================================================================
void CNumber::SetCol(D3DXCOLOR col)
{
	m_col = col;
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	m_pVtxBuff->Unlock();

}