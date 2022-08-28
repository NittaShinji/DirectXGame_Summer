#include "Monster.h"
#include <cassert>

Monster::Monster() {}
Monster::~Monster() {}

//初期化
void Monster::Initialize(Model* model, const Vector3& position)
{
	// NULLチェック
	assert(model);

	//モデルをインプット
	model_ = model;

	//テクスチャ読み込み
	blockHandle_ = TextureManager::Load("testBlock.png");
	slimeHandle_ = TextureManager::Load("suraimu.png");

	////引数で受け取った速度をメンバ変数に代入
	//enemyVelocity_ = velocity;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			worldTransforms_[i][j].Initialize();
			//引数で受け取った初期座標をセット
			worldTransforms_[i][j].translation_ = position;

			if (i > 1)
			{
				worldTransforms_[i][j].translation_.x += i * 2.0;
			}
		}
	}
}
//更新
void Monster::Update()
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{

			if (input_->PushKey(DIK_1))
			{
				if (i == 0 && j == 0)
				{
					form_[i][j] = Form::Slime;
				}
			}
			
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
			matRot.Matrix4RotationX(worldTransforms_[i][j].rotation_.x);
			matRot.Matrix4RotationY(worldTransforms_[i][j].rotation_.y);
			matRot.Matrix4RotationZ(worldTransforms_[i][j].rotation_.z);

			worldTransforms_[i][j].matWorld_.IdentityMatrix();

			//座標を行列に設定
			matTrans.Matrix4Translation(
				worldTransforms_[i][j].translation_.x,
				worldTransforms_[i][j].translation_.y,
				worldTransforms_[i][j].translation_.z);

			worldTransforms_[i][j].matWorld_ = matScale;
			worldTransforms_[i][j].matWorld_ *= matRot;
			worldTransforms_[i][j].matWorld_ *= matTrans;

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
			worldTransforms_[i][j].TransferMatrix();


#pragma endregion
		}
	}

}
//描画
void Monster::Draw(const ViewProjection& viewProjection)
{
	for (int i = 0; i < blockWidth; i++)
	{
		for (int j = 0; j < blockHeight; j++)
		{
			switch (form_[i][j])
			{
			case Form::Block:
			
				model_->Draw(worldTransforms_[i][j], viewProjection, blockHandle_);
				
				break;

			case Form::Slime:

				model_->Draw(worldTransforms_[i][j], viewProjection, slimeHandle_);

				break;

			default:
				break;
			}
		}
	}
}
