#include "Player.h"
#include "input.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULLチェック
	assert(model);
	assert(textureHandle);

	//受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ワールド変換の初期化
	worldTransform_.Initialize();

}

void Player::Update()
{
	
#pragma region キャラクターの移動処理

	const float playerSpeed = 0.05f;

	// キャラクターの移動ベクトル
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

	if (input_->PushKey(DIK_UP))
	{
		playerMove.y = playerSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		playerMove.y = -playerSpeed;
	}
	else
	{
		playerMove.y = 0.0f;
	}

	if (input_->PushKey(DIK_Q))
	{
		playerMove.z = playerSpeed;
	}
	else if (input_->PushKey(DIK_E))
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

#pragma region 移動制限

	//移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 34.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

#pragma endregion

#pragma region 行列の更新
	//スケーリング用行列を宣言
	Matrix4 matScale;
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸用回転用行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;
	//座標用行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//キャラクター移動処理
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float scaleZ = 1.0f;

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

	//WorldMatrix(worldTransform_.matWorld_, matScale, matRot, matTrans);

	//親行列と掛け算代入
	if (worldTransform_.parent_ != nullptr)
	{
		worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	}

	//ワールド行列を転送
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

void Player::Draw(DebugCamera* debugCamera_, uint32_t textureHandle)
{
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
}