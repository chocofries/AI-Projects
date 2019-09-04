/**************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2005 - 2018 Media Design School

File Name	:	shaderloader.h
Description	:	System to manage efficient loading of shaders
Author   	:	Zheng Yu Bu
mail		:	zheng.yu5315@mediadesign.school.nz
**************************************************/
#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include<string>
#include <map>


class ShaderLoader
{
private:
	std::string ReadShader(const char *filename);
	GLuint CreateShader(GLenum shaderType, std::string source, const  char* shaderName);

public:
	ShaderLoader(void);
	~ShaderLoader(void);
	GLuint CreateProgram(const char* VertexShaderFilename, const  char* FragmentShaderFilename, const char* vertexShaderName, const  char* fragmentShaderName);

private:
	std::map<std::string, GLuint> vertexShaderMap;
	std::map<std::string, GLuint> fragmentShaderMap;
	std::map<std::string, GLuint> programMap;
};
