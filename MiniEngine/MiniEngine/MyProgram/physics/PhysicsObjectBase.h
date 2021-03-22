#pragma once

namespace Engine {
	class Model;
	class TkmFile;
	namespace prefab {
		class ModelRender;
	}

	/// <summary>
	/// 物理オブジェクトのベースクラス。
	/// </summary>
	class CPhysicsObjectBase
	{
		//仮想関数。
	public:
		/// <summary>
		/// ゴーストオブジェクトを解放。
		/// </summary>
		virtual void Release() = 0;
	private:
		/// <summary>
		/// 静的物理オブジェクト作成処理の共通処理。
		/// </summary>
		virtual void CreateCommon(const Vector3& pos, const Quaternion& rot) = 0;

	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CPhysicsObjectBase()
		{
		}
		/// <summary>
		/// デストラクタ。
		/// </summary>
		virtual ~CPhysicsObjectBase()
		{
		}
		/// <summary>
		/// ボックス形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="size">サイズ。</param>
		void CreateBox(const Vector3& pos, const Quaternion& rot, const Vector3& size);
		/// <summary>
		/// カプセル形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="radius">カプセルの半径。</param>
		/// <param name="height">カプセルの高さ。</param>
		void CreateCapsule(const Vector3& pos, const Quaternion& rot, const float radius, const float height);
		/// <summary>
		/// 球体形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="radius">球体の半径。</param>
		void CreateSphere(const Vector3& pos, const Quaternion& rot, const float radius);
		/// <summary>
		/// メッシュ形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="tkmFile">tkmファイル。</param>
		void CreateMesh(const Vector3& pos, const Quaternion& rot,const TkmFile& tkmFile);
		/// <summary>
		/// メッシュ形状のゴーストオブジェクトを作成。(Modelクラス。)
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="scale">スケール。</param>
		/// <param name="model">スキンモデル。</param>
		void CreateMesh(const Vector3& pos, const Quaternion& rot, const Vector3 & scale, const Model& model);
		/// <summary>
		/// メッシュ形状のゴーストオブジェクトを作成。(ModelRenderクラス。)
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="scale">スケール。</param>
		/// <param name="model">モデルレンダー。</param>
		void CreateMesh(const Vector3& pos, const Quaternion& rot, const Vector3& scale, prefab::ModelRender* model);

	protected:
		std::unique_ptr<ICollider> m_sphere;		//コライダー。
	};///class CPhysicsObjectBase

}///namespace Engine
