#ifndef K_ARBRE
#define K_ARBRE

#define K 4

/* Structure d'un point */
typedef struct Point {
	float x;
	float y;
	float z;
}Point;

/* Structure d'un element */
typedef struct struct_element {
	int valeur;
	Point x1;
	Point x2;
}struct_element;


/* Structure de l'élement */
typedef struct_element * element;

typedef struct struct_karbre {
	struct struct_element *e;
	struct struct_karbre **fils;
}struct_karbre;

typedef struct_karbre * karbre;

/* Crée un kArbrevide */
karbre kArbreVide();

/* Libere la mémoire */
void freekArbre (karbre A);

/* Fonction a nombre d'argument variable */
element kCreeElement(int valeur, Point x1, Point x2);

karbre kConsArbre(element e, ... );

karbre kFils(int ieme, karbre A);

element kRacine(karbre A);

int kEstVide(karbre A);

void kAfficher(karbre A);

Point kCreePoint(float x, float y, float z);

int intersetionCubeSphere(Point A, Point B, Point S, int R);

karbre boule2arbre( int x, int y, int z, int r);

karbre boule2arbrebis( Point S, Point A, Point B, int r);

karbre intersetion(karbre A1, karbre A2);

#endif

