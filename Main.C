#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GHSi.h>
#include "Rotate3D.h"
#include "Proto.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>

#include "MathFunctions.h"
#include "Matrix.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexBuffer.h"

extern ULONG MyRed, MyBlue, MyBlack, MyWhite, MyGreen, MyPink;
extern ULONG TempScala[100];
extern WORD  BgndX;

#define VSIZE (sizeof(X) / sizeof(UWORD))

//                                                 linkes Gebäude                                                                                          rechter turm                                                                                                                                                spitzdach                                      dach bei nebengebäude
//            vordere Fläche          hintere fläche         rechte fläche         linke fläche           obere fläche         untere fläche         Vordere Fläche        hintere Fläche          rechte Fläche          linke Fläche            obere Fläche          untere Fläche      vorderes dreieck  rechtes dreieck   hintere dreieck   linkes dreieck          links              vorne                rechts               hinten
UWORD X[] = { 0, 200, 200,   0,      0, 200, 200,   0,    200, 200, 200, 200,    0,   0,   0,   0,      0, 200, 200,   0,    0, 200, 200,   0,    200, 300, 300, 200,    200, 300, 300, 200,    300, 300, 300, 300,    200, 200, 200, 200,    200, 300, 300, 200,    200, 300, 300, 200,    200, 300, 250,    300, 300, 250,    300, 200, 250,    200, 200, 250,      0,    0,   0,      0, 200, 200,   0,    200, 200, 200,      0,   0, 200, 200,    350,   0,   0 };
UWORD Y[] = { 0,   0, 100, 100,      0,   0, 100, 100,      0,   0, 100, 100,    0, 100, 100,   0,    100, 100, 100, 100,    0,   0,   0,   0,      0,   0, 200, 200,      0,   0, 200, 200,      0,   0, 200, 200,      0,   0, 200, 200,    200, 200, 200, 200,      0,   0,   0,   0,    200, 200, 300,    200, 200, 300,    200, 200, 300,    200, 200, 300,    100,  100, 150,    100, 100, 150, 150,    100, 100, 150,    100, 150, 150, 100,      0, 350,   0 };
UWORD Z[] = { 0,   0,   0,   0,    100, 100, 100, 100,      0, 100, 100,   0,    0,   0, 100, 100,      0,   0, 100, 100,    0,   0, 100, 100,      0,   0,   0,   0,    100, 100, 100, 100,      0, 100, 100,   0,      0, 100, 100,   0,      0,   0, 100, 100,      0,   0, 100, 100,      0,   0,  50,      0, 100,  50,    100, 100,  50,    100,   0,  50,    100,    0,  50,      0,   0,  50,  50,      0, 100,  50,    100,  50,  50, 100,      0,   0, 350 };

UWORD XTransform[VSIZE];
UWORD YTransform[VSIZE];
UWORD ZTransform[VSIZE];
UWORD WTransform[VSIZE];

BYTE indices[] = {
	// Linkes gebäude
	0,   1,  2,  3,  0, -1, // Vordere Fläche
	4,   5,  6,  7,  4, -1, // Hintere Fläche
	8,   9, 10, 11,  8, -1, // Rechte  Fläche
	12, 13, 14, 15, 12, -1, // Linke   Fläche
	16, 17, 18, 19, 16, -1, // obere   Fläche
	20, 21, 22, 23, 20, -1, // untere  Fläche

	// Rechter turm
	24, 25, 26, 27, 24, -1, // Vordere Fläche
	28, 29, 30, 31, 28, -1, // Hintere Fläche
	32, 33, 34, 35, 32, -1, // Rechte Fläche
	36, 37, 38, 39, 36, -1, // Linke Fläche
	40, 41, 42, 43, 40, -1, // obere Fläche
	44, 45, 46, 47, 44, -1, // untere Fläche

	// Spitzdach turm
	48, 49, 50, 48, -1, 		// Vorderes Dreieck
	51, 52, 53, 51, -1, 		// Rechtes Dreieck
	54, 55, 56, 54, -1, 		// Hinteres Dreieck
	57, 58, 59, 57, -1, 		// Linkes Dreieck

	// Linkes Gebäude Dach
	60, 61, 62, 60, -1,		// Links
	63, 64, 65, 66, 63, -1,	// Vorne
	67, 68, 69, 67, -1,		// Rechts
	70, 71, 72, 73, 70, -1,	// Hinten
  -1 };

bool shouldManualRotate = false;
double rotationsMatrix[16];

double Alpha = 0.0;
double Beta  = 0.0;
double Gamma = 0.0;
double angleOffset = 0.0025;
double normalenVektor[3];

