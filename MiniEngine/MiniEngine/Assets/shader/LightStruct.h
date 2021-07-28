/*
* ライトの構造体、定数。
*/

static const int MAX_POINT_LIGHT = 1024;				//ポイントライトの最大数。

//ディレクションライト。
struct SDirectionalLight {
	float4 color;		//ライトの色。
	float3 direction;	//ライトの方向。
	unsigned int lightingMaterialIDGroup;		//ライトの影響を与えるマテリアルを決めるID。
};

//ポイントライト。
struct SPointLight
{
	float3		position;		//ポイントライトの位置。
	float3		positionInView;	//ビュー空間でのポイントライトの座標。
	float4		color;			//ポイントライトのカラー。
	float4		attn;			//減衰定数。x:ポイントライトの影響が届く範囲。y:ポイントライトの減衰率。
								//yが大きくなると、減衰が強くなる。1.0で線形の減衰率になる。z,w:未使用。
	float4		directionAndAngle;	//指向性ライトの方向と照射角度。
									//xyzに方向、wに角度(ラジアン単位)が入る。
									//hasDirectionが1の時に有効となる。
	unsigned int hasDirection;		//このポイントライトが指向性ライトかどうかのフラグ。(スポットライトかどうか。)
	unsigned int lightingMaterialIDGroup;	//ライトの影響を与えるマテリアルIDのグループ。
};
