#shader vertex
#version 330 core

#define M_PI 3.1415926535897932384626433832795
#define RADIUS 175.0f

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;

uniform mat4 MVP;
uniform int texWidth;
uniform int texHeight;
uniform sampler2D heightMap;

uniform float heightFactor;
uniform int textureOffset;

out vec2 v_TexCoord;
out vec3 vertexNormal;
out vec3 fragPos;

vec4 posPreMVP;
vec4 potAdj[6];

float heightValue;

float sectorCount = 250;
float stackCount = 125;
float sectorStep = 2 * M_PI / sectorCount;
float stackStep = M_PI / stackCount;

vec2 CalculateTextureCoordinates(float j, float i)
{
	return vec2((j + textureOffset)/sectorCount, i / stackCount);
}

vec4 CalculateVertexPosition(float j, float i)
{
	float sectorAngle, stackAngle, xy, x, y, z;
	sectorAngle = j * sectorStep;
	stackAngle = M_PI / 2.0f - i * stackStep;

	xy = RADIUS * cos(stackAngle);
	x = xy * cos(sectorAngle);
	y = xy * sin(sectorAngle) + heightFactor * texture(heightMap, CalculateTextureCoordinates(j, i)).x;
	z = RADIUS * sin(stackAngle);

	return vec4(x, y, z, 1);
}

void SetVertexNormal()
{
	if(tex_coord.x == 0 || tex_coord.x == sectorCount + 1 || tex_coord.y == 0 || tex_coord.y == stackCount + 1)
		return;

	vertexNormal = vec3(0, 0, 0);

	float j = tex_coord.x, i = tex_coord.y;
	
	potAdj[0] = CalculateVertexPosition(j - 1, i - 1);
	potAdj[1] = CalculateVertexPosition(j - 1, i    );
	potAdj[2] = CalculateVertexPosition(j    , i + 1);
	potAdj[3] = CalculateVertexPosition(j + 1, i + 1);
	potAdj[4] = CalculateVertexPosition(j + 1, i    );
	potAdj[5] = CalculateVertexPosition(j    , i - 1);

	vertexNormal += cross(vec3(potAdj[1] - posPreMVP), vec3(potAdj[0] - posPreMVP));
	vertexNormal += cross(vec3(potAdj[2] - posPreMVP), vec3(potAdj[1] - posPreMVP));
	vertexNormal += cross(vec3(potAdj[3] - posPreMVP), vec3(potAdj[2] - posPreMVP));
	vertexNormal += cross(vec3(potAdj[4] - posPreMVP), vec3(potAdj[3] - posPreMVP));
	vertexNormal += cross(vec3(potAdj[5] - posPreMVP), vec3(potAdj[4] - posPreMVP));
	vertexNormal += cross(vec3(potAdj[0] - posPreMVP), vec3(potAdj[5] - posPreMVP));
}

void main()
{
	v_TexCoord = CalculateTextureCoordinates(tex_coord.x, tex_coord.y);
	heightValue = texture(heightMap, v_TexCoord).x;

	posPreMVP = vec4(position.x, position.y + heightFactor * heightValue, position.z, 1.0f);

	fragPos = vec3(posPreMVP);
	vertexNormal = fragPos;
	SetVertexNormal();

	gl_Position = MVP * posPreMVP;
}



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D colorMap;
uniform vec3 camPos;
uniform vec3 lightPos;

in vec2 v_TexCoord;
in vec3 vertexNormal;
in vec3 fragPos;

vec3 ambient = vec3(0.075f, 0.075f, 0.075f);
vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);
vec3 specular = vec3(1.0f, 1.0f, 1.0f);

void main()
{
	
	vec3 L = normalize(lightPos - fragPos);
	vec3 V = normalize(camPos - fragPos);
	vec3 N = normalize(vertexNormal);
	vec3 R = reflect(-L, N);

	float diffuseCoefficient = max(0.f, -dot(N, L));
	float specularCoefficient = pow(max(0.f, dot(V, R)), 100);

	diffuse *= diffuseCoefficient;
	specular *= specularCoefficient;

	color = vec4(texture(colorMap, v_TexCoord).xyz * (ambient + diffuse + specular), 1.0f);	
}