#include "stdafx.h"
#include "LightManager.h"
#include "prefab/light/DirectionLight.h"

namespace Engine {
	using namespace prefab;

	void CLightManager::Init()
	{
		//�A���r�G���g���C�g�̏������B
		SetAmbientLight({ 0.5f,0.5f,0.5f });
		//���C�g�̃p�����[�^�p�̒萔�o�b�t�@���쐬�B
		m_lightParamCB.Init(sizeof(SLightParam), nullptr);
		//�f�B���N�V�������C�g�p�̃X�g���N�`���o�b�t�@���쐬�B
		m_directionLightSB.Init(sizeof(SDirectionLight), MAX_DIRECTION_LIGHT, nullptr);
	}

	void CLightManager::AddLight(prefab::CLightBase* light)
	{

		//���C�g�̐U�蕪��
		const std::type_info& typeInfo = typeid(*light);
		if (typeInfo == typeid(CLightBase)) {

		}
		else if (typeInfo == typeid(CDirectionLight)) {
			//�o�^�ς݂����ׂ�B
			auto findIt = std::find(m_directionLidhts.begin(), m_directionLidhts.end(), light);
			if (findIt == m_directionLidhts.end()) {
				//�V�K�o�^�B
				m_directionLidhts.push_back(reinterpret_cast<CDirectionLight*>(light));
			}
			else {
				//���ɓo�^����Ă���B
				return;
			}
		}
	}

	void CLightManager::RemoveLight(prefab::CLightBase* light)
	{
		//�o�^����������B
		const std::type_info& typeInfo = typeid(*light);
		if (typeInfo == typeid(CLightBase)) {
			
		}
		else if(typeInfo == typeid(CDirectionLight)){
			m_directionLidhts.erase(
				std::remove(m_directionLidhts.begin(),m_directionLidhts.end(),light),
				m_directionLidhts.end()			
			);
		}
	}

	void CLightManager::LightUpdate()
	{
		//�f�B���N�V�������C�g�̃X�g���N�`���[�o�b�t�@���X�V�B


	}

	void CLightManager::Render(RenderContext& rc)
	{
		//���C�g�p�����[�^�̃f�[�^���X�V�B
		m_lightParamCB.CopyToVRAM(&m_lightParam);
		//�f�B���N�V�������C�g�̃f�[�^���X�V�B
		m_directionLightSB.Update(&m_rawDirectionLight);
	}

	void CLightManager::SendLightDataToGPU(RenderContext& rc)
	{
		//���ɏ����Ȃ�������B
	}

}	////////namespace Engine
