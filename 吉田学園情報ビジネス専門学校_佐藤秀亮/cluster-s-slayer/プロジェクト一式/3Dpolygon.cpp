//=============================================================================
//
// Polygon処理
//
//=============================================================================
#include "3Dpolygon.h"
#include "manager.h"
#include "Renderer.h"
#include "scene3D.h"
#include "model.h"
//=============================================================================
//コンストラクタ
//=============================================================================
C3DPolygon::C3DPolygon()
{
	m_bDraw = true;
	m_bBillboard = false;
	m_bCanCulling = false;
}

//=============================================================================
//デストラクタ
//=============================================================================
C3DPolygon::~C3DPolygon()
{

}

//=============================================================================
//初期化
//=============================================================================
HRESULT C3DPolygon::Init()
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
	pVtx[0].pos = D3DXVECTOR3(-m_Scale.x, m_Scale.y, m_Scale.z);
	pVtx[1].pos = D3DXVECTOR3(m_Scale.x, m_Scale.y, m_Scale.z);
	pVtx[2].pos = D3DXVECTOR3(-m_Scale.x, -m_Scale.y, -m_Scale.z);
	pVtx[3].pos = D3DXVECTOR3(m_Scale.x, -m_Scale.y, -m_Scale.z);

	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2(1.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 1.0);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	m_pVtxBuff->Unlock();


	return S_OK;

}

//=============================================================================
//終了
//=============================================================================
void C3DPolygon::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//更新
//=============================================================================
void C3DPolygon::Update()
{
}

//=============================================================================
//描画
//=============================================================================
void C3DPolygon::Draw(D3DXMATRIX ParentMtx)
{
	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス
	D3DXMATRIX mtxParent = ParentMtx;//親のマトリックス
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//ライト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//上から書き込むことをしないようにする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	if (m_bCanCulling == true)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングをする
	}
	////向きを反映
	if (!m_bBillboard)
	{
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}
	else
	{

	}
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ペアレント設定が有効ならペアレントする
	if (m_bCanParent)
	{
		if (m_pParentModel)
		{
			mtxParent = m_pParentModel->GetMatrix();
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
	//元に戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//カリングする判定なら
	if (m_bCanCulling == true)
	{
		// カリングをする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

void C3DPolygon::Draw()
{
	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//ライト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//上から書き込むことをしないようにする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	if (m_bCanCulling == true)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングをする
	}
	////向きを反映
	if (!m_bBillboard)
	{
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}
	else
	{
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

	}
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

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
	//元に戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//カリングする判定なら
	if (m_bCanCulling == true)
	{
		// カリングをする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

}

//=============================================================================
//クリエイト
//=============================================================================
C3DPolygon *C3DPolygon::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale,
	const int& nTexture, const D3DXCOLOR& col, const bool& bBill)
{
	//インスタンス生成
	C3DPolygon *pPolygon = new C3DPolygon;

	pPolygon->m_pos = pos;
	pPolygon->m_Scale = scale;
	pPolygon->m_rot = rot;
	pPolygon->m_pTexture = CManager::GetTexture()->GetTexture((CTexture::Type)nTexture);
	pPolygon->m_col = col;
	pPolygon->m_bBillboard = bBill;
	if (pPolygon != NULL)
	{
		pPolygon->Init();
	}

	return pPolygon;
}

//=============================================================================
//カラー変更
//=============================================================================
void C3DPolygon::SetCol(D3DXCOLOR col)
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