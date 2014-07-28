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
void normalize(vertex & v);

void draw_triangle(vertex v[3]) {
	vertex n = v[0];
	normalize(n);
	glNormal3f(n.x, n.y, n.z);
	glVertex3f(v[0].x, v[0].y, v[0].z);
	n = v[1];
	normalize(n);
	glNormal3f(n.x, n.y, n.z);
	glVertex3f(v[1].x, v[1].y, v[1].z);
	n = v[2];
	normalize(n);
	glNormal3f(n.x, n.y, n.z);
	glVertex3f(v[2].x, v[2].y, v[2].z);
}

// Main loop
void main_loop_function() {
	static float angle = 0;
	// And depth (used internally to block obstructed objects)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Load identity matrix
	glLoadIdentity();
	GLfloat light_position[] = { 1, 1, 0.5, 0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

	glTranslatef(0, 0, -9);
	glPushMatrix();

	glRotatef(angle, 0, 1, 0);
	glTranslatef(0, 0, -2.2);

	drawSphereTS(1, 4, 8);

	glPopMatrix();
	glPushMatrix();

	glRotatef(angle, 0, 1, 0);
	glTranslatef(0, 0, 2.2);

	drawSphereT(1, 8, 16);

	glPopMatrix();
	glPushMatrix();

	glRotatef(angle, 1, 0, 0);
	glTranslatef(0, 4, 0);

	drawSphereT(2, 16, 32);

	glPopMatrix();
	glRotatef(angle, 1, 0, 0);
	glTranslatef(0, -3, 0);

	drawSphereT(1.5, 64, 128);

	glutSwapBuffers();
	angle += 1;
}
// Initialze OpenGL perspective matrix
void GL_Setup(int width, int height) {
	GLfloat mat_specular[] = { 1, 1, 1, 1 };
	GLfloat mat_ambient[] = { 0.2, 0.2, 0, 1 };
	GLfloat mat_diffuse[] = { 1, 1, 0, 0.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1, 1, 0.5, 0 };
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
	glBegin(GL_TRIANGLES);
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
			} else {
				if (!first) {
					vertex t1[3] = { prevCircle[xySector], prev, current };
					vertex t2[3] = { prevCircle[xySector - 1], prev,
							prevCircle[xySector] };
					draw_triangle(t1);
					draw_triangle(t2);
				}
				prev = current;
			}
			first = false;
			currentCircle.push_back(current);
		}
		prevCircle = currentCircle;
	}
	glEnd();
}

void drawSphereTS(float r, int hsectors, int wsectors) {
	glBegin(GL_TRIANGLE_STRIP);
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
	}
	glEnd();
}

void drawTriangleStrip(vertex & v) {
	vertex normal = v;
	normalize(normal);
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(v.x, v.y, v.z);
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
	normalize(normal);
}

void normalize(vertex & v) {
	float len = (float) (sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)));
	if (len == 0.0f)
		len = 1.0f;
	v.x /= len;
	v.y /= len;
	v.z /= len;
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

