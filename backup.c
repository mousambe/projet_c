#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	int code;
	char intitule[150];
	int annee;
	char departement[70];
} formation;


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


int verifierFormation(int); //prend un code de formation et vérifie s'il existe
formation chercherFormation(int); //prend un code de formation et retourne la formation en question
void ajouterFormation(); //ajoute une formation
int verifierMembre(int); //prend un numéro de membre et vérifie s'il existe ****
void ajouterMembre(); //ajoute un membre
membre chercherMembre(int); //prend un numéro de membre et retourne le membre en question s'il existe
void modifierMembre(int); //prend un numéro de membre et modifie le membre en question
void supprimerMembre(int); //prend un numéro de membre et supprime le membre en question
void afficherMembre(int); //prend un numéro de membre et affiche ses détails, s'il existe
void afficherAllMembres(); //affiche tous les membres existants
Membre getMembres(); //Prend les infos de tous les membres existants et les stocke dans une liste chainee
char* catInfos(membre); //Prend un membre en paramètre et retourne une chaine de caractère contenant ses informations
Membre insererDansListe(Membre,membre); //permet d'insérer un membre dans la liste chaînée contenant tous les membres
void saveMembres(Membre); //Enregister la liste chaînée contenant les membres dans le fichier tampon

int verifierFormation (int code)
{
	FILE* fform = fopen("formations.dat", "rb+");
	formation f;
	int trouve = 0;
	while(fread(&f,sizeof(f),1,fform) && trouve==0) {
		if(f.code == code)
			trouve = 1;
		else
			trouve = 0;
	}
	return trouve;
}

char* catInfos(membre m) {
	int nbChar = 11+50+120+150+100+30;
	char* infos = (char*)malloc((nbChar)*sizeof(char));
	char numeroS[11],codeS[11],anneeS[5];
	sprintf(numeroS,"%d",m.numero);
	strcat(infos, numeroS);
	strcat(infos,":");
	strcat(infos, m.nom);
	strcat(infos,":");
	strcat(infos, m.prenoms);
	strcat(infos,":");
	strcat(infos, m.adresse);
	strcat(infos,":");
	strcat(infos, m.email);
	strcat(infos,":");
	strcat(infos, m.telephone);
	
	if(m.nbFormations > 0) {
		strcat(infos,":");
		formation f;
		for(int i=0; i<m.nbFormations; i++) {
			f = chercherFormation(m.formations[i]);
			nbChar += (11+150+5+70);
			infos = realloc(infos, nbChar*sizeof(char));
			sprintf(codeS,"%d",f.code);
			sprintf(anneeS,"%d",f.annee);
			strcat(infos, codeS);
			strcat(infos,":");
			strcat(infos, f.intitule);
			strcat(infos,":");
			strcat(infos, anneeS);
			strcat(infos,":");
			strcat(infos, f.departement);
			if(i < m.nbFormations-1)
				strcat(infos,":");
		}
	}
	return infos;
}

Membre insererDansListe(Membre mem, membre m){
	Membre p = (Membre)malloc(sizeof(struct membres));
    p->m = m;
	Membre tete = mem;
	while(tete->suiv != NULL)
		tete = tete->suiv;
	tete->suiv = p;
        p->suiv = NULL;
	return mem;
}

Membre getMembres() {
	Membre mem = (Membre)malloc(sizeof(struct membres));
	FILE* fmem = fopen("membres.dat", "rb+");
	membre m;
	while(fread(&m,sizeof(m),1,fmem)) {
		mem = insererDansListe(mem,m);
	}
	return mem;
}

void saveMembres(Membre mem) {
	FILE* f = fopen("tamp.bin", "rb");
	char* infos;
	mem = mem->suiv;
	while(mem->suiv != NULL) {
		fprintf(f, "%s\n", catInfos(mem->m));
		mem = mem->suiv;
	}
	fclose(f);
}

void afficherAllMembres() {
	//
}

void afficherMembre(int numero) {
	if(!verifierMembre(numero)) {
		printf("Membre inexistant\n");
		return;
	}
	membre m = chercherMembre(numero);
	printf("%d:%s:%s:%s:%s:%s",m.numero,m.nom,m.prenoms,m.adresse,m.email,m.telephone);
	formation f;
	for (int i = 0; i < m.nbFormations; ++i) {
		f = chercherFormation(m.formations[i]);
		printf(":%d:%s:%d:%s",f.code,f.intitule,f.annee,f.departement);
	}
	printf("\n");
}

void ajouterFormation() {
	formation f;
	printf("*********************** Ajout Formation ***********************\n");
	printf("Donnez le code de la nouvelle formation : \n");
	scanf("%d", &f.code);
	while(f.code < 0 || verifierFormation(f.code)==1) {
		printf("Code invalide ou formation existante. Reessayez :\n");
		scanf("%d", &f.code);
	}
	printf("Donnez l'intitule de la nouvelle formation : \n");
	scanf("%s", f.intitule);
	printf("Donnez l'annee de la nouvelle formation : \n");
	scanf("%d", &f.annee);
	printf("Donnez le departement de la nouvelle formation : \n");
	scanf("%s", f.departement);
	FILE* fform = fopen("formations.dat", "ab+");
	fwrite(&f,sizeof(f),1,fform);
	fclose(fform);
}

