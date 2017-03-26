#include <stdlib.h>
#include <string.h>
#include "../headers/membres.h"
#include "formations.c"

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
	FILE* fmem = fopen("files/membres.dat", "rb+");
	membre m;
	while(fread(&m,sizeof(m),1,fmem)) {
		mem = insererDansListe(mem,m);
	}
	return mem;
}

void saveMembres(Membre mem) {
	FILE* f = fopen("files/tamp.bin", "rb");
	char* infos;
	mem = mem->suiv;
	while(mem->suiv != NULL) {
		fprintf(f, "%s\n", catInfos(mem->m));
		mem = mem->suiv;
	}
	fclose(f);
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
void supprimerMembre(int numero) {
	membre m;
	FILE* f1 = fopen("files/membres.dat", "rb+");
	FILE* f2 = fopen("files/tamp.dat" ,"ab+");
	while(fread(&m,sizeof(m),1,f1)) {
		if(m.numero != numero)
			fwrite(&m,sizeof(m),1,f2);
	}
	fclose(f1);
	fclose(f2);
	remove("files/membres.dat");
	rename("files/tamp.dat","files/membres.dat");
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
	FILE* fichier = fopen("files/membres.dat", "ab+");
	fwrite(&m, sizeof(m), 1, fichier);
	fclose(fichier);
	printf("Membre modifie avec succes\n");
}

membre chercherMembre(int numero) {
	FILE* fmem = fopen("files/membres.dat", "rb+");
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
	FILE* fmem = fopen("files/membres.dat", "rb+");
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
	FILE* fichier = fopen("files/membres.dat", "ab+");
	fwrite(&new, sizeof(new), 1, fichier);
	fclose(fichier);
	printf("Membre ajoute avec succes\n");
}