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

//STACK - LSI

struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

void pushStack(Nod** varfStiva,Masina masina) {
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->info = masina;
	nodNou->next = (*varfStiva);
	(*varfStiva) = nodNou;
}

Masina popStack(Nod** varfStiva) {
	if ((*varfStiva) != NULL)
	{
		Nod* temp = *varfStiva;
		Masina m = temp->info;
		*varfStiva = temp->next;
		free(temp);
		return m;
	}
	else
	{
		return (Masina) { -1, 0, 0, NULL, NULL, '-' };
	}
}

int isEmptyStack(Nod* stiva) {
	return stiva == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* varfStiva = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&varfStiva, m);
	}
	fclose(f);
	return varfStiva;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while ((*stiva)) {
		Masina m = popStack(stiva);
		free(m.model);
		free(m.numeSofer);
	}
}

int size(Nod* stiva) {
	int count = 0;
	while (stiva) {
		count++;
		stiva = stiva->next;
	}
	return count;
}

//QUEUE - LDI

struct NodDublu {
	struct NodDublu* next;
	struct NodDublu* prev;
	Masina info;
};
typedef struct NodDublu NodDublu;

struct Coada {
	NodDublu* front;
	NodDublu* rear;
};
typedef struct Coada Coada;

void enqueue(Coada* coada,Masina masina) {
	//adauga o masina in coada
	NodDublu* nodNou = (NodDublu*)malloc(sizeof(NodDublu));
	nodNou->info = masina;
	nodNou->next = NULL;
	nodNou->prev = coada->rear;
	if(coada->rear) {
		coada->rear->next = nodNou;
	}
	else {
		coada->front = nodNou;
	}
	coada->rear = nodNou;
}

Masina dequeue(Coada* coada) {
	//extrage o masina din coada
	if (coada->front) {
		Masina m = coada->front->info;
		NodDublu* temp = coada->front;
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

void dezalocareCoadaDeMasini(Coada* coada) {
	while (coada->front != NULL) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
}


//metode de procesare
Masina getMasinaByID_Stiva(Nod* stiva, int id) {
	while (stiva != NULL) {
		if (stiva->info.id == id) {
			return stiva->info;
		}
		stiva = stiva->next;
	}
	Masina m = { -1, 0, 0, NULL, NULL, '-' };
	return m;
}

Masina getMasinaByID_Coada(Coada coada, int id) {
	NodDublu* current = coada.front;
	while (current != NULL) {
		if (current->info.id == id) {
			return current->info;
		}
		current = current->next;
	}
	Masina m = { -1, 0, 0, NULL, NULL, '-' };
	return m;
}


float calculeazaPretTotalStiva(Nod** stiva) {
	Nod* temp=NULL;
	float suma = 0;
	while (!isEmptyStack(*stiva)) {
		Masina m = popStack(stiva);
		suma += m.pret;
		pushStack(&temp, m);
	}
	while (!isEmptyStack(temp)) {
		pushStack(stiva, popStack(&temp));
	}
	return suma;
}

float calculeazaPretTotalCoada(Coada* coada) {
	float suma = 0;
	NodDublu* current = coada->front;
	while (current != NULL) {
		suma += current->info.pret;
		current = current->next;
	}
	return suma;
}



int main() {

	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	//Masina masinaExtrasaStiva = popStack(&stiva);
	//afisareMasina(masinaExtrasaStiva);

	Coada coada = citireCoadaDeMasiniDinFisier("masini.txt");
	//Masina masinaExtrasaCoada = dequeue(&coada);
	//afisareMasina(masinaExtrasaCoada);

	printf("Pret total: %.f", calculeazaPretTotalStiva(&stiva));
	

	printf("\nPret total Coada: %.2f\n", calculeazaPretTotalCoada(&coada));

	int idCautat = 2;
	Masina mStiva = getMasinaByID_Stiva(stiva, idCautat);
	printf("\nMasina cu ID %d in stiva:\n", idCautat);
	afisareMasina(mStiva);

	Masina mCoada = getMasinaByID_Coada(coada, idCautat);
	printf("Masina cu ID %d in coada:\n", idCautat);
	afisareMasina(mCoada);

	dezalocareStivaDeMasini(&stiva);
	dezalocareCoadaDeMasini(&coada);

	return 0;
}