﻿#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <cassert>
#include <math.h>
#include <WinApp.h>
#include <random>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159 /* 桁数はもっと多い方がいいかも */
#endif

void MatrixSet(WorldTransform& worldTransform);
void WorldMatrix(Matrix4 matWorld, Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans);
void RayMatrixSet(WorldTransform& worldTransform);
void MatrixBigSet(WorldTransform& worldTransform);
float ToRadian(float angle);
float ToAngle(float radian);

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete rayModel;
	delete debugCamera_;
	delete player_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	textureRayHandle_ = TextureManager::Load("ray.png");

	//3Dモデルの生成
	model_ = Model::Create();
	rayModel = Model::Create();

	//プレイヤーの生成
	player_ = new Player();
	//プレイヤーの初期化
	player_->Initialize(model_, textureHandle_);


	ray.Initialize();
	//光線の大きさ
	ray.scale_ = { 0.1,0.1,50.0 };

	//光線ベクトル
	//ray.translation_ = { startray.x,startray.y,startray.z };
	ray.translation_ = { 4.0f,2.0f,0.0f };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを生成する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

}

void GameScene::Update() 
{
	//視点移動処理
	Vector3 eyeMove = { 0.0f,0.0f,0.0f };

	//視点移動の速さ
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W))
	{
		eyeMove.z += kEyeSpeed;
	}
	else if (input_->PushKey(DIK_S))
	{
		eyeMove.z -= kEyeSpeed;
	}

	if (input_->PushKey(DIK_K))
	{
		eyeMove.y += kEyeSpeed;
	}
	else if (input_->PushKey(DIK_G))
	{
		eyeMove.y -= kEyeSpeed;
	}


	if (input_->PushKey(DIK_O))
	{
		eyeMove.x += kEyeSpeed;
	}
	else if (input_->PushKey(DIK_P))
	{
		eyeMove.x -= kEyeSpeed;
	}


	//視点移動(ベクトルの加算)
	viewProjection_.eye += eyeMove;

	//注視点の移動ベクトル
	Vector3 kTargetMove = { 0,0,0 };

	//注視点の移動の速さ
	const float kTargetSpeed = 0.2f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_R))
	//{
	//	kTargetMove.x -= kTargetSpeed;
	//}
	//else if (input_->PushKey(DIK_E))
	//{
	//	kTargetMove.x += kTargetSpeed;
	//}

	//注視点移動(ベクトルの加算)
	viewProjection_.target += kTargetMove;

	//カメラの処理
	if (isDebugCameraActive_)
	{
		debugCamera_->Update();

		ViewProjection debug = debugCamera_->GetViewProjection();

		viewProjection_.matView = debug.matView;
		viewProjection_.matProjection = debug.matProjection;
		viewProjection_.TransferMatrix();
	}
	else
	{
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}

	////キャラクター移動処理
	//float scaleX = 1.0f;
	//float scaleY = 1.0f;
	//float scaleZ = 1.0f;

	////x.y,z方向のスケーリングを設定
	//worldTransforms_[0].scale_ = { scaleX,scaleY,scaleZ };

	////スケーリング行列を宣言
	//Matrix4 matScale;

	////スケーリング倍率を行列に設定
	//matScale.Matrix4Scaling(scaleX, scaleY, scaleZ);

	////x.y,z方向の回転角を乱数で設定
	//worldTransforms_[1].rotation_ = { 0.0f,0.0f,0.0f };

	////合成用回転行列を宣言
	//Matrix4 matRot;

	////各軸用回転用行列を宣言
	//Matrix4 matRotX, matRotY, matRotZ;

	////各軸の回転要素を設定
	//matRotX.Matrix4RotationX(worldTransforms_[0].rotation_.x);
	//matRotY.Matrix4RotationY(worldTransforms_[0].rotation_.y);
	//matRotZ.Matrix4RotationZ(worldTransforms_[0].rotation_.z);
	////各軸の回転行列を合成
	//matRotZ *= matRotX;
	//matRotZ *= matRotY;
	//matRot = matRotZ;

	/*const float playerSpeed = 0.05f;*/

	player_->Update();

	//キャラクターの移動ベクトル
	//Vector3 playerMove = { 0,0,0 };

	/*if (input_->PushKey(DIK_RIGHT))
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
	}*/

	//上半身回転処理

	//キャラクターの角度ベクトル
	Vector3 playerRot = { 0,0,0 };

	const float playerRotSpeed = 0.05f;

	////押した方向で移動ベクトルを変更
	//if (input_->PushKey(DIK_U))
	//{
	//	//胸パーツのY軸まわり角度を減少
	//	playerRot.y -= playerRotSpeed;
	//	worldTransforms_[PartId::kChest].rotation_.y += playerRot.y;
	//}
	//else if (input_->PushKey(DIK_I))
	//{
	//	//胸パーツのY軸まわり角度を増加
	//	playerRot.y += playerRotSpeed;
	//	worldTransforms_[PartId::kChest].rotation_.y += playerRot.y;
	//}

	////下半身回転処理
	//if (input_->PushKey(DIK_J))
	//{
	//	playerRot.y += playerRotSpeed;
	//	worldTransforms_[PartId::kHip].rotation_.y += playerRot.y;
	//}
	//else if (input_->PushKey(DIK_K))
	//{

	//	playerRot.y += playerRotSpeed;
	//	worldTransforms_[PartId::kHip].rotation_.y += playerRot.y;
	//}

	//worldTransforms_[PartId::kRoot].translation_.x += playerMove.x;


	//worldTransforms_[0].translation_ += playerMove;

	////ワールド行列の計算
	////平行移動行列を宣言
	//Matrix4 matTrans = MathUtility::Matrix4Identity();

	//matTrans.Matrix4Translation(worldTransforms_[0].translation_.x, worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);

	//worldTransforms_[0].matWorld_.IdentityMatrix();

	//worldTransforms_[0].matWorld_ = matScale;
	//worldTransforms_[0].matWorld_ *= matRot;
	//worldTransforms_[0].matWorld_ *= matTrans;

	////行列の転送
	//worldTransforms_[0].TransferMatrix();

	////子の更新

	////x.y,z方向のスケーリングを設定
	//worldTransforms_[1].scale_ = { 1.0f,1.0f,1.0f };

	////スケーリング行列を宣言
	//Matrix4 matScale2;

	////スケーリング倍率を行列に設定
	//matScale2.Matrix4Scaling(scaleX, scaleY, scaleZ);

	////x.y,z方向の回転角を乱数で設定
	//worldTransforms_[1].rotation_ = { 0.0f,0.0f,0.0f };

	////合成用回転行列を宣言
	//Matrix4 matRot2;

	////各軸用回転用行列を宣言
	//Matrix4 matRotX2, matRotY2, matRotZ2;

	////各軸の回転要素を設定
	//matRotX2.Matrix4RotationX(worldTransforms_[1].rotation_.x);
	//matRotY2.Matrix4RotationY(worldTransforms_[1].rotation_.y);
	//matRotZ2.Matrix4RotationZ(worldTransforms_[1].rotation_.z);
	////各軸の回転行列を合成
	//matRotZ2 *= matRotX2;
	//matRotZ2 *= matRotY2;
	//matRot2 = matRotZ2;

	//x.y,z方向の平行移動(座標)を乱数で設定
	//worldTransforms_[0].translation_ = { 0.0f,5.0f,0.0f };

	//Matrix4 matTrans2 = MathUtility::Matrix4Identity();

	//matTrans2.Matrix4Translation(worldTransforms_[1].translation_.x, worldTransforms_[1].translation_.y, worldTransforms_[1].translation_.z);

	////スケーリング・回転・平行移動を合成した行列を計算
	////ワールドトランスフォームに代入する
	//worldTransforms_[1].matWorld_.IdentityMatrix();

	//worldTransforms_[1].matWorld_ = matScale2;
	//worldTransforms_[1].matWorld_ *= matRot2;
	//worldTransforms_[1].matWorld_ *= matTrans2;

	//worldTransforms_[1].matWorld_ *= worldTransforms_[0].matWorld_;
	//worldTransforms_[1].TransferMatrix();
	//
	////x.y,z方向のスケーリングを設定
	//worldTransforms_[2].scale_ = { 1.0f,1.0f,1.0f };

	////スケーリング行列を宣言
	//Matrix4 matScale3;

	////スケーリング倍率を行列に設定
	//matScale3.Matrix4Scaling(scaleX, scaleY, scaleZ);

	////x.y,z方向の回転角を乱数で設定
	//worldTransforms_[2].rotation_ = { 0.0f,0.0f,0.0f };

	////合成用回転行列を宣言
	//Matrix4 matRot3;

	////各軸用回転用行列を宣言
	//Matrix4 matRotX3, matRotY3, matRotZ3;

	////各軸の回転要素を設定
	//matRotX3.Matrix4RotationX(worldTransforms_[2].rotation_.x);
	//matRotY3.Matrix4RotationY(worldTransforms_[2].rotation_.y);
	//matRotZ3.Matrix4RotationZ(worldTransforms_[2].rotation_.z);
	////各軸の回転行列を合成
	//matRotZ3 *= matRotX3;
	//matRotZ3 *= matRotY3;
	//matRot3 = matRotZ3;

	//x.y,z方向の平行移動(座標)を乱数で設定
	//worldTransforms_[2].translation_ = { 0.0f,8.0f,0.0f };

	//Matrix4 matTrans3 = MathUtility::Matrix4Identity();

	//matTrans3.Matrix4Translation(worldTransforms_[2].translation_.x, worldTransforms_[2].translation_.y, worldTransforms_[2].translation_.z);

	////スケーリング・回転・平行移動を合成した行列を計算
	////ワールドトランスフォームに代入する
	//worldTransforms_[2].matWorld_.IdentityMatrix();

	//worldTransforms_[2].matWorld_ = matScale3;
	//worldTransforms_[2].matWorld_ *= matRot3;
	//worldTransforms_[2].matWorld_ *= matTrans3;

	//worldTransforms_[2].matWorld_ *= worldTransforms_[1].matWorld_;
	//worldTransforms_[2].TransferMatrix();

