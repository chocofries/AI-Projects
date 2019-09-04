#version 450 core

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;

uniform float currentTime;
uniform float texMix;
uniform float texMarked;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D tex3;

void main(void) 
{
	//vec3 colorTemp = fragColor * abs(sin(currentTime));
	color = mix(texture(tex3, fragTexCoord),mix(texture(tex, fragTexCoord), texture(tex2, fragTexCoord), texMix),texMarked);
}