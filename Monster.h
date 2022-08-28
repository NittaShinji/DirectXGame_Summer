#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"

class Monster
{
public:

	//�`�ԃt�F�[�Y
	enum class Form
	{
		Block = 0,		//���܂�Ă��Ȃ����
		Slime,		//�X���C��
	};

	//�s���t�F�[�Y
	enum class Phase
	{
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};

	Monster();
	~Monster();

	//������
	void Initialize(Model* model, const Vector3& position);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

private:

	static const int blockWidth = 10;
	static const int blockHeight = 10;

	// ���[���h�ϊ��f�[�^
	//WorldTransform worldTransforms_[blockWidth][blockHeight];
	WorldTransform worldTransforms_[blockWidth][blockHeight];

	// ���f��
	Model* model_ = nullptr;
	// �C���v�b�g
	Input* input_ = nullptr;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t blockHandle_ = 0u;
	uint32_t slimeHandle_ = 0u;

	//��ԕω��ϐ�
	Form form_[blockWidth][blockHeight] = { Form::Block };
	//Form form_ = { Form::Block };



	////�X�̃u���b�N�p�̏�ԕω��ϐ�
	//int formChange[blockWidth][blockHeight] = { 0 };

};

