#version 430 core

in vec2 TexCoords;

out vec4 FragColour;

uniform sampler2D image;

void main() {
	FragColour = texture(image, TexCoords);
}