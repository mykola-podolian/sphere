#include <GL/glut.h>
#include <iostream>
#include <cmath>
#define window_width  640
#define window_height 480

float const PI = 3.14159265;
float const RAD_PER_DEGREE = PI / 180;

void drawCircle(float r, int hsectors, int wsectors);

struct vertex {
	float x, y, z;
};

// Main loop
void main_loop_function() {
	static float angle = 0;
	// And depth (used internally to block obstructed objects)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Load identity matrix
	glLoadIdentity();

	glTranslatef(0, 0, -3);
	glRotatef(angle, 1, 1, 0);

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(255, 0, 0);
//	float c[3] = {0,255,255};
//	glMaterialfv(GL_FRONT,GL_DIFFUSE, c);
//	glMaterialfv(GL_FRONT,GL_SPECULAR, c);
//	glMaterialfv(GL_FRONT,GL_AMBIENT, c);
//	glNormal3f(1, 1, 0.01);
//
//	glColor3f(255, 0, 0);
//	glVertex3f(-1, -1, -1);
//	glVertex3f(-1, 1, -1);
//	glVertex3f(1, 1, -1);
//	glVertex3f(-1, -1, -1);
//	glVertex3f(1, 1, -1);
//	glVertex3f(1, -1, -1);
//
//	glColor3f(0, 255, 0);
//	glVertex3f(-1, -1, 1);
//	glVertex3f(1, 1, 1);
//	glVertex3f(-1, 1, 1);
//	glVertex3f(-1, -1, 1);
//	glVertex3f(1, -1, 1);
//	glVertex3f(1, 1, 1);
//
//	glColor3f(255, 255, 0);
//	glVertex3f(1, -1, -1);
//	glVertex3f(1, 1, -1);
//	glVertex3f(1, 1, 1);
//	glVertex3f(1, 1, 1);
//	glVertex3f(1, -1, 1);
//	glVertex3f(1, -1, -1);
//
//	glColor3f(0, 0, 255);
//	glVertex3f(-1, -1, -1);
//	glVertex3f(-1, 1, 1);
//	glVertex3f(-1, 1, -1);
//	glVertex3f(-1, -1, -1);
//	glVertex3f(-1, -1, 1);
//	glVertex3f(-1, 1, 1);
//
//	glColor3f(255, 255, 255);
//	glVertex3f(-1, 1, -1);
//	glVertex3f(1, 1, 1);
//	glVertex3f(1, 1, -1);
//	glVertex3f(-1, 1, -1);
//	glVertex3f(-1, 1, 1);
//	glVertex3f(1, 1, 1);
//
//	glColor3f(255, 0, 255);
//	glVertex3f(-1, -1, -1);
//	glVertex3f(1, -1, -1);
//	glVertex3f(1, -1, 1);
//	glVertex3f(-1, -1, -1);
//	glVertex3f(1, -1, 1);
//	glVertex3f(-1, -1, 1);

	drawCircle(1, 40, 80);

	glEnd();
	glutSwapBuffers();
	angle += 1;
}
// Initialze OpenGL perspective matrix
void GL_Setup(int width, int height) {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	float c[3] = {255,255,255};
//	glLightfv(GL_LIGHT0, GL_SPECULAR, c);
	glDepthFunc(GL_LESS);
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, width, height);
	gluPerspective(45, (float) width / height, .1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void drawCircle(float r, int hsectors, int wsectors) {
	float color = 1;
	glVertex3f(0, 0, 1);
	vertex prev[wsectors];
	for (int zSector = 1; zSector < hsectors; zSector++) {
		float zArc = 90 - float(zSector * 180) / hsectors;
		float zR = r * cos(zArc * RAD_PER_DEGREE);
		float zCord = r * sin(zArc * RAD_PER_DEGREE);
		for (int xySector = 0; xySector < wsectors; xySector++) {
			float xyArc = float(xySector * 360) / wsectors;
			float xCord = zR * cos(xyArc * RAD_PER_DEGREE);
			float yCord = zR * sin(xyArc * RAD_PER_DEGREE);
			glVertex3f(xCord, yCord, zCord);
			vertex current = { xCord, yCord, zCord };
			color -= 0.0003;
			glColor3f(color, 1-color,0);
			if (zSector == 1) {
				glVertex3f(0, 0, r);
			}
//			else if(zSector == (hsectors)){
//				glVertex3f(0, 0, -r);
//			}
			else {
				glVertex3f(prev[xySector].x, prev[xySector].y,
						prev[xySector].z);
			}
			prev[xySector] = current;
		}
	}
	glVertex3f(0, 0, -1);
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

