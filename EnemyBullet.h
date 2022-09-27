#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& veclocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//消滅
	static const int32_t kLifeTime = 120;
	//デスタイマー
	int32_t deathTimer = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};