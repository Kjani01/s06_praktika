//
// Fragment Shader für den morphologischen Operator 
// Erosion
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

uniform vec2 offsetsB[25] = vec2[](	vec2(-2,  2), vec2(-2,  1), vec2(-2,  0), vec2(-2,  1), vec2(-2,  2),
									vec2(-1,  2), vec2(-1,  1), vec2(-1,  0), vec2(-1,  1), vec2(-1,  2),
									vec2( 0,  2), vec2( 0,  1), vec2( 0,  0), vec2( 0,  1), vec2( 0,  2),
									vec2( 1,  2), vec2( 1,  1), vec2( 1,  0), vec2( 1,  1), vec2( 1,  2),
									vec2( 2,  2), vec2( 2,  1), vec2( 2,  0), vec2( 2,  1), vec2( 2,  2)	);

uniform vec2 offsetsC[49] = vec2[](	vec2(-3,  3), vec2(-2,  3), vec2(-1,  3), vec2(0,  3), vec2(1,  3), vec2(2,  3), vec2(3,  3), 
										vec2(-3,  2), vec2(-2,  2), vec2(-1,  2), vec2(0,  2), vec2(1,  2), vec2(2,  2), vec2(3,  2),
										vec2(-3,  1), vec2(-2,  1), vec2(-1,  1), vec2(0,  1), vec2(1,  1), vec2(2,  1), vec2(3,  1),
										vec2(-3,  0), vec2(-2,  0), vec2(-1,  0), vec2(0,  0), vec2(1,  0), vec2(2,  0), vec2(3,  0),
										vec2(-3, -1), vec2(-2, -1), vec2(-1, -1), vec2(0, -1), vec2(1, -1), vec2(2, -1), vec2(3, -1),
										vec2(-3, -2), vec2(-2, -2), vec2(-1, -2), vec2(0, -2), vec2(1, -2), vec2(2, -2), vec2(3, -2),
										vec2(-3, -3), vec2(-2, -3), vec2(-1, -3), vec2(0, -3), vec2(1, -3), vec2(2, -3), vec2(3, -3)	);

void main()
{

	float window = 3.0;
	
	if (param1[3] == 5) {
		window = 5.0;
	}
	else if (param1[3] == 7)  {
		window = 7.0;
	}

	vec4 texel = texture(textureMap, texCoords);
	vec4 minimum = texel;
	vec4 tmp;

	for (int i = 0; i < window*window; i++) {
		
			if (window == 5.0) {
				tmp = texture(textureMap, texCoords + offsetsB[i]);
			}
			else if (window == 7.0) {
				tmp = texture(textureMap, texCoords + offsetsC[i]);
			}
			else {
				tmp = texture(textureMap, texCoords + offsets[i]);
			}

		if (min(tmp, minimum) == tmp) {
			minimum = tmp;
		}
	}

    fragColor =	minimum;

}