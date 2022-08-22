#include "Player.h"
#include "input.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULL�`�F�b�N
	assert(model);
	assert(textureHandle);

	//�󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();

}

void Player::Update()
{

#pragma region �L�����N�^�[�̐��񏈗�

	const float rootSpeed = 0.01f;

	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 playerRoot = { 0,0,0 };

	if (input_->PushKey(DIK_C))
	{
		playerRoot.x = rootSpeed;
	}
	else if (input_->PushKey(DIK_Z))
	{
		playerRoot.x = -rootSpeed;
	}
	else
	{
		playerRoot.x = 0.0f;
	}

	worldTransform_.rotation_.x += playerRoot.x;

#pragma endregion

#pragma region �L�����N�^�[�̈ړ�����

	const float playerSpeed = 0.05f;

	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 playerMove = { 0,0,0 };

	if (input_->PushKey(DIK_RIGHT))
	{
		playerMove.x = playerSpeed;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		playerMove.x = -playerSpeed;
	}
	else
	{
		playerMove.x = 0.0f;
	}

	worldTransform_.translation_.x += playerMove.x;

#pragma endregion

#pragma region �ړ�����

	//�ړ����E���W
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 34.0f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

#pragma endregion

#pragma region �L�����N�^�[�̍U��

	//�L�����N�^�[�̍U��
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	/*if (bullet_)
	{
		bullet_->Update();
	}*/

#pragma endregion

#pragma region �s��̍X�V
	//�X�P�[�����O�p�s���錾
	Matrix4 matScale;
	//�����p��]�s���錾
	Matrix4 matRot;
	//�e���p��]�p�s���錾
	Matrix4 matRotX, matRotY, matRotZ;
	//���W�p�s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//�L�����N�^�[�ړ�����
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float scaleZ = 1.0f;

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//��]�s����s��ɐݒ�
	matRot.Matrix4RotationX(worldTransform_.rotation_.x);
	matRot.Matrix4RotationY(worldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(worldTransform_.rotation_.z);

	worldTransform_.matWorld_.IdentityMatrix();

	//���W���s��ɐݒ�
	matTrans.Matrix4Translation(
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	worldTransform_.matWorld_ = matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	//WorldMatrix(worldTransform_.matWorld_, matScale, matRot, matTrans);

	//�e�s��Ɗ|���Z���
	if (worldTransform_.parent_ != nullptr)
	{
		worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	}

	//���[���h�s���]��
	worldTransform_.TransferMatrix();

#pragma endregion


	debugText_->SetPos(50, 130);
	debugText_->Printf("worldTranslation[0]:(%f,%f,%f)",
		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

#ifdef DEBUG

	if (input_->TriggerKey(Q))
	{
		isDebugCameraActive_ = true;
	}
#endif
}

void Player::Attack()
{
	//�e�̐������A������
	if (input_->PushKey(DIK_SPACE))
	{
		//���L�����̍��W���R�s�[
		Vector3 position = worldTransform_.translation_;

		//�e�𐶐����A������
		//PlayerBullet* newBullet = new PlayerBullet();
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_ ,worldTransform_.translation_);

		//�e��o�^����
		//bullet_.reset(newBullet);
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(ViewProjection& viewProjection, uint32_t textureHandle)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	/*if (bullet_)
	{
		bullet_->Draw(viewProjection);
	}*/
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}