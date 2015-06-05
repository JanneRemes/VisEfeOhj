
#version 330

uniform mat4 unifViewTransform;
uniform mat4 unifWorldTransform;

uniform sampler2D unifDiffuseSampler;
in      vec2      textureCoords;
out     vec4      outColour;

// Lighting
const   uint  LIGHT_TYPE_POINT = 0u;
const   uint  LIGHT_TYPE_SPOT  = 1u;
// - Common
uniform uint  l_type;      // Light Type
uniform float l_range;     // Light Range
uniform vec3  l_position;  // Light Position
uniform vec4  lc_ambient;  // Light Color Ambient
uniform vec4  lc_diffuse;  // Light Color Diffuse
uniform vec4  lc_specular; // Light Color Specular
in      vec3  normal;      // Normal
in      vec4  position;

// - Pointlight

vec4 pointlight()
{
	float d = length(position.xyz - l_position);

	float d2 = d / (1 - pow(d / l_range, 2));
	float atten = 1 / pow(d2 / l_range + 1, 2);
	/**/
	/*
	float kc = 1;                   // Constant attenuation factor
	float kl = 2 / l_range;         // Linear attenuation factor
	float kq = 1 / pow(l_range, 2); // Quadratic attenuation factor
	float atten = 1 / (kc + kl * d + kq * pow(d, 2));
	/**/

	mat4 normalTransform = transpose(inverse(unifViewTransform * unifWorldTransform));
	vec3 fnormal = (normalTransform * vec4(normal, 0)).xyz;

	vec3 view = -normalize((unifViewTransform * unifWorldTransform * position).xyz);
	vec3 half = normalize(normalize(l_position) + view);

	float li_diffuse = max(0, dot(fnormal, normalize(l_position)));
	float li_specular = max(0, pow(dot(fnormal, half), 16));

	vec4 ambient = lc_ambient;
	vec4 diffuse = lc_diffuse * li_diffuse;//*texture(unifDiffuseSampler, textureCoords);
	vec4 specular = lc_specular * li_specular;// *texture(unifDiffuseSampler, textureCoords);
	return (ambient + diffuse + specular) * atten;
}

// - Spotlight
uniform float sl_cutoff;	// Spotlight Cutoff Angle

vec4 spotlight()
{
	// Ambient
	vec4 ambient = lc_ambient;

	// Spotlight cutoff test
	float angle = abs(dot(normalize(l_position), normalize(position.xyz)));

	if (angle > sl_cutoff)
	{
		return lc_ambient;
	}

	// Diffuse
	mat4 normalTransform = transpose(inverse(unifViewTransform * unifWorldTransform));
	vec3 fnormal = (normalTransform * vec4(normal, 0)).xyz;
	float li_diffuse = max(0, dot(fnormal, normalize(l_position)));
	vec4 diffuse = lc_diffuse * li_diffuse;//*texture(unifDiffuseSampler, textureCoords);

	// Specular
	vec3 view = -normalize((unifViewTransform * unifWorldTransform * position).xyz);
	vec3 half = normalize(normalize(l_position) + view);
	float li_specular = max(0, pow(dot(fnormal, half), 16));
	vec4 specular = lc_specular * li_specular;// *texture(unifDiffuseSampler, textureCoords);

	// Attenuation
	float d = length(position.xyz - l_position);
	float d2 = d / (1 - pow(d / l_range, 2));
	float atten = 1 / pow(d2 / l_range + 1, 2);

	// Final color
	return (ambient + diffuse + specular) * atten;
}

void main()
{
	if (l_type == LIGHT_TYPE_POINT)
	{
		outColour = pointlight();
	}
	else if (l_type == LIGHT_TYPE_SPOT)
	{
		outColour = spotlight();
	}
}
