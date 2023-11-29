#version 430

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture;
uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirectionLight {
    int enable;
    vec3 direction;
    vec3 lightColor;
};

struct PointLight {
    int enable;
    vec3 position;  
    vec3 lightColor;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight {
    int enable;
    vec3 position;
    vec3 direction;
    vec3 lightColor;
    float cutOff;

    float constant;
    float linear;
    float quadratic;      
}; 

uniform Material material;
uniform DirectionLight dl;
uniform PointLight pl;
uniform Spotlight sl;

// Function to calculate ambient lighting
vec4 calculateAmbient(Material material, vec3 lightColor, float attenuation) {
    return vec4(lightColor * material.ambient, 1.0) * attenuation;
}

// Function to calculate diffuse lighting
vec4 calculateDiffuse(Material material, vec3 lightColor, vec3 lightDir, vec3 normal, float attenuation) {
    float diffuseCoefficient = max(dot(normalize((-1) * lightDir), normalize(normal)), 0.0);
    return vec4(lightColor * material.diffuse, 1.0) * diffuseCoefficient * attenuation;
}

// Function to calculate specular lighting
vec4 calculateSpecular(Material material, vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float shininess, float attenuation) {
    vec3 reflectDir = reflect(lightDir, normalize(normal));
    float specularCoefficient = pow(max(dot(normalize(reflectDir), normalize(viewDir)), 0.0), shininess);
    return vec4(lightColor * material.specular, 1.0) * specularCoefficient * attenuation;
}

void main() {
    vec4 ambient = vec4(0.0);
    vec4 diffuse = vec4(0.0);
    vec4 specular = vec4(0.0);

    // Directional light
    if (dl.enable == 1) {
        ambient += calculateAmbient(material, dl.lightColor, 1.0);
        diffuse += calculateDiffuse(material, dl.lightColor, dl.direction, Normal, 1.0);
        specular += calculateSpecular(material, dl.lightColor, dl.direction, Normal, normalize(viewPos - FragPos), material.shininess, 1.0);
    }

    // Point light
    if (pl.enable == 1) {
        float dist = length(FragPos - pl.position);
        float attenuation = 1.0 / (pl.constant + pl.linear * dist + pl.quadratic * dist * dist);
        ambient += calculateAmbient(material, pl.lightColor, attenuation);
        diffuse += calculateDiffuse(material, pl.lightColor, FragPos - pl.position, Normal, attenuation);
        specular += calculateSpecular(material, pl.lightColor, FragPos - pl.position, Normal, normalize(viewPos - FragPos), material.shininess, attenuation);
    }

    // Spot light
    if (sl.enable == 1) {
        float dist = length(FragPos - sl.position);
        float attenuation = 1.0 / (sl.constant + sl.linear * dist + sl.quadratic * dist * dist);
        ambient += calculateAmbient(material, sl.lightColor, attenuation);

        vec3 lightDir = FragPos - sl.position;
        float spotFactor = dot(normalize(lightDir), normalize(sl.direction));
        
        if (spotFactor > sl.cutOff) {
            diffuse += calculateDiffuse(material, sl.lightColor, lightDir, Normal, attenuation);
            specular += calculateSpecular(material, sl.lightColor, lightDir, Normal, normalize(viewPos - FragPos), material.shininess, attenuation);
        }
    }

    color = (ambient + diffuse + specular) * texture(ourTexture, TexCoord);
}