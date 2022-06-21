#ifndef _SCORE_H_
#define _SCORE_H_
#include "scene.h"
class C2DNumber;
#define MAX_SCORE (8)//最大描画枚数


class CScore : public CScene
{
public:
	CScore(OBJTYPE nPriority = OBJTYPE_UI);
	~CScore();
	static CScore*Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	HRESULT Init(D3DXVECTOR3 pos, float fsize);
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetScore();//スコアの描画設定
	void SetTrussSmallScore();//何桁出すか
	void CreateNumber(const int& nTruss);
	int GetScore() {return m_nScore;}
	void AddScore(int AddScore) { m_nScore += AddScore; };//スコアの加算
	void SetFitScore(int nScore) { m_nScore = nScore; }
private:
	C2DNumber *m_apNumber[MAX_SCORE];
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;

	int m_nAddScore;//スコア加算用変数
	int							m_nMaxTruss;//最大の桁数
	int m_nScore;//現在のスコア
};
#endif
