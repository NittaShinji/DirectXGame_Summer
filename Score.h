#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Debugtext.h"
#include "ViewProjection.h"

class Score
{
public:
	Score();
	~Score();

	//������
	void Initialize(Model* model);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

	void GetScore(int score);

	void UpdateTransform();

private:

	// ���[���h�ϊ��f�[�^
	//WorldTransform worldTransforms_[10];
	WorldTransform worldTransform_;

	// ���f��
	Model* model_ = nullptr;

	// �e�N�X�`���n���h��
	//uint32_t textureHandle_ = 0u;

	// �����n���h��
	uint32_t numHandle_[10];

	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
	
	int score;

	//�X�R�A�̊���ϐ�
	int divideNum;

	/// <summary>
	/// �X�R�A��z��̗v�f���ɕϊ�
	/// </summary>
	  
	
	
	int scoreToCount = 0;
	int scoreToCountTen = 0;

	//���f���̑傫��
	float scaleX = 8.0f;
	float scaleY = 8.0f;
	float scaleZ = 8.0f;

};
