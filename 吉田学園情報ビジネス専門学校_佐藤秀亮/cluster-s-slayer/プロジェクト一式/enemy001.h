#ifndef _ENEMY001_H_
#define _ENEMY001_H_
#include "enemy.h"

class CSound;
class CDamage_Collision;

#define MAX_KEYSET (10) //最大キーセット数
#define MAX_COLRADIUS (400.0f)
#define MAX_ATTACKSTART_RADIUS (150.0f)//攻撃開始範囲
#define ENEMY001_ADD_SPEED (0.2)//ゲージを増やす量
#define INVINCIBLE_TIME (30)//無敵時間
class CEnemy001 : public CEnemy
{
public:

	typedef enum
	{
		NEUTRAL = 0,
		MOVE,
		ATTACKWAIT,
		ATTACK,
		DETH,
		MAX
	}MOTION;

	typedef enum
	{
		NONE = 0,
		FIRE,
		BULLIZAD,
	}MAGIC;

	CEnemy001(OBJTYPE nPriority = CScene::OBJTYPE_ENEMY);
	
	~CEnemy001();
	 HRESULT Init();
	 void Uninit();
	 void Update();
	 void Draw();
	 void AIAttack();
	 void Attack();
	 void AIMove();
	 void AddLife(int nPower, int nType);
	 void AddLife(int nLife);//体力の増減
									//静的メンバー関数
	static CEnemy001 *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	CDamage_Collision *m_pDamageCollision;
	float m_fAttackCollisionTime;
};


#endif // !_PLAYER_H_

