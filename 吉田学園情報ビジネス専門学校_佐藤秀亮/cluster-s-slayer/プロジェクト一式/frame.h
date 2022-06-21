//--------------------------------------
//‚QDƒ|ƒŠƒSƒ“‚Ì‰æ‚è‚Ìˆ—
//--------------------------------------
#ifndef _FRAME_H
#define _FRAME_H
#include "scene.h"

//‘O•ûéŒ¾
class CPolygon;

class CFrame : public CScene
{
public:
	CFrame(OBJTYPE nPriority = OBJTYPE_FRAME);
	~CFrame();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CFrame *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col);
private:
	CPolygon *pPolygonFrame;
	bool m_bEnd;//I—¹”»’è
};


#endif //_RESULT_H