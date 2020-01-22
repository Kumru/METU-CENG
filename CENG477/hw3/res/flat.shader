#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

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

vec2 CalculateTextureCoordinates(float x, float y)
{
	return vec2(1 - ((x + textureOffset) / texWidth), 1 - (y / texHeight));
}

int determineRegionCode(float x, float y)
{
	int regionCode = 127;
	if (x == 0)
	{
		regionCode &= ~(1 << 0);
		regionCode &= ~(1 << 1);
	}
	else if (x == texWidth)
	{
		regionCode &= ~(1 << 3);
		regionCode &= ~(1 << 4);
	}

	if (y == 0)
	{
		regionCode &= ~(1 << 0);
		regionCode &= ~(1 << 5);
	}
	else if (y == texHeight)
	{
		regionCode &= ~(1 << 2);
		regionCode &= ~(1 << 3);
	}
	return regionCode;
}

void SetVertexNormal()
{
	potAdj[0] = vec4(posPreMVP.x - 1, 0, posPreMVP.z - 1, 1);
	potAdj[1] = vec4(posPreMVP.x - 1, 0, posPreMVP.z    , 1);
	potAdj[2] = vec4(posPreMVP.x    , 0, posPreMVP.z + 1, 1);
	potAdj[3] = vec4(posPreMVP.x + 1, 0, posPreMVP.z + 1, 1);
	potAdj[4] = vec4(posPreMVP.x + 1, 0, posPreMVP.z    , 1);
	potAdj[5] = vec4(posPreMVP.x    , 0, posPreMVP.z - 1, 1);

	int regionCode = determineRegionCode(posPreMVP.x, posPreMVP.z);

	for(int i = 0; i < 6; i++)
	{
		if((regionCode & (1 << i)) > 0)
		{
			potAdj[i].y = heightFactor * texture(heightMap,
					CalculateTextureCoordinates(potAdj[i].x, potAdj[i].z)).x;
		}
	}

	if ((regionCode & 3) == 3)
		vertexNormal += cross(vec3(potAdj[1] - posPreMVP), vec3(potAdj[0] - posPreMVP));
	if ((regionCode & 6) == 6)
		vertexNormal += cross(vec3(potAdj[2] - posPreMVP), vec3(potAdj[1] - posPreMVP));
	if ((regionCode & 12) == 12)
		vertexNormal += cross(vec3(potAdj[3] - posPreMVP), vec3(potAdj[2] - posPreMVP));
	if ((regionCode & 24) == 24)
		vertexNormal += cross(vec3(potAdj[4] - posPreMVP), vec3(potAdj[3] - posPreMVP));
	if ((regionCode & 48) == 48)
		vertexNormal += cross(vec3(potAdj[5] - posPreMVP), vec3(potAdj[4] - posPreMVP));
	if ((regionCode & 33) == 33)
		vertexNormal += cross(vec3(potAdj[0] - posPreMVP), vec3(potAdj[5] - posPreMVP));
}

void main()
{
	v_TexCoord = CalculateTextureCoordinates(position.x, position.z);
	heightValue = texture(heightMap, v_TexCoord).x;
	
	posPreMVP = vec4(position.x, heightFactor * heightValue, position.z, 1.0f);
	
	fragPos = vec3(posPreMVP);
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

vec3 ambient  = vec3(0.075f, 0.075f, 0.075f);
vec3 diffuse  = vec3(1.0f, 1.0f, 1.0f);
vec3 specular = vec3(1.0f, 1.0f, 1.0f);

void main()
{
	vec3 L = normalize(lightPos - fragPos);
	vec3 V = normalize(camPos - fragPos);
	vec3 N = normalize(vertexNormal);
	vec3 R = reflect(-L, N);

	float diffuseCoefficient  = max(0.f, -dot(N, L));
	float specularCoefficient = pow(max(0.f, dot(V, R)), 100);

	diffuse  *= diffuseCoefficient;
	specular *= specularCoefficient;

	color = vec4(texture(colorMap, v_TexCoord).xyz * (ambient + diffuse + specular), 1.0f);
}