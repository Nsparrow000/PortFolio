//=============================================================================
// ïΩñ èàóù [plane.h]
// Author : ç≤ì°èGó∫
//=============================================================================
#ifndef _PLANE_H_
#define _PLANE_H_

#include "main.h"
#include "scene3d.h"

//É}ÉNÉçíËã`
#define CIRCLE (float(rand() % 324) / 100.0f - float(rand() % 324) / 100.0f) //â~
#define CIRCLE2 (float(rand() % 648) / 100.0f - float(rand() % 648) / 100.0f) //â~

class CPlane : public CScene3D
{
public:
	CPlane(int nPriority);
	~CPlane();
	HRESULT Init(const int nPolygonType, int nTex);
	void Uninit();
	void Update();
	void Draw();

	static CPlane *Create(const int nPolygonType, int nTex);
private:
};

#endif