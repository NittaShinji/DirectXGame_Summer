#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Debugtext.h"

class Monster
{
public:

	//形態フェーズ
	enum class Form
	{
		Block = 0,		//生まれていない状態
		IsSelected,		//選択されている状態
		Slime,			//スライム
	};

	//行動フェーズ
	enum class Phase
	{
		Approach,	//接近する
		Leave,		//離脱する
	};

	Monster();
	~Monster();

	//初期化
	void Initialize(Model* model, const Vector3& position);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

private:

	static const int blockWidth = 30;
	static const int blockHeight = 30;

	// ワールド変換データ
	//WorldTransform worldTransforms_[blockWidth][blockHeight];
	WorldTransform worldTransforms_[blockWidth][blockHeight];

	// モデル
	Model* model_ = nullptr;
	// インプット
	Input* input_ = nullptr;
	// デバックテキスト
	DebugText* debugText_ = nullptr;

	// テクスチャハンドル
	uint32_t blockHandle_ = 0u;
	uint32_t selectHandle_ = 0u;
	uint32_t slimeHandle_ = 0u;

	//状態変化変数
	Form form_[blockWidth][blockHeight] = { Form::Block };
	//Form form_ = { Form::Block };

	//キャラクターの大きさ
	float scaleX = 8.0f;
	float scaleY = 8.0f;
	float scaleZ = 8.0f;

	int prevBlockX;
	int prevBlockY;

	//選択可能かどうかの変数
	bool changedSelect;

	//選択カーソルのクールタイマーの設定時間
	static const int32_t kSelectTime = 17;

	//選択カーソルのクールタイマー
	int32_t selectTimer_ = kSelectTime;

	//int selectTimer;

	//ブロックが選択されているか
	//int isSelected[blockWidth][blockHeight] = { 0 };

	////個々のブロック用の状態変化変数
	//int formChange[blockWidth][blockHeight] = { 0 };

};

