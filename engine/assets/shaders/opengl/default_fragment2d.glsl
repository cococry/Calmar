#version 460 core

layout(location = 0) out vec4 oColor;
layout(location = 1) out int oEntityId;

in vec4 vColor;
in vec2 vTexCoord;
in flat float vTexIndex;
in flat int vEntityId;

uniform sampler2D uTextures[32];

void main() {
    oColor = texture(uTextures[int(vTexIndex)], vTexCoord) * vColor;

    oEntityId = vEntityId;
}