void calculate_rotation_matrix()
	{
	double sa = sin(Alpha);
	double ca = cos(Alpha);
	double sb = sin(Beta);
	double cb = cos(Beta);
	double sg = sin(Gamma);
	double cg = cos(Gamma);

	rotationsMatrix[0 + 0 * 4] = cb * cg;
	rotationsMatrix[1 + 0 * 4] = -sg * cb;
	rotationsMatrix[2 + 0 * 4] = sb;
	rotationsMatrix[3 + 0 * 4] = 0;

	rotationsMatrix[0 + 1 * 4] = sa * sb * cg + ca * sg;
	rotationsMatrix[1 + 1 * 4] = -sg * sa * sb + ca * cg;
	rotationsMatrix[2 + 1 * 4] = -sa * cb;
	rotationsMatrix[3 + 1 * 4] = 0;

	rotationsMatrix[0 + 2 * 4] = ca * (-sb) * cg + sa * sg;
	rotationsMatrix[1 + 2 * 4] = -sb * ca * sg + sa * cg;
	rotationsMatrix[2 + 2 * 4] = ca * cb;
	rotationsMatrix[3 + 2 * 4] = 0;

	rotationsMatrix[0 + 3 * 4] = 0;
	rotationsMatrix[1 + 3 * 4] = 0;
	rotationsMatrix[2 + 3 * 4] = 0;
	rotationsMatrix[3 + 3 * 4] = 1;
	}

void calculate_transform()
	{
	// create columns of matrix
	double column0[] = { rotationsMatrix[0], rotationsMatrix[4], rotationsMatrix[8], rotationsMatrix[12] };
	double column1[] = { rotationsMatrix[1], rotationsMatrix[5], rotationsMatrix[9], rotationsMatrix[13] };
	double column2[] = { rotationsMatrix[2], rotationsMatrix[6], rotationsMatrix[10], rotationsMatrix[14] };
	double column3[] = { rotationsMatrix[3], rotationsMatrix[7], rotationsMatrix[11], rotationsMatrix[15] };

	// Multiply 4x4 matrix with coordinates
	for (UBYTE i = 0; i < VSIZE; ++i)
		{
		XTransform[i] = column0[0] * X[i] + column1[0] * Y[i] + column2[0] * Z[i] + column3[0] + (XSIZE / 2);
		YTransform[i] = column0[1] * X[i] + column1[1] * Y[i] + column2[1] * Z[i] + column3[1] + (YSIZE / 2);
		ZTransform[i] = column0[2] * X[i] + column1[2] * Y[i] + column2[2] * Z[i] + column3[2] + (XSIZE / 2);
		WTransform[i] = column0[3] * X[i] + column1[3] * Y[i] + column2[3] * Z[i] + column3[3] + (XSIZE / 2);
		}
	}

void draw_normals(double normalenVektor[], BYTE lastPos, BYTE *pos)
	{
	//CLine(XTransform[pos], YTransform[pos], normalenVektor[0], normalenVektor[1], MyGreen);
	}

void draw_lines()
	{
	BYTE last, next;
	for (BYTE *currentIndex = indices; -1 != *currentIndex; currentIndex++)
		{
		last = *currentIndex++;

		// Guckt die Fläche nach vorne oder nach hinten?
		// -> Nach vorne: Zeichnen
		// -> Nach hinten: überspringen
		// Punkte die nebeneinander liegen -> Kreuzprodukt -> Vorzeichen der z-Achse des Ergebnisses

		BYTE a = next--;
		BYTE b = *(currentIndex - 1);
		BYTE c = *(currentIndex);

		double BA[3];
		double BC[3];

		// Create first directional vector
		BA[0] = XTransform[a] - XTransform[b];
		BA[1] = YTransform[a] - YTransform[b];
		BA[2] = ZTransform[a] - ZTransform[b];

		// Create second directional vector
		BC[0] = XTransform[c] - XTransform[b];
		BC[1] = YTransform[c] - YTransform[b];
		BC[2] = ZTransform[c] - ZTransform[b];

		// Cross product over the z-axis
		cross(normalenVektor, BC, BA);

		// Wenn fläche ausgeblendet werden soll, dann vorspringen bis zum nächsten polygon
		if (normalenVektor[2] < 0.0)
			{
			while (-1 != *currentIndex)
				currentIndex++;

			continue;
			}

		// Fläche soll gezeichnet werden, da die Fläche zum Betrachter zeigt
		while (-1 != *currentIndex)
			{
			next = *currentIndex++;
			CLine(XTransform[last], YTransform[last], XTransform[next], YTransform[next], MyGreen);
			last = next;
			}
		}

	// Achsen
	CLine(XTransform[0], YTransform[0], XTransform[74], YTransform[74], MyBlue);
	Text(XTransform[74], YTransform[74], "X");

	CLine(XTransform[0], YTransform[0], XTransform[75], YTransform[75], MyWhite);
	Text(XTransform[75], YTransform[75], "Y");

	CLine(XTransform[0], YTransform[0], XTransform[76], YTransform[76], MyGreen);
	Text(XTransform[76], YTransform[76], "Z");
	}

