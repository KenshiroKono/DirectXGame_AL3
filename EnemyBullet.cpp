#include"EnemyBullet.h"
#include "MyMatrix.h"
#include <assert.h>

void EnemyBullet::OnCollision() { 
	isDead_ = true; }


Vector3 EnemyBullet::GetBulletPosition() {
	//���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("red.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update() {
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ *= Mat(worldTransform_);
	worldTransform_.translation_ -= velocity_;
	//�s��̓]��
	worldTransform_.TransferMatrix();
	//���Ԍo�߂ŏ�����
	if (--deathTimer <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}