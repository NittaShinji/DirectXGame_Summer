#include "Skydome.h"
#include <cassert>

Skydome::Skydome(){}

Skydome::~Skydome(){}

void Skydome::Initialize(Model* model)
{
	// NULL�`�F�b�N
	assert(model);

	//�󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//worldTransform_.scale_ = { 100,100,100 };
	worldTransform_.translation_ = {0,0,0};
}

void Skydome::Update()
{

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
	float scaleX = 100.0f;
	float scaleY = 100.0f;
	float scaleZ = 100.0f;

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

}

void Skydome::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
