#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Pacient {
	int id;
	int varsta;
	int gradUrgenta; //1 - grad scazut, 5 - grad ridicat
	char* nume;
	char* diagnostic;
};

typedef struct Pacient Pacient;

struct Heap {
	Pacient* vector;
	int lungime;
	int nrPacienti;
};

typedef struct Heap Heap;

struct Nod {
	Pacient info;
	struct Nod* next;
};

typedef struct Nod Nod;

Pacient citirePacientDinFisier(FILE* file) {
	char buffer[256];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Pacient p;
	aux = strtok(buffer, sep);

	p.id = atoi(aux);

	p.varsta = atoi(strtok(NULL, sep));

	p.gradUrgenta = atoi(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	p.nume = malloc(strlen(aux) + 1);
	strcpy_s(p.nume, strlen(aux) + 1, aux);


	aux = strtok(NULL, sep);
	p.diagnostic = malloc(strlen(aux) + 1);
	strcpy_s(p.diagnostic, strlen(aux) + 1, aux);


	return p;
}

void afisarePacient(Pacient pacient) {
	printf("Id: %d\n", pacient.id);
	printf("Nume pacient: %s\n", pacient.nume);
	printf("Varsta: %d\n", pacient.varsta);
	printf("Diagnostic: %s\n", pacient.diagnostic);
	printf("Grad de urgenta: %d\n", pacient.gradUrgenta);
}

void afisareLista(Nod* lista) {
	if (lista) {
		Nod* p = lista;
		while (p) {
			afisarePacient(p->info);
			p = p->next;
		}
	}
}

Heap initializareHeap(int lungime) {
	Heap h;
	h.lungime = lungime;
	h.nrPacienti = 0;
	h.vector = (Pacient*)malloc(lungime * sizeof(Pacient));
	return h;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozStanga = 2 * pozitieNod + 1;	
	int pozDreapta = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;

	//comparatie copil stang
	if (pozStanga < heap.nrPacienti && heap.vector[pozMax].gradUrgenta < heap.vector[pozStanga].gradUrgenta) {
		pozMax = pozStanga;
	}

	//comparatie copil drept
	if (pozDreapta < heap.nrPacienti && heap.vector[pozMax].gradUrgenta < heap.vector[pozDreapta].gradUrgenta) {
		pozMax = pozDreapta;
	}

	if (pozMax != pozitieNod) {
		Pacient aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrPacienti - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDePacientiDinFisier(const char* numeFisier) {
	Heap h = initializareHeap(10);
	FILE* file = fopen(numeFisier, "r");

	while (!feof(file)) {
		Pacient p = citirePacientDinFisier(file);
		h.vector[h.nrPacienti] = p;
		h.nrPacienti++;
	}
	fclose(file);

	for (int i = (h.nrPacienti - 2) / 2; i >= 0; i--) {
		filtreazaHeap(h, i);
	}

	return h;
}

void afisareHeap(Heap heap) {
	if (heap.vector) {
		for (int i = 0; i < heap.nrPacienti; i++) {
			afisarePacient(heap.vector[i]);
		}
	}
}

void afisareHeapAscuns(Heap heap) {
	for (int i = heap.nrPacienti; i < heap.lungime; i++) {
		afisarePacient(heap.vector[i]);
	}
}

Pacient extragePacient(Heap* heap) {
	if (heap->nrPacienti > 0) {
		Pacient aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrPacienti - 1];
		heap->vector[heap->nrPacienti - 1] = aux;
		heap->nrPacienti--;

		for (int i = (heap->nrPacienti - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
		return aux;
	}
	Pacient p;
	p.diagnostic = NULL;
	p.nume = NULL;
	return p;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].nume);
		free(heap->vector[i].diagnostic);
	}
	free(heap->vector);
	heap->lungime = 0;
	heap->nrPacienti = 0;
	heap->vector = NULL;
}

void inserareInLista(Nod** lista, Pacient p) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = p;
	nou->next = *lista;

	(*lista) = nou;
}

Nod* pacientiCuGradDeUrgentaRidicat(Heap* heap, int gradUrgenta) {
	Nod* lista = NULL;
	while (heap->vector[0].gradUrgenta >= gradUrgenta) {
		Pacient p = extragePacient(heap);
		inserareInLista(&lista, p);
	}
	return lista;
}

int main() {
	Heap h = citireHeapDePacientiDinFisier("pacienti.txt");
	printf("Elementele sunt: \n");
	afisareHeap(h);

	Nod* lista = pacientiCuGradDeUrgentaRidicat(&h, 4);
	printf("Lista:\n");
	afisareLista(lista);

	printf("Elementele ascunse sunt: \n");
	afisareHeapAscuns(h);
	dezalocareHeap(&h);
	while (lista) {
		Nod* aux = lista;
		lista = lista->next;
		free(aux);
	}
	free(lista);
	return 0;

}