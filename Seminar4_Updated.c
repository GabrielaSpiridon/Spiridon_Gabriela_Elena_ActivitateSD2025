#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct StructuraMasina Masina;
typedef struct nod nod;

struct nod {
	Masina info;
	nod* next;
};


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(nod** cap, Masina masinaNoua) {
	nod* nou = malloc(sizeof(nod));
	nou->info = masinaNoua;
	nou->next = NULL;

	if ((*cap) != NULL) {
		nod* aux = (*cap);
		while (aux->next != NULL) {
			aux = aux->next;
		}

		aux->next = nou;
	}
	else {
		*cap = nou;
	}

}

void adaugaLaInceputInLista(nod** cap, Masina masinaNoua) {

	nod* nou = malloc(sizeof(nod));
	nou->info = masinaNoua;
	nou->next = cap;

	(*cap) = nou;
}

nod* citireListaMasiniDinFisier(const char* numeFisier) {

	FILE* fisierMasini = fopen(numeFisier, "r");

	nod* cap = NULL;

	while (!feof(fisierMasini)) {
		Masina m;
		m = citireMasinaDinFisier(fisierMasini);
		adaugaMasinaInLista(&cap, m);
	}

	fclose(fisierMasini);

	return cap;
}

void dezalocareListaMasini(nod** cap) {

	while ((*cap) != NULL) {
		nod* temp;
		temp = (*cap)->next;
		free((*cap)->info.model);
		free((*cap)->info.numeSofer);
		free((*cap));
		(*cap) = temp;
	}
}

float calculeazaPretMediu(nod* cap) {

	float suma = 0;
	int numarElemente = 0;

	while (cap != NULL) {
		suma += cap->info.pret;
		numarElemente++;
		cap = cap->next;
	}

	return suma / numarElemente;
}

void stergeMasiniDinSeria(nod** cap, char serieCautata) {
	nod* temp = *cap;
	nod* prev = NULL;
	while (temp != NULL) {
		if (temp->info.serie == serieCautata) {
			if (prev == NULL) {
				*cap = temp->next;
			}
			else {
				prev->next = temp->next;
			}
			free(temp->info.model);
			free(temp->info.numeSofer);
			nod* toDelete = temp;
			temp = temp->next;
			free(toDelete);
		}
		else {
			prev = temp;
			temp = temp->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(nod* cap, const char* numeSofer) {
	float suma = 0;
	while (cap != NULL) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}


int main() {

	nod* cap = citireListaMasiniDinFisier("masini.txt");

	afisareListaMasini(cap);

	float pretMediu = calculeazaPretMediu(cap);
	printf("\n Media este: %f", pretMediu);

	stergeMasiniDinSeria(&cap, 'A');
	printf("\nLista dupa stergerea masinilor cu seria A:\n");
	afisareListaMasini(cap);

	float pretTotal = calculeazaPretulMasinilorUnuiSofer(cap, "Gigel");
	printf("\nPretul total al masinilor lui Gigel este: %.2f\n", pretTotal);


	dezalocareListaMasini(&cap);


	return 0;
}