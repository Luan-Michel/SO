#include <stdio.h>
#include <stdlib.h>
#define minha_memoria 1024

typedef struct hole{
    int tam;
    struct hole *prox;
    struct hole *ante;
}hole;

struct hole *header;  //raiz
hole *next = NULL;
//int minha_memoria = 1024;

void inicializa_mem(int mem);
void * meu_aloca(int size);
void meu_desaloca(hole * ponteiro);
void mostra_mem();

int main()
{
  hole *a1, *a2, *a3, *a4, *a5, *a6, *a7, *a8, *a9;
  inicializa_mem(minha_memoria);
  mostra_mem();
  a1 = (hole *)meu_aloca(35);
  a2 = (hole *)meu_aloca(40);
  a3 = (hole *)meu_aloca(20);
  a4 = (hole *)meu_aloca(30);
  a5 = meu_aloca(25);
  a6 = meu_aloca(40);
  a7 = meu_aloca(60);
  a8 = meu_aloca(80);
  a9 = meu_aloca(100);
  mostra_mem();
  meu_desaloca(a1);
  mostra_mem();
  meu_desaloca(a2);
  mostra_mem();
  meu_desaloca(a3);
  mostra_mem();
  meu_desaloca(a4);
  mostra_mem();  
  a1 = meu_aloca(500);
  mostra_mem();
}

void mostra_mem(){							//função 'mostra memoria': percorre a lista encadeada e mostra os valores
  struct hole *atual;
  atual = header;
  int a=0;
  printf("\nEstado - tam - cont*\n");					//cont utilizado somente como contador
  while(atual != NULL) {
    if(atual->tam > 0) printf("Free: \t%d  \t%d\n", atual->tam, a ); 	//se houver espaço disponível apresenta um valor positivo
    else printf("Full: \t%d  \t%d\n", atual->tam,a );			//se houver espaço alocado apresenta um valor negativo
    atual = atual->prox;
    a++;
  }
}

void inicializa_mem(int mem)
{
	header = malloc(mem);  				//header recebe um ponteiro para o tamanho requesitad
	header -> tam = mem - sizeof(struct hole);    	//O tamanho restante é 1024 menos o tamanho já utilizado por hole
	header -> ante = NULL; 				//Anterior n�o aponta para nada ainda
	header -> prox = NULL; 				//Proximo n�o aponta para nada ainda
}

/* 
void *meu_aloca(int size)	//next-fit
{
	if(next ==NULL){
		next = header;
	}
	hole *aux = next;						//começa procurando espaço no ultimo alocado 'next'
	while(aux != NULL){
		if((aux->tam) > (int)(size+sizeof(hole))){
			hole *p = aux->prox;
			aux->prox = (aux+sizeof(hole)+size);
			aux->prox->tam = aux->tam-size-sizeof(hole);
			aux->prox->prox = p;
			aux->prox->ante = aux;
			aux->tam = size*(-1);
			next = aux;					//se aloca, atribui a posição da alocação ao 'next'
			return aux;
		}
		aux=aux->prox;
	}
	
	aux = header;
	
	while(aux != next){						//se chegou ao fim sem encontrar uma posição, volta ao começo 
		if((aux->tam) > (int)(size+sizeof(hole))){		//e refaz o processo
			hole *p = aux->prox;
			aux->prox = (aux+sizeof(hole)+size);
			aux->prox->tam = aux->tam-size-sizeof(hole);
			aux->prox->prox = p;
			aux->prox->ante = aux;
			aux->tam = size*(-1);
			next = aux;
			return aux;
		}
		aux=aux->prox;
	}
	
	printf("memoria cheia\n");
	return -1;	
}
*/

