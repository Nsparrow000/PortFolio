//===================================================
//階層構造の設定をするクラス
//===================================================
#ifndef _LAYER_STRUCTURE_H
#define _LAYER_STRUCTURE_H
#include "main.h"
#include "character_partsdata.h"

class CModel;
class CCharacterPartsData;

class CLayer_Structure
{
#define MAX_LAYER_PARTS (20)

public:
	CLayer_Structure();
	~CLayer_Structure();
	//void SetLayer_Structure(const char *sFileName, CModel **apModel, int nType = 0);
	void SetLayer_Structure(const char *sFileName, vector<CModel*>& pModel,int nType = 0);
	void SetLayer_Structure(const char *sFileName, CCharacterPartsData::PartsData *PartsData, int nType = 0);

	int	 GetMaxParts(void) { return m_MaxParts; }

private:
	int		m_MaxParts;//パーツの最大数

};


#endif // !_LAYER_STRUCTURE_H

