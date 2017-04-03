typedef int *tpElem;

typedef struct registro{
	tpElem *dado;
	struct registro *ant;
	struct registro *prox;
}tpLista;

void insere(tpLista *noatual,tpElem x);
void mostra(tpLista *noatual);
tpLista* procura(tpLista *noatual, tpElem x);
int retira(tpLista *noatual, tpElem x);
void esvazia(tpLista *noatual);
