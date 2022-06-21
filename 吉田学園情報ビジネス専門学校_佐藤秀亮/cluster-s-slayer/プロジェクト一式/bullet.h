//-------------------------------------------
//アシストクリスタルの処理
//------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_
#include "scene.h"
#define MAX_STACKBULLET (3)
class CPlayer;
class CModel;
class CAssistCrystal;
class C3DPolygon;
class CBullet : public CScene
{
public:
	CBullet(OBJTYPE nPriority = CScene::OBJTYPE_EFFECT);
	~CBullet();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	//静的メンバー関数
	static CBullet *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float& fMoveSpeed, const int& nPower);
	bool IsCollision(const D3DXVECTOR3& Hitpos, const float& fRadius);

private:
	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_rot;//向き
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	CAssistCrystal *m_pParent;
	D3DXMATRIX	m_mtxParent;						//ワールドマトリックス
	C3DPolygon *m_pBullet;
	int m_nTimer;
	int m_nPower;
	float m_fMoveSpeed;
	bool m_bMoveDirection;
	bool m_bUninit;
};

#endif // !_BULLET_H_

