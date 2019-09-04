#pragma once
#include <iostream>
#include <SOIL.h>
#include <vector>
#include <thread>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shaderloader.h"
#include "camera.h"
#include "textureloader.h"
#include "texter.h"
#include "inputmanager.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Initialise();
	virtual void Update();
	virtual void Draw();

	void SetCamera(Camera* _cam);

	void SetX(float _x);
	void SetY(float _y);
	void SetZ(float _z);
	void SetXRot(float _xRot);
	void SetYRot(float _yRot);
	void SetZRot(float _zRot);
	void SetXScale(float _xScale);
	void SetYScale(float _yScale);
	void SetZScale(float _zScale);
	void SetRotationAngle(float _angle);

	void SetImageFilePath(const char* _imagefilepath);
	void SetShaderLoader(ShaderLoader* _sloader);
	void SetTextureLoader(TextureLoader* _tloader);
	void SetInputManager(InputManager* _imanager);
	void SetVBO(GLuint* _vbo);
	void SetVecTexts(std::vector<Texter*>* _vecTexts);
	void SetIsActive(bool _isActive);

	void LoadTheImage();

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetXRot() const;
	float GetYRot() const;
	float GetZRot() const;
	float GetXScale() const;
	float GetYScale() const;
	float GetZScale() const;
	bool GetIsActive() const;

	int GetScreenX() const;
	int GetScreenY() const;

protected:
	float x;
	float y;
	float z;
	float xRot;
	float yRot;
	float zRot;
	float xScale;
	float yScale;
	float zScale;
	float rotationAngle;

	bool isActive;

	//Position relative to screen (in pixels)
	int screenX;
	int screenY;

	const char* imageFilePath;
	unsigned char* image;
	int texW;
	int texH;
	bool isLoaded; //If the image has been loaded

	ShaderLoader* shaderLoader;
	TextureLoader* textureLoader;
	InputManager* inputManager;
	Camera* gameCamera;
	GLuint* VBO;
	GLuint myVBO;
	GLuint VAO;
	GLuint EBO;
	GLuint program;
	GLuint texture;
	GLuint fillLoc;
	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projLoc;
	GLuint texLoc;

	std::vector<Texter*>* vecTexts;

	glm::vec3 fillColor;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};