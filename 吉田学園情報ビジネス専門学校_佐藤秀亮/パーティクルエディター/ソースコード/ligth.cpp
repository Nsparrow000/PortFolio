//**************************************************************
//ライトの処理(ligth.cpp)
//Author：佐藤秀亮
//**************************************************************


#include "ligth.h"

D3DLIGHT9 g_ligth[3];//ライトの情報

void InitLigth(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	D3DXVECTOR3 vecDir;			//設定用方向ベクトル
	//デバイスの取得
	pDevice = GetDevice();
	//**************************************************************
	//ライトの設定
	//**************************************************************
	for (int nCntligth = 0; nCntligth < 3; nCntligth++)
	{
		//ライトのクリア
		ZeroMemory(&g_ligth[nCntligth], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_ligth[nCntligth].Type = D3DLIGHT_DIRECTIONAL;//水平光源

		//ライトの拡散光を設定
		g_ligth[nCntligth].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//ライト０の方向を設定
		vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
		D3DXVec3Normalize(&vecDir, &vecDir);//正当化
		g_ligth[0].Direction = vecDir;

		//ライト１の方向を設定
		vecDir = D3DXVECTOR3(-0.2f, 0.8f, 0.4f);
		D3DXVec3Normalize(&vecDir, &vecDir);//正当化
		g_ligth[1].Direction = vecDir;

		//ライト２の方向を設定
		vecDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXVec3Normalize(&vecDir, &vecDir);//正当化
		g_ligth[2].Direction = vecDir;

		//各ライトの設定する
		pDevice->SetLight(nCntligth, &g_ligth[nCntligth]);
		//各ライトを有効
		pDevice->LightEnable(nCntligth, TRUE);
	}
}

void UninitLigth(void)
{

}

void UpdateLigth(void)
{

}
