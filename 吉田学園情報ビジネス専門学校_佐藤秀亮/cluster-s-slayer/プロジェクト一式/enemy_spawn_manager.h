//--------------------------------------------
//敵の出現設定処理
//作成者：林
//--------------------------------------------
#ifndef _ENEMY_SPAWN_MANAGER_H_
#define _ENEMY_SPAWN_MANAGER_H_
#include "scene.h"

class CEnemySpawnManager : public CScene
{
public:
	CEnemySpawnManager(OBJTYPE nPriority = CScene::OBJTYPE_MANAGER);
	~CEnemySpawnManager();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	//静的メンバー関数
	static CEnemySpawnManager *Create(const D3DXVECTOR3& pos);

private:
	D3DXVECTOR3 m_pos;
	int m_nSpawnCnt;//出現のカウンター
	int m_nSpawnTime;//出現するまでの時間
};


#endif // !_PLAYER_H_

