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

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();
	//���[���h���W���擾
	Vector3 GetBulletPosition();
	//���a���擾
	float GetRadius() { return r; }
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;
	//����
	static const int32_t kLifeTime = 120;
	//�f�X�^�C�}�[
	int32_t deathTimer = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
	//���a
	const float r = 2.0f;

};