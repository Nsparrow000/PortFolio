//--------------------------------------
//ファイル選択表示用の処理
//--------------------------------------
#ifndef _RESULTSELECTBUTTON_H
#define _RESULTSELECTBUTTON_H
#include "scene2D.h"
//前方宣言
class CPolygon;
class CFrame;

class CResultSelectButton : public CScene
{
public:
	CResultSelectButton(OBJTYPE nPriority = OBJTYPE_UI);
	~CResultSelectButton();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CResultSelectButton *Create(const D3DXVECTOR3& pos,
		const D3DXVECTOR3& size, const int& tex);
	void PopSelectBottom();

	void SelectBottom();
	void SetEnd(bool bEnd) { m_bEnd = bEnd; }
	//選択状態設定
	void SetSelect(bool bSelect) { m_bIsSelect = bSelect; }
	//決定状態設定
	void SetDecision(bool bDecision) { m_bIsDecision = bDecision; }
	//決定状態取得
	bool GetDecision() { return m_bIsDecision; }

	CPolygon *GetSelectButton() { return pSelectBottom; }
private:
	CPolygon *pSelectBottom;
	CFrame *pFrame;//２Dポリゴンの縁
	bool m_bIsSelect;//現在選択しているか
	bool m_bIsDecision;//現在決定しているか
	bool m_bPopSelectBottom;
	bool m_bCanAlphaDown;
	bool m_bEnd;//終了判定
};


#endif //_RESULT_H
