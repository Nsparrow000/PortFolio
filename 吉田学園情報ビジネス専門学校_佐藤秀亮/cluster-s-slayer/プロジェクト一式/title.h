//--------------------------------------
//タイトルの処理
//--------------------------------------
#ifndef _TITLE_H
#define _TITLE_H

#include "main.h"
#include "scene.h"
static const int max_TitleButton = 4;
static const int max_TitlePercent = 3;

//前方宣言
class CBg;
class CPolygon;
class CSound;
class CTitleSelectButton;
class CScore;
class CTitle
{
public:
	enum TYPE
	{
		TITLE_PRESS_ENTER,
		TITLE_FADE,
		TITLE_SELECT_DOWN,
		TITLE_FILE_SELECT,
		TITLE_FILE_DELETE,
		TITLE_MAX
	};

	enum POLYGON
	{
		PORYGON_LOGO,	// タイトルロゴ
		PORYGON_FILEBG,	// 背景をちょっと暗くする
		PORYGON_FILE01,	// ファイル1
		PORYGON_FILE02,	// ファイル2
		PORYGON_FILE03,	// ファイル3
		PORYGON_FILEDELETE,	// ファイル削除
		POLYGON_MAX
	};

	CTitle();
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void NextMode(bool bNextMode) { m_bNextMode = bNextMode; }

private:
	void ButtonCreate(int num);

	CPolygon	*m_Polygon[POLYGON_MAX];
	CPolygon	*m_TitleLogo;
	CScore *m_pPercent[max_TitlePercent];
	int			m_nNowType;		// 現在のタイトルアニメーション
	bool		m_bNextMode;	// 次のモードに行くための
	float		m_fAlpha;		// 画面の明るさ

	CTitleSelectButton *pSkillBottom[max_TitleButton];
	int m_nSelectType;//現在選択しているボタンの種類
	int m_nDecisionType;//現在決定しているボタンの種類
};

#endif //_TITLE_H