#shader vertex
#version 330 core

in vec2 vertexPosition;
in vec4 vertexColor;

out vec4 frag_VertexColor;

uniform mat4 MVP;

void main()
{
	gl_Position.xy = (MVP * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	frag_VertexColor = vertexColor;
}


#shader fragment
#version 330 core

out vec4 fragColor;

in vec4 frag_VertexColor;

void main()
{
	fragColor = frag_VertexColor;
}