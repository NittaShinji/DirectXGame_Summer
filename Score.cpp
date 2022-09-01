#include "Score.h"
#include <cassert>

Score::Score(){}
Score::~Score(){}

void Score::Initialize(Model* model)
{
	// NULLチェック
	assert(model);

	//モデルをインプット
	model_ = model;

	score = 0;

	//テクスチャ読み込み
	numHandle_[0] = TextureManager::Load("0.png");
	numHandle_[1] = TextureManager::Load("1.png");
	numHandle_[2] = TextureManager::Load("2.png");
	numHandle_[3] = TextureManager::Load("3.png");
	numHandle_[4] = TextureManager::Load("4.png");
	numHandle_[5] = TextureManager::Load("5.png");
	numHandle_[6] = TextureManager::Load("6.png");
	numHandle_[7] = TextureManager::Load("7.png");
	numHandle_[8] = TextureManager::Load("8.png");
	numHandle_[9] = TextureManager::Load("9.png");

	// シングルトンインスタンスを取得する
	debugText_ = DebugText::GetInstance();

	for (int i = 0; i < 9; i++)
	{
		/*worldTransforms_[i].Initialize();
		worldTransforms_[i].translation_ = { 0,0,0 };*/

		
	}

	worldTransform_.Initialize();
	worldTransform_.translation_ = { -10,10,0 };
	divideNum = 1;
	score = 1;
}

void Score::Update()
{
	score += 11;

	//一の位の処理
	if (score > 0 && score < 10)
	{
		scoreToCount = (score % divideNum);
	}
	else if (score > 10 && score < 100)
	{
		divideNum = 10;
		scoreToCount = (score % divideNum);
	}
	else if (score > 100 && score < 1000)
	{
		divideNum = 100;
		scoreToCount = (score % divideNum) % (divideNum / 10);
		//scoreToCount = (scoreToCount );
	}
	else if (score > 1000 && score < 10000)
	{
		divideNum = 1000;
		scoreToCount = (score % divideNum) % (divideNum / 100) % (divideNum / 10);
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


	/*debugText_->SetPos(300, 580);
	debugText_->Printf("score:%d",
		score);*/

}
//
void Score::GetScore(int scores)
{
	score = scores;
}


void Score::Draw(const ViewProjection& viewProjection)
{
	/*for (int  i = 0; i < 9; i++)
	{
		divideNum = divideNum * 10;
	}*/
		//scoreToCount = (score % divideNum) / (divideNum / 10);
		//model_->Draw(worldTransforms_[scoreToCount], viewProjection, numHandle_[scoreToCount]);
		



	//model_->Draw(worldTransform_, viewProjection, numHandle_[scoreToCount]);






		//divideNum = divideNum / 10;

		//scoreToCount = (score % divideNum)}

	

	//model_->Draw(worldTransform_, viewProjection, numHandle_[0]);

	/*for (int i = 0; i < 9; i++)
	{
		divide = divide * 10;
	}
	for (int i = 0; i < 9; i++)
	{
		scoreToElementCount = (score % divide) / (divide / 10);
		DrawGraph(i * 64 + 350, 300, figuresPicture[scoreToElementCount], true);
		divide = divide / 10;
	}*/
}
//void Score::UpdateTransform()
//{
//#pragma region 行列の更新
//	//スケーリング用行列を宣言
//	Matrix4 matScale;
//	//合成用回転行列を宣言
//	Matrix4 matRot;
//	//各軸用回転用行列を宣言
//	Matrix4 matRotX, matRotY, matRotZ;
//	//座標用行列を宣言
//	Matrix4 matTrans = MathUtility::Matrix4Identity();
//
//	//スケーリング倍率を行列に設定
//	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);
//
//	//回転行列を行列に設定
//	matRot.Matrix4RotationX(worldTransforms_[i].rotation_.x);
//	matRot.Matrix4RotationY(worldTransforms_[i].rotation_.y);
//	matRot.Matrix4RotationZ(worldTransforms_[i].rotation_.z);
//
//	worldTransforms_[i].matWorld_.IdentityMatrix();
//
//	//座標を行列に設定
//	matTrans.Matrix4Translation(
//		worldTransforms_[i].translation_.x,
//		worldTransforms_[i].translation_.y,
//		worldTransforms_[i].translation_.z);
//
//	worldTransforms_[i].matWorld_ = matScale;
//	worldTransforms_[i].matWorld_ *= matRot;
//	worldTransforms_[i].matWorld_ *= matTrans;
//
//	//ワールド行列を転送
//	worldTransforms_[i].TransferMatrix();
//
//#pragma endregion
//
//}
