#include "Select.h"
#include "Block.h"
#include <cassert>

Select::Select(){}
Select::~Select(){}

void Select::Initialize(Model* model)
{
	// NULLチェック
	assert(model);

	//モデルをインプット
	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("testBlock3.png");

	// シングルトンインスタンスを取得する
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
	

#pragma region 行列の更新
	//スケーリング用行列を宣言
	Matrix4 matScale;
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸用回転用行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;
	//座標用行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//スケーリング倍率を行列に設定
	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//回転行列を行列に設定
	matRot.Matrix4RotationX(worldTransform_.rotation_.x);
	matRot.Matrix4RotationY(worldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(worldTransform_.rotation_.z);

	worldTransform_.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans.Matrix4Translation(
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	worldTransform_.matWorld_ = matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	//ワールド行列を転送
	worldTransform_.TransferMatrix();

#pragma endregion

}
void Select::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}