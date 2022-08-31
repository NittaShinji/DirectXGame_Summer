#include "Select.h"
#include "Block.h"
#include <cassert>

Select::Select(){}
Select::~Select(){}

void Select::Initialize(Model* model)
{
	// NULL�`�F�b�N
	assert(model);

	//���f�����C���v�b�g
	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("testBlock3.png");

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0,0,0};
}

void Select::receive()
{
	assert(block_);

	boxPos = block_->GetSelectPosition();

}

void Select::Update()
{	
	receive();
	
	worldTransform_.translation_ = boxPos;
	

#pragma region �s��̍X�V
	//�X�P�[�����O�p�s���錾
	Matrix4 matScale;
	//�����p��]�s���錾
	Matrix4 matRot;
	//�e���p��]�p�s���錾
	Matrix4 matRotX, matRotY, matRotZ;
	//���W�p�s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

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

	//���[���h�s���]��
	worldTransform_.TransferMatrix();

#pragma endregion

}
void Select::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}