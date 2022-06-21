//=============================================
//ïêäÌÇÃÉNÉâÉX
//=============================================
#ifndef _SWORD_H_
#define _SWORD_H_
#include "weapon.h"
class CModel;

class CSword : public CWeapon
{
public:
	CSword();
	~CSword();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CSword *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,CModel *pParent);
	bool IsCollision(const D3DXVECTOR3& Hitpos, const float& fRadius);
	void SetCombo(int nCombo) { m_nCombo = nCombo; }
private:
	D3DXVECTOR3 m_lastpos;
	int m_nCombo;
};


#endif // !_SWORD_H_
