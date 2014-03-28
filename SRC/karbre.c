#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "karbre.h"

#define N 5
#define DIV 2
#define MIN_WIDTH 1

static int profondeur = 0;

karbre kArbreVide() {
	karbre A;
	if ( (A = calloc(1, sizeof(struct_karbre))) == NULL ) {
		printf("Probleme de calloc karbre\n");
		exit(EXIT_FAILURE);
	}
	if ( (A->e = calloc(1, sizeof(struct_element))) == NULL ) {
		free(A);
		printf("Out Of Memory - Element\n");
		exit(EXIT_FAILURE);
	}

	if ( (A->fils = calloc(K, sizeof(struct_karbre))) == NULL ) {
		free(A->e);
		free(A);
		printf("Out of Memory - tableau\n");
		exit(EXIT_FAILURE);
	}

	return A;
}

void freekArbre ( karbre A ) {
	free(A->fils);
	free(A->e);
	free(A);
}

element kCreeElement(int valeur, Point x1, Point x2) {
	element e;
	
	if ( (e = calloc(1, sizeof(struct_element))) == NULL) {
		printf("Erreur allocation element\n");
		exit(EXIT_FAILURE);
	}
	e->valeur = valeur;
	e->x1 = x1;
	e->x2 = x2;
	return e;
}

karbre kConsArbre( element e, ... ) {
	karbre A, B = NULL;

	A = kArbreVide();
	A->e = e;
	int i = 0;
	va_list pile;
	va_start(pile, e);

	e = va_arg(pile, element);
	while( e != NULL && i < K ) {
		B = kArbreVide();
		B->e = e;
		A->fils[i++] = B;

		e = va_arg(pile, element);
	}
	va_end(pile);

	return A;

}


karbre kFils(int ieme, karbre A) {
	if (A == NULL || ieme < 0 || ieme >= K)
		return NULL;
	return A->fils[ieme];
}

element kRacine(karbre A) {
	if (A == NULL)
		return NULL;
	return A->e;
}

/* Retourne 1 si le premier fils est null
 * 0 sinon
 * Pour cause : On considere que les arbres sont remplis a partir du fils 0
 * Ainsi le fils 1 .. N ne peuvent etre rempli si le fils 0 n'est pas rempli*
 */

int kEstVide(karbre A) {
	return kFils(0, A) == NULL;
}

void kAfficher(karbre A) {
	int i;
	
	printf("\n");
	printf("├");
	for ( i = profondeur; i > 0; i-- ) 
		printf("────");
	profondeur++;
	
	printf(" %d [ (%f;%f;%f) - (%f;%f;%f) ]\n", A->e->valeur,A->e->x1.x,A->e->x1.y,A->e->x1.z,A->e->x2.x,A->e->x2.y,A->e->x2.z);
	
	for ( i = 0; i <= K; i++)
		if ( A->fils[i] != NULL)
			kAfficher(A->fils[i]);
	profondeur--;
}

Point kCreePoint(float x, float y,float z) {
	Point A;
	A.x = x;
	A.y = y;
	A.z = z;
	return A;
}

/* On considere le fait suivant :
 * Que le point A est le point le plus bas
 * Et ainsi que le point B ets le point le plus haut
 * Retourne -1 Si le cube est entierement dans la sphere
 * Retourne 0 Si le cube est en dehors de la sphere
 * Retourne 1 Si le cube interesect la sphere
 */
int intersetionCubeSphere(Point A, Point B, Point S, int R) {
	int r1, r2;
	float D = pow(R, 2);

	if ( S.x < A.x ) D -= pow(S.x - A.x, 2);
	else if (S.x > B.x ) D -= pow(S.x - B.x, 2);
	
	if ( S.y < A.y ) D -= pow(S.y - A.y, 2);
	else if ( S.y > B.y ) D -= pow(S.y - B.y, 2);

	if ( S.z < A.z ) D -= pow(S.z - A.z, 2);
	else if ( S.z > B.z ) D -= pow(S.z - B.z, 2);
	
	if ( D > 0 ) {
		r1 = pow ( S.x - A.x , 2) + pow ( S.y - A.y, 2) + pow ( S.z - A.z, 2);
		r2 = pow ( S.x - B.x , 2) + pow ( S.y - B.y, 2) + pow ( S.z - B.z, 2);
		if ( r1 < D && r2 < D )
			return -1;
		else
			return 1;
	}
	else
		return 0;
	
}

