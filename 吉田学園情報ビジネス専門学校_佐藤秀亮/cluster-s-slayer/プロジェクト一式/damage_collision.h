//=============================================
//武器のクラス
//=============================================
#ifndef _DAMAGE_COLLISION_H_
#define _DAMAGE_COLLISION_H_
#include "main.h"
class CModel;

class CDamage_Collision
{
public:
	CDamage_Collision();
	~CDamage_Collision();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CDamage_Collision *Create(const D3DXVECTOR3 pos,const float fSize,CModel *pParent);
	bool IsCollision(const D3DXVECTOR3& Hitpos, const float& fRadius);
	
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_lastpos;
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	CModel *m_pParent;//親モデルのポインタ
	float m_fSize;

};


#endif // !_DAMAGE_COLLISION_H_
