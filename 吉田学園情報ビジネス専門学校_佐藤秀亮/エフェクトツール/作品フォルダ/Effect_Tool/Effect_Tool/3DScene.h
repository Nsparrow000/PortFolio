//=============================================================================
// 3D側エフェクト管理[3DScene.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _3DSCENE_H_
#define _3DSCENE_H_

#include "manager.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;

class C3DScene : public CScene
{
public:
	C3DScene();
	~C3DScene();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	static C3DScene *Create();

private:
	static int m_SerectNam;
	static bool m_bFade;
	static int m_nDirey;	//発生のディレイ
	static bool m_bPresetStart;	//プリセットの再生中
	static int m_nPattarn;	//発生エフェクト


	CKeyboard *m_pKeyboard;
	CGamePad *m_pGamePad;

};

#endif // _3DSCENE_H_
