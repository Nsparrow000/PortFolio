//--------------------------------------
//タイトルの処理
//--------------------------------------
#ifndef _GAME_H
#define _GAME_H
#include "main.h"
#include "scene.h"

//前方宣言
class CBg;
class CPlayer;
class CScene3D;
class CModel_Spawner;
class CScore;
class CField;
class CMeshSphere;
class CParticle;
class CPolygon;
class CStage;
class CCamera;
class CEnemySpawnManager;
class CGauge;
class CGametimer;

class CGame
{
public:
	typedef enum
	{
		LEVEL_EASY=0,
		LEVEL_NORMAL,
		LEVEL_HARD,
		LEVEL_MAX
	}LEVEL;
	struct Achievement
	{
		bool bGetATKup;
		bool bGetHeal;
		bool bGetOverHeal;
		bool bGetSheild;
		bool bGetBeam;
		bool bGetBlackHole;
		bool bGetRocket;
		bool bGetRushAttack;
	};
	CGame();
	~CGame();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CPlayer *GetPlayer() { return m_Player; }
	static CModel_Spawner	*GetModel() { return m_pModel; }
	static CScore			*GetScore() { return m_pScore; }
	static CMeshSphere		*GetMeshSphere() { return m_pMeshSphere; }
	static CPolygon			*GetCursol() { return m_Cursol; }
	static CParticle		*GetParticle() { return m_Particle; }
	static CGauge		*GetHPGauge() { return m_pHPGauge; }
	static CGauge		*GetExpGauge() { return m_pExpGauge; }
	static CGametimer		*GetGameTimer() { return m_pGametimer; }
	static Achievement GetAchievement() { return m_Achievement; }
private:
	static CEnemySpawnManager		*m_pEnemySpawnManager;
	static CScore		*m_pScore;
	static CBg			*m_pBg;
	static CPlayer		*m_Player;
	static CModel_Spawner *m_pModel;
	static CField		*m_pField;//床
	static CField		*m_pSea;//床
	static CPolygon		*m_Polygon;
	static CPolygon		*m_Cursol;
	static CMeshSphere	*m_pMeshSphere;
	static CStage		*m_pStage;
	static CGauge	*m_pExpGauge;
	static CGauge	*m_pHPGauge;
	static CGametimer	*m_pGametimer;
	static CParticle	*m_Particle;
	static Achievement m_Achievement;
	bool m_bPush;
	bool m_bEnd;
	int m_nCntDelay;//ディレイのカウンタ
	int m_nGameTimeSecond;
	float				m_fAlpha;
	bool				m_bNextMode;//二重押し防止変数
	bool m_bIsClear;
	int					m_nTimer;//カウンター
	CScene3D			*m_pFlore;//床
};


#endif //_RESULT_H