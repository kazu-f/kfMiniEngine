/*!
 * @brief	シンプルなモデル表示用のシェーダー。
 */



//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;
	float2 uv 		: TEXCOORD0;
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;
	float2 uv 			: TEXCOORD0;
};


/// <summary>
/// モデル用の頂点シェーダーのエントリーポイント。
/// </summary>
SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;

	psIn.pos = mul(mWorld, vsIn.pos);	//１：モデルの頂点をワールド座標系に変換。	
	psIn.pos = mul(mView, psIn.pos);	//２：ワールド座標系からカメラ座標系に変換。
	psIn.pos = mul(mProj, psIn.pos);	//３：カメラ座標系からスクリーン座標系に変換。
	psIn.uv = vsIn.uv;

	return psIn;
}
/// <summary>
/// モデル用のピクセルシェーダーのエントリーポイント
/// </summary>
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	return float4( 1.0f, 0.0f, 0.0f, 1.0f);
}