void supprimerMembre(int numero) {
	membre m;
	FILE* f1 = fopen("membres.dat", "rb+");
	FILE* f2 = fopen("tamp.dat" ,"ab+");
	while(fread(&m,sizeof(m),1,f1)) {
		if(m.numero != numero)
			fwrite(&m,sizeof(m),1,f2);
	}
	fclose(f1);
	fclose(f2);
	remove("membres.dat");
	rename("tamp.dat","membres.dat");
	printf("Membre %d supprime avec succes !\n",numero);
}

void modifierMembre(int numero) {
	printf("************************ Modification d'un membre **************************\n");
	membre m;
	supprimerMembre(numero);
	m.numero = numero;
	printf("Donnez le nouveau nom du membre :\n");
	scanf("%s", m.nom);
	printf("Donnez le(s) nouveau(x) prenom(s) du membre :\n");
	scanf("%s", m.prenoms);
	printf("Donnez la nouvelle adresse du membre :\n");
	scanf("%s", m.adresse);
	printf("Donnez le nouveau mail du membre :\n");
	scanf("%s", m.email);
	printf("Donnez le nouveau telephone du membre :\n");
	scanf("%s", m.telephone);
	printf("Donnez le nouveau nombre de formations suivies par le membre :\n");
	scanf("%d", &m.nbFormations);
	while(m.nbFormations < 0) {
		printf("Nombre invalide. Reessayez :\n");
		scanf("%d", &m.nbFormations);
	}
	if(m.nbFormations > 0) {
		int codeFormation;
		int i = 0;
		while(i < m.nbFormations) {
			printf("Donnez le code de la formation :\n");
			scanf("%d", &codeFormation);
			while(codeFormation < 0 || verifierFormation(codeFormation==0)) {
				printf("Code invalide ou formation inexistante. Reessayez :\n");
				scanf("%d", &codeFormation);
			}
			m.formations[i] = codeFormation;
			i++;
		}
	}
	FILE* fichier = fopen("membres.dat", "ab+");
	fwrite(&m, sizeof(m), 1, fichier);
	fclose(fichier);
	printf("Membre modifie avec succes\n");
}


formation chercherFormation(int code) {
	FILE* fform = fopen("formations.dat", "rb+");
	formation f;
	while(fread(&f,sizeof(f),1,fform)) {
		if(f.code == code)
			return f;
	}
}

membre chercherMembre(int numero) {
	FILE* fmem = fopen("membres.dat", "rb+");
	rewind(fmem);
	membre m;
	while(fread(&m,sizeof(m),1,fmem)) {
		if(m.numero == numero)
			return m;
	}
	fclose(fmem);
}


int verifierMembre (int numero)
{
	FILE* fmem = fopen("membres.dat", "rb+");
	membre m;
	int trouve = 0;
	while(fread(&m,sizeof(m),1,fmem) && trouve==0) {
		if(m.numero == numero)
			trouve = 1;
		else
			trouve = 0;
	}
	return trouve;
}

void ajouterMembre() {
	printf("************************ Ajout d'un nouveau membre **************************\n");
	membre new;
	printf("Donnez le numero du nouveau membre :\n");
	scanf("%d", &new.numero);
	while(new.numero < 0 || verifierMembre(new.numero)) {
		printf("Numero invalide ou membre existant. Reessayez :\n");
		scanf("%d", &new.numero);
	}
	printf("Donnez le nom du nouveau membre :\n");
	scanf("%s", new.nom);
	printf("Donnez le(s) prenom(s) du nouveau membre :\n");
	scanf("%s", new.prenoms);
	printf("Donnez l'adresse du nouveau membre :\n");
	scanf("%s", new.adresse);
	printf("Donnez le mail du nouveau membre :\n");
	scanf("%s", new.email);
	printf("Donnez le telephone du nouveau membre :\n");
	scanf("%s", new.telephone);
	printf("Donnez le nombre de formations suivies par le membre :\n");
	scanf("%d", &new.nbFormations);
	while(new.nbFormations <= 0) {
		printf("Nombre invalide. Reessayez :\n");
		scanf("%d", &new.nbFormations);
	}
	if(new.nbFormations > 0) {
		int codeFormation;
		int i = 0;
		while(i < new.nbFormations) {
			printf("Donnez le code de la formation :\n");
			scanf("%d", &codeFormation);
			while(codeFormation < 0 || verifierFormation(codeFormation==0)) {
				printf("Code invalide ou formation inexistante. Reessayez :\n");
				scanf("%d", &codeFormation);
			}
			new.formations[i] = codeFormation;
			i++;
		}
	}
	FILE* fichier = fopen("membres.dat", "ab+");
	fwrite(&new, sizeof(new), 1, fichier);
	fclose(fichier);
	printf("Membre ajoute avec succes\n");
}

void main() {
	//
}