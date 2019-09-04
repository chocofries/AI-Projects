#pragma once
#include "ShaderLoader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gameobject.h"
#include "camera.h"


class Quad : public GameObject
{
public:
	Quad();
	~Quad();

	virtual void Initialise();

	virtual void Update();
	virtual void Draw();
};