
//ディレクションライト。
struct SDirectionalLight {
	float4 color;		//ライトの色。
	float3 direction;	//ライトの方向。
	unsigned int lightingMaterialIDGroup;		//ライトの影響を与えるマテリアルを決めるID。
};

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
	int4  Indices : BLENDINDICES0;
	float4 Weights : BLENDWEIGHT0;
};

//頂点シェーダーへの入力。
struct SVSIn {
	float4 pos : POSITION;		//モデルの頂点座標。
	float3 normal : NORMAL;		//法線。
	float3 Tangent : TANGENT;	//法線に対して横？に垂直なベクトル。
	float3 biNormal : BINORMAL;	//従法線。法線に対して上？に垂直なベクトル。
	float2 uv : TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用の頂点データ。
};

//ピクセルシェーダーへの入力。
struct SPSIn {
	float4 pos : SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal : NORMAL;		//法線。
	float3 Tangent : TANGENT;	//
	float3 biNormal : BINORMAL;	//従法線。
	float2 uv : TEXCOORD0;	//uv座標。
	float3 worldPos : TEXCOORD1;	//ワールド空間でのピクセルの座標。
};

//GBufferに書き込むための構造体。
struct PSOut_GBuffer {
	float4 albedo : SV_Target0;		//アルベドカラー。
	float4 normal : SV_Target1;		//法線。
	float4 worldPos : SV_Target2;	//ワールド座標。
	float4 specMap : SV_Target3;		//スペキュラ。
	float shadow : SV_Target4;		//シャドウ。
	float2 reflection : SV_Target5;	//反射率。
};

//シャドウマップ用の頂点シェーダーへの入力。
struct SShadowMapVSIn {
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;	//uv座標。
	SSkinVSIn skinVert;
};
//シャドウマップ用のピクセルシェーダーへの入力。
struct SShadowMapPSIn {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;	//uv座標。
};

