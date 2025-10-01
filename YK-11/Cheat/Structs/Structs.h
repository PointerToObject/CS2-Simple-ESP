#pragma once
#include "../../Includes.h"

#define PADDING__(a, b) a##b
#define PADDING_(line) PADDING__(pad_, line)
#define PADDING(size) private: char PADDING_(__COUNTER__)[size]; public:

#pragma pack(push, 1)

struct Vector2
{
	float x, y;
};

struct Vector3
{
	float x, y, z;
};

struct view_matrix_t
{
	float* operator[ ](int index)
	{
		return matrix[index];
	}
	float matrix[4][4];
};

bool W2S(const Vector3& WorldPos, Vector2& out, view_matrix_t matrix);

struct Cache
{
	view_matrix_t vm;
	Vector3 Position;
	Vector3 CameraPos;
	unsigned int health;
	__int8 team;
	std::string name;
};

struct BaseEntity
{
	PADDING(844);
	uint32_t health; // 0x34C

	PADDING(155);
	__int8 m_iTeamNum; // 0x3EB
}; 

struct BasePawn
{
	PADDING(5560);
	Vector3 Position; // 0x15B8
}; 

#pragma pack(pop)

#undef PADDING
#undef PADDING_
#undef PADDING__