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
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Vector3 velocity_;
	uint32_t textureHandle_ = 0u;

	//����
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};