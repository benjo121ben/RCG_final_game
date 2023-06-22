#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPos;
} ubo;

layout(set = 1, binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    float ka = 0.05f;
    float kd = 0.5f;
    float ks = 2.0f;
    float shininessVal = 80.0f;

    mat4 inv_view_matrix = inverse(ubo.view);
    vec3 camPos = vec3(inv_view_matrix[3][0], inv_view_matrix[3][1], inv_view_matrix[3][2]);

    vec3 camDir = normalize(camPos - fragPos);
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(ubo.lightPos - fragPos); //direction TO light

    float lambertian = max(dot(N, L), 0.0);

    vec3 H = normalize(L + camDir);      // halways
    float specAngle = max(dot(N, H), 0.0);
    float specular = pow(specAngle, shininessVal);

    outColor = texture(texSampler, fragTexCoord) * vec4((ka * vec3(1,1,1) + kd * lambertian * vec3(1,1,1) + specular * ks * vec3(1,1,1)), 1);
    //outColor = vec4(1) * (ka + kd * lambertian  + specular * ks);
}