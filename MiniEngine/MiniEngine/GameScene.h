#pragma once

//�@�\�̎��^�]�p�N���X�B

class GameScene:public IGameObject
{
public:
	GameScene();
	~GameScene();
	bool Start();		//�������B
	void Release();		//����������K�v�Ȃ炱��B
	void Update();		//�X�V�����B
	void ForwardRender(RenderContext& rc) override;		//�`�揈���B

private:
	//Light light;		//���C�g�B
	Model	robotModel;			//���f���B
	ModelInitData initData;
	Skeleton m_skeleton;
	CAnimation m_animation;
	std::vector<CAnimationClipPtr> m_animationClips;	//�A�j���[�V�����N���b�v�̔z��B
	std::vector<prefab::CDirectionLight*> m_lightArray;
	Vector3 m_lightDir = Vector3::Zero;
	bool isPBR = true;
};

