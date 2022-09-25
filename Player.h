#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "WorldTransform.h"

#include"PayerBullet.h"
#pragma once

///< summary>
///自キャラ
///< summary>
class Player {
public:
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//更新処理
	void Update();
	//描画処理
	void Draw(ViewProjection& viewProjection);
	//撃つ処理
	void Attack();
	//回転処理
	Matrix4 Rotate();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//入力処理
	Input* input_ = nullptr;
	//デバックテキスト
	DebugText* debugText_ = nullptr;


	PlayerBullet* bullet_ = nullptr;
};