void *meu_aloca(int size)	//worst-fit
{
	hole *aux = header;
	int pior=0;
	
	while(aux != NULL){							//procura pelo pior espaço
		if(aux->tam > (int)(size+sizeof(struct hole)) && (aux->tam > pior)){
			pior = aux->tam;
		}
		aux = aux->prox;
	}
	
	if(!pior){								//se não encontra espaço retorna a mensagem
		printf("memoria cheia\n");
		return -1;
	}
	aux = header;
	
	while(aux != NULL){							//procura pela posição da pior escolha
		if(aux->tam == pior){						//para alocar
			//printf("z%d\n",aux->tam);
			hole *p = aux->prox;
			aux->prox = (aux+sizeof(hole)+size);
			aux->prox->tam = aux->tam-size-sizeof(hole);
			aux->prox->prox = p;
			aux->prox->ante = aux;
			aux->tam = size*(-1);
			return aux;
		}
		aux= aux->prox;
	}
	
}


/*
void *meu_aloca(int size)	//best-fit
{
	hole *aux = header;
	int melhor=0, flag=0;
	
	while(aux != NULL){							//procura pelo melhor espaço
		if(aux->tam > (int)(size+sizeof(struct hole)) && (aux->tam < melhor || flag==0)){
			melhor = aux->tam;
			flag = 1;
		}
		aux = aux->prox;
	}
	
	if(!flag){								//se não encontra um espaço que caiba
		printf("memoria cheia\n");					//retorna uma mensagem
		return -1;
	}
	aux = header;
	while(aux != NULL){							//procura o espaço com o melhor encaixe
		if(aux->tam == melhor){						//para alocar
			hole *p = aux->prox;
			aux->prox = (aux+sizeof(hole)+size);
			aux->prox->tam = aux->tam-size-sizeof(hole);
			aux->prox->prox = p;
			aux->prox->ante = aux;
			aux->tam = size*(-1);
			return aux;
		}
		aux= aux->prox;
	}
}
* 
*/
/*
void *meu_aloca(int size)	//first-fit
{
	hole *aux = header;
	while(aux != NULL){							//percorre a lista encadeada
		if((aux->tam) > (int)(size+sizeof(hole))){			//quando encotra uma posição que caiba
			hole *p = aux->prox;					//ja faz o encaixe
			aux->prox = (aux+sizeof(hole)+size);
			aux->prox->tam = aux->tam-size-sizeof(hole);
			aux->prox->prox = p;
			aux->prox->ante = aux;
			aux->tam = size*(-1);
			if(p != NULL){
				p->ante = aux->prox;
			}
			return aux;
		}
		aux=aux->prox;
	}
	printf("memoria cheia\n");
	return -1;
}
*/

void meu_desaloca(hole * ponteiro)
{
	if(ponteiro == -1){
		printf("posição invalida\n");
		return;
	}
	
	hole *aux = (hole *)ponteiro;
	
	aux->tam = aux->tam*(-1);
	
	if(aux->ante != NULL && aux->ante->tam >= 0){
		if(next == aux) next = aux->ante;
		if(aux->tam < 0) aux->ante->tam = ((aux->tam)*(-1))+(aux->ante->tam)+(int)(sizeof(hole));
		else aux->ante->tam = (aux->tam)+(aux->ante->tam)+(int)(sizeof(hole));
		aux->ante->prox = aux->prox;
		aux = aux->ante;
		if(aux->prox != NULL) aux->prox->ante = aux;					//atualiza o ponteiro 'ante' da proxima estrutura 
	}
	
	if(aux->prox != NULL && aux->prox->tam >= 0){						//verifica se não é o ultimo elemento e se o proximo é um buraco
		if(next == aux->prox) next = aux;
		if(aux->tam < 0) aux->tam = ((aux->tam)*(-1))+aux->prox->tam+(int)(sizeof(hole));//se sim, adiciona o tamanho do ultimo ao seu
		else aux->tam = (aux->tam)+aux->prox->tam+(int)(sizeof(hole));
		aux->prox = aux->prox->prox;
		if(aux->prox != NULL) aux->prox->ante = aux;					//e aponta para o proximo elemento depois do proximo			
	}
		
}
