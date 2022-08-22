#pragma once

#include "Model.h"
#

//���L�����̒e
class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

	void Initialize(Model* model, const Vector3& position);
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

private:

	// ���[���h�ϊ��f�[�^
	WorldTransform bulletWorldTransform_;
	// ���f��
	Model* buleetModel_ = nullptr;

	// �e�N�X�`���n���h��
	uint32_t bulletHandle_ = 0u;

};

