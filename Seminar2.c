#include<stdio.h>
#include<stdlib.h>

struct Masina {
	int id;
	int nrKm;
	char* model;
	float pret;
	char tipMotor;
};

struct Masina initializare(int id, int nrKm, const char* model, float pret, char tipMotor) {
	struct Masina m;
	m.id = id;
	m.nrKm = nrKm;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy(m.model, model);
	m.pret = pret;
	m.tipMotor = tipMotor;
	return m;
}

void afisare(struct Masina m) {
	printf("Id: %d, nr km: %d, model: %s, pret: %f, tip motor: %c", m.id, m.nrKm, m.model, m.pret, m.tipMotor);

}

void afisareVector(struct Masina* vector, int nrElemente) {
	for (int i = 0;i < nrElemente;i++)
	{
		afisare(vector[i]);
		printf("\n");
	}
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int *nrElementeCopiate) {

	struct Masina* vectorNou = NULL;
	
	if (vector != NULL && (*nrElementeCopiate) > 0)
	{

		if (nrElementeCopiate > nrElemente)
		{
			*nrElementeCopiate = nrElemente;
		}

		vectorNou = malloc(sizeof(struct Masina) * (*nrElementeCopiate));

		for (int i = 0; i < *nrElementeCopiate;i++)
		{
			vectorNou[i] = initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor);
		}

		return vectorNou;

	}
}

void dezalocare(struct Masina** vector, int* nrElemente) {

	for (int i = 0; i < (*nrElemente); i++)
	{
		if ((*vector)[i].model != NULL)
		{
			free((*vector)[i].model);
		}
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;

}

void copiazaMasiniIeftine(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {

	*dimensiune = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret <= prag) {
			(*dimensiune)++;
		}
	}
	
	*vectorNou = malloc(sizeof(struct Masina) * (*dimensiune));
	int contor = 0;

	for (int i = 0;i < dimensiune;i++) {
		
		if (vector[i].pret <= prag) {
			(*vectorNou)[contor] = vector[i];
			(*vectorNou)[contor].model = malloc(sizeof(char) * strlen(vector[i].model) + 1);
			strcpy_s((*vectorNou)[contor].model, (strlen(vector[i].model) + 1), vector[i].model);
			contor++;
		}
	}


}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie) {

	for (int i = 0; i < nrElemente; i++) {
		if (strstr(vector[i].model, conditie) != NULL) {
			return vector[i];
		}
	}

	struct Masina masinaNula;
	masinaNula.id = -1; 
	return masinaNula;
}
	


int main() 
{
	int nrMasini = 3;
	struct Masina* vectorMasina;
	vectorMasina = malloc(sizeof(struct Masina) * nrMasini);
	vectorMasina[0] = initializare(1, 1000, "Logan", 1000.4, 'd');
	vectorMasina[1] = initializare(2, 2000, "Golf", 2000.4, 'g');
	vectorMasina[2] = initializare(3, 3000, "GL", 3502.4, 'm');

	afisareVector(vectorMasina,nrMasini);


	// primele n elemenete copiate
	int nrMasiniPartiale = 1;
	struct Masina *vectorMasinaPartial = malloc(sizeof(struct Masina) * nrMasiniPartiale);
	vectorMasinaPartial = copiazaPrimeleNElemente(vectorMasina, nrMasini, &nrMasiniPartiale);
	printf("\nElemente copiate: \n");
	afisareVector(vectorMasinaPartial, nrMasiniPartiale);

	dezalocare(&vectorMasinaPartial, &nrMasiniPartiale);

	printf("\nMasini ieftine: \n");
	copiazaMasiniIeftine(vectorMasina, nrMasini, 1500, &vectorMasinaPartial, &nrMasiniPartiale);
	afisareVector(vectorMasinaPartial, nrMasiniPartiale);
	
	dezalocare(vectorMasinaPartial,nrMasiniPartiale);
	dezalocare(vectorMasina,nrMasini);

	struct Masina masinaGasita = getPrimulElementConditionat(vectorMasina, nrMasini, "Go");
	if (masinaGasita.id != -1) {
		printf("\nMasina gasita: ");
		afisare(masinaGasita);
	}
	else {
		printf("\nNu s-a gasit nicio masina care sa indeplineasca conditia.");
	}

	dezalocare(&vectorMasina, &nrMasini);


	return 0;
}