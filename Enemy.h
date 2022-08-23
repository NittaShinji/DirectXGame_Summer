#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "DebugCamera.h"
#include "ViewProjection.h"
#include "Debugtext.h"
#include "Input.h"

//�s���t�F�[�Y

class Enemy
{
	
public:

	enum class Phase
	{
		Cube,	//�u���b�N���
		Move,	//�ړ�����
		Dead	//���S����
	};

	Enemy();
	~Enemy();

	//������
	void Initialize(Model* model, const Vector3& position);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

	void PhsaeCube();
	void PhsaeMove();


private:

	// ���[���h�ϊ��f�[�^
	WorldTransform enemyWorldTransforms_[100];
	// ���f��
	Model* enemyModel_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t blockHandle_ = 0u;
	uint32_t enemyHandle_ = 0u;

	// �C���v�b�g
	Input* input_ = nullptr;
	// �f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	// �G�̑��x
	const float kEnemySpeed = 0.1f;
	
	//�t�F�[�Y
	Phase phase_ = Phase::Cube;

	//�u���b�N�����Ă��邩�ǂ���
	bool isblock[100] = { 0 };

	bool isMove[100] = { 0 };

	//�ڋ߃t�F�[�Y�̑��x
	Vector3 CubeSpeed = { 0,0,0.1 };

	//���E�t�F�[�Y�̑��x
	Vector3 MoveSpeed = { 0,0,0.1 };

	//���S�t�F�[�Y�̑��x
	float deadSpeed = 0.1;

	//�ړ��̐؂�ւ��t���O
	bool moveChange[100] = { 0 };

	//����
	static const int32_t kLifeTime = 400 * 5;

	//�f�X�^�C�}�[
	int32_t blockTimer_[100];
};

