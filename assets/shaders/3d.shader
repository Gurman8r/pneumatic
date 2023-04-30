#pragma shader vertex
#version 460 core

layout(location = 0) in vec4 Vertex_Position;
layout(location = 1) in vec4 Vertex_Normal;
layout(location = 2) in vec4 Vertex_Texcoord;
layout(location = 3) in vec4 Vertex_Tangent;
layout(location = 4) in vec4 Vertex_Bitangent;

layout(location = 0) out vec4 Frag_Position;
layout(location = 1) out vec4 Frag_Normal;
layout(location = 2) out vec4 Frag_Texcoord;
layout(location = 3) out vec4 Frag_Tangent;
layout(location = 4) out vec4 Frag_Bitangent;

layout(std140, binding = 0) uniform SceneState_Uniforms {
	mat4 CameraProj;
	mat4 CameraView;
};

layout(std140, binding = 1) uniform RenderPass_Uniforms {
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
	Frag_Position = (CameraProj * CameraView * Transform) * Vertex_Position;
	Frag_Normal = Vertex_Normal;
	Frag_Texcoord = Vertex_Texcoord;
	Frag_Tangent = Vertex_Tangent;
	Frag_Bitangent = Vertex_Bitangent;
	gl_Position = Frag_Position;
}

#pragma shader pixel
#version 460 core

layout(location = 0) in vec4 Frag_Position;
layout(location = 1) in vec4 Frag_Normal;
layout(location = 2) in vec4 Frag_Texcoord;
layout(location = 3) in vec4 Frag_Tangent;
layout(location = 4) in vec4 Frag_Bitangent;

layout(location = 0) out vec4 Out_Color;

layout(location = 0) uniform sampler2D Diffuse_Map;
layout(location = 1) uniform sampler2D Specular_Map;

void main() {
	Out_Color = texture(Diffuse_Map, Frag_Texcoord.xy);
}