//----------------------------------------------
//ゲージの処理
//Author::林海斗
//--------------------------------------------

#ifndef _GAUGE_H_
#define _GAUGE_H_
#include "scene.h"

class CPolygon;
class CGaugeBer;

class CGauge : public CScene
{
public:
	typedef enum
	{
		L_ADD=0,//左側に向かってゲージが増える
		R_ADD,//右側に向かってゲージが増える
	}ADDTYPE;//ゲージの増え方

	CGauge(OBJTYPE nPriority);
	~CGauge();
	static CGauge*Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const D3DXCOLOR& col,
		const float& MaxGauge, const int& nValue, const int& type);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ResetGauge(int nGaugeType);//ゲージの増減
	void SetGauge(float Set, int nGaugeType);//ゲージの増減
	void SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, int AddType, int nGaugeType);
	CGaugeBer *GetGaugeBer(int nGaugeType) { return m_pGaugeBer[nGaugeType]; }
private:
	CPolygon *m_pFrame;//ゲージの枠
	CGaugeBer *m_pGaugeBer[2];
};

#endif // !_TIME_H_*/#pragma once
