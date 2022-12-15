#shader vertex
#version 330 core

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec4 frag_VertexColor;
out vec2 frag_VertexUV;

uniform mat4 MVP;

void main()
{
	gl_Position.xy = (MVP * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	frag_VertexColor = vertexColor;
	frag_VertexUV = vec2(vertexUV.x, 1.0 -vertexUV.y);
}


#shader fragment
#version 330 core

out vec4 fragColor;

in vec4 frag_VertexColor;
in vec2 frag_VertexUV;

uniform sampler2D genericSampler;

void main()
{
	vec4 currentTexture = texture(genericSampler, frag_VertexUV);
	fragColor = vec4(frag_VertexColor.rgb, currentTexture.r);
	//fragColor = vec4(frag_VertexUV.x, frag_VertexUV.y, 0.2, 1.0);
}