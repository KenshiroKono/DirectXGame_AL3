#pragma once

#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "WorldTransform.h"
#include "memory"
#include "list"
#include"PayerBullet.h"

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

	//間隔
	static const int kFireInterval = 20;
	//回転処理
	Matrix4 Rotate();
	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };
	//半径を取得
	float GetRadius() { return r; }

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

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//発射タイマー
	int32_t bullletTime = 0;
	//半径
	const float r = 1.0f;

};