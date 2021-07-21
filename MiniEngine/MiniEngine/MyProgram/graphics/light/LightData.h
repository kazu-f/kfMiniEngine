#pragma once

/// <summary>
/// 各種ライトのデータ
/// </summary>

namespace Engine
{
	/// <summary>
	/// ディレクションライト。
	/// </summary>
	struct SDirectionLight {
		Vector4 color;		//ライトのカラー。
		Vector3 direction;	//ライトの方向。
		unsigned int lightingMaterialIDGroup;
	};

	/// <summary>
	/// アンビエントライト。
	/// </summary>
	struct SAmbientLight {
		Vector3 color;			//環境光の色。
	};
	/// <summary>
	/// ポイントライト。
	/// </summary>
	struct SPointLight
	{
		Vector3		position;		//ポイントライトの位置。
		Vector3		positionInView;	//ビュー空間でのポイントライトの座標。
		Vector4		color;			//ポイントライトのカラー。
		Vector4		attn;			//減衰定数。x:ポイントライトの影響が届く範囲。y:ポイントライトの減衰率。
									//yが大きくなると、減衰が強くなる。1.0で線形の減衰率になる。z,w:未使用。
		Vector4		directionAndAngle;	//指向性ライトの方向と照射角度。
										//xyzに方向、wに角度(ラジアン単位)が入る。
										//hasDirectionが1の時に有効となる。
		unsigned int hasDirection;		//このポイントライトが指向性ライトかどうかのフラグ。(スポットライトかどうか。)
		unsigned int lightingMaterialIDGroup;	//ライトの影響を与えるマテリアルIDのグループ。
	};
	///// <summary>
	///// スポットライト。
	///// </summary>
	//struct SSpotLight {
	//	Vector3		position;			//スポットライトの位置。
	//	Vector4		directionAndAngle;	//ライトの方向と照射角度。	
	//									//xyzに方向、wに角度(ラジアン単位)が入る。
	//	Vector3		color;				//スポットライトのカラー。
	//	Vector4		attn;				//減衰定数。x:ポイントライトの影響が届く範囲。y:スポットライトの減衰率。
	//									//yが大きくなると、減衰が強くなる。1.0で線形の減衰率になる。z,w:未使用
	//	unsigned int lightingMaterialIDGroup;	//ライトの影響を与えるマテリアルIDのグループ。
	//};
}