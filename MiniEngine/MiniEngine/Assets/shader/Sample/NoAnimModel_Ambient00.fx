
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ライト用の定数バッファ。
cbuffer LightCb : register(b1){
	float3 directionalLightDir;		//ディレクションライトの方向。
	float3 directionalLightcolor;	//ディレクションライトのカラー。
	float3 eyePos;					//カメラの視点。
	float specPow;					//スペキュラの絞り。
	float3 ambinet;					//環境光。
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
	float3 worldPos		: TEXCOORD1;	//ワールド空間でのピクセルの座標。
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
	psIn.worldPos = psIn.pos.xyz;
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
	float3 lig = 0.0f;
	//////////////////////////////////////////////////////
	// 拡散反射を計算
	//////////////////////////////////////////////////////
	{
		float d = dot( psIn.normal, -directionalLightDir);	//ライトの逆方向と法線で内積を計算する。
		if( d < 0.0f){	//内積の計算結果はマイナスになるので、if文で判定する。
			d = 0.0f;
		}
		lig += d * directionalLightcolor; //拡散反射光を足し算する。
	}
	//////////////////////////////////////////////////////
	// スペキュラ反射を計算
	//////////////////////////////////////////////////////
	{
		//ライトをあてる物体から視点に向かって伸びるベクトルを計算する。
		float3 eyeToPixel = eyePos - psIn.worldPos;
		eyeToPixel = normalize(eyeToPixel);
		
		//光の物体に当たって、反射したベクトルを求める。
		float3 reflectVector = reflect(directionalLightDir, psIn.normal);
		//反射した光が目に飛び込んて来ているかどうかを、内積を使って調べる。
		float d = dot(eyeToPixel, reflectVector);
		if( d < 0.0f){
			d = 0.0f;
		}
		d = pow(d, specPow);
		//スペキュラ反射の光を足し算する。
		lig += directionalLightcolor * d;
	}
	
	//////////////////////////////////////////////////////
	//環境光を計算。
	//////////////////////////////////////////////////////
	lig += ambinet;

	float4 texColor = g_texture.Sample(g_sampler, psIn.uv);
	texColor.xyz *= lig; //光をテクスチャカラーに乗算する。
	return texColor;	
}
