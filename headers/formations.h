#ifndef formations_h
#define formations_h

#endif

typedef struct {
	int code;
	char intitule[150];
	int annee;
	char departement[70];
} formation;

int verifierFormation(int); //prend un code de formation et vérifie s'il existe
formation chercherFormation(int); //prend un code de formation et retourne la formation en question
void ajouterFormation(); //ajoute une formation

//704890156