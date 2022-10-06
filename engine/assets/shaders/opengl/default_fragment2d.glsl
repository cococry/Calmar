#version 460 core

out vec4 oColor;

in vec4 vColor;
in vec2 vTexCoord;
in flat float vTexIndex;

uniform sampler2D uTextures[32];

void main() {
    oColor = texture(uTextures[int(vTexIndex)], vTexCoord) * vColor;
}