//逆光ライト。光源の位置を考慮した逆光ライト。


static const int NUM_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの本数。

//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ディレクションライト。
struct DirectionalLight{
	float3 direction;	//ライトの方向。
	float3 color;		//ライトの色。
};
//ライト用の定数バッファ。
cbuffer LightCb : register(b1){
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
	float3 eyePos;					//カメラの視点。
	float rimPow;					//リムライトの絞り。
	float3 eyeDirection;			//カメラの方向。
	float specPow;					//スペキュラの絞り。
	float3 ambinentLight;			//環境光。
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


//拡散反射光を計算。
float3 CalcDiffuselLight( SPSIn psIn )
{
	float3 lig = 0;
	for( int i = 0; i < NUM_DIRECTIONAL_LIGHT; i++){
		float d = dot( psIn.normal, -directionalLight[i].direction);	//ライトの逆方向と法線で内積を計算する。
		if( d < 0.0f){	//内積の計算結果はマイナスになるので、if文で判定する。
			d = 0.0f;
		}
		lig += d * directionalLight[i].color; //拡散反射光を足し算する。
	}
	return lig;
}
float3 CalcSpecularLight( SPSIn psIn )
{
	float3 lig = 0;
	for( int i = 0; i < NUM_DIRECTIONAL_LIGHT; i++){
		//ライトをあてる物体から視点に向かって伸びるベクトルを計算する。
		float3 eyeToPixel = eyePos - psIn.worldPos;
		eyeToPixel = normalize(eyeToPixel);
		
		//光の物体に当たって、反射したベクトルを求める。
		float3 reflectVector = reflect(directionalLight[i].direction, psIn.normal);
		//反射した光が目に飛び込んて来ているかどうかを、内積を使って調べる。
		float d = dot(eyeToPixel, reflectVector);
		if( d < 0.0f){
			d = 0.0f;
		}
		d = pow(d, specPow);
		//スペキュラ反射の光を足し算する。
		lig += directionalLight[i].color * d;
	}
	return lig;
}

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
	lig += CalcDiffuselLight( psIn );

	//////////////////////////////////////////////////////
	// スペキュラ反射を計算
	//////////////////////////////////////////////////////
	lig += CalcSpecularLight( psIn );
	
	//////////////////////////////////////////////////////
	// 環境光を計算
	//////////////////////////////////////////////////////
	lig += ambinentLight; //足し算するだけ

	//////////////////////////////////////////////////////
	// リムライトを計算
	//////////////////////////////////////////////////////
	float d = dot( psIn.normal, -eyeDirection );
	float rimPowerBase = pow( (1.0f - d), rimPow );
	for( int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++){
		float rimPower = max( 0.0f, dot( directionalLight[ligNo].direction, -eyeDirection));
		rimPower *= rimPowerBase;
		lig += directionalLight[ligNo].color * rimPower;
	}
	
	
	float4 texColor = g_texture.Sample(g_sampler, psIn.uv);
	texColor.xyz *= lig; //光をテクスチャカラーに乗算する。
	return texColor;	
}
