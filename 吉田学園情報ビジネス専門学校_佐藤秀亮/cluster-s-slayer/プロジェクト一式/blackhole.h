//-------------------------------------------
//ブラックホールの処理
//------------------------------------------
#ifndef _BLACKHOLE_H_
#define _BLACKHOLE_H_
#include "scene.h"
class CEnemy;

class CBlackHole : public CScene
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
		int m_HoleExpansionTime;
		int m_nLevel;
		float m_fArea;
	};
	CBlackHole(OBJTYPE nPriority);
	~CBlackHole();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Suction();//敵を吸い付ける処理
	void Head();
	//静的メンバー関数
	static CBlackHole *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy *pEnemy,const int& nLevel);

private:
	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_rot;//向き
	D3DXVECTOR3 m_scale;//大きさ
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	CEnemy *m_pEnemyData;
	State m_State;//status
	int m_nDeleteTimer;
	int m_nLevel;
	float m_fMoveSpeed;
	bool m_bUninit;
	bool m_bHoleExpansion;//ブラックホールの展開フラグ
};

#endif // !_BLACKHOLE_H_

