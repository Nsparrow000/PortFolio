//ーーーーーーーーーーーーーーーーーーーーーーーーー
//ポリゴンのヘッダー
//Author：satou syusuke
//ーーーーーーーーーーーーーーーーーーーーーーーーー

#ifndef _FIELD_H_
#define _FIELD_H_

#define MAX_FIELD (4)
#define MS_X (1.0f)			//床の半径X
#define MS_Y (1.0f)			//床の半径Y
#define MS_Z (1.0f)			//床の半径Z

#define FLOO_MAX_VTX_X (4)	//横ブロックの数
#define FLOO_MAX_VTX_Z (4)	//奥行ブロックの数
#define FLOO_VTX_NUM_MAX ((FLOO_MAX_VTX_X + 1) * (FLOO_MAX_VTX_Z + 1))//全体頂点の数
#define FLOO_IDX_NUM_MAX ((FLOO_MAX_VTX_X + 1) * (FLOO_MAX_VTX_Z + 1) + (FLOO_MAX_VTX_Z - 1) * (FLOO_MAX_VTX_X + 3))//インデックスの数
#define FLOO_POLY_NUM_MAX (2 * FLOO_MAX_VTX_X * FLOO_MAX_VTX_Z + (FLOO_MAX_VTX_Z * 4) - 4))	//△の数


typedef enum
{
	FIELDTYPE_NOMAL,
	FIELDTYPE_MAX,
} FIELDTYPE;

typedef struct
{
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 rot;						//回転
	D3DXMATRIX mtxwold;						//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffField;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuffField;	//インデックスバッファへのポインタ
	float fWidth;							//幅
	float fDepht;							//奥行
	int ntype;								//タイプ
	bool bUse;								//使用してるか
} Field;


//プレイヤー用の宣言
HRESULT InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepht,int ntype);

#endif
