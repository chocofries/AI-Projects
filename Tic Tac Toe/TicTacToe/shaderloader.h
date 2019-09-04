#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include<string>
#include <map>


class ShaderLoader
{
private:
	std::string ReadShader(char *filename);
	GLuint CreateShader(GLenum shaderType, std::string source, char* shaderName);

public:
	ShaderLoader(void);
	~ShaderLoader(void);
	GLuint CreateProgram(char* VertexShaderFilename, char* FragmentShaderFilename, char* vertexShaderName, char* fragmentShaderName);

private:
	std::map<std::string, GLuint> vertexShaderMap;
	std::map<std::string, GLuint> fragmentShaderMap;
	std::map<std::string, GLuint> programMap;
};
