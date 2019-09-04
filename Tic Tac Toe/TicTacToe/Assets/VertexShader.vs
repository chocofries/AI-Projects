#version 450 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 fragColor;
out vec2 fragTexCoord;

uniform float posX;
uniform float posY;
uniform float texU;
uniform float texV;
uniform int flipX;
uniform int flipY;

void main(void)
{
	gl_Position = vec4(vertex.x + posX,vertex.y + posY,vertex.z, 1.0);
	fragColor = color;
	fragTexCoord = vec2(flipX*texCoord.x+texU,flipY*texCoord.y+texV);
}