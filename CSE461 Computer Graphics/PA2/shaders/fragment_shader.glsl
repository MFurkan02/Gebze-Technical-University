// === Fragment Shader for Textured and Lit Materials ===
#version 330 core

// Output final fragment color
out vec4 FragColor;

// Inputs from vertex shader
in vec2 TexCoords;   // Texture coordinates
in vec3 FragPos;     // Fragment position in world space
in vec3 Normal;      // Normal vector at the fragment

// === Texture samplers for multiple textures ===
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;

// === Material structure for surface properties ===
struct Material {
    vec3 ambient;    // Ambient reflectivity
    vec3 diffuse;    // Diffuse reflectivity
    vec3 specular;   // Specular reflectivity
    float shininess; // Shininess coefficient for specular highlights
};
uniform Material materials[8];    // Array of materials to choose from
uniform int activeMaterial;       // Index of the currently active material

// === Directional light structure ===
struct DirectionalLight {
    vec3 direction;  // Direction of the light
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight dirLight;

// === Point light structure (position-based light) ===
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;   // Constant attenuation factor
    float linear;     // Linear attenuation factor
    float quadratic;  // Quadratic attenuation factor
};
uniform PointLight pointLight;

// === Spotlight structure ===
struct SpotLight {
    vec3 position;    // Position of the spotlight
    vec3 direction;   // Direction it’s pointing
    float cutOff;     // Inner cutoff angle (cosine value)
    float outerCutOff;// Outer cutoff angle (cosine value)

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;   // Attenuation factors
    float linear;
    float quadratic;
};
uniform SpotLight spotLight;

// Viewer/camera position
uniform vec3 viewPos;

// Texture configuration
uniform int activeTexture;   // Which texture to use
uniform bool useBlending;    // Whether blending two textures is enabled
uniform float blendFactor;   // Blend factor between base and blend texture

// === Returns the currently active texture color ===
vec4 getActiveTextureColor() {
    if (activeTexture == 0) return texture(texture1, TexCoords);
    else if (activeTexture == 1) return texture(texture2, TexCoords);
    else if (activeTexture == 2) return texture(texture3, TexCoords);
    else if (activeTexture == 3) return texture(texture4, TexCoords);
    else if (activeTexture == 4) return texture(texture5, TexCoords);
    else if (activeTexture == 5) return texture(texture6, TexCoords);
    else if (activeTexture == 6) return texture(texture7, TexCoords);
    else return texture(texture1, TexCoords); // Default fallback
}

// === Calculates directional light contribution ===
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, Material mat) {
    vec3 lightDir = normalize(-light.direction);  // Light direction towards fragment
    float diff = max(dot(normal, lightDir), 0.0); // Diffuse intensity
    vec3 reflectDir = reflect(-lightDir, normal); // Reflected light vector
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess); // Specular intensity

    // Combine components
    vec3 ambient = light.ambient * mat.ambient;
    vec3 diffuse = light.diffuse * diff * mat.diffuse;
    vec3 specular = light.specular * spec * mat.specular;

    return ambient + diffuse + specular;
}

// === Calculates point light contribution with attenuation ===
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material mat) {
    vec3 lightDir = normalize(light.position - fragPos); // Light direction from fragment
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);

    // Calculate attenuation based on distance
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // Combine with attenuation
    vec3 ambient = light.ambient * mat.ambient * attenuation;
    vec3 diffuse = light.diffuse * diff * mat.diffuse * attenuation;
    vec3 specular = light.specular * spec * mat.specular * attenuation;

    return ambient + diffuse + specular;
}

// === Calculates spotlight contribution with cutoff and attenuation ===
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material mat) {
    vec3 lightDir = normalize(light.position - fragPos); // Direction from light to fragment
    float theta = dot(lightDir, normalize(-light.direction)); // Angle between light direction and fragment direction

    // Spotlight intensity smoothing between inner and outer cutoff
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // Combine spotlight contribution with intensity and attenuation
    vec3 ambient = light.ambient * mat.ambient * attenuation;
    vec3 diffuse = light.diffuse * diff * mat.diffuse * attenuation * intensity;
    vec3 specular = light.specular * spec * mat.specular * attenuation * intensity;

    return ambient + diffuse + specular;
}

// === Main Fragment Shader Logic ===
void main() {
    // Normalize input vectors
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Fetch the active material
    Material mat = materials[activeMaterial];

    // Combine all light contributions
    vec3 result = vec3(0.0);
    result += calcDirLight(dirLight, norm, viewDir, mat);
    result += calcPointLight(pointLight, norm, FragPos, viewDir, mat);
    result += calcSpotLight(spotLight, norm, FragPos, viewDir, mat);

    // Fetch texture color
    vec4 texColor = getActiveTextureColor();

    // Optional blending with texture7 using linear interpolation
    if (useBlending) {
        vec4 blendColor = texture(texture7, TexCoords);  // Always blend with texture7
        texColor = mix(texColor, blendColor, blendFactor);
    }

    // Final fragment color = lighting * texture color
    FragColor = vec4(result, 1.0) * texColor;
}
