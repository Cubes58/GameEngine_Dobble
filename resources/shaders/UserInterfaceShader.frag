#version 430 core

in vec2 TexCoords;

out vec4 FragColour;

uniform sampler2D image;
uniform vec4 colour;

void main() {
	FragColour = mix(texture(image, TexCoords), colour, 0.5f);
}