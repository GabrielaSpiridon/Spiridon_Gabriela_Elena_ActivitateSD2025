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

struct Nod {
	Masina info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);

	char* aux;
	Masina m;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, sep));

	m.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m.model = malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);

	m.serie = *strtok(NULL, sep);
	return m;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id); 
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** arbore, Masina masinaNoua) {
	if (!(*arbore)) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->st = NULL;
		nou->dr = NULL;
		(*arbore) = nou;
	}
	else
	{
		if ((*arbore)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*arbore)->st), masinaNoua);
		}
		else if((*arbore)->info.id<masinaNoua.id)
		{
			adaugaMasinaInArbore(&((*arbore)->dr), masinaNoua);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	Nod* arbore = NULL;
	FILE* f = fopen(numeFisier, "r");

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
	}
	fclose(f);

	return arbore;
}

void afisareMasiniDinArboreInOrdine(Nod* arbore) {
	if (arbore) {
		afisareMasiniDinArboreInOrdine(arbore->st);
		afisareMasina(arbore->info);
		afisareMasiniDinArboreInOrdine(arbore->dr);
	}
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore == NULL) {
		return 0;
	}
	int inaltimeSt = calculeazaInaltimeArbore(arbore->st);
	int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);
	if (inaltimeSt > inaltimeDr) {
		return 1 + inaltimeSt;
	}
	else {
		return 1 + inaltimeDr;
	}
}

Nod* gasesteMinim(Nod* radacina) {
	Nod* curent = radacina;
	while (curent && curent->st != NULL)
	{
		curent = curent->st;
	}
	return curent;
}

Nod* stergeNod(Nod* radacina, int idCautat) {
	if (radacina == NULL) {
		return radacina;
	}

	if (idCautat < radacina->info.id) {
		radacina->st = stergeNod(radacina->st, idCautat);
	}
	else if (idCautat > radacina->info.id) {
		radacina->dr = stergeNod(radacina->dr, idCautat);
	}
	else {
		//nodul a fost gasit
		
		//nodul nu are descendenti sau are doar unul 
		if (radacina->st == NULL) {
			Nod* temp = radacina->dr;
			free(radacina->info.numeSofer);
			free(radacina->info.model);
			free(radacina);
			return temp;
		}
		else if (radacina->dr == NULL) {
			Nod* temp = radacina->st;
			free(radacina->info.numeSofer);
			free(radacina->info.model);
			free(radacina);
			return temp;
		}

		//nodul are 2 descendenti
		Nod* temp = gasesteMinim(radacina->dr);
		free(radacina->info.numeSofer);
		free(radacina->info.model);

		radacina->info = temp->info;
		radacina->dr = stergeNod(radacina->dr, temp->info.id);
	}
	return radacina;
}

Nod* subarboreCuInaltimeMaxima(Nod* radacina) {
	if (radacina == NULL) {
		return NULL;
	}

	int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
	int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
	
	if (inaltimeSt >= inaltimeDr) {
		return radacina->st;
	}
	else {
		return radacina->dr;
	}
}

int numarNoduriAflateInSubordine(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}
	return 1 + numarNoduriAflateInSubordine(radacina->st) + numarNoduriAflateInSubordine(radacina->dr);
}

Nod* subarboreCuMaiMulteNoduri(Nod* radacina) {
	if (radacina == NULL) {
		return NULL;
	}
	int noduriStanga = numarNoduriAflateInSubordine(radacina->st);
	int noduriDreapta = numarNoduriAflateInSubordine(radacina->dr);

	if (noduriStanga >= noduriDreapta) {
		return radacina->st;
	}
	else {
		return radacina->dr;
	}
}


void dezalocareArboreDeMasini(Nod** arbore) {
	if ((*arbore)) {
		dezalocareArboreDeMasini(&(*arbore)->st);
		dezalocareArboreDeMasini(&(*arbore)->dr);
		free((*arbore)->info.numeSofer);
		free((*arbore)->info.model);
		(*arbore) = NULL;
	}
}

int main() {
	Nod* arboreMasini = NULL;
	const char* numeFisier = "masini.txt";

	printf("--- Citire arbore din fisier ---\n");
	arboreMasini = citireArboreDeMasiniDinFisier(numeFisier);
	if (arboreMasini == NULL) {
		printf("Eroare la citirea fisierului sau fisierul este gol.\n");
		return 1;
	}

	printf("\n--- Afisare arbore in ordine (Initial) ---\n");
	afisareMasiniDinArboreInOrdine(arboreMasini);

	printf("\n--- Testare calculeazaInaltimeArbore ---\n");
	int inaltime = calculeazaInaltimeArbore(arboreMasini);
	printf("Inaltimea arborelui: %d\n", inaltime);

	printf("\n--- Testare numarNoduriAflateInSubordine (pentru intregul arbore) ---\n");
	int numarNoduri = numarNoduriAflateInSubordine(arboreMasini);
	printf("Numarul total de noduri in arbore: %d\n", numarNoduri);

	printf("\n--- Testare subarboreCuInaltimeMaxima ---\n");
	Nod* subarboreMaxInaltime = subarboreCuInaltimeMaxima(arboreMasini);
	if (subarboreMaxInaltime) {
		printf("Radacina subarborelui cu inaltimea mai mare (ID): %d\n", subarboreMaxInaltime->info.id);
		printf("Inaltimea subarborelui: %d\n", calculeazaInaltimeArbore(subarboreMaxInaltime));
	}
	else {
		printf("Nu exista un subarbore cu inaltime maxima (arbore gol sau nod frunza).\n");
	}

	printf("\n--- Testare subarboreCuMaiMulteNoduri ---\n");
	Nod* subarboreMaxNoduri = subarboreCuMaiMulteNoduri(arboreMasini);
	if (subarboreMaxNoduri) {
		printf("Radacina subarborelui cu mai multe noduri (ID): %d\n", subarboreMaxNoduri->info.id);
		printf("Numarul de noduri din acest subarbore: %d\n", numarNoduriAflateInSubordine(subarboreMaxNoduri));
	}
	else {
		printf("Nu exista un subarbore cu mai multe noduri (arbore gol sau nod frunza).\n");
	}

	// --- Teste pentru stergere nod ---
	printf("\n--- Testare stergeNod ---\n");

	int idDeSters1 = 4;
	printf("\nStergerea nodului cu ID: %d\n", idDeSters1);
	arboreMasini = stergeNod(arboreMasini, idDeSters1);
	printf("Arbore dupa stergerea ID %d:\n", idDeSters1);
	afisareMasiniDinArboreInOrdine(arboreMasini);

	printf("\n--- Dezalocare arbore ---\n");
	dezalocareArboreDeMasini(&arboreMasini);
	if (arboreMasini == NULL) {
		printf("Arborele a fost dezalocat cu succes (pointerul radacina este NULL).\n");
	}
	else {
		printf("Dezalocarea arborelui a esuat (pointerul radacina nu este NULL).\n");
	}

	return 0;
}