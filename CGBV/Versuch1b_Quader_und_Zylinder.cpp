// Versuch1b.cpp
// Ausgangssoftware des 1. Praktikumsversuchs 
// zur Vorlesung Echtzeit-3D-Computergrahpik
// von Prof. Dr. Alfred Nischwitz
// Programm umgesetzt mit der GLTools Library und Vertex-Arrays
#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif
#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include <math.h>
#include <math3d.h>
#include <GL/freeglut.h>
#include <AntTweakBar.h>

GLShaderManager shaderManager;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeline;
GLFrustum viewFrustum;

GLBatch quad1;
GLBatch quad2;
GLBatch quad3;
GLBatch quad4;
GLBatch quad5;
GLBatch quad6;
GLBatch boden;
GLBatch boden2;
GLBatch zylrand;


// Definition der Kreiszahl 
#define GL_PI 3.1415f

// Rotationsgroessen
static float rotation[] = { 0, 0, 0, 0 };

// Flags fuer Schalter
bool bCull = false;
bool bOutline = false;
bool bDepth = true;

//TESSELATION
unsigned int tesselation = 1;
float scsize = 1;

void CreateGeometry();
void RenderScene(void);

//Set Funktion fuer GUI, wird aufgerufen wenn Variable im GUI geaendert wird
void TW_CALL SetTesselation(const void *value, void *clientData)
{
	//printf("SETTESS CALLED\n");
	//Pointer auf gesetzten Typ casten (der Typ der bei TwAddVarCB angegeben wurde)
	const unsigned int* uintptr = static_cast<const unsigned int*>(value);

	//Setzen der Variable auf neuen Wert
	if (*uintptr > 0 && *uintptr < 33) {
		tesselation = *uintptr;
	}

	//Hier kann nun der Aufruf gemacht werden um die Geometrie mit neuem Tesselationsfaktor zu erzeugen
	//RenderScene();
	CreateGeometry();
	//RenderScene();

}

//Get Funktion fuer GUI, damit GUI Variablen Wert zum anzeigen erhaelt
void TW_CALL GetTesselation(void *value, void *clientData)
{
	//printf("GETTESS CALLED\n");
	//Pointer auf gesetzten Typ casten (der Typ der bei TwAddVarCB angegeben wurde)
	unsigned int* uintptr = static_cast<unsigned int*>(value);

	//	//Variablen Wert and GUI weiterreichen
	*uintptr = tesselation;
}



//GUI
TwBar *bar;
void InitGUI()
{
	bar = TwNewBar("TweakBar");
	TwDefine(" TweakBar size='200 400'");
	TwAddVarRW(bar, "Model Rotation", TW_TYPE_QUAT4F, &rotation, "");
	TwAddVarRW(bar, "Depth Test?", TW_TYPE_BOOLCPP, &bDepth, "");
	TwAddVarRW(bar, "Culling?", TW_TYPE_BOOLCPP, &bCull, "");
	TwAddVarRW(bar, "Backface Wireframe?", TW_TYPE_BOOLCPP, &bOutline, "");
	//Hier weitere GUI Variablen anlegen. Für Farbe z.B. den Typ TW_TYPE_COLOR4F benutzen
	//Tesselation Faktor als unsigned 32 bit integer definiert
	TwAddVarRW(bar, "Size", TW_TYPE_FLOAT, &scsize, "");
	TwAddVarCB(bar, "Tesselation", TW_TYPE_UINT32, SetTesselation, GetTesselation, NULL, "");
}

