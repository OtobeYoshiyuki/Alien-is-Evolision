#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
class MiniMap
{
private:
	//�~�j�}�b�v�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_world;

	//�~�j�}�b�v�p�̔z��
	std::vector<std::vector<OtobeLib::ITextureBase*>> m_miniMapTex;

	//�����~�j�}�b�v�p�̔z��
	std::vector<OtobeLib::ITextureBase*>	m_moveTex;

	//RECT�A�j���[�V�����p�̃~�j�}�b�v�̔z��
	std::vector<OtobeLib::ITextureBase*>	m_miniAnimeTex;

	//RECT�A�j���[�V�����p�̃Q�[���f�[�^�p�̔z��
	std::vector<OtobeLib::ITextureBase*>	m_gameAnimeTex;

	//�X�e�[�W�̃Q�[���f�[�^�p�̔z��
	std::vector<OtobeLib::ITextureBase*>	m_gameTex;
public:
	//�R���X�g���N�^
	MiniMap();

	//�f�X�g���N�^
	~MiniMap() {};

	//������
	void Initialize(std::vector<std::vector<std::string>>& before,std::vector<std::vector<int>>& after);

	//�X�V
	void Update();

	//�`��
	void Render();

public:
	//���[���h�s����擾����
	const DirectX::SimpleMath::Matrix& GetWorld()const { return m_world; }

	//�z��̏���ݒ肷��
	void ArraySetUp();

	//�z��̒��g���`�F�b�N����
	const bool CheckArray(const std::string& target);

	//�z��̒��g���q�����ǂ����m�F����
	const bool CheckChild(const std::string& child);

	//�z��̒��g���v���C���[�֘A�̂��̂��ǂ���
	const bool CheckPlayers(const std::string& players);

	//�~�j�}�b�v�̔z����擾����
	std::vector<std::vector<OtobeLib::ITextureBase*>>& GetMiniMap() { return m_miniMapTex; }

private:
	//�~�j�}�b�v�̃C���X�^���X�𐶐�����
	void CreateMiniMap(std::unique_ptr<OtobeLib::ITextureBase>& pchild,const int& col,
		const DirectX::XMVECTORF32& color, const std::string& name,const RECT& rect);
};

