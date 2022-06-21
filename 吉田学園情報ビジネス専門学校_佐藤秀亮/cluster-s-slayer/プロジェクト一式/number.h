#ifndef _NUMBER_H
#define _NUMBER_H
#include "main.h"
#include "smallscore.h"
class CSmallScore;
class CNumber
{
public:
	CNumber();
	~CNumber();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber);
	void SetCol(D3DXCOLOR col);//カラーを変える
	D3DXCOLOR GetCol() { return m_col; }
	void SetPos(D3DXVECTOR3 pos);//カラーを変える
	static CNumber *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale,CSmallScore *pParent,const bool& bCanParent);//インスタンス生成処理
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;
	LPDIRECT3DTEXTURE9		m_pTexture;				//テクスチャへのポインタ
	CSmallScore *m_pParent;//親の情報
	D3DXVECTOR3					m_Scale;				//敵の大きさ
	D3DXVECTOR3					m_pos;				//基準の位置
	D3DXVECTOR3					m_rot;				//基準の位置
	D3DXCOLOR m_col;
	bool m_bCanParent;
	bool						m_bUninit;
};


#endif // !_NUMBER_H

