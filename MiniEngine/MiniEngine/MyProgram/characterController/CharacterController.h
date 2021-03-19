#pragma once

#include "physics/CapsuleCollider.h"
#include "physics/RigidBody.h"


namespace Engine {
	/// <summary>
	/// キャラクターコントローラー。
	/// </summary>
	class CCharacterController
	{
	public:
		CCharacterController()
		{
		
		}
		~CCharacterController()
		{
			RemoveRigidBody();
		}
		/// <summary>
		/// キャラクターコントローラーの初期化。
		/// </summary>
		/// <remarks>
		/// カプセルコライダーを作成して、当たり判定を取る。
		/// </remarks>
		/// <param name="radius">カプセルコライダーの半径。</param>
		/// <param name="height">カプセルコライダーの高さ。</param>
		/// <param name="position">初期座標。</param>
		void Init(float radius, float height, const Vector3& position);
		/// <summary>
		/// キャラコンの移動を実行。
		/// </summary>
		/// <param name="moveSpeed">移動速度。</param>
		/// <param name="deltaTime">経過時間(秒)。デフォルトでは1フレームの経過時間</param>
		/// <returns>移動後のキャラクターの座標。</returns>
		const Vector3& Execute(Vector3& moveSpeed, float deltaTime = GameTime().GetFrameDeltaTime());
	public:
		/// <summary>
		/// 座標の取得。
		/// </summary>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// 座標を設定。
		/// </summary>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// 剛体を取得。
		/// </summary>
		CRigidBody* GetBody()
		{
			return &m_rigidBody;
		}
		/// <summary>
		/// 剛体を物理エンジンから削除。
		/// </summary>
		void RemoveRigidBody();

	private:
		bool				m_isInited = false;				//初期化済み？
		Vector3				m_position = Vector3::Zero;		//座標。
		bool				m_isJump = false;				//ジャンプ中か？
		bool				m_isOnGround = true;			//地面上にいるか？
		CCapsuleCollider	m_collider;						//コライダー。
		float				m_radius = 0.0f;				//カプセルコライダーの半径。
		float				m_height = 0.0f;				//カプセルコライダーの高さ。
		CRigidBody			m_rigidBody;					//剛体。

	};///class CCharacterController; 

}///namespace Engine;
