//
// Fragment Shader für Sobel Operator
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
		vec4 x;
		vec4 y;

		x +=texture(textureMap, texCoords + offsets[2]) * 1;
		x +=texture(textureMap, texCoords + offsets[1]) * 2;
		x +=texture(textureMap, texCoords + offsets[0]) * 1;
		x +=texture(textureMap, texCoords + offsets[8]) * -1;
		x +=texture(textureMap, texCoords + offsets[7]) * -2;
		x +=texture(textureMap, texCoords + offsets[6]) * -1;

		x = x * texel;

		y +=texture(textureMap, texCoords + offsets[2]) * 1;
		y +=texture(textureMap, texCoords + offsets[5]) * 2;
		y +=texture(textureMap, texCoords + offsets[8]) * 1;
		y +=texture(textureMap, texCoords + offsets[0]) * -1;
		y +=texture(textureMap, texCoords + offsets[3]) * -2;
		y +=texture(textureMap, texCoords + offsets[6]) * -1;

		y = y * texel;

		texel = sqrt(x * x + y * y);

		fragColor =	texel;

}