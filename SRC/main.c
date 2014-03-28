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

	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y2, z1);
	
	/*face droite*/	
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);
	glVertex3f(x2, y1, z1);
	
	/*face bas*/
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y2, z1);
	glVertex3f(x1, y2, z1);
	
	/*face arrière*/
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);

	glVertex3f(x1, y2, z1);
	glVertex3f(x2, y2, z1);	
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
	Point svlookat = lookat;
	float speed = 1.5;

    	if (modifiers == GLUT_ACTIVE_SHIFT || modifiers == GLUT_ACTIVE_ALT)
		speed = 3.1337;	

	if ( touche == 's' || touche == 'S') {
		svlookat.x -= speed * vue_plane.x;
		svlookat.y -= speed * vue_plane.y;
	}
	else if ( touche == 'z' || touche == 'Z') {
		svlookat.x += speed * vue_plane.x;
		svlookat.y += speed * vue_plane.y;
	}
	else if ( touche == 'q' || touche == 'Q') { 
		svlookat.x += -(speed * vue_plane.y);
		svlookat.y += speed * vue_plane.x;
	}	
	else if ( touche == 'd' || touche == 'D') {
		svlookat.x += speed * vue_plane.y;
		svlookat.y += -(speed * vue_plane.x);
	}
	else if ( touche == '2') {
		svlookat.z--;
	}
	else if ( touche == '8' ) {
		svlookat.z++;
	}
	else if ( (int)touche == 27) /* METHODE DE GROS PATARASSE A CHANGER */
		exit(EXIT_FAILURE);
	
	if ( svlookat.x > 2 && svlookat.y > 2 && svlookat.x < 1248 && svlookat.y < 1248 && svlookat.z > 5) {
		lookat = svlookat;
	} 
	
	
	change_vue();
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

void tree(int x, int y, int z) {
	float ratio = 0.2;
	int i;
	float angle;
	/* TRONC Arbre */
	glColor3f(0.95, 0.7, 0.05);
	glPushMatrix();
		glTranslatef(x, y, 0);
		glBegin(GL_LINE_LOOP);
		glVertex3f(-(z*ratio), 0, 0);
		glVertex3f((z*ratio), 0, 0);
		glVertex3f((z*ratio), 0, z);
		glVertex3f(-(z*ratio), 0, z);
		glEnd();

		/* Ligne milieu TRONC */
		glBegin(GL_LINE_STRIP);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, z);
		glEnd();

		/* Génération Arbre TRONC */
		glBegin(GL_LINE_LOOP);
		glVertex3f(0, -(z*ratio), 0);
		glVertex3f(0, (z*ratio), 0);
		glVertex3f(0, (z*ratio), z);
		glVertex3f(0, -(z*ratio), z); 		
		glEnd();

		/* Sapin les murs */
		glColor3f(0, 1, 0);
		glTranslatef(0, 0, z); 
		/*glutWireCone(Largeur Base, Hauteur Cone, Nombre de Facette, Nombre de Facette) */
		glutWireCone(z, 3 * z, z, z);

		/*glutSolidCone(10., 25., 10, 10);
		glutWireSphere(10., 10, 10);*/
	glPopMatrix();
}

void Affichage() {
	int i, j;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (double)WIDTH/HEIGHT, 1, 600);
