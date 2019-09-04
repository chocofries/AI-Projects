#include "quad.h"

Quad::Quad()
{
}

Quad::~Quad()
{
	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &VAO);
}

void Quad::Initialise()
{
	GLfloat quadVerts[] =
	{
		//--------------------------------------------- First Triangle (Left-bottom)
		// Position				 // Color			 //Tex Coords
		-0.24f, -0.24f, 0.0f,	 0.0f, 1.0f, 1.0f,	 0.0f, 1.0f,	//Left
		0.24f, -0.24f, 0.0f,	 1.0f, 0.0f, 1.0f,	 1.0f, 1.0f,	//Right
		-0.24f,  0.24f, 0.0f,	 1.0f, 1.0f, 0.0f,	 0.0f, 0.0f,	//Top
		//--------------------------------------------- Second Triangle (Right-top)
		0.24f,  0.24f, 0.0f,	 0.0f, 1.0f, 1.0f,	 1.0f, 0.0f		//Left
	};

	GLuint quadIndices[] =
	{
		0, 1, 2, //1st Triangle
		3, 2, 1  //2nd Triangle
	};

	program = shaderLoader->CreateProgram("Assets/VertexShader.vs", "Assets/FragmentShader.fs", "vertShader0", "fragShader0");

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	//EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	//Vertex Attributes
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	//Get Uniform Locations
	modelLoc = glGetUniformLocation(program, "model");
	viewLoc = glGetUniformLocation(program, "view");
	projLoc = glGetUniformLocation(program, "proj");
	texLoc = glGetUniformLocation(program, "tex");
}

void Quad::Update()
{
	////Position
	glm::vec3 objPosition = glm::vec3(x, y, z);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), objPosition);

	//Rotation
	glm::vec3 rotationAxisZ = glm::vec3(xRot, yRot, zRot);
	glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxisZ);

	//Scale
	glm::vec3 objScale = glm::vec3(xScale, yScale, zScale);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), objScale);

	////Model Matrix
	model = translationMatrix * rotationZ * scaleMatrix;

	////View Matrix
	view = gameCamera->GetView();

	////Projection Matrix
	proj = gameCamera->GetProjection();

	glutPostRedisplay();
}

void Quad::Draw()
{
	glUseProgram(program);

	//Model Matrix
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//View Matrix
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//Projection Matrix
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	//Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(texLoc, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Drawing based on verts
	glBindVertexArray(VAO);
	glPolygonMode(GL_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
