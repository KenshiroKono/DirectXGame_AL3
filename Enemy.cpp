#include "Enemy.h"
#include <cassert>
#include "MyMatrix.h"


void Enemy::Approach_move() {
	//�ړ�
	Vector3 move = { 0, 0, -0.2f };
	worldTransform_.translation_ += move;
	//���̈ʒu�ɒB�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	if (bullletTime-- < 0) {
	
		Fire();
	
	}

}
void Enemy::Leave_move() {
	//�ړ�
	Vector3 move = { 0, 0, 0.2f };
	worldTransform_.translation_ += move;
	if (worldTransform_.translation_.z > 100.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Fire() {

	//�e�̑��x
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = Vec_rot(velocity, worldTransform_.matWorld_);

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));

	bullletTime = kFireInterval;

}

void Enemy::ApproachInitialize() {

	bullletTime = kFireInterval;

}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 10, 5.0f, 30.0f };

	ApproachInitialize();
}

void Enemy::Update() {
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach_move();
		break;
	case Phase::Leave:
		Leave_move();

		break;
	}

	
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

#pragma region �ړ�����
	worldTransform_.matWorld_ = matIdentity();
	worldTransform_.matWorld_ = Mat(worldTransform_);
	//�s��̓]��
	worldTransform_.TransferMatrix();

	debugText_->SetPos(50, 90);
	debugText_->Printf(
		"Player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);

#pragma endregion

}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}