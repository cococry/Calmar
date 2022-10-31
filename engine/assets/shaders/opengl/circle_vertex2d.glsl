#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aLocalPosition;
layout(location = 2) in vec4 aColor;
layout(location = 3) in float aThickness;
layout(location = 4) in float aFade;
layout(location = 5) in int aEntityId;

uniform mat4 uViewProj;

out vec3 vLocalPosition;
out vec4 vColor;
out float vThickness;
out float vFade;
out flat int vEntityId; 

void main() {
    vLocalPosition = aLocalPosition;
    vColor = aColor;
    vThickness = aThickness;
    vFade = aFade;
    vEntityId = aEntityId;

    gl_Position = uViewProj * vec4(aPosition, 1.0);
}