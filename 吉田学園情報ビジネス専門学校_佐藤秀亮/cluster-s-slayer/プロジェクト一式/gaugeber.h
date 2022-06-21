//----------------------------------------------
//ゲージバーの処理
//Author::林海斗
//--------------------------------------------

#ifndef _GAUGEBER_H_
#define _GAUGEBER_H_
#include "scene.h"
#include "Scene2D.h"
#include "texture.h"

class CPolygon;

class CGaugeBer : public CScene2D
{
public:
	typedef enum
	{
		L_ADD = 0,//左側に向かってゲージが増える
		R_ADD,//右側に向かってゲージが増える
	}ADDTYPE;//ゲージの増え方

	CGaugeBer(OBJTYPE nPriority);
	~CGaugeBer();
	static CGaugeBer*Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const D3DXCOLOR& col,
		const float& MaxGauge, const int& nValue, const int& type);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ResetGauge();//ゲージの増減
	void SetGauge(float Set);//ゲージの増減
	void SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, int AddType);
	float GetGaugeValue() { return m_fValue; }
	float GetGaugeValueMax() { return m_fValueMax; }

	void SetGaugeValueMax(float fValueMax) { m_fValueMax = fValueMax; }

private:
	D3DXVECTOR3			m_Scale;		//ゲージの大きさ
	ADDTYPE				m_AddType;		//ゲージの増えるタイプ
	float				m_fMaxGauge;	//gaugeの最大数
	float				m_fValueMax;	//ゲージの値の最大数
	float				m_fValue;		//ゲージの数値

};

#endif // !_TIME_H_*/#pragma once
