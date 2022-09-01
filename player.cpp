#include "Player.h"
#include "RailCamera.h"
#include <cassert>
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULLチェック
	assert(model);
	assert(textureHandle);
	//assert(railCamera_);
	
	//受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//レールカメラのワールド行列を取得
	//railWorldTransform = railCamera_->GetWorldTransform();

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	/*worldTransform_.parent_ = &railWorldTransform;*/

	playerHp = 1;

}

void Player::Update()
{
	//レールカメラのワールド行列を取得
	//railWorldTransform = railCamera_->GetWorldTransform();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet){
			return bullet->IsDead();
	});

#pragma region キャラクターの旋回処理

	const float rootSpeed = 0.01f;

	// キャラクターの移動ベクトル
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

#pragma region キャラクターの移動処理

	const float playerSpeed = 0.5f;

	// キャラクターの移動ベクトル
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

#pragma region 移動制限

	//移動限界座標
	const float kMoveLimitX = 190.0f;
	const float kMoveLimitZ = -20.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.z = max(worldTransform_.translation_.z, -kMoveLimitZ);
	worldTransform_.translation_.z = min(worldTransform_.translation_.z, +kMoveLimitZ);

#pragma endregion

#pragma region キャラクターの攻撃

	//キャラクターの攻撃
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	/*if (bullet_)
	{
		bullet_->Update();
	}*/

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
	float scaleX = 9.0f;
	float scaleY = 9.0f;
	float scaleZ = 9.0f;

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
	/*if (worldTransform_.parent_ != nullptr)
	{
		worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	}*/

	//レールカメラのワールド行列を取得
	/*Matrix4 railCameraMatWorld = railCamera_->GetMatWorld();
	worldTransform_.matWorld_ *= railCameraMatWorld;*/

	//worldTransform_.matWorld_ *= 

	//ワールド行列を転送
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
	//発射タイマーをカウントダウン
	bulletCoolTimer--;
	//指定時間に達した
	if (bulletCoolTimer <= 0)
	{
		//弾の生成し、初期化
		if (input_->PushKey(DIK_RETURN))
		{
			////自キャラの座標をコピー
			//Vector3 position = worldTransform_.translation_;

			//　弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = Root(velocity, worldTransform_);

			//弾を生成し、初期化
			//PlayerBullet* newBullet = new PlayerBullet();
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//newBullet->Initialize(model_ ,worldTransform_.translation_,velocity);

			//自機の平行移動成分の情報を取得
			worldResultTransform.x = worldTransform_.matWorld_.m[3][0];
			worldResultTransform.y = worldTransform_.matWorld_.m[3][1];
			worldResultTransform.z = worldTransform_.matWorld_.m[3][2];

			newBullet->Initialize(model_, worldResultTransform, velocity);

			//弾を登録する
			//bullet_.reset(newBullet);
			bullets_.push_back(std::move(newBullet));
		}

		//発射タイマーを初期化
		bulletCoolTimer = kFireInterval;

	}
}

void Player::Draw(ViewProjection& viewProjection, uint32_t textureHandle)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::TimerInitialize()
{
	//発射タイマーを初期化
	bulletCoolTimer = kFireInterval;
}

void Player::ReadyFire()
{

}

Vector3 Player::Root(Vector3 velocity, WorldTransform worldTransform_)
{	
	//単位ベクトル
	Vector3 frontVec = { 0.0f, 0.0f, 1.0f };
	//結果用のベクトル
	Vector3 resultVec = { 0.0f, 0.0f, 0.0f };

	resultVec.x = (cos(worldTransform_.rotation_.y) * frontVec.x +
		sin(worldTransform_.rotation_.y) * frontVec.z);
	resultVec.z = (-sinf(worldTransform_.rotation_.y) * frontVec.x +
		cosf(worldTransform_.rotation_.y) * frontVec.z);

	return resultVec;

}

Vector3 Player::GetLocalPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
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
	//半径を入れる変数
	float playerRadius;
	//ワールド行列の平行移動成分を取得(ワールド座標)
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