#pragma endregion


	//大元から順に更新していく

	//MatrixSet(worldTransforms_[0]);
	//MatrixBigSet(worldTransforms_[1]);


	const float raySpeed = 0.10f;

	//光線の移動ベクトル
	Vector3 rayMove = { 0,0,0 };


	if (input_->PushKey(DIK_RIGHT))
	{
		rayMove.x = raySpeed;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		rayMove.x = -raySpeed;
	}
	else if (input_->PushKey(DIK_UP))
	{
		rayMove.y = raySpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		rayMove.y = -raySpeed;
	}
	else if (input_->PushKey(DIK_1))
	{
		rayMove.z = -raySpeed;
		moveCount += raySpeed;
	}
	else if (input_->PushKey(DIK_2))
	{
		rayMove.z = raySpeed;
		moveCount += raySpeed;
	}
	else
	{
		rayMove.x = 0.0f;
		rayMove.y = 0.0f;
		rayMove.z = 0.0f;
	}



	ray.translation_.x += rayMove.x;
	ray.translation_.y += rayMove.y;
	ray.translation_.z += rayMove.z;

	//光線の更新
	RayMatrixSet(ray);

	//始点
	startray.x = ray.translation_.x;
	startray.y = ray.translation_.y;
	startray.z = ray.translation_.z - ray.scale_.z;

	//終点
	endaray.x = ray.translation_.x;
	endaray.y = ray.translation_.y;
	endaray.z = ray.translation_.z + ray.scale_.z;

	//方向ベクトル
	Vector3 rayDirection = { 0.0f,0.0f,0.0f };

	rayDirection.x = endaray.x - startray.x;
	rayDirection.y = endaray.y - startray.y;
	rayDirection.z = endaray.z - startray.z;

	//正規化した方向ベクトル
	Vector3 rayDN = { 0.0f,0.0f,0.0f };

	//光線ベクトルを正規化
	rayDN = rayDirection.normalize();

	Vector3 b = { 0.0f,0.0f,0.0f };

	//レイとオブジェクトのベクトル
	/*b = { (worldTransforms_[0].translation_.x - startray.x),
		(worldTransforms_[0].translation_.y - startray.y),
		(worldTransforms_[0].translation_.z - startray.z) };*/

	float rayToObject = 0.0f;

	//下の線の長さ
	rayToObject = b.dot(rayDN);


	//Vector3 Q = { 0.0f,0.0f,0.0f };

	//Q.x = ((rayDN.x * rayToObject) + startray.x);
	//Q.y = ((rayDN.y * rayToObject) + startray.y);
	//Q.z = ((rayDN.z * rayToObject) + startray.z);

	Vector3 rayVec = { 0.0f,0.0f,0.0f };
	rayVec.x = rayDN.x * rayToObject;
	rayVec.y = rayDN.y * rayToObject;
	rayVec.z = rayDN.z * rayToObject;

	Vector3 objectRay = { 0.0f,0.0f,0.0f };
	objectRay.x = rayVec.x - b.x;
	objectRay.y = rayVec.y - b.y;
	objectRay.z = rayVec.z - b.z;

	float collisionLength = 0.0f;
	collisionLength = objectRay.length();

	////垂線
	//Vector3 vertical = { 0.0f,0.0f,0.0f };
	//float verticalLength = 0.0f;

	//vertical.x = worldTransforms_[0].translation_.x - Q.x;
	//vertical.y = worldTransforms_[0].translation_.y - Q.y;
	//vertical.z = worldTransforms_[0].translation_.z - Q.z;

	////オブジェクトとレイの距離
	//verticalLength = vertical.length();

	//if (collisionLength < worldTransforms_[0].scale_.z * 2 - 0.3)
	//{
	//	rayCollision = 1;
	//}
	//else
	//{
	//	rayCollision = 0;
	//}

	//if (startray.z > worldTransforms_[0].translation_.z && endaray.z > worldTransforms_[0].translation_.z)
	//{
	//	rayCollision = 0;
	//}
	//else if (startray.z < 0 && endaray.z < 0)
	//{
	//	rayCollision = 0;
	//}


	//if (moveCount >= 50)
	//{
	//	rayCollision = 0;
	//}
	//else if (moveCount <= -50)
	//{
	//	rayCollision = 0;
	//}

	//行列の再計算
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 110);
	debugText_->Printf("forAngleY(Degree):%f", ToAngle(viewProjection_.fovAngleY));

	/*debugText_->SetPos(50, 130);
	debugText_->Printf("worldTranslation[0]:(%f,%f,%f)", worldTransforms_[0].translation_.x, worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);

	debugText_->SetPos(50, 150);
	debugText_->Printf("worldTranslation[1]:(%f,%f,%f)", worldTransforms_[1].translation_.x, worldTransforms_[1].translation_.y, worldTransforms_[1].translation_.z);*/

	debugText_->SetPos(50, 170);
	debugText_->Printf("rayCollision):%d", rayCollision);
	debugText_->SetPos(50, 190);
	debugText_->Printf("moveCount):%f", moveCount);
	debugText_->SetPos(50, 210);
	debugText_->Printf("startray.z):%f", startray.z);
	debugText_->SetPos(50, 230);
	debugText_->Printf("endaray.z):%f", endaray.z);
	/*debugText_->SetPos(50, 250);
	debugText_->Printf("z):%f", worldTransforms_[0].translation_.z);*/

}

