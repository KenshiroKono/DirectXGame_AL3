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
///���L����
///< summary>
class Player {
public:
	//������
	void Initialize(Model* model, uint32_t textureHandle);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(ViewProjection& viewProjection);
	//������
	void Attack();

	//�Ԋu
	static const int kFireInterval = 20;
	//��]����
	Matrix4 Rotate();
	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g���擾
	std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };
	//���a���擾
	float GetRadius() { return r; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���͏���
	Input* input_ = nullptr;
	//�f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//���˃^�C�}�[
	int32_t bullletTime = 0;
	//���a
	const float r = 1.0f;

};