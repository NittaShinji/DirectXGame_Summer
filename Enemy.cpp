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
	blockHandle_ = TextureManager::Load("suraimuBlock.png");
	enemyHandle_ = TextureManager::Load("suraimu.png");

	////引数で受け取った速度をメンバ変数に代入
	//enemyVelocity_ = velocity;

	//ワールドトランスフォームの初期化

	for (int i = 0; i < 100; i++)
	{
		enemyWorldTransforms_[i].Initialize();

		//引数で受け取った初期座標をセット
		enemyWorldTransforms_[i].translation_ = position;
		enemyWorldTransforms_[i].translation_.x = i * 2 + -10;

		blockTimer_[i] = kLifeTime + (i * 50);
	}
}

//更新
void Enemy::Update()
{
	/*switch (phase_)
	{
	case Phase::Cube:
		
	default:
		PhsaeCube();
		break;

	case Phase::Move:
		
		PhsaeMove();
		break;
	}*/

	PhsaeCube();
	PhsaeMove();
	
	Vector3 enemyVelocity_ = { 0, 0, kEnemySpeed } ;

	//座標を移動させる(1フレーム分の移動量を足しこむ)
	//enemyWorldTransform_.translation_ -= enemyVelocity_;

#pragma region 行列の更新
	for (int i = 0; i < 100; i++)
	{
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
		matRot.Matrix4RotationX(enemyWorldTransforms_[i].rotation_.x);
		matRot.Matrix4RotationY(enemyWorldTransforms_[i].rotation_.y);
		matRot.Matrix4RotationZ(enemyWorldTransforms_[i].rotation_.z);

		enemyWorldTransforms_[i].matWorld_.IdentityMatrix();

		//座標を行列に設定
		matTrans.Matrix4Translation(
			enemyWorldTransforms_[i].translation_.x,
			enemyWorldTransforms_[i].translation_.y,
			enemyWorldTransforms_[i].translation_.z);

		enemyWorldTransforms_[i].matWorld_ = matScale;
		enemyWorldTransforms_[i].matWorld_ *= matRot;
		enemyWorldTransforms_[i].matWorld_ *= matTrans;

		//WorldMatrix(enemyWorldTransforms_[i].matWorld_, matScale, matRot, matTrans);

		//ワールド行列を転送
		enemyWorldTransforms_[i].TransferMatrix();
	}

#pragma endregion

}

//描画
void Enemy::Draw(const ViewProjection& viewProjection)
{
	for (int i = 0; i < 100; i++)
	{
		/*switch (phase_)
		{
		case Phase::Cube:
		default:
			enemyModel_->Draw(enemyWorldTransforms_[i], viewProjection, blockHandle_);
			break;

		case Phase::Move:
			enemyModel_->Draw(enemyWorldTransforms_[i], viewProjection, enemyHandle_);
			break;
		}*/

		enemyModel_->Draw(enemyWorldTransforms_[i], viewProjection, blockHandle_);
		enemyModel_->Draw(enemyWorldTransforms_[i], viewProjection, enemyHandle_);
	}
	
	//enemyModel_->Draw(enemyWorldTransform_, viewProjection, enemyHandle_);
}

void Enemy::PhsaeCube()
{
	for (int i = 0; i < 100; i++)
	{
		if (--blockTimer_[i] <= 0)
		{
			isblock[i] = true;
		}

		//ブロックが壊れる処理
		/*if (input_->PushKey(DIK_2))
		{
			isblock = 1;
		}*/
		//enemyWorldTransform_.translation_ -= CubeSpeed;
		//既定の位置に到達したら離脱
		if (isblock[i] == 1)
		{
			isMove[i] = 1;
		}
	}
}

void Enemy::PhsaeMove()
{
	for (int i = 0; i < 100; i++)
	{
		if (isMove[i] == 1)
		{
			if (moveChange[i] == 0)
			{
				enemyWorldTransforms_[i].translation_ -= MoveSpeed;

				if (enemyWorldTransforms_[i].translation_.z <= -40.0f)
				{
					moveChange[i] = 1;
				}
			}

			//移動(ベクトルを加算)

				//enemyWorldTransform_.translation_ -= MoveSpeed;
			if (moveChange[i] == 1)
			{
				enemyWorldTransforms_[i].translation_ += MoveSpeed;

				if (enemyWorldTransforms_[i].translation_.z >= 40.0f)
				{
					moveChange[i] = 0;
				}
			}
		}
	}
}