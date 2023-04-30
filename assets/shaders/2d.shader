#pragma shader vertex
#version 460 core

layout(location = 0) in vec4 Vertex_Position;
layout(location = 1) in vec4 Vertex_Texcoord;

layout(location = 0) out vec4 Frag_Position;
layout(location = 1) out vec4 Frag_Texcoord;

layout(std140, binding = 0) uniform Scene_State_Uniforms {
	mat4 CameraProj;
	mat4 CameraView;
};

layout(std140, binding = 1) uniform Render_Pass_Uniforms {
	mat4 Placeholder;
};

layout(std140, binding = 2) uniform Transforms_Uniforms {
	mat4 Transform;
};

layout(std140, binding = 3) uniform Material_Uniforms {
	vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
	float Shininess;
};

void main() {
	Frag_Position = Vertex_Position;
	Frag_Texcoord = Vertex_Texcoord;
	gl_Position = Frag_Position;
}

#pragma shader pixel
#version 460 core

layout(location = 0) in vec4 Frag_Position;
layout(location = 1) in vec4 Frag_Texcoord;

layout(location = 0) out vec4 Out_Color;

layout(location = 0) uniform sampler2D Diffuse_Map;
layout(location = 1) uniform sampler2D Specular_Map;

void main() {
	Out_Color = texture(Diffuse_Map, Frag_Texcoord.xy);
}