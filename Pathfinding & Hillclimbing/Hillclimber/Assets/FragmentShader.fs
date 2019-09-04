#version 450 core

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;

uniform float currentTime;
uniform sampler2D tex;
uniform vec3 fillColor;

void main(void)
{
	color = vec4(fillColor, 1.0f);
	//color = texture(tex, fragTexCoord);
}