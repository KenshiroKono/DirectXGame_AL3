#pragma once


#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<assert.h>
#include"PayerBullet.h"
#include<memory>
#include<list>

//�s���t�F�[�Y
enum class Phase {
	Approach, //�ڋ߂���
	Leave,    //���E����
};

class Enemy {
public:
	//������
	void Initialize(Model* model, uint32_t textureHandle);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(ViewProjection& viewProjection);

	//�e�p�^�[���̏���
	void Approach_move();
	void Leave_move();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

};