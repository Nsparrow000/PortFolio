#ifndef _SMALLSCORE_H
#define _SMALLSCORE_H
//----------------------------------------
//攻撃を与えたときに出る数値の処理
//----------------------------------------
#include "scene2D.h"
#define MAXSMALLSCORE (8)
class CNumber;

class CSmallScore : public CScene
{
public:

	CSmallScore(OBJTYPE nPriority);
	~CSmallScore();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetSmallScore(int nSmallScore);
	void AddSmallScore(int nValue);
	void SetTrussSmallScore();//何桁出すか
	void SetColor(D3DXCOLOR col);//何桁出すか
	void AddAlpha();
	void DefAlpha();

	D3DXMATRIX GetMatrix() { return m_mtxWorld; }//ワールドマトリックスの取得

	int CSmallScore::GetSmallScore(void) { return m_nSmallScore; }
	static CSmallScore *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const D3DXCOLOR& col, const int& nSmallScore);//インスタンス生成処理

private:
	D3DXVECTOR3 m_rot;//向き
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	D3DXMATRIX m_mtxView;									//ビューマトリックス
	D3DXCOLOR m_col;
	D3DXVECTOR3					m_Scale;				//敵の大きさ
	D3DXVECTOR3					m_pos;				//基準の位置
	float						m_fMove;
	int							m_nSmallScore;			//現在のスコア
	CNumber						*m_pNumber[MAXSMALLSCORE];
	int							m_nMaxTruss;//最大の桁数
	int m_nCounter;
	bool						m_bUninit;
	bool m_bCanAddAlpha;//α値加算できるか
	bool m_bCanDefAlpha;//α値減算できるか

};


#endif // !_SMALLSCORE_H

