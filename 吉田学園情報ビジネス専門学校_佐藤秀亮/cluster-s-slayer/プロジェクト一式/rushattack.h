//--------------------------------------------
//スキル追撃の処理
//作成者：林
//--------------------------------------------
#ifndef _RASHATTACK_H_
#define _RASHATTACK_H_
#include "scene.h"
class CModel;
class CEnemy;

class CRushAttack:public CScene
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
		float m_fSizeDiameter;
		int m_nPower;
	};
	CRushAttack(OBJTYPE nPriority);
	~CRushAttack();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void UpperAttack();
	//静的メンバー関数
	static CRushAttack *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,CEnemy *pEnemy);
	bool IsCollision(const D3DXVECTOR3& Hitpos, const float& fRadius);

private:
	CModel *m_pRushSword;
	CEnemy *m_pEnemyData;
	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_rot;//向き
	D3DXVECTOR3 m_scale;//大きさ
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	State m_State;
	int m_nStartTimer;//追撃が始まるまでのカウンター
	int m_nEndTimer;//追撃が終わるまでのカウンター
	bool m_bCanAttackStart;//スキルの発動可能か
	bool m_bIsDeath;
};


#endif // !_PLAYER_H_

