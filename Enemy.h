#pragma once

#include "Model.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	//������
	void Initialize(Model* model, const Vector3& position);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

private:

	// ���[���h�ϊ��f�[�^
	WorldTransform enemyWorldTransform_;
	// ���f��
	Model* enemyModel_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t enemyHandle_ = 0u;

	// �G�̑��x
	const float kEnemySpeed = 0.1f;
	
};


