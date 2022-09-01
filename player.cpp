#include "Player.h"
#include "RailCamera.h"
#include <cassert>
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULL�`�F�b�N
	assert(model);
	assert(textureHandle);
	//assert(railCamera_);
	
	//�󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���J�����̃��[���h�s����擾
	//railWorldTransform = railCamera_->GetWorldTransform();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	/*worldTransform_.parent_ = &railWorldTransform;*/

	playerHp = 1;

}

void Player::Update()
{
	//���[���J�����̃��[���h�s����擾
	//railWorldTransform = railCamera_->GetWorldTransform();

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet){
			return bullet->IsDead();
	});

#pragma region �L�����N�^�[�̐��񏈗�

	const float rootSpeed = 0.01f;

	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 playerRoot = { 0,0,0 };

	/*if (input_->PushKey(DIK_C))
	{
		playerRoot.y = rootSpeed;
	}
	else if (input_->PushKey(DIK_Z))
	{
		playerRoot.y = -rootSpeed;
	}
	else
	{
		playerRoot.y = 0.0f;
	}*/

	//worldTransform_.rotation_.y += playerRoot.y;

#pragma endregion

#pragma region �L�����N�^�[�̈ړ�����

	const float playerSpeed = 0.5f;

	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 playerMove = { 0,0,0 };

	if (input_->PushKey(DIK_D))
	{
		playerMove.x = playerSpeed;
	}
	else if (input_->PushKey(DIK_A))
	{
		playerMove.x = -playerSpeed;
	}
	else
	{
		playerMove.x = 0.0f;
	}

	/*if (input_->PushKey(DIK_W))
	{
		playerMove.y = playerSpeed;
	}
	else if (input_->PushKey(DIK_S))
	{
		playerMove.y = -playerSpeed;
	}
	else
	{
		playerMove.y = 0.0f;
	}*/

	if (input_->PushKey(DIK_W))
	{
		playerMove.z = playerSpeed;
	}
	else if (input_->PushKey(DIK_S))
	{
		playerMove.z = -playerSpeed;
	}
	else
	{
		playerMove.z = 0.0f;
	}

	worldTransform_.translation_.x += playerMove.x;
	worldTransform_.translation_.y += playerMove.y;
	worldTransform_.translation_.z += playerMove.z;
#pragma endregion

#pragma region �ړ�����

	//�ړ����E���W
	const float kMoveLimitX = 190.0f;
	const float kMoveLimitZ = -20.0f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.z = max(worldTransform_.translation_.z, -kMoveLimitZ);
	worldTransform_.translation_.z = min(worldTransform_.translation_.z, +kMoveLimitZ);

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
	float scaleX = 9.0f;
	float scaleY = 9.0f;
	float scaleZ = 9.0f;

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
	/*if (worldTransform_.parent_ != nullptr)
	{
		worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	}*/

	//���[���J�����̃��[���h�s����擾
	/*Matrix4 railCameraMatWorld = railCamera_->GetMatWorld();
	worldTransform_.matWorld_ *= railCameraMatWorld;*/

	//worldTransform_.matWorld_ *= 

	//���[���h�s���]��
	worldTransform_.TransferMatrix();

#pragma endregion


	/*debugText_->SetPos(50, 130);
	debugText_->Printf("worldTranslation[0]:(%f,%f,%f)",
		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);*/

#ifdef DEBUG

	if (input_->TriggerKey(Q))
	{
		isDebugCameraActive_ = true;
	}
#endif
}

void Player::Attack()
{
	//���˃^�C�}�[���J�E���g�_�E��
	bulletCoolTimer--;
	//�w�莞�ԂɒB����
	if (bulletCoolTimer <= 0)
	{
		//�e�̐������A������
		if (input_->PushKey(DIK_RETURN))
		{
			////���L�����̍��W���R�s�[
			//Vector3 position = worldTransform_.translation_;

			//�@�e�̑��x
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
			velocity = Root(velocity, worldTransform_);

			//�e�𐶐����A������
			//PlayerBullet* newBullet = new PlayerBullet();
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//newBullet->Initialize(model_ ,worldTransform_.translation_,velocity);

			//���@�̕��s�ړ������̏����擾
			worldResultTransform.x = worldTransform_.matWorld_.m[3][0];
			worldResultTransform.y = worldTransform_.matWorld_.m[3][1];
			worldResultTransform.z = worldTransform_.matWorld_.m[3][2];

			newBullet->Initialize(model_, worldResultTransform, velocity);

			//�e��o�^����
			//bullet_.reset(newBullet);
			bullets_.push_back(std::move(newBullet));
		}

		//���˃^�C�}�[��������
		bulletCoolTimer = kFireInterval;

	}
}

void Player::Draw(ViewProjection& viewProjection, uint32_t textureHandle)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::TimerInitialize()
{
	//���˃^�C�}�[��������
	bulletCoolTimer = kFireInterval;
}

void Player::ReadyFire()
{

}

Vector3 Player::Root(Vector3 velocity, WorldTransform worldTransform_)
{	
	//�P�ʃx�N�g��
	Vector3 frontVec = { 0.0f, 0.0f, 1.0f };
	//���ʗp�̃x�N�g��
	Vector3 resultVec = { 0.0f, 0.0f, 0.0f };

	resultVec.x = (cos(worldTransform_.rotation_.y) * frontVec.x +
		sin(worldTransform_.rotation_.y) * frontVec.z);
	resultVec.z = (-sinf(worldTransform_.rotation_.y) * frontVec.x +
		cosf(worldTransform_.rotation_.y) * frontVec.z);

	return resultVec;

}

Vector3 Player::GetLocalPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos = worldTransform_.translation_;
	return worldPos;
}

Vector3 Player::GetWorldPosition()
{
	worldResultTransform.x = worldTransform_.matWorld_.m[3][0];
	worldResultTransform.y = worldTransform_.matWorld_.m[3][1];
	worldResultTransform.z = worldTransform_.matWorld_.m[3][2];
	return worldResultTransform;
}

float Player::GetRadius()
{
	//���a������ϐ�
	float playerRadius;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	playerRadius = radius;
	return playerRadius;
}

void Player::OnCollision(){
	playerHp -= 1;
}

bool Player::loseGame()
{
	if (playerHp == 0)
	{
		return true;
	}
}