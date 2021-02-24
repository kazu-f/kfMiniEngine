
/// <summary>
/// サウンドエンジン
/// </summary>

#pragma once


#define INPUTCHANNELS 2		//入力チャンネル数。
#define OUTPUTCHANNELS 8	//最大出力チャンネル数。
namespace Engine {

	namespace prefab {

	}
	/// <summary>
	/// サウンドエンジンクラス。
	/// </summary>
	class CSoundEngine
	{
	public:
		CSoundEngine();		//コンストラクタ。
		~CSoundEngine();	//デストラクタ。
		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// 解放。
		/// </summary>
		void Release();
		/// <summary>
		/// 更新。
		/// </summary>
		void Update();

		//TODOサウンドソース関連。
		/// <summary>
		/// 3Dサウンドソースを追加。
		/// </summary>
		
		/// <summary>
		/// 3Dサウンドソースを解放。
		/// </summary>

		/// <summary>
		/// サウンドリスナーの座標を設定。
		/// </summary>
		/// <remarks>3D音源を使っているときの計算に使われる。</remarks>
		/// <param name="pos">座標。</param>
		void SetListenerPosition(const Vector3& pos)
		{
			m_listenerPosition = pos;
		}
		/// <summary>
		/// サウンドリスナーの前方向を設定。
		/// </summary>
		/// <remarks>3D音源を使っているときの計算に使われる。</remarks>
		/// <param name="front">前方向ベクトル。</param>
		void SetListenerFront(const Vector3& front)
		{
			m_listener.OrientFront.x = front.x;
			m_listener.OrientFront.y = front.y;
			m_listener.OrientFront.z = front.z;
		}
		/// <summary>
		/// サウンドリスナーの上方向を設定。
		/// </summary>
		/// <remarks>3D音源を使っているときの計算に使われる。</remarks>
		/// <param name="up">上方向ベクトル。</param>
		void SetListenerUp(const Vector3& up)
		{
			m_listener.OrientTop.x = up.x;
			m_listener.OrientTop.y = up.y;
			m_listener.OrientTop.z = up.z;
		}
		//Todo XAudio2のソースボイス作成処理。

		/// <summary>
		/// マスターボイスを取得。
		/// </summary>
		IXAudio2MasteringVoice* GetMasteringVoice()
		{
			return m_masteringVoice;
		}
		/// <summary>
		/// サブミックスボイス。
		/// </summary>
		IXAudio2SubmixVoice* GetSubmixVoice()
		{
			return m_submixVoice;
		}
		/// <summary>
		/// 出力チャンネルの数を取得。
		/// </summary>
		int GetNumChannel()const
		{
			return m_nChannels;
		}
		//todo 波形データバンクを取得。

		/// <summary>
		/// サウンドエンジンが利用可能かどうか判定。
		/// </summary>
		/// <remarks>trueが返ってきたら利用可能。</remarks>
		bool IsAvailable()const
		{
			return m_isInited;
		}

	private:
		IXAudio2* m_xAudio2 = nullptr;							//XAudio
		IXAudio2MasteringVoice* m_masteringVoice = nullptr;		//マスターボイス。
																//ソースボイスとサブミックスボイスの出力を
																//すべてまとめてサウンドカードへ送る機能を持つ。
		X3DAUDIO_HANDLE m_hx3DAudio;							//3Dオーディオのハンドル。
		IUnknown* m_reverbEffect = nullptr;						//リバーブエフェクト。
		IXAudio2SubmixVoice* m_submixVoice = nullptr;			//サブミックスボイス。
																//関連付けた複数のソースボイスの出力を
																//まとめてエフェクトを掛ける機能を持つ。
		DWORD m_channelMask = 0;								//オーディオチャンネルマスク。
		DWORD m_nChannels = 0;									//チャンネル数。
		Vector3 m_listenerPosition = Vector3::Zero;				//オーディオリスナーの座標。
		float m_fListenerAngle = 0.0f;							//リスナーの向き？
		bool m_UseListenerCone = false;
		bool m_fUseInnerRadius = false;
		bool m_useRedirectToLFE = false;
		bool m_isInited = false;								//初期化済み？
		X3DAUDIO_LISTENER m_listener;							//サウンドリスナー。
		X3DAUDIO_CONE m_emitterCone;							//エミッターコーン？
		//TODO サウンドソースのリストと波形データバンク。

	};///class CSoungEngine;

}///namespace Engine;

