#VERT_START
#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTex;
layout (location = 3) in vec4 vJoints;
layout (location = 4) in vec4 vWeights;

out vec2 texCoord;
out vec4 color;

uniform mat4 jointMatrices[3];
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    // vec4 point = projectionGLM * vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mat4 modelViewProj = projection * view * model;

    mat4 skinMatrix =
    vWeights.x * jointMatrices[uint(vJoints.x)]+
    vWeights.y * jointMatrices[uint(vJoints.y)]+
    vWeights.z * jointMatrices[uint(vJoints.z)]+
    vWeights.w * jointMatrices[uint(vJoints.w)];

    gl_Position = modelViewProj * skinMatrix * vec4(vPos, 1.0f);
    texCoord = vTex;
    // color = vec4(1,1,1,1);
}

#VERT_END
#FRAG_START

#version 450 core

// in vec4 inColor;
in vec2 texCoord;
in vec4 color;

out vec4 FragColor;

uniform sampler2D texSampler;

void main()
{
    // FragColor = mix(texture(texSampler, texCoord), texture(texSampler1, texCoord), 0.2) * inColor;
    // FragColor = texture(texSampler, texCoord) * inColor;
    FragColor = texture(texSampler, texCoord);
}

#FRAG_END