void debug_text()
	{
	char text[128];

	sprintf(text, "Angles: Alpha=%5.3f, Beta=%5.3f, Gamma=%5.3f", Alpha, Beta, Gamma);
	Text(10, 30, text);

	sprintf(text, "RotationsMatrix:");
	Text(10, 90, text);

	sprintf(text, "%+5.3f, %+5.3f, %+5.3f, %+5.3f", rotationsMatrix[0], rotationsMatrix[1], rotationsMatrix[2], rotationsMatrix[3]);
	Text(10, 120, text);

	sprintf(text, "%+5.3f, %+5.3f, %+5.3f, %+5.3f", rotationsMatrix[4], rotationsMatrix[5], rotationsMatrix[6], rotationsMatrix[7]);
	Text(10, 150, text);

	sprintf(text, "%+5.3f, %+5.3f, %+5.3f, %+5.3f", rotationsMatrix[8], rotationsMatrix[9], rotationsMatrix[10], rotationsMatrix[11]);
	Text(10, 180, text);

	sprintf(text, "%+5.3f, %+5.3f, %+5.3f, %+5.3f", rotationsMatrix[12], rotationsMatrix[13], rotationsMatrix[14], rotationsMatrix[15]);
	Text(10, 210, text);

	sprintf(text, "NormalenVektor:");
	Text(10, 270, text);

	sprintf(text, "%+5.3f, %+5.3f, %+5.3f", normalenVektor[0], normalenVektor[1], normalenVektor[2]);
	Text(10, 300, text);

	sprintf(text, "Should Manual Rotate: %d (Space)", shouldManualRotate);
	Text(10, 360, text);
	}

void RedrawCallback()
	{
	glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

/*
int main(int argc, char *argv[])
	{
	extern Display *MyDisplay;
	OpenVideo();
	RedrawCallback();

	time_t t;
	srand((unsigned)time(&t));

	FOREVER
		{
		Clear();

		int useAlpha = 0;
		int useBeta = 0;
		int useGamma = 0;
		int multSign = 1;

		if (!shouldManualRotate)
			{
			// Calculate random angles
			// First calculate which angle to edit
			// then if the offset should be positive or negative

			int desiredAngle = 1 + rand() % 3;
			switch(desiredAngle)
				{
				case 1: // Use alpha angle
					{
					int angleSign = 1 + rand() % 2; // whether the angle should be positive or negative
					useAlpha = 1;
					if (angleSign == 1)
						multSign = -1;
					else
						multSign = 1;
					break;
					}

				case 2: // use beta angle
					{
					int angleSign = 1 + rand() % 2;
					useBeta = 1;
					if (angleSign == 1)
						multSign = -1;
					else
						multSign = 1;
					break;
					}

				case 3: // use gamme angle
					{
					int angleSign = 1 + rand() % 2;
					useGamma = 1;
					if (angleSign == 1)
						multSign = -1;
					else
						multSign = 1;
					break;
					}
				}

			for (int i = 0; i < 100; ++i)
				{
				Clear();

				if (useAlpha)
					Alpha += multSign * angleOffset;

				if (useBeta)
					Beta += multSign * angleOffset;

				if (useGamma)
					Gamma += multSign * angleOffset;

				// Create Rotation Matrix
				calculate_rotation_matrix();

				// Perspective
				multiply_matrices(rotationsMatrix, create_perspective(90.0, 1920.0 / 1080.0, -100.0, 10000.0), rotationsMatrix);

				// transform
				multiply_matrices(rotationsMatrix, create_translation(-400.0, 0.0, 0.0), rotationsMatrix);
				multiply_matrices(rotationsMatrix, create_rotation(to_radians(180.0), 1.0, 0.0, 0.0), rotationsMatrix);
				multiply_matrices(rotationsMatrix, create_scale(2.0, 2.0, 2.0), rotationsMatrix);

				calculate_transform();
				draw_lines();

				debug_text();

				// Flush Events and refresh display
				TestEvents();
				XFlush(MyDisplay);
				usleep(10000);
				}
			}
		else
			{
			// Create Rotation Matrix
			calculate_rotation_matrix();

			// Perspective
			multiply_matrices(rotationsMatrix, create_perspective(90.0, 1920.0 / 1080.0, -100.0, 10000.0), rotationsMatrix);

			// transform
			multiply_matrices(rotationsMatrix, create_translation(-400.0, 0.0, 0.0), rotationsMatrix);
			multiply_matrices(rotationsMatrix, create_rotation(to_radians(180.0), 1.0, 0.0, 0.0), rotationsMatrix);
			multiply_matrices(rotationsMatrix, create_scale(2.0, 2.0, 2.0), rotationsMatrix);

			calculate_transform();
			draw_lines();

			debug_text();

			// Flush Events and refresh display
			TestEvents();
			XFlush(MyDisplay);
			usleep(100000);
			}

		useAlpha = 0;
		useBeta = 0;
		useGamma = 0;
		multSign = 1;
		}

	return 0;
	}
*/

int main(int argc, char *argv[])
	{
	extern Display *MyDisplay;
	OpenVideo();
	RedrawCallback();

	time_t t;
	srand((unsigned)time(&t));

	FOREVER
		{
		RedrawCallback();

		

		TestEvents();
		XFlush(MyDisplay);
		usleep(10000);
		}

	return 0;
	}

	