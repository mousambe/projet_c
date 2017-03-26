#include <stdio.h>
#include "../headers/formations.h"

int verifierFormation (int code)
{
	FILE* fform = fopen("files/formations.dat", "rb+");
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
	FILE* fform = fopen("files/formations.dat", "ab+");
	fwrite(&f,sizeof(f),1,fform);
	fclose(fform);
}

formation chercherFormation(int code) {
	FILE* fform = fopen("files/formations.dat", "rb+");
	formation f;
	while(fread(&f,sizeof(f),1,fform)) {
		if(f.code == code)
			return f;
	}
}