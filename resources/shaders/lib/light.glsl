struct Light {
	int Type;
	float AmbientStrength;
	float SpecularStrength;
	float Shininess;
	vec3 Direction;
	vec3 Position;
	vec3 Color;
	vec3 Attenuation;
};

struct Material {
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

const int MAX_LIGHTS = VDGE_LIGHTS_COUNT;
uniform Light lights[MAX_LIGHTS];
uniform Light sun;
uniform mat3 lightDirectionMatrix;

uniform mat4 view;

Material computePointLight(Light light, vec3 normalEye, vec3 viewDirN, vec3 fragPos) {
	Material material;

	//compute light position in eye coordinates
	vec4 lightPosEye = view * vec4(light.Position, 1.0f);

	//compute distance to light
	float dist = length(lightPosEye.xyz - fragPos);

	//compute attenuation
	float attFactor = 1.0f / (light.Attenuation.x + light.Attenuation.y * dist + light.Attenuation.z * (dist * dist));

	//compute light direction
	vec3 lightDir = normalize(lightPosEye.xyz - fragPos);

	//compute ambient light
	material.Ambient =  attFactor * light.AmbientStrength * light.Color;

	//compute diffuse light
	material.Diffuse = attFactor * max(dot(normalEye, lightDir), 0.0f) * light.Color;

	//compute specular light
	vec3 reflection = reflect(-lightDir, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), light.Shininess);
	
	material.Specular = attFactor * light.SpecularStrength * specCoeff * light.Color;
	
	return material;
}

Material computeDirectionalLight(Light light, vec3 normalEye, vec3 viewDirN, mat3 lightDirMatrix) {
	Material material;

	//compute light direction
	vec3 lightDirN = normalize(lightDirMatrix * light.Direction);

	//compute ambient light
	material.Ambient = light.AmbientStrength * light.Color;

	//compute diffuse light
	material.Diffuse = max(dot(normalEye, lightDirN), 0.0f) * light.Color;

	//compute specular light
	// //compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), light.Shininess);

	material.Specular = light.SpecularStrength * specCoeff * light.Color;

	return material;
}

vec3 modulateWithLightsAndShadow(Light sun,
								Light lights[MAX_LIGHTS],
								vec3 normalEye,
								vec3 viewDirN,
								mat3 lightDirMatrix,
								vec3 fragPos,
								Material material,
								float shadow) {
	Material total = computeDirectionalLight(sun, normalEye, viewDirN, lightDirMatrix);

	for (int i = 0; i < MAX_LIGHTS; ++i) {
		Material curr;

		if (lights[i].Type == 0) {
			curr = computeDirectionalLight(lights[i], normalEye, viewDirN, lightDirMatrix);
		} else if (lights[i].Type == 1) {
			curr = computePointLight(lights[i], normalEye, viewDirN, fragPos);
		} else {
			// TODO: Finish the spot light computation
		}

		total.Ambient += curr.Ambient;
		total.Diffuse += curr.Diffuse;
		total.Specular += curr.Specular;
	}

	total.Ambient *= material.Ambient;
	total.Diffuse *= material.Diffuse;
	total.Specular *= material.Specular;

	return min(total.Ambient + (1.0f - shadow) * (total.Diffuse + total.Specular), 1.0f);
}