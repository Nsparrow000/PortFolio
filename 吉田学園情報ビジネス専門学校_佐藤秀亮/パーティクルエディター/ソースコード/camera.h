//**************************************************************
//カメラのヘッダー
//Author：佐藤秀亮
//**************************************************************

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

typedef struct
{
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 rot;			//カメラの向き
	D3DXVECTOR3 pos;

	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス

} Camera;

//プロトタイプ宣言
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

Camera *GetCamera(void);
#endif