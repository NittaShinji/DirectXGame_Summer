#include "AirCamera.h"

AirCamera::AirCamera() {}
AirCamera::~AirCamera() {}
using namespace MathUtility;

//初期化
void AirCamera::Initialize(const Vector3& position, const Vector3& radian)
{
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = radian;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}
//更新
void AirCamera::Update()
{
	//worldTransform_.translation_.z -= 0.02;
	//worldTransform_.rotation_.z += 0.2;

	viewProjection_.eye = worldTransform_.translation_;
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = Transform(forward, worldTransform_.matWorld_);
	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.up = Transform(up, worldTransform_.matWorld_);

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

	//WorldMatrix(enemyWorldTransform_.matWorld_, matScale, matRot, matTrans);
#pragma endregion

	//ワールド行列を転送
	worldTransform_.TransferMatrix();

	//ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	/*debugText_->SetPos(50, 230);
	debugText_->Printf("viewProjection_.eye.z) : %f", viewProjection_.eye.z);*/

}

Matrix4 AirCamera::GetMatWorld()
{
	//ワールド座標を入れる変数
	Matrix4 matWorld;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	matWorld = worldTransform_.matWorld_;
	return matWorld;
}

WorldTransform AirCamera::GetWorldTransform()
{
	//ワールド座標を入れる変数
	WorldTransform worldTransform;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldTransform = worldTransform_;
	return worldTransform;
}


