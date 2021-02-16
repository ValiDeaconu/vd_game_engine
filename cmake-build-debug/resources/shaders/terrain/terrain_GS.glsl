#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 gTexCoords[];

out vec2 fTexCoords;
out vec3 fPosition;
out vec3 fTangent;
out vec4 fPosition_ls;

out mat3 fNormalMatrix;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 localModel;
uniform mat4 worldModel;

uniform mat4 lightView;
uniform mat4 lightProjection;

uniform vec3 cameraPosition;

uniform int highDetailRange;

uniform usampler2D splatMap;

#include "material_lib.glsl"

uniform vec4 clipPlane;

vec2 getTexCoords(int i) {
    if (i == 0) {
        return gTexCoords[0];
    }
    else if (i == 1) {
        return gTexCoords[1];
    }
    else {
        return gTexCoords[2];
    }
}

vec3 computeTangent(vec3 v0, vec3 v1, vec3 v2, vec2 uv0, vec2 uv1, vec2 uv2) {
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;

    vec2 deltaUV1 = uv1 - uv0;
    vec2 deltaUV2 = uv2 - uv0;

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

    return normalize((e1 * deltaUV2.y - e2 * deltaUV1.y) * r);
}

void main() {
    vec3 tangent = vec3(0.0f);

    float dist = (
        distance(gl_in[0].gl_Position.xyz, cameraPosition) +
        distance(gl_in[1].gl_Position.xyz, cameraPosition) +
        distance(gl_in[2].gl_Position.xyz, cameraPosition)
    ) / 3.0f;

    float displacement[3] = { 0.0f, 0.0f, 0.0f };
    if (dist < highDetailRange) {
        tangent = computeTangent(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gTexCoords[0], gTexCoords[1], gTexCoords[2]);

        for (int k = 0; k < gl_in.length(); ++k) {
            float height = gl_in[k].gl_Position.y;

            uint splatMask = texture(splatMap, getTexCoords(k)).r;

            float scale = 0.0f;
            for (uint i = 0; i < MAX_MATERIALS; ++i) {
                uint msk = (1 << i);
                if ((splatMask & msk) > 0) {
                    scale += texture(materials[i].displaceMap, getTexCoords(k) * materials[i].horizontalScaling).r * materials[i].heightScaling;
                }
            }

            // attenuate the scale factor using the distance to the vertex
            scale *= clamp(-distance(gl_in[k].gl_Position.xyz, cameraPosition) / (highDetailRange - 50) + 1.0f, 0.0f, 1.0f);

            displacement[k] = scale;
        }
    }

    for (int i = 0; i < gl_in.length(); ++i) {
        vec4 worldCoords = gl_in[i].gl_Position + vec4(0.0f, displacement[i], 0.0f, 0.0f);
        vec4 eyeSpaceCoords = view * worldCoords;
        gl_Position = projection * eyeSpaceCoords;

        // Set clipping plane
        gl_ClipDistance[0] = dot(worldCoords, clipPlane);

        fTexCoords = getTexCoords(i);

        fPosition = eyeSpaceCoords.xyz;
        fTangent = tangent;
        fPosition_ls = lightProjection * lightView * worldCoords;

        fNormalMatrix = transpose(inverse(mat3(view * (worldModel + localModel))));

        EmitVertex();
    }

    EndPrimitive();
}