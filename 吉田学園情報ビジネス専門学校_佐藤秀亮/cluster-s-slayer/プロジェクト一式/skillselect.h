//--------------------------------------
//スキル選択画面の処理
//--------------------------------------
#ifndef _SKILLSELECT_H
#define _SKILLSELECT_H
#include "scene.h"
static const int max_Bottom = 3;
//前方宣言
class CPolygon;
class CSkillSelectBottom;

class CSkillSelect : public CScene
{
public:
	enum SelectType
	{
		TYPE001 = 0,
		TYPE002,
		TYPE003,
		TYPEMAX
	};
	enum SkillType
	{
		PassiveType = 0,
		ActionType
	};
	CSkillSelect(OBJTYPE nPriority = OBJTYPE_UI);
	~CSkillSelect();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CSkillSelect *Create();
	void PopSelectWindow();
private:
	CPolygon *pSelectWindow;
	CPolygon *pSelectWindowBg;//選択画面の背景
	CSkillSelectBottom *pSkillBottom[max_Bottom];
	int m_nSelectType;//現在選択しているボタンの種類
	int m_nDecisionType;//現在決定しているボタンの種類
	bool m_bPopSelectWindow;
	bool m_bEnd;//終了判定
};


#endif //_RESULT_H