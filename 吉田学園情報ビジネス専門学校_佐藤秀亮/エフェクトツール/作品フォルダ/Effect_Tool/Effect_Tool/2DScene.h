//=============================================================================
// 2D側エフェクト管理[2DScene.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _2DSCENE_H_
#define _2DSCENE_H_

#include "manager.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;
class CSetEffect;

class C2DScene : public CScene
{
public:
	C2DScene();
	~C2DScene();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	static C2DScene *Create();

	CSetEffect *GetSetEffect() { return m_Set; }

private:
	static int m_SerectNam;
	static bool m_bFade;
	CKeyboard *m_pKeyboard;

	D3DXVECTOR3 m_pSetpos;
	CSetEffect *m_Set;

	//CGamePad *m_pGamePad;

};

#endif // _2DSCENE_H_
