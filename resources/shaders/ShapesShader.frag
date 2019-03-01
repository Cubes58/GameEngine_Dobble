#version 430 core

in vec2 TexCoords;

out vec4 FragColour;

uniform sampler2D image;
uniform vec4 colour;
uniform float mixValue;

void main() {
	FragColour = mix(texture(image, TexCoords), colour, mixValue);
}