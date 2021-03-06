#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <map>
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
		ssm.push_back({ x + x0, y + y0 });

		while (a*a*(y - 0.5) < b*b*(x + 1)) {
			deltaV = b * b*(-2 * x + 1);
			deltaNV = b * b*(-2 * x + 1) + a * a*(2 * y + 1);

			if (fxpyp + deltaV <= 0) {
				fxpyp += deltaV;
				x--;
				ssm.push_back({ x + x0, y + y0 });
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

	struct Varf
	{
		double x, y;
	};

	struct Muchie
	{
		Varf vi, vf;
	};

	struct Intersectie
	{

		bool operator==(const Intersectie &rhs) const {
			return (ymax == rhs.ymax && xmin == rhs.xmin && ratia == rhs.ratia);
		}

		double ymin;
		double ymax;

		double xmin;
		double xmax;
		double ratia;
	};


	vector<vector<Intersectie>> initET(vector<Muchie> p)
	{
		vector<vector<Intersectie>> et;
		double xm, ym, xM, yM;
		bool change;

		for (int i = 0; i < 100; ++i) {
			vector<Intersectie> v;
			et.push_back(v);
		}

		for (auto m : p) {
			if (m.vi.y != m.vf.y) {
				ym = m.vi.y < m.vf.y ? m.vi.y : m.vf.y;
				yM = m.vi.y > m.vf.y ? m.vi.y : m.vf.y;
				xm = ym == m.vi.y ? m.vi.x : m.vf.x;
				xM = yM == m.vi.y ? m.vi.x : m.vf.x;
				et[ym].push_back({ ym, yM, xm, xM, (xm - xM) / (ym - yM) });
			}
		}

		for (int i = 0; i < 100; ++i) {
			do {
				change = false;
				if (et[i].size() == 0) {
					break;
				}
				for (unsigned int j = 1; j < et[i].size(); ++j) {
					if (et[i][j].xmin < et[i][j - 1].xmin) {
						Intersectie aux(et[i][j]);
						et[i][j] = et[i][j - 1];
						et[i][j - 1] = aux;
						change = true;
					}
				}
				//if (et[i].size() > 1) {
				//	cout << et[i][0].xmin << " " << et[i][1].xmin << endl;
				//}
			} while (change);
		}

		return et;
	}

	vector<Intersectie> copyVector(vector<Intersectie> toCopy) {
		vector<Intersectie> v;

		for (unsigned int i = 0; i < toCopy.size(); i++)
			v.push_back(toCopy[i]);
		return v;
	}

	vector<vector<Intersectie>> calculSSM(vector<Muchie> p, vector<vector<Intersectie>> et) {
		vector<Intersectie> activeSSM;
		vector<vector<Intersectie>> finalET;

		int y = -1, k;

		bool done = false;
		for (int i = 0; i < 100; ++i) {
			vector<Intersectie> v;
			finalET.push_back(v);

			if (!et[i].empty() && !done) {
				y = i;
				done = true;
			}
		}

		if (y == -1) {
			return finalET;
		}

		do {
			activeSSM = copyVector(et[y]);
			for (unsigned int i = 1; i < activeSSM.size(); ++i) {
				//cout << activeSSM[i].ymax << "  " << y << endl;
				if (activeSSM[i].ymax == y) {

					cout << "before " << activeSSM.size() << endl;
					vector<Intersectie> aux;
					for (unsigned int j = 0; j < activeSSM.size(); j++) {
						if (i != j) {
							aux.push_back(activeSSM[j]);
						}
					}
					activeSSM.clear();
					for (unsigned int j = 0; j < aux.size(); j++) {
						activeSSM.push_back(aux[j]);
					}
					cout << "after " << activeSSM.size() << endl;
					//activeSSM.erase(remove(activeSSM.begin(), activeSSM.end(), i), activeSSM.end());
				}
			}

			k = activeSSM.size();

			while (k >= 2)
			{
				for (int i = 1; i < k; i++) {
					if (activeSSM[i].xmin < activeSSM[i - 1].xmin) {
						Intersectie aux(activeSSM[i]);
						activeSSM[i] = activeSSM[i - 1];
						activeSSM[i - 1] = aux;
					}
				}
				k--;
			}


			finalET[y] = copyVector(activeSSM);
			y++;
			while (y < 100 && et[y].empty()) {
				y++;
			}

			for (unsigned int i = 0; i < activeSSM.size(); ++i) {
				if (activeSSM[i].ratia != 0) {
					activeSSM[i].xmin += activeSSM[i].ratia;
				}
			}
			
		} while ((!activeSSM.empty() || !et[y].empty()) && y < 100);

		return finalET;
	}

	void convScanDreptunghi(double xm, double xM, double ym, double yM)
	{
		for (int x = xm; x < xM; x++)
			for (int y = ym; y < yM; y++)
				writePixel(x, y);
	}

	void coloreaza(vector<vector<Intersectie>> finalET) {
		int k = 0;
		for (auto i : finalET) {
			if (i.size() > 0) {
				cout << "Intersectie: " << k << endl;
				for (auto j : i) {
					cout << j.xmin << " " << j.ymax << endl;
				}
				cout << endl;
			}
			k++;
		}
	}

	double distance(double x1, double y1, double x2, double y2) {
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}

	map<double, vector<double>> aflareIntersectii(vector<Muchie> p) {
		map<double, vector<double>> intersectii;
		for (double y = 0; y < n; ++y) {
			vector<double> xsFound;
			for (auto m : p) {
				if ((y <= m.vi.y && y >= m.vf.y) || (y <= m.vf.y && y >= m.vi.y)) {
					double minDist = n + 1;
					double xToFind;
					for (double x = min(m.vi.x, m.vf.x); x <= max(m.vi.x, m.vf.x); ++x) {
						double diff = distance(m.vi.x, m.vi.y, x, y) + distance(m.vf.x, m.vf.y, x, y) - distance(m.vi.x, m.vi.y, m.vf.x, m.vf.y);
						if (diff < minDist) {
							minDist = diff;
							xToFind = x;
						}
					}
					xsFound.push_back(xToFind);
				}
			}
			if (!xsFound.empty()) {
				sort(xsFound.begin(), xsFound.end());
				intersectii.insert({ y, xsFound });
			}
		}
		return intersectii;
	}

	vector<Muchie> crearePoligon() {
		int n;
		ifstream f("polygon.txt");

		f >> n;

		int i = 0;
		vector<Muchie> p;

		double x, y;

		f >> x >> y;
		i++;

		glColor3f(1, 0, 0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(convertX(x), convertY(y));
		while (i < n)
		{
			double x2, y2;
			f >> x2 >> y2;
			Varf v1 = { x, y };
			Varf v2 = { x2, y2 };

			Muchie m = { v1, v2 };

			p.push_back(m);

			x = x2;
			y = y2;

			++i;

			glVertex2f(convertX(x), convertY(y));
		}

		Muchie m = { p.back().vf, p.front().vi };
		p.push_back(m);


		glEnd();

		f.close();

		return p;

	}

	void coloreaza_poligon(map<double, vector<double>> intersectii) {

		auto it = intersectii.begin();
		bool shouldColor;
		while (it != intersectii.end())
		{
			for (unsigned int second = 1; second < (*it).second.size(); second += 2) {
				unsigned int first = second - 1;
				for (double iterator = (*it).second[first]; iterator <= (*it).second[second]; ++iterator) {
					writePixel(iterator, (*it).first);
				}
			}

			it++;
		}
	}


	void draw() {
		deseneazaGrila();
		deseneazaAxeOrigine();

		//afisareCerc4(0, 0, 14);
		//umplereElipsa(13, 13, 4, 8);
		vector<Muchie> p = crearePoligon();

		map<double, vector<double>> intersectii = aflareIntersectii(p);
		coloreaza_poligon(intersectii);
		
		//vector<vector<Intersectie>> et = initET(p);
		//coloreaza(et);
		//et = calculSSM(p, et);
		//coloreaza(et);
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