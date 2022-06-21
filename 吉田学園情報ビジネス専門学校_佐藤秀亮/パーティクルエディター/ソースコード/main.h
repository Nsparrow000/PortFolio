//===============================================================================================================
//
// メインヘッダー [main.h]
// Author:sato syusuke
//
//===============================================================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS
#define KEYCHANGEFLEAM (10)//十字キー入力からの操作不能の間

#include <Windows.h>
#include "d3dx9.h"						// 描画処理に必要
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <XInput.h>

#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"						// 入力処理に必要
#include "xaudio2.h"
#include "stdio.h"
#include<time.h>

//===============================================================================================================
// ライブラリのリンク
//===============================================================================================================
#pragma comment(lib, "d3d9.lib")	// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")	// 拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	// コンポーネント使用に必要
#pragma comment(lib, "dinput8.lib")	// 入力処理に必要
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "xinput.lib") //コントローラーのやつで必要って書いてた

//===============================================================================================================
// マクロ定義
//===============================================================================================================
#define SCREEN_WIDTH	(1280)		// スクリーンの幅
#define SCREEN_HEIGTH	(720)		// スクリーンの高さ
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//===============================================================================================================
// 構造体の定義
//===============================================================================================================
// 頂点フォーマットに合わせた構造体
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点処理
	float rhw;			// 1.0で固定
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャの座標
} VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;	// 頂点処理
	D3DXVECTOR3 nor;
	D3DCOLOR col;
	D3DXVECTOR2 tex;	// テクスチャの座標
}VERTEX_3D;


//===============================================================================================================
// プロトタイプ宣言
//===============================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void);

#endif