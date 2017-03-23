#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int numero;
	char* nom;
	char* prenoms;
	char* adresse;
	char* classe;
	char* email;
	char* telephone;
} membre;

typedef struct {
	int code;
	char* intitule;
	int annee;
	char* departement;
} formation;

int tailleFichier()
{
    int taille;
    int f;

    f = open("membres.txt",O_RDONLY,0);
    taille = lseek (f,0, SEEK_END);

    return taille;
}

int nbEnregistrements()
{
    return (tailleFichier()/(STRUCTSIZE));
}

int verifierNumero (int numero)
{
    int num;
    num = 0;
    int enregistrements;
    enregistrements = nbEnregistrements();
    int existe;
    existe = 0;
    while (num != enregistrements) {
        fread (&c,STRUCTSIZE,1,cfp);
        if (c.ID == cID) {
            existe = 1;
        }
        else {
            existe = 0;
        }
    }
    return existe;
}

void ajouterMembre() {
	int numero;
	char* nom, prenoms, adresse, classe, email, telephone;
	printf("Donnez le numero du nouveau membre :\n");
	scanf("%d", numero);
	while(numero <= 0 || verifierNumero(numero)) {
		printf("Numero invalide ou membre existant. Reessayez :\n");
		scanf("%d", numero);
	}
	printf("Donnez le nom du nouveau membre :\n");
	scanf("%s", nom);
	printf("Donnez le(s) prenom(s) du nouveau membre :\n");
	scanf("%s", prenoms);
	printf("Donnez l'adresse du nouveau membre :\n");
	scanf("%s", adresse);
	printf("Donnez la classe du nouveau membre :\n");
	scanf("%s", classe);
	printf("Donnez le mail du nouveau membre :\n");
	scanf("%s", mail);
	printf("Donnez le telephone du nouveau membre :\n");
	scanf("%s", telephone);
	membre nouveau = {numero, nom, prenoms, adresse, classe, mail, telephone};
	FILE * fichier = fopen("membres.txt", "bw");
	fwrite(nouveau, sizeof(nouveau), 1, fichier);
	fclose(fichier);
}