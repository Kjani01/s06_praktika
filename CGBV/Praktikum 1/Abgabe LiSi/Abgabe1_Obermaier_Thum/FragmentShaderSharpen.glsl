//
// Fragment Shader f�r Sharpen Operator
// Angepasst f�r Core Profile
// ---------------------------------
//
// @author: Prof. Dr. Alfred Nischwitz
// @lecturer: Prof. Dr. Alfred Nischwitz
//
// (c)2017 Hochschule M�nchen, HM
//
// ---------------------------------
#version 330


smooth in vec2 texCoords;				// pixelbezogene Texturkoordinate
out vec4 fragColor;					// Ausgabewert mit 4 Komponenten zwischen 0.0 und 1.0
uniform sampler2DRect textureMap;		// Sampler f�r die Texture Map
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

		vec4 original = texture(textureMap, texCoords);
		float c = param1[0] / 100;

		vec4 texel = texture(textureMap, texCoords);

		texel = texel * 4 + texture(textureMap, texCoords + offsets[5]) * -1;
		texel = texel + texture(textureMap, texCoords + offsets[3]) * -1;
		texel = texel + texture(textureMap, texCoords + offsets[1]) * -1;
		texel = texel + texture(textureMap, texCoords + offsets[7]) * -1;

		original += texel * c;

		fragColor =	original;

}