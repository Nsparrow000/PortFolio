//--------------------------------------
//タイトルの処理
//--------------------------------------
#ifndef _RESULT_H
#define _RESULT_H
#include "main.h"
#include "scene.h"
#define MAX_RESULT (4)
#define MAX_TOTALSCORE (3)
#define MAX_TOTALSCORECNT (2)

//前方宣言
class CBg;
class CPolygon;
class CGametimer;
class CResultSelectButton;
class CScore;
class CResult
{
public:
	typedef enum
	{
		GAME_MENU = 0,
		TITLE,
		MAX,
	}TYPE;

	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void NextMode(bool bNextMode) { m_bNextMode = bNextMode; }
	void ScoreCalculating();//総合的なスコアを出すクラス
private:
	CPolygon			*pPolygon[MAX_RESULT];
	CPolygon			*m_Cursol;
	CGametimer			*m_pTimer;
	CResultSelectButton *m_pSelectButton[MAX];
	CScore *m_pEnemyCnt;
	CScore *m_pTotalScore[MAX_TOTALSCORE];//総合点
	CScore *m_pTotalScoreCnt[MAX_TOTALSCORECNT];//倍率の数字
	bool m_bTotalScorePoped[MAX_TOTALSCORE];
	static CBg			*m_pBg;
	bool m_bCanResultAnnouncement;//結果発表
	bool					m_bNextMode;//次のモードに行くための
	int					m_nSelectType;

};


#endif //_RESULT_H