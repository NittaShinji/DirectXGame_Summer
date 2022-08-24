#include "Enemy.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include <cassert>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

//初期化
void Enemy::Initialize(Model* model, const Vector3& position)
{
	// NULLチェック
	assert(model);

	enemyModel_ = model;

	//テクスチャ読み込み
	enemyHandle_ = TextureManager::Load("enemy.png");

	////引数で受け取った速度をメンバ変数に代入
	//enemyVelocity_ = velocity;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期化
	enemyWorldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	enemyWorldTransform_.translation_ = position;

	ApproachInitialize();
}

//更新
void Enemy::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	switch (phase_)
	{
	case Phase::Approach:
	default:
		PhsaeApproach();

		break;

	case Phase::Leave:

		PhsaeLeave();
		break;
	}

	Vector3 enemyVelocity_ = { 0, 0, kEnemySpeed };

	//座標を移動させる(1フレーム分の移動量を足しこむ)
	//enemyWorldTransform_.translation_ -= enemyVelocity_;

#pragma region キャラクターの攻撃

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

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
	matRot.Matrix4RotationX(enemyWorldTransform_.rotation_.x);
	matRot.Matrix4RotationY(enemyWorldTransform_.rotation_.y);
	matRot.Matrix4RotationZ(enemyWorldTransform_.rotation_.z);

	enemyWorldTransform_.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans.Matrix4Translation(
		enemyWorldTransform_.translation_.x,
		enemyWorldTransform_.translation_.y,
		enemyWorldTransform_.translation_.z);

	enemyWorldTransform_.matWorld_ = matScale;
	enemyWorldTransform_.matWorld_ *= matRot;
	enemyWorldTransform_.matWorld_ *= matTrans;

	//WorldMatrix(enemyWorldTransform_.matWorld_, matScale, matRot, matTrans);

	//ワールド行列を転送
	enemyWorldTransform_.TransferMatrix();

#pragma endregion

}

void Enemy::Fire()
{

	//　弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = Root(velocity, enemyWorldTransform_);

	//弾を生成し、初期化
		//PlayerBullet* newBullet = new PlayerBullet();
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(enemyModel_, enemyWorldTransform_.translation_, velocity);

	//弾を登録する
	//bullet_.reset(newBullet);
	bullets_.push_back(std::move(newBullet));

}

//描画
void Enemy::Draw(const ViewProjection& viewProjection)
{
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, enemyHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachInitialize()
{
	//発射タイマーを初期化
	bulletCoolTimer = kFireInterval;
}

void Enemy::PhsaeApproach()
{
	//移動(ベクトルを加算)
	enemyWorldTransform_.translation_ -= ApproachSpeed;
	//既定の位置に到達したら離脱
	if (enemyWorldTransform_.translation_.x < -40.0f)
	{
		phase_ = Phase::Leave;
	}

	//発射タイマーをカウントダウン
	bulletCoolTimer--;
	//指定時間に達した
	if (bulletCoolTimer <= 0)
	{
		//弾を発射
		Fire();
		//発射タイマーを初期化
		bulletCoolTimer = kFireInterval;
	}
}

void Enemy::PhsaeLeave()
{
	//移動(ベクトルを加算)
	enemyWorldTransform_.translation_ += LeaveSpeed;
	if (enemyWorldTransform_.translation_.x > 40.0f)
	{
		phase_ = Phase::Approach;
	}
}

Vector3 Enemy::Root(Vector3 velocity, WorldTransform worldTransform_)
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