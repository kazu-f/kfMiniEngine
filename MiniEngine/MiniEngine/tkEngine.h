#pragma once

#include "util/Stopwatch.h"
#include "HID/GamePad.h"

namespace Engine {

	class GraphicsEngine;

	class TkEngine : Noncopyable {
	public:
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		~TkEngine();
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
	private:
		GraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
		GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B

	public:
		Stopwatch			m_sw;						//�^�C�}�[�N���X�B
	};

	extern TkEngine* g_engine;	//TK�G���W���B
}