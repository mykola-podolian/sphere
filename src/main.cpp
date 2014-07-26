#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>
#define window_width  640
#define window_height 480

float const PI = 3.14159265;
float const RAD_PER_DEGREE = PI / 180;

using namespace std;

struct vertex {
	float x, y, z;
};

void drawSphereT(float r, int hsectors, int wsectors);
void drawSphereTS(float r, int hsectors, int wsectors);
void drawTriangleStrip(vertex & v);
void normal(vertex v[3], vertex & normal);

void draw_triangle(vertex v[3]) {
	vertex n;
	normal(v, n);
	glNormal3f(n.x, n.y, n.z);
	glVertex3f(v[0].x, v[0].y, v[0].z);
	glVertex3f(v[1].x, v[1].y, v[1].z);
	glVertex3f(v[2].x, v[2].y, v[2].z);
}

// Main loop
void main_loop_function() {
	static float angle = 0;
	// And depth (used internally to block obstructed objects)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Load identity matrix
	glLoadIdentity();

	glTranslatef(0, 0, -9);
	glRotatef(angle, 0, 1, 0);
	glTranslatef(0, 0, -1.5);

	glBegin(GL_TRIANGLES);
	drawSphereT(1, 4, 8);
	glEnd();
	glTranslatef(0, 0, 3);
	glBegin(GL_TRIANGLES);
	drawSphereT(1, 16, 32);
	glEnd();

	glTranslatef(0, 0, -1.5);
	glRotatef(angle, 0, -1, 0);
	glRotatef(angle, 1, 0, 0);
	glTranslatef(0, 2.5, 0);
	glBegin(GL_TRIANGLES);
	drawSphereT(1, 128, 256);
	glEnd();
	glTranslatef(0, -5, 0);
	glBegin(GL_TRIANGLES);
	drawSphereT(1, 32, 64);
	glEnd();
	glutSwapBuffers();
	angle += 1;
}
// Initialze OpenGL perspective matrix
void GL_Setup(int width, int height) {
	GLfloat mat_specular[] = { 1, 1, 1, 1 };
	GLfloat mat_ambient[] = { 0, 0.5, 0, 1 };
	GLfloat mat_diffuse[] = { 1, 1, 0, 0.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1, 1, 0, 0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 0.0 };
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GLU_SMOOTH);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glDepthFunc(GL_LESS);
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, width, height);
	gluPerspective(45, (float) width / height, .1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void drawSphereT(float r, int hsectors, int wsectors) {
	vector<vertex> prevCircle;
	for (int zSector = 1; zSector <= hsectors; zSector++) {
		float zArc = 90 - float(zSector * 180) / hsectors;
		float zR = r * cos(zArc * RAD_PER_DEGREE);
		float zCord = r * sin(zArc * RAD_PER_DEGREE);
		vertex prev;
		bool first = true;
		vector<vertex> currentCircle;
		for (int xySector = 0; xySector < (wsectors + 1); xySector++) {
			float xyArc = float(xySector * 360) / wsectors;
			float xCord = zR * cos(xyArc * RAD_PER_DEGREE);
			float yCord = zR * sin(xyArc * RAD_PER_DEGREE);
			vertex current = { xCord, yCord, zCord };
			if (zSector == 1) {
				vertex top = { 0, 0, r };
				if (!first) {
					vertex triangle[3] = { top, prev, current };
					draw_triangle(triangle);
				}
				prev = current;
				first = false;
			} else {
				if (!first) {
					vertex t1[3] = { prevCircle[xySector], prev, current };
					vertex t2[3] = { prevCircle[xySector - 1], prev,
							prevCircle[xySector] };
					draw_triangle(t1);
					draw_triangle(t2);
				}
				prev = current;
				first = false;
			}
			currentCircle.push_back(current);
		}
		prevCircle = currentCircle;
	}
}

void drawSphereTS(float r, int hsectors, int wsectors) {
	float color = 1;
	vertex prevCircleCords[wsectors];
	for (int zSector = 1; zSector <= hsectors; zSector++) {

		float zArc = 90 - float(zSector * 180) / hsectors;
		float zR = r * cos(zArc * RAD_PER_DEGREE);
		float zCord = r * sin(zArc * RAD_PER_DEGREE);
		vertex second;
		vertex first;
		for (int xySector = 0; xySector < (wsectors + 1); xySector++) {
			float xyArc = float(xySector * 360) / wsectors;
			float xCord = zR * cos(xyArc * RAD_PER_DEGREE);
			float yCord = zR * sin(xyArc * RAD_PER_DEGREE);
			//first point (point of previous circle o top)
			if (zSector == 1) {
				first = {0,0,r};
			} else {
				first = prevCircleCords[xySector];
			}
			//second point (point of current circle o bottom)
			if (zSector < hsectors) {
				second = {xCord, yCord, zCord};
			} else {
				second = {0,0, -r};
			}
			drawTriangleStrip(first);
			drawTriangleStrip(second);

			prevCircleCords[xySector] = second;
		}
		color -= 0.025;
		glColor3f(color, 1 - color, 0);
	}
}

void drawTriangleStrip(vertex & v) {
	static vertex prev;
	static vertex pprev;
	static int counter = 0;
	if (counter == 0) {
		pprev = v;
	} else if (counter == 1) {
		prev = v;
	} else {
		vertex triangle[3] = { pprev, prev, v };
		vertex n;
		normal(triangle, n);
		if (n.x != 0 || n.y != 0 || n.z != 0) {
			glNormal3f(n.x, n.y, n.z);
		}
		pprev = prev;
		prev = v;
	}
	glVertex3f(v.x, v.y, v.z);
	counter++;
}

void normal(vertex v[3], vertex & normal) {
	vertex a, b;
	a.x = v[0].x - v[1].x;
	a.y = v[0].y - v[1].y;
	a.z = v[0].z - v[1].z;
	b.x = v[1].x - v[2].x;
	b.y = v[1].y - v[2].y;
	b.z = v[1].z - v[2].z;
	normal.x = (a.y * b.z) - (a.z * b.y);
	normal.y = (a.z * b.x) - (a.x * b.z);
	normal.z = (a.x * b.y) - (a.y * b.x);
	float len = (float) (sqrt(
			(normal.x * normal.x) + (normal.y * normal.y)
					+ (normal.z * normal.z)));
	if (len == 0.0f)
		len = 1.0f;
	normal.x /= len;
	normal.y /= len;
	normal.z /= len;
}

// Initialize GLUT and start main loop
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(window_width, window_height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Hello GL");
	glutIdleFunc(main_loop_function);
	GL_Setup(window_width, window_height);
	glutMainLoop();
}