void GameScene::Draw() {

	//描画用の線のベクトル変数
	Vector3 RedStart = { 0.0f, 0.0f, 0.0f };
	Vector3 RedEnd = { 128.0f, 0.0f, 128.0f };

	Vector3 BlueStart = { 0.0f, 0.0f, 0.0f };
	Vector3 BlueEnd = { 128.0f, 0.0f, 128.0f };

	Vector4 RedColor = { 128.0f, 0.0f, 0.0f, 1.0f };
	Vector4 BlueColor = { 0.0f, 0.0f, 128.0f, 1.0f };

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	
	rayModel->Draw(ray, debugCamera_->GetViewProjection(), textureRayHandle_);

	player_->Draw(debugCamera_, textureHandle_);

	//for (int i = 0; i < 2; i++)
	//{
	//	/*if (i == kRoot) {
	//		continue;
	//	}
	//	else if (i == kSpine)
	//	{
	//		continue;
	//	}
	//	else
	//	{

	//	}*/

	//	model_->Draw(worldTransforms_[0], debugCamera_->GetViewProjection(), textureHandle_);

	//	if (rayCollision == 1)
	//	{
	//		model_->Draw(worldTransforms_[1], debugCamera_->GetViewProjection(), textureHandle_);
	//	}
	//}

	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//ワールド行列の生成
void WorldMatrix(Matrix4 matWorld, Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans)
{
	matWorld = matScale;
	matWorld *= matRot;
	matWorld *= matTrans;
}

//スケーリング・回転・平行移動を宣言
void MatrixSet(WorldTransform& worldTransform)
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
	matRot.Matrix4RotationX(worldTransform.rotation_.x);
	matRot.Matrix4RotationY(worldTransform.rotation_.y);
	matRot.Matrix4RotationZ(worldTransform.rotation_.z);

	worldTransform.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans.Matrix4Translation(
		worldTransform.translation_.x,
		worldTransform.translation_.y,
		worldTransform.translation_.z);

	worldTransform.matWorld_ = matScale;
	worldTransform.matWorld_ *= matRot;
	worldTransform.matWorld_ *= matTrans;

	//WorldMatrix(worldTransform.matWorld_, matScale, matRot, matTrans);

	//親行列と掛け算代入
	if (worldTransform.parent_ != nullptr)
	{
		worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
	}

	//ワールド行列を転送
	worldTransform.TransferMatrix();
}

