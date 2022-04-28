//*****************************************************************************
// 中間セーブみたいなの(IntermediateSave.h)
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _INTERMEIATESAVE_H_
#define _INTERMEIATESAVE_H_

#include "main.h"
#include "manager.h"

class CIntermeiateSave
{
	CIntermeiateSave();
	~CIntermeiateSave();
public:
	//セーブ
	static void IntermeiateSave(CManager::MODE mode, int nPattern, const char *aModelName);
	//ロード
	static void IntermeiateLoad(CManager::MODE mode, const char *aModelName);
private:
};
#endif // !_INTERMEIATESAVE_H_
