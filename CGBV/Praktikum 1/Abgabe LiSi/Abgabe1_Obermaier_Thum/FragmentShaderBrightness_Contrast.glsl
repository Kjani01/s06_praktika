//
// Fragment Shader Helligkeit und Kontrast
// Angepasst für Core Profile
// ---------------------------------
//
// @author: Prof. Dr. Alfred Nischwitz
// @lecturer: Prof. Dr. Alfred Nischwitz
//
// (c)2017 Hochschule München, HM
//
// ---------------------------------
#version 330


smooth in vec2 texCoords;				// pixelbezogene Texturkoordinate
out vec4 fragColor;					// Ausgabewert mit 4 Komponenten zwischen 0.0 und 1.0
uniform sampler2DRect textureMap;		// Sampler für die Texture Map
uniform vec4 param1;					// Param1 X,Y,Z,W in GUI

uniform vec2 offsets[9] = vec2[](	vec2(-1,  1),
									vec2(-1,  0),
									vec2(-1, -1),
									vec2( 0,  1),
									vec2( 0,  0),
									vec2( 0, -1),
									vec2( 1,  1),
									vec2( 1,  0),
									vec2( 1, -1)	);

void main()
{

    vec4 texel = texture(textureMap, texCoords);

	float alpha = texel[3];

	texel = texel - 0.5;
	texel = texel * ((param1[1] / 100) + 1.0);
	texel = texel + (param1[0] / 100);
	texel = texel + 0.5;
	texel[3] = alpha;

	fragColor = texel;

}