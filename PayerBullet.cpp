#include"PayerBullet.h"
#include<assert.h>
#include "MyMatrix.h"

void PlayerBullet::OnCollision() { 
	isDead_ = true; }

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

Vector3 PlayerBullet::GetBulletPosition() {
	//���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::Update() {
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ *= Mat(worldTransform_);
	worldTransform_.translation_ += velocity_;
	//�s��̓]��
	worldTransform_.TransferMatrix();
	//���Ԍo�߂ŏ�����
	if (--deathTimer <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}