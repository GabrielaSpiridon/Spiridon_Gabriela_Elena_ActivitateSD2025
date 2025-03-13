#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	printf("ID: %d \n", masina.id);
	printf("Numar usi: %d \n", masina.nrUsi);
	printf("Pret: %2f \n", masina.pret);
	printf("Model: %s \n", masina.model);
	printf("Nume sofer: %s \n", masina.numeSofer);
	printf("Seria: %c \n \n", masina.serie);;

}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	int i;
	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* aux;
	aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < (*nrMasini); i++) {
		aux[i] = (*masini)[i];
	}
	aux[(*nrMasini)] = masinaNoua;

	free((*masini));

	(*masini) = aux;
	(*nrMasini) = (*nrMasini) + 1;
	
}

Masina citireMasinaFisier(FILE* file) {
	Masina masinaNoua;
	char buffer[100];
	char delimiter[4] = ",;\n";

	fgets(buffer,100,file);

	char* aux;
	aux = strtok(buffer, delimiter);
	masinaNoua.id = atoi(aux);

	masinaNoua.nrUsi = atoi(strtok(NULL,delimiter));

	masinaNoua.pret = atof(strtok(NULL, delimiter));

	aux = strtok(NULL, delimiter);
	masinaNoua.model = malloc(strlen(aux) + 1);
	strcpy_s(masinaNoua.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, delimiter);
	masinaNoua.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(masinaNoua.numeSofer, strlen(aux) + 1, aux);

	aux = strtok(NULL, delimiter);
	masinaNoua.serie = aux[0];

	return masinaNoua;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* f = fopen(numeFisier, "r");
	Masina* masini = NULL;
	while (!feof(f)) {
		Masina masinaNouCitita = citireMasinaFisier(f);
		adaugaMasinaInVector(&masini, nrMasiniCitite,masinaNouCitita);
	}
	fclose(f);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < (*nrMasini); i++)
	{
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	(*vector) = NULL;
	(*nrMasini) = 0;
}

int main() {


	Masina* masini = NULL;
	int nrMasini = 0;

	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);

	afisareVectorMasini(masini, nrMasini);

	dezalocareVectorMasini(&masini, &nrMasini);

	return 0;
}