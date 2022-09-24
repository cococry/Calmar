#version 460 core

out vec4 oColor;

in vec4 vColor;
in vec2 vTexCoord;

uniform sampler2D uTexture;

void main() {
    oColor = texture(uTexture, vTexCoord) * vColor;
}