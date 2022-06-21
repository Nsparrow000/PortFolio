//-------------------------------------------
//ミサイル弾の処理
//------------------------------------------
#ifndef _MISSILE_BULLET_H_
#define _MISSILE_BULLET_H_
#include "scene.h"
class CEnemy;
class CModel;
class CMissile_Bullet : public CScene
{
public:


	CMissile_Bullet(OBJTYPE nPriority = OBJTYPE_EFFECT);
	~CMissile_Bullet();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	//静的メンバー関数
	static CMissile_Bullet *Create(const D3DXVECTOR3& pos,const float& fSize,const int& nPower);
	bool IsCollision(const D3DXVECTOR3& Hitpos, const float& fRadius);

private:
	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_rot;//向き
	D3DXVECTOR3 m_scale;//大きさ
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	CModel*m_pMissileModel;//ミサイルのモデル

	int m_nDeleteTimer;
	int m_nPower;
	float m_fMoveSpeed;
	float m_fRadiusSize;
	bool m_bUninit;
	bool m_bDraw;
};

#endif // !_MISSILE_BULLET_H_

