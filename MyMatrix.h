#pragma once

#include"WorldTransform.h"
#include"DirectXCommon.h"

Matrix4 matIdentity();
//�X�P�[�����O
Matrix4 Mat_size(WorldTransform& w);
//���s�ړ�
Matrix4 Mat_move(WorldTransform& w);
//��]
Matrix4 Mat_rot(WorldTransform& w);
//����
Matrix4 Mat(WorldTransform w);

Vector3 Vec_rot(Vector3 velocity, Matrix4 amount);


float length(Vector3 vec);//���������߂�
Vector3 normalize(Vector3 vec);//���K������