void CreateGeometry()
{

	/*glColor3f(1.0f, 0.0f, 0.0f); // sets color to black.
	glBegin(GL_TRIANGLE_STRIP); // draw in triangle strips
	glVertex2f(0.0f, 0.75f); // top of the roof
	glVertex2f(-0.5f, 0.25f); // left corner of the roof
	glVertex2f(0.5f, 0.25f); // right corner of the roof
	glVertex2f(-0.5f, -0.5f); // bottom left corner of the house
	glVertex2f(0.5f, -0.5f); //bottom right corner of the house
	glEnd();*/

	printf("createGeo called with TESS=%d\n", tesselation);


	M3DVector3f quadVertices[4];
	M3DVector4f quadColors[4];

	// red
	m3dLoadVector4(quadColors[3], 0.635, 0.235, 0.235, 1);
	m3dLoadVector4(quadColors[2], 0.635, 0.235, 0.235, 1);
	m3dLoadVector4(quadColors[1], 0.635, 0.235, 0.235, 1);
	m3dLoadVector4(quadColors[0], 0.635, 0.235, 0.235, 1);

	m3dLoadVector3(quadVertices[0], 0, 100, 0);
	m3dLoadVector3(quadVertices[1], 100, 100, 0);
	m3dLoadVector3(quadVertices[2], 0, 0, 0);
	m3dLoadVector3(quadVertices[3], 100, 0, 0);

	quad1.Begin(GL_TRIANGLE_STRIP, 4);
	quad1.CopyVertexData3f(quadVertices);
	quad1.CopyColorData4f(quadColors);
	quad1.End();

	// blue
	m3dLoadVector4(quadColors[0], 0.235, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[1], 0.235, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[2], 0.235, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[3], 0.235, 0.235, 0.635, 1);

	m3dLoadVector3(quadVertices[0], 0, 0, 0);
	m3dLoadVector3(quadVertices[1], 0, 100, 0);
	m3dLoadVector3(quadVertices[2], 0, 0, 100);
	m3dLoadVector3(quadVertices[3], 0, 100, 100);

	quad2.Begin(GL_TRIANGLE_STRIP, 4);
	quad2.CopyVertexData3f(quadVertices);
	quad2.CopyColorData4f(quadColors);
	quad2.End();

	// cyan
	m3dLoadVector4(quadColors[3], 0.235, 0.635, 0.635, 1);
	m3dLoadVector4(quadColors[2], 0.235, 0.635, 0.635, 1);
	m3dLoadVector4(quadColors[1], 0.235, 0.635, 0.635, 1);
	m3dLoadVector4(quadColors[0], 0.235, 0.635, 0.635, 1);

	m3dLoadVector3(quadVertices[0], 100, 0, 0);
	m3dLoadVector3(quadVertices[1], 100, 100, 0);
	m3dLoadVector3(quadVertices[2], 100, 0, 100);
	m3dLoadVector3(quadVertices[3], 100, 100, 100);

	quad4.Begin(GL_TRIANGLE_STRIP, 4);
	quad4.CopyVertexData3f(quadVertices);
	quad4.CopyColorData4f(quadColors);
	quad4.End();

	// green
	m3dLoadVector4(quadColors[0], 0.235, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[1], 0.235, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[2], 0.235, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[3], 0.235, 0.635, 0.235, 1);

	m3dLoadVector3(quadVertices[0], 0, 100, 100);
	m3dLoadVector3(quadVertices[1], 100, 100, 100);
	m3dLoadVector3(quadVertices[2], 0, 0, 100);
	m3dLoadVector3(quadVertices[3], 100, 0, 100);

	quad3.Begin(GL_TRIANGLE_STRIP, 4);
	quad3.CopyVertexData3f(quadVertices);
	quad3.CopyColorData4f(quadColors);
	quad3.End();

	// yellow
	m3dLoadVector4(quadColors[0], 0.635, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[1], 0.635, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[2], 0.635, 0.635, 0.235, 1);
	m3dLoadVector4(quadColors[3], 0.635, 0.635, 0.235, 1);

	m3dLoadVector3(quadVertices[0], 0, 100, 0);
	m3dLoadVector3(quadVertices[1], 100, 100, 0);
	m3dLoadVector3(quadVertices[2], 0, 100, 100);
	m3dLoadVector3(quadVertices[3], 100, 100, 100);

	quad5.Begin(GL_TRIANGLE_STRIP, 4);
	quad5.CopyVertexData3f(quadVertices);
	quad5.CopyColorData4f(quadColors);
	quad5.End();

	// magenta
	m3dLoadVector4(quadColors[0], 0.635, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[1], 0.635, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[2], 0.635, 0.235, 0.635, 1);
	m3dLoadVector4(quadColors[3], 0.635, 0.235, 0.635, 1);

	m3dLoadVector3(quadVertices[0], 0, 0, 0);
	m3dLoadVector3(quadVertices[1], 100, 0, 0);
	m3dLoadVector3(quadVertices[2], 0, 0, 100);
	m3dLoadVector3(quadVertices[3], 100, 0, 100);

	quad6.Begin(GL_TRIANGLE_STRIP, 4);
	quad6.CopyVertexData3f(quadVertices);
	quad6.CopyColorData4f(quadColors);
	quad6.End();


	//18 Vertices anlegen
	/*M3DVector3f konusVertices[18];
	M3DVector4f konusColors[18];
	// Die Spitze des Konus ist ein Vertex, den alle Triangles gemeinsam haben;
	// um einen Konus anstatt einen Kreis zu produzieren muss der Vertex einen positiven z-Wert haben
	m3dLoadVector3(konusVertices[0], 0, 0, 75);
	m3dLoadVector4(konusColors[0], 0, 1, 0, 1);
	// Kreise um den Mittelpunkt und spezifiziere Vertices entlang des Kreises
	// um einen Triangle_Fan zu erzeugen
	int iPivot = 1;
	int i = 1;
	for (float angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8.0f))
	{
		// Berechne x und y Positionen des naechsten Vertex
		float x = 50.0f*sin(angle);
		float y = 50.0f*cos(angle);
		// Alterniere die Farbe zwischen Rot und Gruen
		if ((iPivot % 2) == 0)
			m3dLoadVector4(konusColors[i], 0.635, 0.235, 0.235, 1);
		else
			m3dLoadVector4(konusColors[i], 0, 0.6, 1, 1);
		// Inkrementiere iPivot um die Farbe beim naechsten mal zu wechseln
		iPivot++;
		// Spezifiziere den naechsten Vertex des Triangle_Fans
		m3dLoadVector3(konusVertices[i], x, y, 0);
		i++;
	}
	konus.Begin(GL_TRIANGLE_FAN, 18);
	konus.CopyVertexData3f(konusVertices);
	konus.CopyColorData4f(konusColors);
	konus.End();*/

	int tesselationCylinder = tesselation * 2;
	int vertSizeCyl = 4 * tesselationCylinder + 2;
	int vertSizeRand = 2 * vertSizeCyl - 2;


	printf("tessCyl: %d, bodenverts: %d, randverts: %d\n", tesselationCylinder, vertSizeCyl, vertSizeRand);

	// Erzeuge einen weiteren Triangle_Fan um den Boden zu bedecken
	auto bodenVertices = new M3DVector3f[vertSizeCyl];
	auto bodenColors = new M3DVector4f[vertSizeCyl];
	auto boden2Vertices = new M3DVector3f[vertSizeCyl];
	auto boden2Colors = new M3DVector4f[vertSizeCyl];
	auto randVertices = new M3DVector3f[vertSizeRand];
	//auto randVertices = new M3DVector3f[10];

	// Das Zentrum des Triangle_Fans ist im Ursprung
	m3dLoadVector3(bodenVertices[0], 0, 0, -75.0);
	m3dLoadVector4(bodenColors[0], 1, 0, 0, 1);
	int i = 1;
	for (float angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / (tesselationCylinder * 2)))
	{
		// Berechne x und y Positionen des naechsten Vertex
		float x = 50.0f*sin(angle);
		float y = 50.0f*cos(angle);

		// Spezifiziere den naechsten Vertex des Triangle_Fans
		m3dLoadVector3(bodenVertices[i], x, y, -75.0);
		m3dLoadVector3(boden2Vertices[i], x, y, 75.0);

		m3dLoadVector3(randVertices[2 * i - 2], x, y, -75.0);
		m3dLoadVector3(randVertices[2 * i - 1], x, y, 75.0);

		printf("Schleife i: %d, Wert x: %f, Wert y: %f\n\n", i, x, y);

		i++;
	}

	for (int i = 0; i < vertSizeCyl; i++) {
		m3dLoadVector4(bodenColors[i], 0.2*i, 0.2, 0.2, 1);
		m3dLoadVector4(boden2Colors[i], 0.2, 0.2*i, 0.2, 1);
	}

	m3dLoadVector3(bodenVertices[vertSizeCyl - 1], bodenVertices[1][0], bodenVertices[1][1], bodenVertices[1][2]);

	boden = GLBatch();
	boden.Begin(GL_TRIANGLE_FAN, vertSizeCyl);
	boden.CopyVertexData3f(bodenVertices);
	boden.CopyColorData4f(bodenColors);
	boden.End();


	// Das Zentrum des Triangle_Fans ist im Ursprung
	m3dLoadVector3(boden2Vertices[0], 0, 0, 75.0);
	m3dLoadVector4(boden2Colors[0], 1, 0, 0, 1);
	i = 1;
	/*for (float angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8.0f))
	{
		// Berechne x und y Positionen des naechsten Vertex
		float x = 50.0f*sin(angle);
		float y = 50.0f*cos(angle);
		// Alterniere die Farbe zwischen Rot und Gruen
		m3dLoadVector4(boden2Colors[i], 0.8, 0.2, 0.2, 1);
		// Spezifiziere den naechsten Vertex des Triangle_Fans
		m3dLoadVector3(boden2Vertices[i], x, y, 75.0);
		m3dLoadVector3(randVertices[2 * i], x, y, 75.0);
		i++;
	}*/
	//m3dLoadVector3(randVertices[0], 0, 0, 0);

	m3dLoadVector3(boden2Vertices[vertSizeCyl - 1], boden2Vertices[1][0], boden2Vertices[1][1], boden2Vertices[1][2]);


	m3dLoadVector3(randVertices[vertSizeRand - 2], randVertices[0][0], randVertices[0][1], randVertices[0][2]);
	m3dLoadVector3(randVertices[vertSizeRand - 1], randVertices[1][0], randVertices[1][1], randVertices[1][2]);

	for (int i = 0; i < vertSizeCyl; i++) {
		printf("Schleife i: %d, Wert x: %f, Wert y: %f, Wert z: %f\n", i, bodenVertices[i][0], bodenVertices[i][1], bodenVertices[i][2]);
	}

	boden2 = GLBatch();
	boden2.Begin(GL_TRIANGLE_FAN, vertSizeCyl);
	boden2.CopyVertexData3f(boden2Vertices);
	boden2.CopyColorData4f(boden2Colors);
	boden2.End();

	auto randColors = new M3DVector4f[vertSizeRand];
	for (int i = 0; i < vertSizeRand; i++) {
		m3dLoadVector4(randColors[i], 0.7, 0.5, 0.2, 1);
	}

	zylrand = GLBatch();
	zylrand.Begin(GL_QUAD_STRIP, vertSizeRand);
	zylrand.CopyVertexData3f(randVertices);
	zylrand.CopyColorData4f(randColors);
	zylrand.End();
}