//	glFrustum(-5, 5, -5, 5, 5, 500);
	gluLookAt(lookat.x, lookat.y, lookat.z, vue.x, vue.y , vue.z, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glColor3f(0, 0, 0);
	//afficher_surface(-50, 50, GL_QUADS);
	
	tree(-30, 10, 10);
	tree(-50, 40, 10);
	tree(-50, 20, 10);
	tree(15, -100, 100);
	tree(-60, 40, 50);
	/* LE SOLEILLLLLLL */
	glPushMatrix();
		glTranslatef(0,150,300); 
		glColor3f(1, 1, 0);
		glutWireSphere(20., 15, 15);
	glPopMatrix();

	/* 1250 : LARGEUR laby = HAUTEUR laby ici*/ 
	/* Génération du sol */
	glColor3f(0, 1, 1);
	for ( i = 0; i < 1250; i += 25 ) {
		for ( j = 0; j < 1250; j += 25) { 
			glBegin(GL_LINE_LOOP);
			glVertex3f(i, j, 0);
			glVertex3f(i + 25, j, 0);
			glVertex3f(i + 25, j + 25, 0); 
			glVertex3f(i, j + 25, 0);
			glEnd();
		}
	}
	
	/* Mur confinement */
	glColor4f(1, 0, 0,0.1);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 5);
	glVertex3f(0, 1250, 5);
	glVertex3f(0, 1250, 10);
	glVertex3f(0, 0, 10);

	glVertex3f(0, 1250, 5);
	glVertex3f(1250, 1250, 5);
	glVertex3f(1250, 1250, 10);
	glVertex3f(0, 1250, 10);

	glVertex3f(1250, 1250, 5);
	glVertex3f(1250, 0, 5);
	glVertex3f(1250, 0, 10);
	glVertex3f(1250, 1250, 10);

	glVertex3f(1250, 0, 5);
	glVertex3f(0, 0, 5);
	glVertex3f(0, 0, 10);
	glVertex3f(1250, 0, 10);
	glEnd();

	glColor3f(1, 0, 0);

	/* boiboite */
	glBegin(GL_QUADS);
	/* Boiboite de la face gauche au spawn */
	glVertex3f(0, 0, 4.5);
	glVertex3f(0, 1250, 4.5);
	glVertex3f(0, 1250, 5);
	glVertex3f(0, 0, 5);

	glVertex3f(0, 0, 10);
	glVertex3f(0, 1250, 10);
	glVertex3f(0, 1250, 10.5);
	glVertex3f(0, 0, 10.5);

	/* Autre boibote */
	glVertex3f(0, 1250, 4.5);
	glVertex3f(1250, 1250, 4.5);
	glVertex3f(1250, 1250, 5);
	glVertex3f(0, 1250, 5);

	glVertex3f(0, 1250, 10);
	glVertex3f(1250, 1250, 10);
	glVertex3f(1250, 1250, 10.5);
	glVertex3f(0, 1250, 10.5);

	/* autre boiboite */
	glVertex3f(1250, 1250, 4.5);
	glVertex3f(1250, 0, 4.5);
	glVertex3f(1250, 0, 5);
	glVertex3f(1250, 1250, 5);

	glVertex3f(1250, 1250, 10);
	glVertex3f(1250, 0, 10);
	glVertex3f(1250, 0, 10.5);
	glVertex3f(1250, 1250, 10.5);

	/* autre boiboite */
	glVertex3f(1250, 0, 4.5);
	glVertex3f(0, 0, 4.5);
	glVertex3f(0, 0, 5);
	glVertex3f(1250, 0, 5);

	glVertex3f(1250, 0, 10);
	glVertex3f(0, 0, 10);
	glVertex3f(0, 0, 10.5);
	glVertex3f(1250, 0, 10.5);


	glEnd();


	glutWarpPointer(MID_WIDTH, MID_HEIGHT);
	
	glutPostRedisplay();
	glutSwapBuffers();
}

int main ( int argc, char * argv[] ) {
	float xlookat = 12.5, ylookat = 12.5, zlookat = 10;

	vec = kCreePoint(0, EPSILON, 0);
	vue_plane = kCreePoint(0, 1, 0);
	vue = kCreePoint(xlookat + vec.x, ylookat + vec.y, zlookat + vec.z);
	lookat = kCreePoint(xlookat, ylookat, zlookat);

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(WIDTH, HEIGHT); //glutFullScreen() - plein écran
	glutInitWindowPosition(300, 50);

	glutCreateWindow("Laby");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

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