//スケーリング・回転・平行移動を宣言
void MatrixBigSet(WorldTransform& worldTransform)
{
	//スケーリング用行列を宣言
	Matrix4 matScale3;
	//合成用回転行列を宣言
	Matrix4 matRot3;
	//各軸用回転用行列を宣言
	Matrix4 matRotX3, matRotY3, matRotZ3;
	//座標用行列を宣言
	Matrix4 matTrans3 = MathUtility::Matrix4Identity();

	//キャラクター移動処理
	float scaleX3 = 8.0f;
	float scaleY3 = 8.0f;
	float scaleZ3 = 8.0f;

	//スケーリング倍率を行列に設定
	matScale3.Matrix4Scaling(scaleX3, scaleY3, scaleZ3);

	//回転行列を行列に設定
	matRot3.Matrix4RotationX(worldTransform.rotation_.x);
	matRot3.Matrix4RotationY(worldTransform.rotation_.y);
	matRot3.Matrix4RotationZ(worldTransform.rotation_.z);

	worldTransform.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans3.Matrix4Translation(
		worldTransform.translation_.x,
		worldTransform.translation_.y,
		worldTransform.translation_.z);

	worldTransform.matWorld_ = matScale3;
	worldTransform.matWorld_ *= matRot3;
	worldTransform.matWorld_ *= matTrans3;

	//WorldMatrix(worldTransform.matWorld_, matScale, matRot, matTrans);

	//ワールド行列を転送
	worldTransform.TransferMatrix();
}