void DrawSquare() {
	quad1.Draw();
	quad2.Draw();
	quad3.Draw();
	quad4.Draw();
	quad5.Draw();
	quad6.Draw();
}

void DrawCylinder() {
	//modelViewMatrix.PushMatrix();
	boden.Draw();
	//modelViewMatrix.Translate(0.0, 0.0, 20.0);

	zylrand.Draw();

	boden2.Draw();
	//modelViewMatrix.PopMatrix();
}

// Aufruf draw scene
void RenderScene(void)
{
	// Clearbefehle für den color buffer und den depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Schalte culling ein falls das Flag gesetzt ist
	if (bCull)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	// Schalte depth testing ein falls das Flag gesetzt ist
	if (bDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	// Zeichne die Rückseite von Polygonen als Drahtgitter falls das Flag gesetzt ist
	if (bOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);

	// Speichere den matrix state und führe die Rotation durch
	modelViewMatrix.PushMatrix();

	modelViewMatrix.LoadIdentity();
	modelViewMatrix.Scale(scsize, scsize, scsize);

	M3DMatrix44f rot;
	m3dQuatToRotationMatrix(rot, rotation);
	modelViewMatrix.MultMatrix(rot);
	//modelViewMatrix.Translate(0.0, 0.0, -200.0);
	//setze den Shader für das Rendern
	shaderManager.UseStockShader(GLT_SHADER_FLAT_ATTRIBUTES, transformPipeline.GetModelViewProjectionMatrix());
	//Zeichne Konus
	//konus.Draw();
	DrawCylinder();
	//DrawSquare();
	//Auf fehler überprüfen
	gltCheckErrors(0);
	// Hole die im Stack gespeicherten Transformationsmatrizen wieder zurück
	modelViewMatrix.PopMatrix();

	TwDraw();
	// Vertausche Front- und Backbuffer
	glutSwapBuffers();
	glutPostRedisplay();
}

// Initialisierung des Rendering Kontextes
void SetupRC()
{
	// Schwarzer Hintergrund
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// In Uhrzeigerrichtung zeigende Polygone sind die Vorderseiten.
	// Dies ist umgekehrt als bei der Default-Einstellung weil wir Triangle_Fans benützen
	glFrontFace(GL_CW);

	//initialisiert die standard shader
	shaderManager.InitializeStockShaders();
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
	//erzeuge die geometrie
	CreateGeometry();
	InitGUI();
}

void SpecialKeys(int key, int x, int y)
{
	TwEventKeyboardGLUT(key, x, y);
	// Zeichne das Window neu
	glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
	GLfloat nRange = 300.0f;

	// Verhindere eine Division durch Null
	if (h == 0)
		h = 1;
	// Setze den Viewport gemaess der Window-Groesse
	glViewport(0, 0, w, h);
	// Ruecksetzung des Projection matrix stack
	projectionMatrix.LoadIdentity();

	// Definiere das viewing volume (left, right, bottom, top, near, far)
	if (w <= h)
		//viewFrustum.SetOrthographic(-nRange, nRange, -nRange * float(h) / float(w), nRange * float(h) / float(w), nRange, 5*nRange);
		viewFrustum.SetOrthographic(-nRange, nRange, -nRange * float(h) / float(w), nRange * float(h) / float(w), -nRange, nRange);
	else
		//viewFrustum.SetOrthographic(-nRange * float(w) / float(h), nRange * float(w) / float(h), -nRange, nRange, nRange, 5 * nRange);
		viewFrustum.SetOrthographic(-nRange * float(w) / float(h), nRange * float(w) / float(h), -nRange, nRange, -nRange, nRange);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	// Ruecksetzung des Model view matrix stack
	modelViewMatrix.LoadIdentity();

	TwWindowSize(w, h);
}

void ShutDownRC()
{
	//GUI aufräumen
	TwTerminate();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Versuch1");
	glutCloseFunc(ShutDownRC);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Veralteter Treiber etc.
		std::cerr << "Error: " << glewGetErrorString(err) << "\n";
		return 1;
	}

	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT); // same as MouseMotion
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);

	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);

	TwInit(TW_OPENGL_CORE, NULL);
	SetupRC();
	glutMainLoop();

	return 0;
}
