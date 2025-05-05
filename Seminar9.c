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


struct Nod
{
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


void adaugaMasinaInArbore(Nod** arbore, Masina masinaNoua) {
	if (!(*arbore))
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->st = NULL;
		nou->dr = NULL;
		(*arbore) = nou;
	}
	else
	{
		if ((*arbore)->info.id > masinaNoua.id)
			adaugaMasinaInArbore(&((*arbore)->st), masinaNoua);
		else if ((*arbore)->info.id < masinaNoua.id)
			adaugaMasinaInArbore(&((*arbore)->dr), masinaNoua);
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	Nod* arbore = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
	}
	fclose(f);

	return arbore;
}

//in ordine - SRD - elementele sunt sortate in ordine crescatoare
void afisareMasiniDinArboreInOrdine(Nod* arbore) {
	if (arbore)
	{
		afisareMasiniDinArboreInOrdine(arbore->st);
		afisareMasina(arbore->info);
		afisareMasiniDinArboreInOrdine(arbore->dr);
	}
}

//pre ordine - RSD
void afisareMasiniDinArborePreOrdine(Nod* arbore) {
	if (arbore)
	{
		afisareMasina(arbore->info);
		afisareMasiniDinArborePreOrdine(arbore->st);
		afisareMasiniDinArborePreOrdine(arbore->dr);
	}
}

//post ordine - SDR
void afisareMasiniDinArborePostOrdine(Nod* arbore) {
	if (arbore)
	{
		afisareMasiniDinArborePostOrdine(arbore->st);
		afisareMasiniDinArborePostOrdine(arbore->dr);
		afisareMasina(arbore->info);
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if ((*arbore))
	{
		dezalocareArboreDeMasini(&(*arbore)->st);
		dezalocareArboreDeMasini(&(*arbore)->dr);
		free((*arbore)->info.numeSofer);
		free((*arbore)->info.model);
		free(*arbore);
		(*arbore) = NULL;
	}
}

Masina getMasinaByID(Nod* arbore, int id) {
	Masina m;
	m.id = -1;
	if (arbore)
	{
		if (arbore->info.id < id)
			return getMasinaByID(arbore->dr, id);
		else if (arbore->info.id > id)
			return getMasinaByID(arbore->st, id);
		else return arbore->info;
	}
	return m;
}

int determinaNumarNoduri(Nod* arbore) {
	if (arbore == NULL)
		return 0;
	return 1 + determinaNumarNoduri(arbore->st) + determinaNumarNoduri(arbore->dr);
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore == NULL)
		return 0;
	int inaltimeSt = calculeazaInaltimeArbore(arbore->st);
	int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);
	return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
}

float calculeazaPretTotal(Nod* arbore) {
	if (arbore == NULL)
		return 0;

	float totalStanga = calculeazaPretTotal(arbore->st);
	float totalDreapta = calculeazaPretTotal(arbore->dr);

	return totalDreapta + totalStanga + arbore->info.pret;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore == NULL)
		return 0;

	float total = 0;
	if (strcmp(arbore->info.numeSofer, numeSofer) == 0) {
		total += arbore->info.pret;
	}
	total += calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer);
	total += calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
	return total;
}

int main() {
	Nod* radacina = NULL;

	radacina = citireArboreDeMasiniDinFisier("masini.txt");
	//afisareMasiniDinArboreInOrdine(radacina);
	//afisareMasiniDinArborePostOrdine(radacina);

	int id = 8;
	printf("Masina cautata este: \n");
	Masina m = getMasinaByID(radacina, id);
	afisareMasina(m);

	float pret = calculeazaPretTotal(radacina);
	printf("Pretul total al masinilor este: %.2f", pret);

	printf("\nNumarul total de noduri: %d\n", determinaNumarNoduri(radacina));
	printf("Inaltimea arborelui: %d\n", calculeazaInaltimeArbore(radacina));

	const char* numeSoferCautat = "Ionescu";
	float totalSofer = calculeazaPretulMasinilorUnuiSofer(radacina, numeSoferCautat);
	printf("Pretul total al masinilor soferului %s este: %.2f\n", numeSoferCautat, totalSofer);


	dezalocareArboreDeMasini(&radacina);

	return 0;
}