#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
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

//  - - - - - - - - - STACK - LSI - - - - - - - - - 
struct NodLSI {
	Masina info;
	struct NodLSI* next;
};
typedef struct NodLSI NodLSI;

void pushStack(NodLSI** varfStiva, Masina masinaNoua) {
	NodLSI* nodNou = malloc(sizeof(NodLSI));
	nodNou->info = masinaNoua;
	nodNou->next = (*varfStiva);
	(*varfStiva) = nodNou;
}

Masina popStack(NodLSI** varfStiva) {
	if ((*varfStiva) != NULL) {
		NodLSI* temp = *varfStiva;
		Masina m = temp->info;
		*varfStiva = temp->next;
		free(temp);
		return m;
	}
	else {
		return (Masina) { -1, 0, 0, NULL, NULL, '-' };
	}
}

int isEmpty(NodLSI* varfStiva) {
	return varfStiva == NULL;
}

NodLSI* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodLSI* varfStiva = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&varfStiva, m);
	}
	fclose(f);
	return varfStiva;
}

void dezalocareStivaMasini(NodLSI** stiva) {
	while ((*stiva)) {
		Masina m = popStack(stiva);
		free(m.model);
		free(m.numeSofer);
	}
}

int size(NodLSI* stiva) {
	int count = 0;
	while (stiva) {
		count++;
		stiva = stiva->next;
	}
	return count;
}

Masina getMasinaByID_Stiva(NodLSI* stiva, int id) {
	while (stiva != NULL) {
		if (stiva->info.id == id) {
			return stiva->info;
		}
		stiva = stiva->next;
	}
	Masina m = { -1,0,0,NULL,NULL };
	return m;
}


float calculeazaPretTotalStiva(NodLSI* stiva) {
	float sumaTotala = 0;
	while (stiva != NULL) {
		sumaTotala += stiva->info.pret;
		stiva = stiva->next;
	}
	return sumaTotala;
}


//  - - - - - - - - - QUEUE - LDI - - - - - - - - - 

struct NodLDI {
	struct NodLDI* next;
	struct NodLDI* prev;
	Masina info;
};
typedef struct NodLDI NodLDI;

struct Coada {
	NodLDI* front;
	NodLDI* rear;
};
typedef struct Coada Coada;

void enqueue(Coada* coada, Masina masina) {
	NodLDI* nodNou = (NodLDI*)malloc(sizeof(NodLDI));
	nodNou->info = masina;
	nodNou->next = NULL;
	nodNou->prev = coada->rear;
	if (coada->rear) {
		coada->rear->next = nodNou;
	}
	else {
		coada->front = nodNou;
	}
	coada->rear = nodNou;
}

Masina dequeue(Coada* coada) {
	if (coada->front) {
		Masina m = coada->front->info;
		NodLDI* temp = coada->front;
		coada->front = coada->front->next;
		free(temp);
		if (coada->front == NULL) {
			coada->rear = NULL;
		}
		else {
			coada->front->prev = NULL;
		}
		return m;
	}
	else {
		Masina masinaNula;
		masinaNula.id = -1;
		return masinaNula;
	}
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Coada coada;
	coada.front = NULL;
	coada.rear = NULL;

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue((&coada), m);
	}
	fclose(f);
	return coada;
}

Masina getMasinaByID_Coada(Coada coada, int id) {
	NodLDI* current = coada.front;
	while (current != NULL) {
		if (current->info.id == id) {
			return current->info;
		}
		current = current->next;
	}
	Masina m = { -1, 0, 0, NULL, NULL, '-' };
	return m;
}


float calculeazaPretTotalCoada(Coada* coada) {
	float suma = 0;
	NodLDI* current = coada->front;
	while (current != NULL) {
		suma += current->info.pret;
		current = current->next;
	}
	return suma;
}




void dezalocareCoadaDeMasini(Coada* coada) {
	while (coada->front != NULL) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
}

int main() {
	NodLSI* stiva = citireStackMasiniDinFisier("masini.txt");

	printf("Pret total stiva: %.2f ", calculeazaPretTotalStiva(stiva));

	Coada coada = citireCoadaDeMasiniDinFisier("masini.txt");

	printf("\nPret total Coada: %.2f\n", calculeazaPretTotalCoada(&coada));

}