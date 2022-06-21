#ifndef _MISSILE_H
#define _MISSILE_H
#include "scene.h"
class CModel;
class C3DPolygon;
class CMissile : public CScene
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
		float m_SearchRange;//ミサイルを打つ範囲
		int m_nPower;
		int m_nCntSearchMax;//最大サーチ数
	};

	CMissile(OBJTYPE nPriority = CScene::OBJTYPE_EFFECT);
	~CMissile();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMaxTime(float fTime) { m_fMaxTime = fTime; }
	void LaunchMissile();
	void SearchEnemy(const D3DXVECTOR3 EnemyPos,const float fRadius);
	static CMissile *Create();//インスタンス生成処理

private:
	D3DXVECTOR3	m_Scale;//敵の大きさ
	D3DXVECTOR3 *m_pEnemyPos;
	State m_State;
	float m_fIntervalTimer;//ミサイル発射まで数える
	float m_fMaxTime;
	int m_nCntSearch;//サーチ回数
	bool m_bCanSearch;
	bool m_bCanShotMissile;
	bool						m_bUninit;
	bool m_bDraw;
};


#endif // !_MISSILE_H

