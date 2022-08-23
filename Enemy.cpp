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

	//ワールドトランスフォームの初期化
	enemyWorldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	enemyWorldTransform_.translation_ = position;
}



//更新
void Enemy::Update()
{
	/*switch (phase_)
	{
	case Phase::Approach:
	default:
		PhsaeApproach();
		
		break;

	case Phase::Leave:
		
		PhsaeLeave();
		break;
	}*/

	//メンバ関数ポインタに入っている関数を呼び出す
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

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

//描画
void Enemy::Draw(const ViewProjection& viewProjection)
{
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, enemyHandle_);
}

void Enemy::PhsaeApproach()
{
	//移動(ベクトルを加算)
	enemyWorldTransform_.translation_ -= ApproachSpeed;
	//既定の位置に到達したら離脱
	if (enemyWorldTransform_.translation_.z < -40.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::PhsaeLeave()
{
	//移動(ベクトルを加算)
	enemyWorldTransform_.translation_ += LeaveSpeed;
	if (enemyWorldTransform_.translation_.z > 40.0f)
	{
		phase_ = Phase::Approach;
	}
}

//staticで宣言したメンバ関数ポインタテーブルの実態
void (Enemy::* Enemy::spFuncTable[])() = {
	&Enemy::PhsaeApproach,	//接近
	&Enemy::PhsaeLeave,		//離脱
};