#include "stdafx.h"
#include "system/system.h"
#include "GameScene.h"





#define USE_UNITY_CHAN 0
#define USE_NINJA 1
#define USE_ROBO 2

#define USE_MODEL 2


///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	GameScene game;
	game.Init();

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();
	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		
		
		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////

		//�X�V�����B
		game.Update();
		//�`�揈���B
		game.Draw(renderContext);


		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		//�����_�����O�I���B
		g_engine->EndFrame();
	}
	return 0;
}


