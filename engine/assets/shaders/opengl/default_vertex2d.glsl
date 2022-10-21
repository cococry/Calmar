#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in int aEntityId;

uniform mat4 uViewProj;

out vec4 vColor;
out vec2 vTexCoord;
out flat float vTexIndex;
out flat int vEntityId; 

void main() {
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
    vEntityId = aEntityId;
    gl_Position = uViewProj * vec4(aPosition, 1.0);
}