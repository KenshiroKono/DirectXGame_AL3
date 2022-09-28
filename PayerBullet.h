#pragma once

#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"	
class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	//ワールド座標を取得
	Vector3 GetBulletPosition();
	//半径を取得
	float GetRadius() { return r; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 velocity_;
	uint32_t textureHandle_ = 0u;

	//寿命
	static const int32_t kLifeTime = 60*5 ;
	//デスタイマー
	int32_t deathTimer = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
	//半径
	const float r = 2.0f;

};