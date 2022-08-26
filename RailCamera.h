#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Debugtext.h"

class RailCamera
{
public:
	RailCamera();
	~RailCamera();

	//初期化
	void Initialize(const Vector3& position,const Vector3& radian);
	//更新
	void Update();

	//ゲッター
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//ワールド変換データを取得
	WorldTransform GetWorldTransform();

	//ワールド行列を取得
	Matrix4 GetMatWorld();
	
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	// デバックテキスト
	DebugText* debugText_ = nullptr;
};

