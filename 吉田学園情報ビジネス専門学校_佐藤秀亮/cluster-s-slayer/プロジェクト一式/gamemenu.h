//--------------------------------------
//ゲーム開始前のメニュー画面
//--------------------------------------
#ifndef _GAMEMENU_H
#define _GAMEMENU_H

#include "main.h"
#include "scene.h"
#include "player.h"
//前方宣言
class CBg;
class CPolygon;
class CSound;
class CTitleSelectButton;

class CGameMenu
{
public:
	enum BUTTON
	{
		// 各スキルのボタンがここにあるとする
		MENUBUTTON_GAME_START = CPlayer::Skill_Max,	// ゲーム開始
		MENUBUTTON_RETURN_TO_TITLE,	// タイトルへ遷移
		MENUBUTTON_MAX
	};

	CGameMenu();
	~CGameMenu();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void NextMode(bool bNextMode) { m_bNextMode = bNextMode; }

private:
	bool m_bNextMode;	// 次のモードに行くための

	CTitleSelectButton *pButton[MENUBUTTON_MAX];
	int m_nSelectType;//現在選択しているボタンの種類
	int m_nDecisionType;//現在決定しているボタンの種類
};

#endif //_TITLE_H