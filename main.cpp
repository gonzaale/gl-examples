#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "./freeglut-3.2.1/include/GL/freeglut.h"

#define PI 3.14159

using namespace std;

bool leftDown = false, rightDown = false;
int lastPos[2];
float cameraPos[4] = {0,1,4,1};
int windowWidth = 640, windowHeight = 480;
double yRot = 0;
int curExample = 1;

float specular[] = { 1.0, 1.0, 1.0, 1.0 };
float shininess[] = { 50.0 };

void example1() {
	float theta = PI / 2;
	float rotate = 75.0;

	for (int i = 0; i < 10; i++) {
		glLoadIdentity();
		gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0, 0, 0, 0, 1, 0);

		glTranslatef(1.4 * cos(theta), 1.4 * sin(theta), 0.0);
		glRotatef(rotate, 0.0, 0.0, 1.0);
		glutSolidTeapot(0.175);

		theta += PI / 5;
		rotate += 35.0;
	}
}

void example2() {
	glTranslatef(-1.75, 0.0, 0.0);	

	for (int i = 20; i > 0; i--) {
		glTranslatef(0.175, 0.0, 0.0);

		for (int j = 0; j < i + 2; j++) {
			glutSolidCube(0.175);
			glTranslatef(0.0, 0.05, 0.0);
		}

		glTranslatef(0.0, -0.05 * (i + 2), 0.0);
	}


}

void example3() {
	glTranslatef(-1.5, 1.2, 0.0);

	for (int i = 6; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			glutSolidTeapot(0.175);
			glTranslatef(0.6, 0.0, 0.0);
		}

		glTranslatef(-0.6 * i + 0.3, -0.6, 0.0);
	}
}

void draw_tree() {
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.15, 1.0, 64, 64);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.35, 0.4, 64, 64);
	glTranslatef(0.0, 0.0, 0.25);
	glutSolidCone(0.3, 0.4, 64, 64);
	glTranslatef(0.0, 0.0, 0.25);
	glutSolidCone(0.25, 0.4, 64, 64);
	glPopMatrix();
}

void example4() {
	glPushMatrix();

	glTranslatef(-1.2, 0.0, 0.8);
	draw_tree();

	glTranslatef(0.7, 0.0, -2.7);
	draw_tree();

	glTranslatef(2.1, 0.0, 1.9);
	draw_tree();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.0, 0.0, 1.3);
	glScalef(0.4, 0.4, 0.4);
	glutSolidIcosahedron();

	glPopMatrix();

	glBegin(GL_TRIANGLES);
	glVertex2f(-0.75, 0.5);
	glVertex2f(0.75, 0.5);
	glVertex2f(0, 2.0);
	glEnd();

}

void display() {
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glBegin(GL_LINES);
		glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(1,0,0); // x axis
		glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,1,0); // y axis
		glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,1); // z axis
	glEnd(/*GL_LINES*/);

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,windowWidth,windowHeight);

	float ratio = (float)windowWidth / (float)windowHeight;
	gluPerspective(50, ratio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0, 0, 0, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, cameraPos);

	glRotatef(yRot,0,1,0);

	switch(cur_example) {
	case 1:
		example1();
		break;
	case 2:
		example2();
		break;
	case 3:
		example3();
		break;
	case 4:
		example4();
		break;
	}

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) leftDown = (state == GLUT_DOWN);
	else if (button == GLUT_RIGHT_BUTTON) rightDown = (state == GLUT_DOWN);

	lastPos[0] = x;
	lastPos[1] = y;
}

void mouseMoved(int x, int y) {
	if (leftDown) yRot += (x - lastPos[0])*.1;
	if (rightDown) {
		for (int i = 0; i < 3; i++)
			cameraPos[i] *= pow(1.1,(y-lastPos[1])*.1);
	}


	lastPos[0] = x;
	lastPos[1] = y;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	curProblem = key-'0';
    if (key == 'q' || key == 'Q' || key == 27){
        exit(0);
    }
	glutPostRedisplay();
}

void reshape(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("GL Examples");

	glutDisplayFunc(display);
	glutMotionFunc(mouseMoved);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
