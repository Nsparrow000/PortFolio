//---------------------------
// Author:佐藤秀亮
// 平面(plane.cpp)
//---------------------------
#include "plane.h"
#include "manager.h"
#include "renderer.h"

CPlane::CPlane(int nPriority) : CScene3D::CScene3D(OBJTYPE::OBJTYPE_EFFECT)
{

}

CPlane::~CPlane()
{

}

//初期化処理
HRESULT CPlane::Init(const int nPolygonType, int nTex)
{
	CScene3D::Init(nPolygonType, (CTexture::Type)nTex);
	return S_OK;
}

//終了処理
void CPlane::Uninit()
{
	CScene3D::Uninit();
}

//更新処理
void CPlane::Update()
{

}

//描画処理
void CPlane::Draw()
{
#if 0
	//LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	//D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	//pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	//													//ラインティングを無視する
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	////透明な部分を描画しないようにする
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	////ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&m_mtxWorld);

	////向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	////位置を反映
	//D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	////ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	////頂点バッファをデータストリームに設定
	//pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	////頂点フォーマット
	//pDevice->SetFVF(FVF_VERTEX_3D);
	//if (m_nTexType != -1)
	//{
	//	pDevice->SetTexture(0, m_pTexture[m_nTexType]);    //テクスチャの設定
	//}
	//else
	//{
	//	pDevice->SetTexture(0, NULL);    //テクスチャの設定
	//}
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
	//	0,  //開始する始点のインデックス
	//	2); //描画するプリミティブ数

	////ラインティングを有効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	////デフォルトに戻す
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
#endif

	CScene3D::Draw();
}

//作成
CPlane *CPlane::Create(const int nPolygonType, int nTex)
{
	CPlane *pPlane;
	pPlane = new CPlane(OBJTYPE::OBJTYPE_EFFECT);
	if (pPlane != NULL)
	{
		pPlane->Init(nPolygonType, nTex);
	}
	return pPlane;
}

//旧エフェクト変更処理
#if 0
void CPlane::SetSize(D3DXVECTOR3 size)
{
	//m_size = size;
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, size.z);
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, size.z);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, -size.z);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, -size.z);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CPlane::ChangeColor(D3DXCOLOR col)
{
	VERTEX_3D *pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//エフェクトカラー変更
//=============================================================================
void CPlane::ColorChange(D3DCOLORVALUE color)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ

				   //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の色
	pVtx[0].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);
	pVtx[1].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);
	pVtx[2].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);
	pVtx[3].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//エフェクトサイズ変更
//=============================================================================
void CPlane::ChangeSize(D3DXVECTOR3 size)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ

				   //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, size.z);
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, size.z);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, -size.z);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, -size.z);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//平面エフェクト（回転）
//=============================================================================
void CPlane::SetPosField(D3DXVECTOR3 pos, D3DXVECTOR3 Size, float Rotate, float Rotate2)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x + (cosf(Rotate2)) * Size.x, Size.y, pos.z + (sinf(Rotate))* Size.x);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (sinf(Rotate))  * Size.x, Size.y, pos.z - (cosf(Rotate2))* Size.x);
	pVtx[2].pos = D3DXVECTOR3(pos.x - (sinf(Rotate))  * Size.x, Size.y, pos.z + (cosf(Rotate2))* Size.x);
	pVtx[3].pos = D3DXVECTOR3(pos.x - (cosf(Rotate2)) * Size.x, Size.y, pos.z - (sinf(Rotate))* Size.x);


	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//=============================================================================
//ビルボードサイズ変更
//=============================================================================
void CPlane::BillboardSize(float size)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ

				   //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size, size, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size, size, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size, -size, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size, -size, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
#endif