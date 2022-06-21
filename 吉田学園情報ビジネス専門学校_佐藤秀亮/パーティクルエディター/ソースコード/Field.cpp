//ーーーーーーーーーーーーーーーーーーーーーーーーー

#include "main.h"
#include "input.h"
#include "Field.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureField[FIELDTYPE_MAX] = {};//テクスチャへのポインタ
Field g_Field[MAX_FIELD];

//初期化処理
HRESULT InitField(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
	pDevice = GetDevice();//デバイスの取得
	
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_Field[nCntField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Field[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Field[nCntField].pVtxBuffField = NULL;
		g_Field[nCntField].pIdxBuffField = NULL;
		g_Field[nCntField].fWidth = 0.0f;
		g_Field[nCntField].fDepht = 0.0f;
		g_Field[nCntField].ntype = 0;
		g_Field[nCntField].bUse = false;

	}
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field002.jpg", &g_pTextureField[FIELDTYPE_NOMAL]);

	return S_OK;
}

//終了処理
void UninitField(void)//追加の可能性があるため項目は作っとく
{
	for (int nCntTex = 0; nCntTex < FIELDTYPE_MAX; nCntTex++)
	{
		//テクスチャ破棄
		if (g_pTextureField[nCntTex] != NULL)
		{
			g_pTextureField[nCntTex]->Release();
			g_pTextureField[nCntTex] = NULL;
		}
	}

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		//頂点バッファ破棄
		if (g_Field[nCntField].pVtxBuffField != NULL)
		{
			g_Field[nCntField].pVtxBuffField->Release();
			g_Field[nCntField].pVtxBuffField = NULL;
		}
		//インデックス破棄
		if (g_Field[nCntField].pIdxBuffField != NULL)
		{
			g_Field[nCntField].pIdxBuffField->Release();
			g_Field[nCntField].pIdxBuffField = NULL;
		}
	}
}

//更新処理
void UpdateField(void)//追加の可能性があるため項目は作っとく
{

}

//描画処理
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		
		if (g_Field[nCntField].bUse == true)
		{

			D3DXMatrixIdentity(&g_Field[nCntField].mtxwold);
			//スケール
			g_Field[nCntField].mtxwold._11 = g_Field[nCntField].fWidth;

			g_Field[nCntField].mtxwold._33 = g_Field[nCntField].fDepht;

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field[nCntField].rot.y, g_Field[nCntField].rot.x, g_Field[nCntField].rot.z);
			D3DXMatrixMultiply(&g_Field[nCntField].mtxwold, &g_Field[nCntField].mtxwold, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Field[nCntField].pos.x, g_Field[nCntField].pos.y, g_Field[nCntField].pos.z);
			D3DXMatrixMultiply(&g_Field[nCntField].mtxwold, &g_Field[nCntField].mtxwold, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Field[nCntField].mtxwold);

			//バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_Field[nCntField].pVtxBuffField, 0, sizeof(VERTEX_3D));
			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_Field[nCntField].pIdxBuffField);

			pDevice->SetFVF(FVF_VERTEX_3D);//頂点フォーマットの設定

			pDevice->SetTexture(0, g_pTextureField[g_Field[nCntField].ntype]);//テクスチャの設定

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
										  0,								//インデックス バッファの開始地点から最初のインデックスまでのオフセット
										  nCntField * FLOO_VTX_NUM_MAX,		//描画で使用される最小の頂点番号
										  FLOO_IDX_NUM_MAX,					//頂点の数
										  nCntField * FLOO_IDX_NUM_MAX,		//インデックス配列の読み取りを開始する位置
										  FLOO_POLY_NUM_MAX;				//△の数
		}
	}
}

void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepht, int ntype)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_Field[nCntField].bUse == false)
		{
			g_Field[nCntField].pos = pos;
			g_Field[nCntField].rot = rot;
			g_Field[nCntField].fWidth = fWidth;
			g_Field[nCntField].fDepht = fDepht;
			g_Field[nCntField].ntype = ntype;

			//頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * FLOO_VTX_NUM_MAX * MAX_FIELD, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_Field[nCntField].pVtxBuffField, NULL);
			//インデックスバッファ作成
			pDevice->CreateIndexBuffer(sizeof(WORD) * FLOO_IDX_NUM_MAX * MAX_FIELD, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_Field[nCntField].pIdxBuffField, NULL);

			//頂点情報
			VERTEX_3D*pVtx;//頂点情報へのポインタ
			//頂点バッファをロックし、頂点データへのポインタを取得
			g_Field[nCntField].pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);
			for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
			{
				int nVtxNum = 0;
				for (int nCntZ = 0; nCntZ < FLOO_MAX_VTX_Z + 1; nCntZ++)
				{
					for (int nCntX = 0; nCntX < FLOO_MAX_VTX_X + 1; nCntX++, nVtxNum++)
					{
						pVtx[nVtxNum].pos = D3DXVECTOR3(-MS_X + (((MS_X * 2) / FLOO_MAX_VTX_X) * nCntX), 0.0f, MS_Z - (((MS_Z * 2) / FLOO_MAX_VTX_Z) * nCntZ));
						pVtx[nVtxNum].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
						pVtx[nVtxNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[nVtxNum].tex = D3DXVECTOR2(0.0f + (1.0f * nCntX), 0.0f + (1.0f * nCntZ));
					}
				}
				nVtxNum = 0;
				pVtx += FLOO_VTX_NUM_MAX;	//頂点を進める
			}

			//頂点バッファをアンロックする
			g_Field[nCntField].pVtxBuffField->Unlock();

			WORD *pIdx;		//インデックス情報へのポインタ

			//インデックスバッファをロックし、番号データへのポインタを取得
			g_Field[nCntField].pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

			for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
			{
				//角被さり以外の頂点
				for (int nCntZ = 0; nCntZ < FLOO_MAX_VTX_Z; nCntZ++)
				{
					for (int nCntX = 0; nCntX < FLOO_MAX_VTX_X + 1; nCntX++)
					{
						pIdx[(nCntX * 2) + 0 + ((FLOO_MAX_VTX_X + 2) * 2) * nCntZ] = ((FLOO_MAX_VTX_X + 1) + nCntX) + ((FLOO_MAX_VTX_X + 1) * nCntZ);
						pIdx[(nCntX * 2) + 1 + ((FLOO_MAX_VTX_X + 2) * 2) * nCntZ] = (0 + nCntX) + ((FLOO_MAX_VTX_X + 1) * nCntZ);
					}
				}
				//角被さりの頂点
				for (int nCntZ = 0; nCntZ < FLOO_MAX_VTX_Z - 1; nCntZ++)
				{
					pIdx[(((FLOO_MAX_VTX_X + 1) * 2 + 0) * (nCntZ + 1)) + (2 * nCntZ)] = FLOO_MAX_VTX_X + ((FLOO_MAX_VTX_X + 1) * nCntZ);
					pIdx[(((FLOO_MAX_VTX_X + 1) * 2 + 1) * (nCntZ + 1)) + (1 * nCntZ)] = (FLOO_MAX_VTX_X * 2 + 2) + ((FLOO_MAX_VTX_X + 1) * nCntZ);
				}
				pIdx += FLOO_IDX_NUM_MAX;	//インデックスを進める
			}

			//インデックスバッファをアンロックする
			g_Field[nCntField].pIdxBuffField->Unlock();
			g_Field[nCntField].bUse = true;
			break;
		}
	}
}