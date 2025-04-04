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

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* temp = (Nod*)malloc(sizeof(Nod));

	temp->info = masinaNoua;
	temp->next = NULL;

	if ((*cap) == NULL) {
		*cap = temp;
	}
	else {
		Nod* aux = *cap;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = temp;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;

	ht.dim = dimensiune;

	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}

	return ht;
}

int calculeazaHash(const char* cheieNumeSofer, int dimensiune) {
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
	//suma int(literelor numelui soferului) % dimensiune
	if (dimensiune != 0) {
		unsigned int suma = 0;
		for (int i = 0; i < strlen(cheieNumeSofer); i++) {
			suma += (int)cheieNumeSofer[i];
		}

		int hashCode = 0;

		hashCode = suma % dimensiune;
		return hashCode;
	}
	else {
		return -1;
	}

}

void inserareMasinaInTabela(HashTable hash, Masina masinaNoua) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	// determinam cheia -> numele soferului

	int pozitie = calculeazaHash(masinaNoua.numeSofer, hash.dim);

	//verificam daca avem coliziune

	if (hash.vector[pozitie] == NULL) {
		//nu avem coliziune
		adaugaMasinaInLista(&(hash.vector[pozitie]), masinaNoua);
	}
	else {
		//avem coliziune
		adaugaMasinaInLista(&(hash.vector[pozitie]), masinaNoua);
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {

	FILE* f = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dimensiune);

	while (!feof(f)) {
		Masina masinaNoua = citireMasinaDinFisier(f);
		inserareMasinaInTabela(hash, masinaNoua);
	}

	fclose(f);

	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Suntem la culsterul %d\n", i + 1);
		afisareListaMasini(ht.vector[i]);
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		Nod* current = ht->vector[i];
		while (current != NULL) {
			Nod* temp = current;
			current = current->next;
			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}


float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	int count = 0;

	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL) {
			count++;
		}
	}

	float* medii = (float*)malloc(count * sizeof(float));
	*nrClustere = count;
	int index = 0;


	for (int i = 0; i < ht.dim; i++) {
		Nod* current = ht.vector[i];
		if (current != NULL) {
			float suma = 0;
			int nr_masini = 0;
			while (current != NULL) {
				suma += current->info.pret;
				nr_masini++;
				current = current->next;
			}
			medii[index++] = suma / nr_masini;
		}
	}
	return medii;
}

Masina getMasinaDupaCheie(HashTable ht, const char* numeSofer) {
	Masina m;
	m.id = -1;
	int pozitie = calculeazaHash(numeSofer, ht.dim);

	if (ht.vector[pozitie] == NULL || pozitie < 0 || pozitie> ht.dim)
	{
		return m;
	}

	Nod* aux = ht.vector[pozitie];
	while (aux != NULL && strcmp(aux->info.numeSofer, numeSofer) != 0)
	{
		aux = aux->next;
	}
	if (aux != NULL) {
		m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
		strcpy(m.numeSofer, numeSofer);

		m.model = (char*)malloc(strlen(aux->info.model) + 1);
		strcpy(m.model, aux->info.model);

		m.id = aux->info.id;
		m.nrUsi = aux->info.nrUsi;
		m.pret = aux->info.pret;
		m.serie = aux->info.serie;
	}

	return m;
}

int main() {

	HashTable hash = citireMasiniDinFisier("masini.txt", 5);
	afisareTabelaDeMasini(hash);

	printf("Masina lui Ionescu este: ");

	Masina masina = getMasinaDupaCheie(hash, "Ionescu");

	if (masina.id != -1) {
		afisareMasina(masina);
		free(masina.numeSofer);
		free(masina.model);
	}

	int nrClustere;
	float* medii = calculeazaPreturiMediiPerClustere(hash, &nrClustere);
	printf("\nPreturi medii per cluster:\n");
	for (int i = 0; i < nrClustere; i++) {
		printf("Cluster %d: %.2f\n", i + 1, medii[i]);
	}
	free(medii);


	dezalocareTabelaDeMasini(&hash);

	printf("\nDimensiune tabela dupa dezalocare: %d", hash.dim);


	return 0;
}