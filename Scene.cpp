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
	
	// ワールド変換の初期化
	titleTransform_.Initialize();
	startTransform_.Initialize();

	//worldTransform_.scale_ = { 100,100,100 };
	titleTransform_.translation_ = { 0,5,0 };
	startTransform_.translation_ = { 0,-12,0 };
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
}

void Scene::TitleDraw(ViewProjection& viewProjection) {
	model_->Draw(titleTransform_, viewProjection, titleHandle_);
	model_->Draw(startTransform_, viewProjection, startHandle_);
}

void Scene::UpdateMatrix()
{

}