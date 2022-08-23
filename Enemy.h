#pragma once

#include "Model.h"

//�s���t�F�[�Y

class Enemy
{
	
public:

	enum class Phase
	{
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};

	Enemy();
	~Enemy();

	//������
	void Initialize(Model* model, const Vector3& position);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

	//�G�̍U���t�F�[�Y
	void PhsaeApproach();
	void PhsaeLeave();

private:

	// ���[���h�ϊ��f�[�^
	WorldTransform enemyWorldTransform_;
	// ���f��
	Model* enemyModel_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t enemyHandle_ = 0u;

	// �G�̑��x
	const float kEnemySpeed = 0.1f;
	
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
	//�ڋ߃t�F�[�Y�̑��x
	Vector3 ApproachSpeed = { 0,0,0.1 };
	//���E�t�F�[�Y�̑��x
	Vector3 LeaveSpeed = { 0,0,0.1 };

	//�����o�֐��|�C���^�̃e�[�u��
	static void (Enemy::*spFuncTable[])();

};

