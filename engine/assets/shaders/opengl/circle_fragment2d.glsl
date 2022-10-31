#version 460 core

layout(location = 0) out vec4 oColor;
layout(location = 1) out int oEntityId;

in vec3 vLocalPosition;
in vec4 vColor;
in float vThickness;
in float vFade;
in flat int vEntityId; 

void main() {
    /*
    float dstnc = 1.0f - length(vLocalPosition);
    vec3 color = vec3(smoothstep(0.0, vFade, dstnc));
    color *= vec3(smoothstep(vThickness + vFade, vThickness, dstnc));

    oColor = vColor; 
    oColor.rgb *= color;

    oEntityId = vEntityId;  
    */
    oColor = vec4(0.2, 0.3, 0.8, 1.0);
    oEntityId = 10;
}