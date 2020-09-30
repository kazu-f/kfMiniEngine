//ランバート拡散反射サンプル00。
//拡散反射光のみを確認するためのサンプルです。



//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};


//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//法線。
	float2 uv 		: TEXCOORD0;	//UV座標。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線。
	float2 uv 			: TEXCOORD0;	//uv座標。
};

//モデルテクスチャ。
Texture2D<float4> g_texture : register(t0);	

//サンプラステート。
sampler g_sampler : register(s0);

/// <summary>
/// モデル用の頂点シェーダーのエントリーポイント。
/// </summary>
SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;

	psIn.pos = mul(mWorld, vsIn.pos);			//モデルの頂点をワールド座標系に変換。	
	psIn.pos = mul(mView, psIn.pos);			//ワールド座標系からカメラ座標系に変換。
	psIn.pos = mul(mProj, psIn.pos);			//カメラ座標系からスクリーン座標系に変換。
	psIn.normal = mul(mWorld, vsIn.normal);		//法線をワールド座標系に変換。
	psIn.uv = vsIn.uv;

	return psIn;
}
/// <summary>
/// モデル用のピクセルシェーダーのエントリーポイント
/// </summary>
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	float3 directionalLightDir = float3( 1.0f, 0.0f, 0.0f);		//ディレクションライトの方向。真横からあたっている。
	float3 directionalLightcolor = float3( 1.0f, 1.0f, 1.0f);	//ディレクションライトのカラー。真っ赤なカラー。
	//ランバート拡散反射光を計算する。
	float3 lig = 0.0f;
	float d = dot( psIn.normal, -directionalLightDir);	//ライトの逆方向と法線で内積を計算する。
	if( d < 0.0f){	//内積の計算結果はマイナスになるので、if文で判定する。
		d = 0.0f;
	}
	lig = d * directionalLightcolor; //内積の結果(0.0～1.0)をライトの影響力と考えて、ライトのカラーに乗算する。
									 //これをランバート拡散反射光と考える。
	return float4(lig, 1.0f);		 //拡散反射のみを返す。
}
