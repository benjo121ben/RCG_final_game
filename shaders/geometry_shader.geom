#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_geometry_shader : enable

//gets 3 vertex triangle from vertex shader
layout(triangles) in;

layout(location = 0) in vec3 infragColor[];
layout(location = 1) in vec2 infragTexCoord[];

//vertices of vertex shader array of vec4
in gl_PerVertex {
    vec4 gl_Position;
} gl_in[];

//output a triangle strip depending on the amount of instances (1 instance = 3 vertices)
layout(triangle_strip, max_vertices = 9) out;

//one for every vertex
layout(location = 0) out vec3 outfragColor;
layout(location = 1) out vec2 outfragTexCoord;



void main() {

    //invert the texture coordinates
    int invertedIndex = gl_in.length() - 1;

    for (int i = 0; i < gl_in.length(); ++i) {
        //move it slightly towards the lower right corner
        gl_Position = gl_in[i].gl_Position + vec4(2.0, 3.0, 0, 1);

        //output the original color and texture coordinate to the fragment shader
        outfragColor = infragColor[i];
        outfragTexCoord = infragTexCoord[invertedIndex--];

        EmitVertex();
    }
    EndPrimitive();
}
