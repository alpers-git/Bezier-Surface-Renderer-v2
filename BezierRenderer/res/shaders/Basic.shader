#shader vertex
#version 430 core 

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_NormalMat;

out vec2 v_TexCoord;
out vec3 L, E, N;
out float ldistance;

uniform vec4 u_LightPosition;

uniform mat4 u_MVP;

void main()
{
	ldistance = distance(u_LightPosition.xyz, position.xyz);

	vec3 pos = (u_MVP * position).xyz;

	// vector from vertex position to light source
	// check for directional light

	if (u_LightPosition.w == 0.0)
		L = normalize(u_LightPosition.xyz);
	else
		L = normalize(u_LightPosition.xyz - pos);

	// Because the eye point the is at the orgin
	// the vector from the vertex position to the eye is

	E = -normalize(pos);

	// halfway vector

	//vec3 H = normalize(L + E);

	// Transform vertex normal into eye coordinates
	N = normalize(mat3(u_NormalMat) * normal);

	gl_Position = u_MVP * position ;
	v_TexCoord = texCoord;
};

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 L, E, N;
in float ldistance;

//uniform vec4 u_Color;
uniform vec4 u_AmbientProduct;
uniform vec4 u_DiffuseProduct;
uniform vec4 u_SpecularProduct;
uniform float u_Shininess;
uniform sampler2D u_Texture;

void main()
{
	float fatt = 1.0 / (5.0 + 5.0 * ldistance + 5.0 * ldistance * ldistance);
	vec4 fColor;

	vec3 H = normalize(L + E);
	vec4 ambient = u_AmbientProduct;

	float Kd = max(dot(L, N), 0.0);
	vec4  diffuse = Kd * u_DiffuseProduct;

	float Ks = pow(max(dot(N, H), 0.0), u_Shininess);
	vec4  specular = Ks * u_SpecularProduct;

	if (dot(L, N) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);

	fColor = ambient + diffuse + specular;
	fColor.a = 1.0;
	fColor * fatt;
	color = fColor * texture(u_Texture, v_TexCoord);

	/*vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;*/
};