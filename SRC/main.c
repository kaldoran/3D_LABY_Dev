#include "GL/gl.h"
#include "GL/glut.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "karbre.h"

#define HEIGHT 800
#define MID_HEIGHT HEIGHT / 2
#define WIDTH 700
#define MID_WIDTH WIDTH / 2

#define EPSILON 20
#define SENSIBILITE 0.2

Point vue, lookat, vec;
float theta = 90., phi = 0.;

/* USELESS */

void affiche_cube(int x1, int y1, int z1, int x2, int y2, int z2 , GLenum mode) {

	glColor3f(0, 1, 0);
	glBegin( mode );
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y1, z2);
	
	glVertex3f(x1, y2, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);
	
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);
	
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y1, z1);
	
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y1, z1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y1, z2);
	glEnd();
}

void change_vue() {
	vue.x = lookat.x + vec.x;
	vue.y = lookat.y + vec.y;
	vue.z = lookat.z + vec.z;
}

/* Gestion du clavier */
void gererClavier(unsigned char touche, int x , int y) {
	
	if ( touche == 's') {
		lookat.x -= vec.x;
		lookat.y -= vec.y;
	}
	else if ( touche == 'z') {
		lookat.x += vec.x;
		lookat.y += vec.y;
	}
	else if ( touche == 'd') { 
		lookat.x++;
		vue.x++;
	}	
	else if ( touche == 'q') {
		lookat.y--;
		vue.x--;
	}
	else if ( touche == '2') {
		lookat.z--;
	}
	else if ( touche == '8' ) {
		lookat.z++;
	}
	else if ( (int)touche == 27) /* METHODE DE GROS PATARASSE A CHANGER */
		exit(EXIT_FAILURE);

	printf("Touche : %c - Souris : %d %d \n", touche, x , y);
	change_vue();
}
void afficher_surface(int x, int y, GLenum  mode) {
	glBegin(mode);
	glVertex3f(x, x, 0);
	glVertex3f(x, y, 0);
	glVertex3f(y, y, 0);
	glVertex3f(y, x, 0);
	glEnd();

}

void vMotion(int x, int z) {
	float tmp;

	theta -= (x - MID_WIDTH) * SENSIBILITE;
	phi -= (z - MID_HEIGHT) * SENSIBILITE;

	if (theta > 360 )
		theta = 0;
	else if ( theta < -360 )
		theta = 0;
	if ( phi > 89 )
		phi = 89;
	else if ( phi < -89)
		phi = -89;

	printf("theta : %f - phi : %f, vue.x : %f - vue.y : %f\n", theta, phi, vue.x, vue.y);	
	//theta = (double)(deltaX * 90) / MID_WIDTH;
	tmp = cos(phi * M_PI / 180);

	vec.x = tmp * cos(theta * M_PI / 180);
	vec.y = tmp * sin(theta * M_PI / 180);
	vec.z = sin(phi * M_PI / 180);

	change_vue();
	/*
	float centerX, centerY, deltaX, deltaZ, sin_angle, cos_angle;
	centerX  = largeur_rep / 2;
	centerY = hauteur_rep / 2;
	deltaX = (x - centerX) / SENSIBILITE;
	deltaZ = y - centerY;
	
	float angleX;

	angleX = atan(deltaX / EPSILON);
	

	angle += angleX;
	sin_angle = sin(angle);
	cos_angle = cos(angle);

	vue.x = xlookat + EPSILON * sin_angle;
	vue.y = ylookat + EPSILON * cos_angle;  
	printf("%f %f", sin_angle, cos_angle);
	if ( sin_angle == 0 && cos_angle == 1 ) 
		angle = 0;

	printf("Je regarde en %f %f - angle %f - Delta : %f %f \n", vue.x, vue.y, angle , deltaX, deltaZ );

	vue.z += deltaZ / SENSIBILITE;
	*/
	return;
}

void Affichage() {
	int i, j;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-5, 5, -5, 10, 5, 100);
	gluLookAt(lookat.x, lookat.y, lookat.z, vue.x, vue.y , vue.z, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0, 0, 0);
	afficher_surface(-50, 50, GL_QUADS);

	glColor3f(0, 1, 1);
	for ( i = -50; i < 50; i+= 10 )
		for ( j = -50; j < 50; j+= 10)
			afficher_surface(i, j + 10, GL_LINE_LOOP);

	glutWarpPointer(MID_WIDTH, MID_HEIGHT);
	
	glutPostRedisplay();
	glutSwapBuffers();
}

int main ( int argc, char * argv[] ) {
	float xlookat = 5, ylookat = -55, zlookat = 10;

	vue = kCreePoint(xlookat, ylookat + EPSILON, zlookat);
	lookat = kCreePoint(xlookat, ylookat, zlookat);

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(WIDTH, HEIGHT); //glutFullScreen() - plein écran
	glutInitWindowPosition(300, 50);

	glutCreateWindow("Laby");
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(MID_HEIGHT , MID_HEIGHT);

	glutDisplayFunc(Affichage);
	glutIdleFunc(Affichage);
		
	glutMotionFunc(vMotion);
	glutPassiveMotionFunc(vMotion); //Souris
	glutKeyboardFunc(gererClavier); //Clavier
	
	glutMainLoop();
	return 0;
}
