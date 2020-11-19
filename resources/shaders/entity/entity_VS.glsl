#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec4 fPosition;
out vec3 fNormal;
out vec2 fTexCoords;

out mat3 fNormalMatrix;
out mat3 fLightDirectionMatrix;
out float fVisibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float fogDensity;
uniform float fogGradient;

#include <fog_VS.glsl>

uniform vec4 clipPlane;

void main() 
{
	// pass normal and texcoords
	fNormal = vNormal;
	fTexCoords = vTexCoords;

	// world coordinates
	vec4 worldPosition = model * vec4(vPosition, 1.0f);

	// clip plane
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	// compute eye space coordinates
	vec4 cameraPosition = view * worldPosition;
	fPosition = cameraPosition;

	// compute normal matrix
	fNormalMatrix = transpose(inverse(mat3(view * model)));
	
	// compute light direction matrix
	fLightDirectionMatrix = transpose(inverse(mat3(view)));
	
	// compute vertex visibility
	fVisibility = getObjectVisibilityThruFog(cameraPosition.xyz, fogDensity, fogGradient);
	
	// compute vertex position
	gl_Position = projection * cameraPosition;
}