karbre boule2arbre( int x, int y, int z, int r) {
	int cote = pow(2, N - 1);
	Point S = kCreePoint( x, y, z);
	Point A = kCreePoint( -cote, -cote, -cote);
	Point B = kCreePoint( cote, cote, cote);
	return boule2arbrebis( S, A, B, r);	
}

karbre boule2arbrebis( Point S, Point A, Point B, int r) {
	Point C, Tmp1, Tmp2;
	int inter;
	karbre Ab;

	inter = intersetionCubeSphere(A, B, S, r);

	if ( inter == 0 )
		return kConsArbre(kCreeElement(0, A, B), NULL);
	else if ( inter == -1 || abs(A.x - B.x) == MIN_WIDTH || abs(A.y - B.y) == MIN_WIDTH || abs(A.z - A.z) == MIN_WIDTH )
		return kConsArbre(kCreeElement(1, A, B), NULL);
	else {

		Ab = kArbreVide();
		Ab->e = kCreeElement(-1, A, B);
		
		C.x = A.x + abs(A.x - B.x) / 2;
		C.y = A.y + abs(A.y - B.y) / 2;
		C.z = A.z + abs(A.z - B.z) / 2;

		Tmp1 = kCreePoint(A.x, A.y, A.z);
		Tmp2 = kCreePoint(C.x, C.y, C.z);
		Ab->fils[0] = boule2arbrebis( S, Tmp1, Tmp2, r);
		
		Tmp1 = kCreePoint(A.x, C.y, A.z);
		Tmp2 = kCreePoint(C.x, B.y, C.z);
		Ab->fils[1] = boule2arbrebis( S, Tmp1, Tmp2, r);
		
		Tmp1 = kCreePoint(C.x, C.y, A.z);
		Tmp2 = kCreePoint(B.x, B.y, C.z);
		Ab->fils[2] = boule2arbrebis( S, Tmp1, Tmp2, r);
		
		Tmp1 = kCreePoint(C.x, A.y, A.z);
		Tmp2 = kCreePoint(B.x, C.y, C.z);
		Ab->fils[3] = boule2arbrebis( S, Tmp1, Tmp2, r);

		Tmp1 = kCreePoint(A.x, A.y, C.z);
		Tmp2 = kCreePoint(C.x, C.y, B.z);
		Ab->fils[4] = boule2arbrebis( S, Tmp1, Tmp2, r);
		
		Tmp1 = kCreePoint(A.x, C.y, C.z);
		Tmp2 = kCreePoint(C.x, B.y, B.z);
		Ab->fils[5] = boule2arbrebis( S, Tmp1, Tmp2, r);

		Tmp1 = kCreePoint(C.x, C.y, C.z);
		Tmp2 = kCreePoint(B.x, B.y, B.z);
		Ab->fils[6] = boule2arbrebis( S, Tmp1, Tmp2, r);

		Tmp1 = kCreePoint(C.x, A.y, C.z);
		Tmp2 = kCreePoint(B.x, C.y, B.z);
		Ab->fils[7] = boule2arbrebis( S, Tmp1, Tmp2, r);
		
		return Ab;

	}

} 

/* L'information se trouvant dans les feuilles,  il n'est pas possible que
 * La racine de l'arbre A1 ou A2 Soit directement une valeur
 * égale a 0 ou 1 
 */
karbre intersetion(karbre A1, karbre A2) {
	int i;
	karbre A;
	
	if ( kRacine(A1)->valeur == 0 || kRacine(A2)->valeur == 0)
		return kConsArbre(kCreeElement(0,kRacine(A2)->x1, kRacine(A2)->x2), NULL);
	if ( kRacine(A1)->valeur == 1 )
		return A2;
	else if ( kRacine(A2)->valeur == 1)
		return A1;
	else {
		A = kConsArbre(kCreeElement(-1,kRacine(A2)->x1, kRacine(A2)->x2), NULL);
		for ( i = 0; i < K; i++)
			A->fils[i] = intersetion(A1->fils[i], A2->fils[i]);
		return A;
	}
}
