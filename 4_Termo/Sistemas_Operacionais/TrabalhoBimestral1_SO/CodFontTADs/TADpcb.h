typedef struct pcb {
    int pid, ppid;
    int tempo_total;
    int tempo_executado; 
	int tempob_inicial;
	int total_espera;
	int tempo_bloqueio;
    int tempo_inic;
    int prioridade;
    int tempo_bloqueado_total;
    int ja_bloqueou;
	char status;
    char bloc;
    struct pcb *prox;

} PCB;

typedef struct filaPCB {
	struct pcb *inicio, *fim;
	char fila;
} filaPCB;

PCB *criaPCB(int pid, int ppid, int prior, int ut) {
    PCB *novoPCB = (PCB*)malloc(sizeof(PCB));
    if(pid >= 11)
        pid++;
	novoPCB->pid = pid;
	novoPCB->ppid = ppid;
	novoPCB->tempo_total = rand() % 11 + 10; // min = 10, max = 20
	novoPCB->tempo_executado = 0;
	novoPCB->tempob_inicial= 0;
    novoPCB->tempo_bloqueio = 10;
	novoPCB->prioridade = prior;
	novoPCB->status=0;
    novoPCB->tempo_inic = ut;
	novoPCB->prox = NULL;
    novoPCB->bloc = 0;
    novoPCB->tempo_bloqueado_total = 0;
    novoPCB->ja_bloqueou=0;
    

    
	return novoPCB;
}

void init(filaPCB *f, char tipo) {
	f->fila=tipo;
	f->inicio = f->fim = NULL;
}

char isEmpty(filaPCB *f) {
	return f->inicio == NULL;
}

void enqueue(filaPCB *f, PCB *processo, int ut) {
	if (f->fila == 'e')
    	processo->total_espera += (ut - processo->tempob_inicial);
	if (isEmpty(f)) {
        f->inicio = processo;
        f->fim = processo;
        processo->prox = processo;  
    } else {
        PCB *atual = f->inicio, *anterior = NULL;
        
        do {
            anterior = atual;
            atual = atual->prox;
        } while (atual != f->inicio && !(processo->prioridade < atual->prioridade));
        
        if (anterior == NULL) {
            processo->prox = f->inicio;
            f->fim->prox = processo;
            f->inicio = processo;
        } else {
            anterior->prox = processo;
            processo->prox = atual;
            
            if (atual == f->inicio) {
                f->fim = processo;
            }
        }
    }
}

PCB *dequeue(filaPCB *f, int ut) {
    if (isEmpty(f)) {
        return NULL;
    }

    PCB *removido = f->inicio;
    if (f->inicio == f->fim) {
        f->inicio = f->fim = NULL;
    } else {
        f->inicio = f->inicio->prox;
        f->fim->prox = f->inicio;
    }
    removido->prox = NULL;
    return removido;
}

void ExibirFila(filaPCB fila){
    PCB *aux = fila.inicio;
    if(aux == NULL) {
        printf("Fila vazia!\n");
    } else {
        do {
            printf("ID: %d, Prior: %d\n", aux->pid, aux->prioridade);
            aux = aux->prox;
        } while(aux != fila.inicio);
    }
}

void decrementaTempoBloqueio(filaPCB *fila) {
    if (!isEmpty(fila)) {
        PCB *atual = fila->inicio;
        do {
            atual->tempo_bloqueio--;
            atual = atual->prox;
        } while (atual != fila->inicio);
    }
}

char temFilho(int pid, filaPCB p, filaPCB m, filaPCB hd) {
    PCB *atual = p.inicio;
    while (atual != NULL) {
        if (atual->ppid == pid) {
            return 1;
        }
        atual = atual->prox;
    }

    atual = m.inicio;
    while (atual != NULL) {
        if (atual->ppid == pid) {
            return 1;
        }
        atual = atual->prox;
    }

    atual = hd.inicio;
    while (atual != NULL) {
        if (atual->ppid == pid) {
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}

void finalizaFilasBloqueio(filaPCB *pronto, filaPCB *mouse, filaPCB *hd, filaPCB *espera, int ut) {
    PCB *aux, *proximo;

    aux = mouse->inicio;
    if (aux != NULL) {
        do {
            proximo = aux->prox;
            enqueue(pronto, aux, ut);
            aux = proximo;
        } while (aux != mouse->inicio);
        mouse->inicio = mouse->fim = NULL;
    }

    aux = hd->inicio;
    if (aux != NULL) {
        do {
            proximo = aux->prox;
            enqueue(pronto, aux, ut);
            aux = proximo;
        } while (aux != hd->inicio);
        hd->inicio = hd->fim = NULL;
    }

    aux = espera->inicio;
    if (aux != NULL) {
        do {
            proximo = aux->prox;
            enqueue(pronto, aux, ut);
            aux = proximo;
        } while (aux != espera->inicio);
        espera->inicio = espera->fim = NULL;
    }

    aux = pronto->inicio;
    if (aux != NULL) {
        do {
            aux->tempo_executado = aux->tempo_total;
            aux = aux->prox;
        } while (aux != pronto->inicio);
    }
}
void relatorio(int finalizados, int bloqueados, float media_espera, int countbloc){
	
    printf("Quantidade de processos finalizados: %d\n", finalizados);
    printf("Quantidade de processos bloqueados: %d\n", bloqueados);
    printf("Tempo medio de espera: %.0f\n", media_espera);
    printf("A quantidade de processos que ficaram entre os estados de Execucao e Pronto: %d\n", countbloc);
}

void moveParaConcluido(filaPCB *pronto, filaPCB *concluido, int ut) {
    PCB *aux, *proximo;

    aux = pronto->inicio;
    if (aux != NULL) {
        do {
            proximo = aux->prox;
            enqueue(concluido, aux, ut);
            aux = proximo;
        } while (aux != pronto->inicio);

        pronto->inicio = pronto->fim = NULL;
    }
}

int countbloc(filaPCB concluido){
    int x=0;
    PCB *aux = concluido.inicio;
    while(aux != concluido.fim){
        aux = aux->prox;
        if(aux->bloc)
            x++;
    }
    return x;
}

void qtdeFilhosfinal(filaPCB concluido,int pid){
    PCB *aux = concluido.inicio;
    if(concluido.inicio != NULL){
		aux = concluido.inicio;
		while(aux != concluido.fim){
			if(aux->ppid == pid)
                printf("   filho: %d\n",aux->pid);
			aux = aux->prox;
		
		}
	}
}

int qtdPCBExePronto(filaPCB Pronto,filaPCB Execucao){
    PCB *aux = Pronto.inicio;
    int contadorPCB = 0;
    if(Pronto.inicio != NULL){
		aux = Pronto.inicio;
		while(aux != Pronto.fim){
			contadorPCB++;
                
			aux = aux->prox;
		
		}
	}
	aux = Execucao.inicio;
	if(Execucao.inicio != NULL){
		aux = Execucao.inicio;
		while(aux != Execucao.fim){
			contadorPCB++;
			aux = aux->prox;
		
		}
	}
	return contadorPCB;
}
