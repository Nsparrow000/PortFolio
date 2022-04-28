//=============================================================================
// シーンのロード処理 [load.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _LOAD_H_
#define _LOAD_H_

#include "main.h"


class CLoad
{
public:
	CLoad();
	~CLoad();
	static void Load(const char *aModelName);
	static void Load2D(const char *aModelName);	//ボタンの読み込み

private:
};

#endif