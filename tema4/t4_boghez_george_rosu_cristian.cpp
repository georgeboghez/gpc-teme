#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <GL/glut.h>

#define dim 300
#define CIRCLE_EDGES 20

using namespace std;

unsigned char prevKey;

class GrilaCarteziana {
private:
	int x, y;
	double dc, dl, cx, cy, epsX, epsY, RADIUS;
public:
	int n, m;
	GrilaCarteziana(int n, int m, int ci, int cj) {
		this->n = n;
		this->m = m;
		this->epsX = 0.1;
		this->epsY = 0.1;
		this->RADIUS = 0.64 / m;
		this->x = ci;
		this->y = cj;
		this->dc = (2 - 2 * epsX) / (m - 1);
		this->dl = (2 - 2 * epsY) / (n - 1);
		this->cx = -1 + epsX + x * dc;
		this->cy = -1 + epsY + y * dl;
	}

	double convertX(int x) {
		return cx + x * dc;
	}

	double convertY(int y) {
		return cy + y * dl;
	}

	// to be replaced
	void drawCircle(float cx, float cy, float r, int num_segments) {
		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		for (int ii = 0; ii < num_segments; ii++) {
			float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments); //get the current angle 
			float x = r * cosf(theta); //calculate the x component 
			float y = r * sinf(theta); //calculate the y component 
			glVertex2f(x + cx, y + cy); //output vertex
		}
		glEnd();
	}

	void drawEllipse(float cx, float cy, float a, float b, int num_segments) {
		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		for (int ii = 0; ii < num_segments; ii++) {
			float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments); //get the current angle 
			float x = a * cosf(theta); //calculate the x component 
			float y = b * sinf(theta); //calculate the y component
			glVertex2f(x + cx, y + cy); //output vertex
		}
		glEnd();
	}


	void drawArc(float cx, float cy, float r, int num_segments) {
		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		for (int ii = 0; ii < num_segments / 4 + 1; ii++) {
			float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments); //get the current angle 
			float x = r * cosf(theta); //calculate the x component 
			float y = r * sinf(theta); //calculate the y component 
			glVertex2f(x + cx, y + cy); //output vertex
		}
		glEnd();
	}


	void afisarePuncteCerc3(int x, int y, vector<pair<int, int>> &m) {
		m.push_back({ x, y });
		m.push_back({ x + 1, y });
		m.push_back({ x - 1, y });
	}


	void afisareCerc4(int x0, int y0, int r) {
		drawArc(convertX(x0), convertY(y0), dc*r, 100);

		int x = r, y = 0;
		int d = 1 - r;
		int dN = 3;
		int dNV = -2 * r + 5;
		vector<pair<int, int>> m;
		afisarePuncteCerc3(x, y, m);

		while (x > y) {
			if (d < 0) {
				d += dN;
				dN += 2;
				dNV += 2;
			}
			else {
				d += dNV;
				dN += 2;
				dNV += 4;
				x--;
			}
			y++;
			afisarePuncteCerc3(x, y, m);
		}

		for (auto coord : m) {
			writePixel(coord.first + x0, coord.second + y0);
		}
	}

	void umplereElipsa(int x0, int y0, float a, float b) {
		drawEllipse(convertX(x0), convertY(y0), dc * a, dc * b, 100);

		int xi = 0, x = 0, y = -b;
		double fxpyp = 0;
		double deltaV, deltaNV, deltaN;

		vector<pair<int, int>> ssm;
		ssm.push_back({ x + x0, y + y0});

		while (a*a*(y - 0.5) < b*b*(x + 1)) {
			deltaV = b * b*(-2 * x + 1);
			deltaNV = b * b*(-2 * x + 1) + a * a*(2 * y + 1);

			if (fxpyp + deltaV <= 0) {
				fxpyp += deltaV;
				x--;
				ssm.push_back({ x + x0, y + y0});
			}
			else if (fxpyp + deltaNV <= 0) {
				fxpyp += deltaNV;
				x--;
				y++;
				ssm.push_back({ x + x0, y + y0 });
			}
			else
			{
				break;
			}
		}

		while (y < 0) {
			deltaNV = b * b*(-2 * x + 1) + a * a*(2 * y + 1);
			deltaN = a * a*(2 * y + 1);
			if (fxpyp + deltaNV <= 0) {
				fxpyp += deltaNV;
				x--;
				y++;
			}
			else
			{
				fxpyp += deltaN;
				y++;
			}
			ssm.push_back({ x + x0, y + y0 });
		}

		for (auto coords : ssm) {
			writePixel(coords.first, coords.second);
			for (int j = coords.second; j <= y0; j++) {
				writePixel(coords.first, j);
			}
		}
	}


	void drawLine(int x0, int y0, int xn, int yn) {
		double x0GL = convertX(x0);
		double y0GL = convertY(y0);
		double xnGL = convertX(xn);
		double ynGL = convertY(yn);
		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(x0GL, y0GL);
		glVertex2f(xnGL, ynGL);
		glEnd();
	}

	void writePixel(int x, int y) {
		double xGL = convertX(x);
		double yGL = convertY(y);
		drawCircle(xGL, yGL, RADIUS, CIRCLE_EDGES);
	}


	void afisareSegmentDreapta3(int x0, int y0, int xn, int yn) {
		drawLine(x0, y0, xn, yn);
		int x = x0;
		int y = y0;

		while (x <= xn)
		{
			drawCircle(convertX(x), convertY(y), RADIUS, CIRCLE_EDGES);
			if (isInsideGrid(x + 1, y)) {
				drawCircle(convertX(x + 1), convertY(y), RADIUS, CIRCLE_EDGES); // E
			}
			x += 2;
			y += 1;
		}
	}

	bool isInsideGrid(int x, int y) {
		return x >= 0 && x < m&& y >= 0 && y < n;
	}

	void afisareSegmentDreapta4(int x0, int y0, int xn, int yn) {
		drawLine(x0, y0, xn, yn);
		int x = x0;
		int y = y0;

		int directionsX[3] = { -1, 0, 1 };
		int directionsY[3] = { -1, 0, 1 };

		while (x <= xn)
		{
			for (int dx : directionsX) {
				for (int dy : directionsY) {
					if (isInsideGrid(x + dx, y + dy))
						drawCircle(convertX(x + dx), convertY(y + dy), RADIUS, CIRCLE_EDGES);
				}
			}

			x += 3;
			y -= 1;
		}
	}

	void deseneazaGrila() {
		glColor3f(0, 0, 0);
		for (int i = 0; i < m; i++) {
			glBegin(GL_LINE_STRIP);
			glVertex2f(-1 + epsX + i * dc, -1 + epsY);
			glVertex2f(-1 + epsX + i * dc, 1 - epsY);
			glEnd();
		}

		for (int i = 0; i < n; i++) {
			glBegin(GL_LINE_STRIP);
			glVertex2f(-1 + epsX, -1 + epsY + i * dl);
			glVertex2f(1 - epsX, -1 + epsY + i * dl);
			glEnd();
		}
	}

	void deseneazaAxeOrigine() {
		glColor3f(0, 0, 1);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-1 + epsX + x * dc, -1 + epsY);
		glVertex2f(-1 + epsX + x * dc, 1 - epsY);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex2f(-1 + epsX, -1 + epsY + y * dl);
		glVertex2f(1 - epsX, -1 + epsY + y * dl);
		glEnd();
	}

	void draw() {
		deseneazaGrila();
		deseneazaAxeOrigine();

		//afisareCerc4(0, 0, 14);
		umplereElipsa(13, 13, 4, 8);
	}
};


void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(2);

	glPolygonMode(GL_FRONT, GL_FILL);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	GrilaCarteziana GC(27, 27, 0, 0);
	GC.draw();
	glFlush();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27)
		exit(0);
	printf("A fost apasata tasta %c\n", prevKey);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}