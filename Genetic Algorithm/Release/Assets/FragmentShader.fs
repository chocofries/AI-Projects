#version 450 core

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;

uniform float currentTime;
uniform sampler2D tex;

void main(void)
{
	vec3 colorTemp;

	colorTemp.r = fragColor.r * abs(sin(currentTime + fragColor.g));
	colorTemp.g = fragColor.g * abs(cos(currentTime + fragColor.b));
	colorTemp.b = fragColor.b * abs(tan(currentTime + fragColor.r));
	
	//color = vec4(colorTemp, 1.0f);
	color = texture(tex, fragTexCoord);
}