#pragma once

#include "util/Stopwatch.h"
#include "graphics/GraphicsConfig.h"

namespace Engine {

	class CGraphicsEngine;
	class CPhysicsWorld;
	class CSoundEngine;

	class TkEngine : Noncopyable {
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		TkEngine();
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		~TkEngine();
		/// <summary>
		/// ��������B
		/// </summary>
		void Release();
		/// <summary>
		/// �Q�[���̏����B
		/// </summary>
		void GameUpdate();
		/// <summary>
		/// �t���[���̊J�n���ɌĂ΂�鏈���B
		/// </summary>
		void BeginFrame();
		/// <summary>
		/// �t���[���̏I�����ɌĂ΂�鏈���B
		/// </summary>
		void EndFrame();
		/// <summary>
		/// �Q�[���G���W���̏������B
		/// </summary>
		void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
		//�C���X�^���X���擾����B
		static TkEngine& Instance()
		{
			static TkEngine instance;
			return instance;
		}
	public:		//Get�֐��n��
		//�Q�[���^�C���B
		const CGameTime& GetGameTime()
		{
			return m_gameTime;
		}
		//�O���t�B�b�N�G���W���B
		CGraphicsEngine* GetGraphicsEngine()
		{
			return m_graphicsEngine;
		}
		//�������[���h�B
		CPhysicsWorld& GetPhyshicsWorld()
		{
			return m_physicsWorld;
		}
		//�T�E���h�G���W���B
		CSoundEngine& GetSoundEngine()
		{
			return m_soundEngine;
		}
	private:
		void GameSleep();

	private:
		CGraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
		CPhysicsWorld m_physicsWorld;					//�������[���h�B
		CSoundEngine m_soundEngine;						//�T�E���h�G���W���B
		GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B
		CGameTime			m_gameTime;					//�Q�[���^�C���B
	public:
		Stopwatch			m_sw;						//�^�C�}�[�N���X�B
	};

	/// <summary>
	/// �G���W�����擾�B
	/// </summary>
	static inline TkEngine& GameEngine()
	{
		return TkEngine::Instance();
	}
	/// <summary>
	/// �Q�[���^�C�����擾�B
	/// </summary>
	static inline const CGameTime& GameTime()
	{
		return GameEngine().GetGameTime();
	}
	/// <summary>
	/// �O���t�B�b�N�G���W�����擾�B
	/// </summary>
	static inline CGraphicsEngine* GraphicsEngine()
	{
		return GameEngine().GetGraphicsEngine();
	}

	/// <summary>
	/// �������[���h���擾�B
	/// </summary>
	static inline CPhysicsWorld& PhysicsWorld()
	{
		return GameEngine().GetPhyshicsWorld();
	}
	/// <summary>
	/// �T�E���h�G���W�����擾�B
	/// </summary>
	static inline CSoundEngine& SoundEngine()
	{
		return GameEngine().GetSoundEngine();
	}
}