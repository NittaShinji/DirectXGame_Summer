#include "Scene.h"
#include <cassert>

Scene::Scene(){}
Scene::~Scene(){}

void Scene::Initialize(Model* model){
	// NULLチェック
	assert(model);

	//受け取ったデータをメンバ変数に記録する
	model_ = model;

	//テクスチャ読み込み
	titleHandle_ = TextureManager::Load("title.png");
	startHandle_ = TextureManager::Load("start.png");
	clearHandle_ = TextureManager::Load("gameclear.png");
	loseHandle_ = TextureManager::Load("gameover.png");
	controlHandle_ = TextureManager::Load("sousa.png");
	spaceHandle_ = TextureManager::Load("space.png");

	
	// ワールド変換の初期化
	titleTransform_.Initialize();
	startTransform_.Initialize();
	controlTransform_.Initialize();

	//worldTransform_.scale_ = { 100,100,100 };
	titleTransform_.translation_ = { 0,5,0 };
	startTransform_.translation_ = { 0,-12,0 };
	controlTransform_.translation_ = { 160,1,50 };
	//controlTransform_.rotation_.x = -2.0;

}

int Scene::GetScene()
{
	return sceneNow;
}

void Scene::Update() {

#pragma region タイトルの行列の更新
	//スケーリング用行列を宣言
	Matrix4 matScale;
	//合成用回転行列を宣言
	Matrix4 matRot;
	//各軸用回転用行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;
	//座標用行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//キャラクター移動処理
	float scaleX = 20.0f;
	float scaleY = 12.0f;
	float scaleZ = 0.01f;

	//スケーリング倍率を行列に設定
	matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	//titleTransform_.rotation_.x = 0.24;

	//回転行列を行列に設定
	matRot.Matrix4RotationX(titleTransform_.rotation_.x);
	matRot.Matrix4RotationY(titleTransform_.rotation_.y);
	matRot.Matrix4RotationZ(titleTransform_.rotation_.z);

	titleTransform_.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans.Matrix4Translation(
		titleTransform_.translation_.x,
		titleTransform_.translation_.y,
		titleTransform_.translation_.z);

	titleTransform_.matWorld_ = matScale;
	titleTransform_.matWorld_ *= matRot;
	titleTransform_.matWorld_ *= matTrans;

	//WorldMatrix(titleTransform_.matWorld_, matScale, matRot, matTrans);

	//親行列と掛け算代入
	if (titleTransform_.parent_ != nullptr)
	{
		titleTransform_.matWorld_ *= titleTransform_.parent_->matWorld_;
	}

	//ワールド行列を転送
	titleTransform_.TransferMatrix();

#pragma endregion

#pragma region スタートの更新
	//スケーリング用行列を宣言
	Matrix4 matScale2;
	//合成用回転行列を宣言
	Matrix4 matRot2;
	//各軸用回転用行列を宣言
	Matrix4 matRotX2, matRotY2, matRotZ2;
	//座標用行列を宣言
	Matrix4 matTrans2 = MathUtility::Matrix4Identity();

	//キャラクター移動処理
	float scaleX2 = 8.0f;
	float scaleY2 = 4.0f;
	float scaleZ2 = 0.01f;

	//スケーリング倍率を行列に設定
	matScale2.Matrix4Scaling(scaleX2, scaleY2, scaleZ2);

	//startTransform_.rotation_.x = 0.24;

	//回転行列を行列に設定
	matRot2.Matrix4RotationX(startTransform_.rotation_.x);
	matRot2.Matrix4RotationY(startTransform_.rotation_.y);
	matRot2.Matrix4RotationZ(startTransform_.rotation_.z);

	startTransform_.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans2.Matrix4Translation(
		startTransform_.translation_.x,
		startTransform_.translation_.y,
		startTransform_.translation_.z);

	startTransform_.matWorld_ = matScale2;
	startTransform_.matWorld_ *= matRot2;
	startTransform_.matWorld_ *= matTrans2;

	//WorldMatrix(startTransform_.matWorld_, matScale, matRot, matTrans);
	//ワールド行列を転送
	startTransform_.TransferMatrix();

#pragma endregion


#pragma region 操作方法の更新
	//スケーリング用行列を宣言
	Matrix4 matScale3;
	//合成用回転行列を宣言
	Matrix4 matRot3;
	//各軸用回転用行列を宣言
	Matrix4 matRotX3, matRotY3, matRotZ3;
	//座標用行列を宣言
	Matrix4 matTrans3 = MathUtility::Matrix4Identity();

	//キャラクター移動処理
	float scaleX3 = 55.0f;
	float scaleY3 = 1.0f;
	float scaleZ3 = 50.f;

	//スケーリング倍率を行列に設定
	matScale3.Matrix4Scaling(scaleX3, scaleY3, scaleZ3);

	
	//回転行列を行列に設定
	matRot3.Matrix4RotationX(controlTransform_.rotation_.x);
	matRot3.Matrix4RotationY(controlTransform_.rotation_.y);
	matRot3.Matrix4RotationZ(controlTransform_.rotation_.z);

	controlTransform_.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans3.Matrix4Translation(
		controlTransform_.translation_.x,
		controlTransform_.translation_.y,
		controlTransform_.translation_.z);

	controlTransform_.matWorld_ = matScale3;
	controlTransform_.matWorld_ *= matRot3;
	controlTransform_.matWorld_ *= matTrans3;

	//WorldMatrix(controlTransform_.matWorld_, matScale, matRot, matTrans);
	//ワールド行列を転送
	controlTransform_.TransferMatrix();
#pragma endregion


}

void Scene::TitleDraw(ViewProjection& viewProjection) 
{
	model_->Draw(titleTransform_, viewProjection, titleHandle_);
	model_->Draw(startTransform_, viewProjection, startHandle_);
}

//void Scene::

void Scene::LoseDraw(ViewProjection& viewProjection) 
{
	model_->Draw(titleTransform_, viewProjection, loseHandle_);
	model_->Draw(startTransform_, viewProjection, spaceHandle_);

}

void Scene::ClearDraw(ViewProjection& viewProjection) 
{
	model_->Draw(titleTransform_, viewProjection, clearHandle_);
}

void Scene::GameDraw(ViewProjection& viewProjection)
{
	model_->Draw(controlTransform_, viewProjection, controlHandle_);
}


void Scene::UpdateMatrix()
{

}