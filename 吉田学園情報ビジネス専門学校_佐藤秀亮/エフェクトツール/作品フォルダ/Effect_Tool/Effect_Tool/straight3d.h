//=============================================================================
// (直線)3Dエフェクト処理 [Straight3D.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _STRAIGHT3D_H_
#define _STRAIGHT3D_H_

#include "main.h"
#include "billEffect.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CCamera;

class CStraight3D : public CBillEffect
{
public:
	typedef enum
	{
		STRAIGHT,	//まっすぐ
		TARGET,	//目標に向かう
		MAX,
	} MOVE_PATTERN;

	CStraight3D(int nPriority);
	~CStraight3D();

	HRESULT Init(D3DXVECTOR3 pos,
		D3DXVECTOR3 Size, 
		D3DXVECTOR3 MinSize,
		D3DXVECTOR3 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nType,
		int nLife,
		CStraight3D::MOVE_PATTERN Pattrn,
		D3DXVECTOR3 Target,
		int Synsetic);

	void Uninit();
	void Update();
	void Draw();

	static CStraight3D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 Size,
		D3DXVECTOR3 MinSize,
		D3DXVECTOR3 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nType,
		int nLife,
		CStraight3D::MOVE_PATTERN Pattrn,
		D3DXVECTOR3 Target,
		int Synsetic);
private:

	D3DXVECTOR3 m_move;	//移動値
	D3DXVECTOR3 m_rot;	//移動値

	CCamera *m_pCamera;	//カメラ

	MOVE_PATTERN m_Pattern;	//動きのパターン
	D3DXVECTOR3 m_Target;	//目標地点

	float m_XZr;	//角度xz
	float m_Yr;	//角度y

};

#endif