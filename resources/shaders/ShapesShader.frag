#version 430 core

in vec2 TexCoords;

out vec4 FragColour;

uniform sampler2D image;
uniform vec4 colour;
uniform float mixValue;
uniform bool usingColourAsTextureMask;

void main() {
	vec4 textureFragColour = texture(image, TexCoords);
	if (usingColourAsTextureMask && textureFragColour.a <= 0.0f)
		discard;
	
	FragColour = mix(textureFragColour, colour, mixValue);
}