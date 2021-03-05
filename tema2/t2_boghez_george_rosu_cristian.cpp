#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#define dim 300

unsigned char prevKey;


void Display1() {
	double xmax, ymax, xmin, ymin;
	double a = 1, b = 2;
	double pi = 4 * atan(1);
	double ratia = 0.05;

	xmax = a - b - 1;
	xmin = a + b + 1;
	ymax = ymin = 0;
	for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = a + b * cos(t);
		xmax = (xmax < x1) ? x1 : xmax;
		xmin = (xmin > x1) ? x1 : xmin;

		x2 = a - b * cos(t);
		xmax = (xmax < x2) ? x2 : xmax;
		xmin = (xmin > x2) ? x2 : xmin;

		y1 = a * tan(t) + b * sin(t);
		ymax = (ymax < y1) ? y1 : ymax;
		ymin = (ymin > y1) ? y1 : ymin;

		y2 = a * tan(t) - b * sin(t);
		ymax = (ymax < y2) ? y2 : ymax;
		ymin = (ymin > y2) ? y2 : ymin;
	}

	xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
	ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x2, y2);
	}
	glEnd();
}



void Display2() {
	double pi = 4 * atan(1);
	double xmax = 8 * pi;
	double ymax = exp(1.1);
	double ratia = 0.05;


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (double x = 0; x < xmax; x += ratia) {
		double x1, y1;
		x1 = x / xmax;
		y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();
}



void Display3() {
	double xmax = 100, y1 = 1, x1 = 0;


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0, 0.99);
	for (double i = 0.5; i <= 100; i += 0.5) {
		x1 += 0.01;
		y1 = i == 0 ? 0.99 : i - floor(i) > ceil(i) - i ? (ceil(i) - i) / i : (i - floor(i)) / i;
		glVertex2f(x1, y1 == 1 ? 0.99 : y1);
	}
	glEnd();
}



void Display4() {
	double pi = 4 * atan(1);
	double a = 0.3, b = 0.2;


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (double t = -pi + 0.01; t < pi; t += 0.01) {
		double x = 2 * (a * cos(t) + b) * cos(t);
		double y = 2 * (a * cos(t) + b) * sin(t);
		glVertex2f(x, y);
	}
	glEnd();
}



void Display5() {
	double pi = 4 * atan(1);
	double a = 0.2;

	double firstX = a / (4 * cos(-pi / 2 + 0.01) * cos(-pi / 2 + 0.01) - 3);
	double firstY = 1;

	double lastX = -1;
	double lastY;
	double x, y;
	bool yModified = false;
	std::vector<double> x_v;
	std::vector<double> y_v;

	glColor3f(0.1, 0.1, 1);
	glBegin(GL_LINE_STRIP);
	for (double t = -pi / 2 + 0.01; t < -0.55; t += 0.01) {
		if (t != -pi / 6 || t != pi / 6) {
			x = a / (4 * cos(t) * cos(t) - 3);
			y = a * tan(t) / (4 * cos(t) * cos(t) - 3);
			if (x < -1) {
				break;
			}

			lastX = x;
			if (y < 1) {
				if (!yModified) {
					firstY = y;
					yModified = true;
				}
				x_v.push_back(x);
				y_v.push_back(y);
				glVertex2f(x, y);
			}
		}
	}
	glVertex2f(lastX, firstY);
	glVertex2f(lastX, firstY);
	glVertex2f(firstX, firstY);
	glEnd();
	for (int i = 0; i < x_v.size() - 1; i += 2) {
		if (abs(y_v[i] - y_v[i + 1]) < 0.01) {
			continue;
		}
		glColor3f(1, 0.1, 0.1);
		glBegin(GL_TRIANGLES);
		glVertex2f(lastX, firstY);
		glVertex2f(x_v[i], y_v[i]);
		glVertex2f(x_v[i + 1], y_v[i + 1]);
		glEnd();

	}

}



void Display6() {
	double pi = 4 * atan(1);
	double a = 0.1, b = 0.2;


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (double t = -10; t < 10; t += 0.1) {
		double x = a * t - b * sin(t);
		double y = a - b * cos(t);
		glVertex2f(x, y);
	}
	glEnd();
}



void Display7() {
	double pi = 4 * atan(1);
	double R = 0.1, r = 0.3;


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 2 * pi; t += 0.01) {
		double x = (R + r) * cos(r / R * t) - r * cos(t + r / R * t);
		double y = (R + r) * sin(r / R * t) - r * sin(t + r / R * t);
		glVertex2f(x, y);
	}
	glEnd();
}



void Display8() {
	double pi = 4 * atan(1);
	double R = 0.1, r = 0.3;


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 2 * pi; t += 0.01) {
		double x = (R - r) * cos(r / R * t) - r * cos(t - r / R * t);
		double y = (R - r) * sin(r / R * t) - r * sin(t - r / R * t);
		glVertex2f(x, y);
	}
	glEnd();
}



void Display9() {
	double pi = 4 * atan(1);
	double R = 0.1, a = 0.4;


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (double t = -pi / 4; t < pi / 4; t += 0.01) {
		double r = a * sqrt(2 * cos(2 * t));
		double x = r * cos(t);
		double y = r * sin(t);
		glVertex2f(x, y);
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (double t = -pi / 4; t < pi / 4; t += 0.01) {
		double r = -a * sqrt(2 * cos(2 * t));
		double x = r * cos(t);
		double y = r * sin(t);
		glVertex2f(x, y);
	}
	glEnd();
}



void Display0() {
	double a = 0.02;


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (double t = 0 + 0.01; t < 10; t += 0.01) {
		double r = a * exp(1 + t);
		double x = r * cos(t);
		double y = r * sin(t);
		glVertex2f(x, y);
	}
	glEnd();
}


void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);



	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		Display1();
		break;
	case '2':
		Display2();
		break;
	case '3':
		Display3();
		break;
	case '4':
		Display4();
		break;
	case '5':
		Display5();
		break;
	case '6':
		Display6();
		break;
	case '7':
		Display7();
		break;
	case '8':
		Display8();
		break;
	case '9':
		Display9();
		break;
	case '0':
		Display0();
		break;
	default:
		break;
	}

	glFlush();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27)
		exit(0);
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
