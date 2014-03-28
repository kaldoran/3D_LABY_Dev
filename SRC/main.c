#include "GL/gl.h"
#include "GL/glut.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include "karbre.h"

#define HEIGHT 800
#define MID_HEIGHT HEIGHT / 2
#define WIDTH 800
#define MID_WIDTH WIDTH / 2

#define EPSILON 20
#define SENSIBILITE 0.07


Point vue, lookat, vec, vue_plane;
float theta = 90., phi = 0.;

/* USELESS */

void affiche_cube(int x1, int y1, int z1, int x2, int y2, int z2 , GLenum mode) {

	glBegin( mode );
	/* face avant */
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y1, z2);
	
	/*face droite*/	
	glVertex3f(x1, y2, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);
	
	/*face bas*/
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);
	
	/*face arrière*/
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y1, z1);
	
	/*face gauche*/
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y1, z1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	
	/*face du haut*/
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
	int modifiers = glutGetModifiers();
	float speed = 1;
    	if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT)
		speed = 2.1337;	

	if ( touche == 's' || touche == 'S') {
		lookat.x -= speed * vue_plane.x;
		lookat.y -= speed * vue_plane.y;
	}
	else if ( touche == 'z' || touche == 'Z') {
		lookat.x += speed * vue_plane.x;
		lookat.y += speed * vue_plane.y;
	}
	else if ( touche == 'q' || touche == 'Q') { 
		lookat.x += -(speed * vue_plane.y);
		lookat.y += speed * vue_plane.x;
	}	
	else if ( touche == 'd' || touche == 'D') {
		lookat.x += speed * vue_plane.y;
		lookat.y += -(speed * vue_plane.x);
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

void afficher_surface(int x, int y, GLenum mode) {
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

	//printf("theta : %f - phi : %f, vue.x : %f - vue.y : %f\n", theta, phi, vue.x, vue.y);	
	tmp = cos(phi * M_PI / 180);

	vue_plane.x = cos(theta * M_PI / 180);
	vue_plane.y = sin(theta * M_PI / 180);

	vec.x = tmp * vue_plane.x;
	vec.y = tmp * vue_plane.y;
	vec.z = sin(phi * M_PI / 180);
	
	change_vue();
	return;
}

void Affichage() {
	int i, j;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-5, 5, -5, 10, 5, 100);
	gluLookAt(lookat.x, lookat.y, lookat.z, vue.x, vue.y , vue.z, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glColor3f(0, 0, 0);
	//afficher_surface(-50, 50, GL_QUADS);

	glColor3f(0, 1, 0);
	glPushMatrix(); 
	glTranslatef(0,0,10); 
	glutWireCone(10., 25., 10, 10);
	//glutSolidCone(10., 25., 10, 10);
	//glutSolidSphere(10., 10, 10);
	//glutWireSphere(15., 10, 10);
	glPopMatrix();


	glColor3f(0.4, 0.4, 0.2);
	glPushMatrix();
	affiche_cube(-2, -2, 0, 2, 2, 10, GL_QUADS);
	

	glPopMatrix();

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

	vec = kCreePoint(0, EPSILON, 0);
	vue_plane = kCreePoint(0, 1, 0);
	vue = kCreePoint(xlookat + vec.x, ylookat + vec.y, zlookat + vec.z);
	lookat = kCreePoint(xlookat, ylookat, zlookat);

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(WIDTH, HEIGHT); //glutFullScreen() - plein écran
	glutInitWindowPosition(300, 50);

	glutCreateWindow("Laby");
	glEnable(GL_DEPTH_TEST);
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
