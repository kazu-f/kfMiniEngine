#include "stdafx.h"
#include "system/system.h"
#include "Scenes/GameScene.h"
#include "Scenes/Title/TitleScene.h"
#include "Scenes/DebugScene.h"
#include "Scenes/LightingDemoScene.h"

#define GAME_SCENE 0
#define DEBUG_SCENE 1
#define LIGHTDEMO_SCENE 2

#define START_SCENE DEBUG_SCENE

void SetInitParam(SInitParam& initParam)
{
	initParam.frameBuffer_W = FRAME_BUFFER_W;	//�t���[���o�b�t�@�̃T�C�Y(��)
	initParam.frameBuffer_H = FRAME_BUFFER_H;	//�t���[���o�b�t�@�̃T�C�Y(����)
	initParam.gameObjectPrioMax = 20;			//�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔�B

	initParam.graphicsConf.shadowConf.isEnable = true;		//�V���h�E�}�b�v�̗L�����t���O�B
	initParam.graphicsConf.shadowConf.shadowMapWidth = 4096;
	initParam.graphicsConf.shadowConf.shadowMapWidth = 4096;
	initParam.graphicsConf.shadowConf.shadowAreas[0] = 2000.0f;		//�V���h�E�}�b�v�̗L���͈�(�ߌi)
	initParam.graphicsConf.shadowConf.shadowAreas[1] = 4000.0f;		//�V���h�E�}�b�v�̗L���͈�(���i)
	initParam.graphicsConf.shadowConf.shadowAreas[2] = 20000.0f;	//�V���h�E�}�b�v�̗L���͈�(���i)
	//initParam.graphicsConf.shadowConf.depthOffset[0] = 0.01f;
	//initParam.graphicsConf.shadowConf.depthOffset[1] = 0.02f;
	//initParam.graphicsConf.shadowConf.depthOffset[2] = 0.02f;
	initParam.graphicsConf.shadowConf.lightHeight = 5000.0f;
	initParam.graphicsConf.postEffectConf.tonemap.isEnable = false;		//�g�[���}�b�v�L�����t���O�B
	initParam.graphicsConf.postEffectConf.tonemap.luminance = 0.24f;	//���邳�B
	initParam.graphicsConf.postEffectConf.isBloom = false;	//�u���[���̗L�����t���O�B
	initParam.graphicsConf.postEffectConf.isFxaa = true;	//�A���`�G�C���A�X�L�����t���O�B
}

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	SInitParam initParam;
	SetInitParam(initParam);

	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"), initParam);

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

#if START_SCENE == DEBUG_SCENE
	NewGO<DebugScene>(0);
#elif START_SCENE == LIGHTDEMO_SCENE
	NewGO<LightingDemoScene>(0);
#elif START_SCENE == GAME_SCENE
	//NewGO<GameScene>(0);
	NewGO<TitleScene>(0);
#endif
	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		/*
			���̖ڕW
			�G���W���̒��ōX�V�������܂Ƃ߂āA
			���̒��ŌĂяo���B
		*/
		GameEngine().GameUpdate();
		
		
		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////



		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		//�����_�����O�I���B
	}
	return 0;
}


