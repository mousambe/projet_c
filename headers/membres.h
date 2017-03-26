#ifndef membres_h
#define membres_h

#endif

typedef struct {
	int numero;
	char nom[50];
	char prenoms[120];
	char adresse[150];
	char email[100];
	char telephone[30];
	int nbFormations;
	int formations[20];
} membre;

typedef struct membres {
	membre m;
	struct membres* suiv;
} *Membre;

int verifierMembre(int); //prend un numéro de membre et vérifie s'il existe ****
void ajouterMembre(); //ajoute un membre
membre chercherMembre(int); //prend un numéro de membre et retourne le membre en question s'il existe
void modifierMembre(int); //prend un numéro de membre et modifie le membre en question
void supprimerMembre(int); //prend un numéro de membre et supprime le membre en question
void afficherMembre(int); //prend un numéro de membre et affiche ses détails, s'il existe
Membre getMembres(); //Prend les infos de tous les membres existants et les stocke dans une liste chainee
char* catInfos(membre); //Prend un membre en paramètre et retourne une chaine de caractère contenant ses informations
Membre insererDansListe(Membre,membre); //permet d'insérer un membre dans la liste chaînée contenant tous les membres
void saveMembres(Membre); //Enregister la liste chaînée contenant les membres dans le fichier tampon
void afficherAllMembres(); //affiche tous les membres existants