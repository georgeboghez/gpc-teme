#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <GL/glut.h>

#define dim 300

unsigned char prevKey;

class GrilaCarteziana {
private:
	int x, y;
	double dc, dl, cx, cy, eps;
public:
	int n, m;
	GrilaCarteziana(int n, int m, int ci, int cj) {
		this->n = n;
		this->m = m;
		this->eps = 0.1;
		this->x = ci;
		this->y = cj;
		this->dc = (2 - 2 * eps) / (m - 1);
		this->dl = (2 - 2 * eps) / (n - 1);
		this->cx = -1 + eps + x * dc;
		this->cy = -1 + eps + y * dl;
	}

	void afisarePuncteCerc3(int x, int y, std::vector<std::pair<int, int>> &M) {
		M.push_back({ x, y });
		M.push_back({ -x, -y });
		M.push_back({ -x, y });
		M.push_back({ x, -y });
		if (x != y) {
			M.push_back({ y, x });
			M.push_back({ -y, -x });
			M.push_back({ -y, x });
			M.push_back({ y, -x });
		}
	}

	double convertX(int x) {
		return cx + x * dc;
	}
	
	double convertY(int y) {
		return cy + y * dl;
	}

	void afisareCerc4(int r) {
		int x = 0, y = r;
		int d = 1 - r;
		int dE = 3, dSE = -2 * r + 5;
		std::vector<std::pair<int, int>> M;
		afisarePuncteCerc3(x, y, M);
		while (y > x) {
			if (d < 0) {
				d += dE;
				dE += 2;
				dSE += 2;
			}
			else
			{
				d += dSE;
				dE += 2;
				dSE += 2;
			}
			x++;
			afisarePuncteCerc3(x, y, M);
		}
		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		for (auto p : M) {
			//printf("(%f, %f)\n", convertX(p.first), convertY(p.second));
			double new_x = convertX(r * cos(p.first));
			double new_y = convertY(r * sin(p.second));
			printf("(%d, %d)\n", p.first, p.second);
			printf("(%f, %f)\n", new_x, new_y);
			glVertex2f(new_x, new_y);
		}
		glEnd();
	}

	// to be replaced
	void drawCircle(float cx, float cy, float r, int num_segments) {
		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		for (int ii = 0; ii < num_segments; ii++) {
			float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments); //get the current angle 
			float x = r * cosf(theta); //calculate the x component 
			float y = r * sinf(theta); //calculate the y component 
			glVertex2f(x + cx, y + cy); //output vertex 
		}
		glEnd();
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
		drawCircle(xGL, yGL, .04, 10);
	}


	void afisareSegmentDreapta3(int x0, int y0, int xn, int yn) {
		drawLine(x0, y0, xn, yn);
		int dx = xn - x0;
		int dy = yn - y0;
		int d = 2 * dy - dx;
		int dE = 2 * dy;
		int dNE = 2 * (dy - dx);

		int x = x0;
		int y = y0;
		
		drawCircle(convertX(x), convertY(y), 0.04, 10);
		while (x < xn) {
			if (d <= 0) {
				d += dE;
				x++;
			}
			else
			{
				d += dNE;
				x++;
				y++;
			}
			drawCircle(convertX(x), convertY(y), .04, 10);
		}
	}


	void afisareSegmentDreapta4(int x0, int y0, int xn, int yn) {
		drawLine(x0, y0, xn, yn);
		int dx = xn - x0;
		int dy = yn - y0;

		int d = 2 * dy - dx;
		
		int dE = 2 * dy;
		int dNE = 2 * (dy - dx);

		int x = x0;
		int y = y0;

		while (x <= xn)
		{
			drawCircle(convertX(x), convertY(y), 0.04, 10);
			drawCircle(convertX(x + 1), convertY(y), 0.04, 10); // E
			drawCircle(convertX(x + 1), convertY(y + 1), 0.04, 10); // NE
			drawCircle(convertX(x), convertY(y + 1), 0.04, 10); // N
			drawCircle(convertX(x - 1), convertY(y + 1), 0.04, 10); // NV
			drawCircle(convertX(x - 1), convertY(y), 0.04, 10); // V
			drawCircle(convertX(x - 1), convertY(y - 1), 0.04, 10); //SV
			drawCircle(convertX(x), convertY(y - 1), 0.04, 10); // S
			drawCircle(convertX(x + 1), convertY(y - 1), 0.04, 10); // SE
			x += 3;
			y -= 1;
		}
	}
	

	void draw() {
		glColor3f(0, 0, 0);
		for (int i = 0; i < m; i++) {
			glBegin(GL_LINE_STRIP);
			glVertex2f(-1 + eps + i * dc, -1 + eps);
			glVertex2f(-1 + eps + i * dc, 1 - eps);
			glEnd();
		}

		for (int i = 0; i < n; i++) {
			glBegin(GL_LINE_STRIP);
			glVertex2f(-1 + eps, -1 + eps + i * dl);
			glVertex2f(1 - eps, -1 + eps + i * dl);
			glEnd();
		}

		glColor3f(0, 0, 1);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-1 + eps + x * dc, -1 + eps);
		glVertex2f(-1 + eps + x * dc, 1 - eps);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex2f(-1 + eps, -1 + eps + y * dl);
		glVertex2f(1 - eps, -1 + eps + y * dl);
		glEnd();

		//writePixel(1, 1);

		afisareSegmentDreapta3(0, 0, 15, 7);

		afisareSegmentDreapta4(0, 15, 15, 10);
	}
};


void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(3);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	GrilaCarteziana GC(16, 16, 0, 0);
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
