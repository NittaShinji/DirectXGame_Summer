#include "Monster.h"
#include "Player.h"
#include "Block.h"
#include <cassert>

Monster::Monster()
{
}

Monster::~Monster()
{
}

//初期化
void Monster::Initialize(Model* model, const Vector3& position, const Vector3& velocity, bool birthMonster)
{
	// NULLチェック
	assert(model);
	
	Model_ = model;

	//テクスチャ読み込み
	monsterHandle_ = TextureManager::Load("mario.jpg");

	//生んだかどうかの情報をコピーする
	birthMonster_ = birthMonster;

	////引数で受け取った速度をメンバ変数に代入
	//enemyVelocity_ = velocity;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	ApproachInitialize();
}

//更新
void Monster::Update()
{
	//assert(block_);

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//生成座標を取得
	/*Vector3 boxPos = block_->GetMonsterPosition();
	birthMonster_ = block_->GetBirthMonster();
	*/	
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

	
	//進めるなら移動関数を呼び出す
	if (isMoved == true)
	{
		//モンスターの衝突時コールバックを呼び出す
		OnCollisionMove();
	}
	//進めないなら方向転換関数を呼びだす
	else if (isMoved == false)
	{
		ChangeDirection();
	}

	if (firstMove == true)
	{
		Vector3 Velocity_ = { kEnemySpeed, 0, 0 };
		//座標を移動させる(1フレーム分の移動量を足しこむ)
		worldTransform_.translation_ += Velocity_;
	}
	
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
	float scaleX = 5.0f;
	float scaleY = 5.0f;
	float scaleZ = 5.0f;

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

	//WorldMatrix(enemyworldTransform_.matWorld_, matScale, matRot, matTrans);

	//ワールド行列を転送
	worldTransform_.TransferMatrix();

#pragma endregion

	//if (birthMonster_ == true)
	//{
	//	//時間経過でデス
	//	if (--deathTimer_ <= 0)
	//	{
	//		isDead_ = true;
	//	}
	//}

	debugText_->SetPos(50, 580);
	debugText_->Printf("worldTranslation:(%f,%f,%f)",
		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	debugText_->SetPos(50, 280);
	debugText_->Printf("isMoved:(%d)",
		isMoved);

	debugText_->SetPos(50, 310);
	debugText_->Printf("direction_:(%d)",
		direction_);
	/*debugText_->SetPos(150, 380);
	debugText_->Printf("worldTranslation:(%f,%f,%f)",
		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);*/

	debugText_->SetPos(50, 540);
	debugText_->Printf("deathTimer_:(%d)",
		deathTimer_);


}

Vector3 Monster::GetLocalPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos = worldTransform_.translation_;
	return worldPos;
}

void Monster::Fire()
{
	//assert(player_);

	//　弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(kBulletSpeed, 0, 0);

	//自機のワールド座標を取得
	//Vector3 playerPos = player_->GetLocalPosition();
	//敵のワールド座標を取得
	Vector3 enemyPos = worldTransform_.translation_;

	//差分ベクトル
	Vector3 difVec;
	//差分ベクトルを求める
	//difVec = playerPos - enemyPos;
	//差分ベクトルを正規化
	difVec.normalize();
	//ベクトルの長さを早さに合わせる
	const float bulletSpeed = 0.2f;
	difVec *= bulletSpeed;

	// 速度ベクトルを自機の向きに合わせて回転させる
	//velocity = Root(velocity, enemyworldTransform_);

	//弾を生成し、初期化
		//PlayerBullet* newBullet = new PlayerBullet();
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(Model_, worldTransform_.translation_, difVec);

	//弾を登録する
	//bullet_.reset(newBullet);
	bullets_.push_back(std::move(newBullet));

}

//描画
void Monster::Draw(const ViewProjection& viewProjection)
{
	Model_->Draw(worldTransform_, viewProjection, monsterHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Monster::ApproachInitialize()
{
	//発射タイマーを初期化
	bulletCoolTimer = kFireInterval;
}

void Monster::PhsaeApproach()
{
	//移動(ベクトルを加算)
	//enemyworldTransform_.translation_ -= ApproachSpeed;
	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.x < -40.0f)
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

void Monster::PhsaeLeave()
{
	//移動(ベクトルを加算)
	//enemyworldTransform_.translation_ += LeaveSpeed;
	if (worldTransform_.translation_.x > 40.0f)
	{
		phase_ = Phase::Approach;
	}
}

Vector3 Monster::Root(Vector3 velocity, WorldTransform worldTransform_)
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

void Monster::OnCollision()
{
	//デスフラグを立てる
	isDead_ = true;
}

void Monster::OnCollisionMove()
{
	firstMove = false;

	Vector3 Velocity_ = { 0, 0, 0 };

	if (direction_ == Direction::Right)
	{
		Velocity_ = { kEnemySpeed, 0, 0 };

		//座標を移動させる(1フレーム分の移動量を足しこむ)
		worldTransform_.translation_ += Velocity_;
	}

	else if (direction_ == Direction::Left)
	{
		Velocity_ = { kEnemySpeed, 0, 0 };
		//座標を移動させる(1フレーム分の移動量を足しこむ)
		worldTransform_.translation_ -= Velocity_;
	}
	else
	{

	}
}

float Monster::GetRadius()
{
	//半径を入れる変数
	float bulletRadius;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	bulletRadius = radius;
	return bulletRadius;
}

void Monster::Move()
{}

//ぶつかった際に方向を変える
void Monster::ChangeDirection()
{
	if (direction_ == Direction::Right && checkDirectionR == false)
	{
		direction_ = Direction::Left;
		checkDirectionR = true;
		checkDirectionL = false;
	}
	else if(direction_ == Direction::Left && checkDirectionL == false)
	{
		direction_ = Direction::Right;
		checkDirectionL = true;
		checkDirectionR = false;
	}
}

void Monster::GetIsMove(bool possibleMove)
{
	isMoved = possibleMove;
}