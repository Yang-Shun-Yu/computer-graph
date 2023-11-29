#version 430

in vec2 TexCoord;
in vec3 color;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
	FragColor = texture(ourTexture, TexCoord);
}
