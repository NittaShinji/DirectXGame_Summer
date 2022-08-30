#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"


class Block
{
public:

	//�`�ԃt�F�[�Y
	enum class Form
	{
		Block = 0,		//���܂�Ă��Ȃ����
		IsSelected,		//�I������Ă�����(�u���b�N��Ԃ̂܂�)
		WasSelected,	//�I�����ꂽ���	(�u���b�N���Ȃ����)
		None,			//�����Ȃ����
		Slime,			//�X���C��
	};

	//�s���t�F�[�Y
	enum class Phase
	{
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};

	Block();
	~Block();

	//������
	void Initialize(Model* model, const Vector3& position);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

	//���[���h���W���擾
	Vector3 GetLocalPosition();

private:

	static const int blockWidth = 30;
	static const int blockHeight = 30;

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
	uint32_t selectHandle_ = 0u;
	uint32_t wasSelectHandle_ = 0u;
	uint32_t slimeHandle_ = 0u;

	//��ԕω��ϐ�
	Form form_[blockWidth][blockHeight] = { Form::Block };
	//Form form_ = { Form::Block };

	//�L�����N�^�[�̑傫��
	float scaleX = 8.0f;
	float scaleY = 8.0f;
	float scaleZ = 8.0f;

	int prevBlockX;
	int prevBlockY;

	//�I���\���ǂ����̕ϐ�
	bool changedSelect;
	bool wasChangedSelect;

	//�I���J�[�\���̃N�[���^�C�}�[�̐ݒ莞��
	static const int32_t kSelectTime = 17;

	//�I���J�[�\���̃N�[���^�C�}�[
	int32_t selectTimer_ = kSelectTime;
	int32_t wasSelectTimer_ = kSelectTime;


	//int selectTimer;

	//�u���b�N���I������Ă��邩
	//int isSelected[blockWidth][blockHeight] = { 0 };

	////�X�̃u���b�N�p�̏�ԕω��ϐ�
	//int formChange[blockWidth][blockHeight] = { 0 };

};