void RayMatrixSet(WorldTransform& worldTransform)
{
	//スケーリング用行列を宣言
	Matrix4 matScale2;
	//合成用回転行列を宣言
	Matrix4 matRot2;
	//各軸用回転用行列を宣言
	Matrix4 matRotX2, matRotY2, matRotZ2;
	//座標用行列を宣言
	Matrix4 matTrans2 = MathUtility::Matrix4Identity();

	//キャラクター移動処理
	float scaleX2 = 0.1f;
	float scaleY2 = 0.1f;
	float scaleZ2 = 50.0f;

	//スケーリング倍率を行列に設定
	matScale2.Matrix4Scaling(scaleX2, scaleY2, scaleZ2);

	//回転行列を行列に設定
	matRot2.Matrix4RotationX(worldTransform.rotation_.x);
	matRot2.Matrix4RotationY(worldTransform.rotation_.y);
	matRot2.Matrix4RotationZ(worldTransform.rotation_.z);

	worldTransform.matWorld_.IdentityMatrix();

	//座標を行列に設定
	matTrans2.Matrix4Translation(
		worldTransform.translation_.x,
		worldTransform.translation_.y,
		worldTransform.translation_.z);

	worldTransform.matWorld_ = matScale2;
	worldTransform.matWorld_ *= matRot2;
	worldTransform.matWorld_ *= matTrans2;

	//WorldMatrix(worldTransform.matWorld_, matScale, matRot, matTrans);

	//ワールド行列を転送
	worldTransform.TransferMatrix();
}

float ToRadian(float angle)
{
	return angle / 180;
}

float ToAngle(float radian)
{
	return  (180 / M_PI) * radian;
}
