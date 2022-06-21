#ifndef _GUARD_H
#define _GUARD_H
#include "scene.h"
class CModel;
class C3DPolygon;
class CGuard : public CScene
{
public:
	enum Level
	{
		Level_1 = 0,
		Level_2,
		Level_3,
		Level_4,
		Level_MAX
	};
	struct State
	{
		int m_nMaxGuard; //ダメージガードの個数
	};

	CGuard(OBJTYPE nPriority = CScene::OBJTYPE_EFFECT);
	~CGuard();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetGuardQuantity(int nGuard);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetHeightPolygon(float Height) { m_fHeight = Height; }
	void SetParent(CModel *pParent);
	int GetMaxGuard() { return m_State.m_nMaxGuard; }
	void SetLevel(int nLevel) { m_nLevel = nLevel; }
	static CGuard *Create(const D3DXVECTOR3& pos,const float& fHeight,const int& nLevel);//インスタンス生成処理

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXMATRIX	m_mtxWorld;
	D3DXVECTOR3	m_Scale;//敵の大きさ
	D3DXVECTOR3	m_pos;	//基準の位置
	D3DXVECTOR3	m_rot;	//基準の位置
	D3DXCOLOR m_col;
	State m_State;
	C3DPolygon **m_pGuardPolygon;
	float m_fHeight;
	int m_nCntGuard; //ダメージガードの個数
	int m_nLevel;
	bool m_bCanParent;
	bool						m_bUninit;
	bool m_bDraw;
};


#endif // !_GUARD_H

