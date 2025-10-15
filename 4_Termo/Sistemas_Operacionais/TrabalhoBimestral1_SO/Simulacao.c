#include <stdio.h>
#include <conio2.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#include "TAD/TADMoldura.h"

#define QUANTUM 10

void executar(void)
{
	srand(time(NULL));

	int unidadeTempo = 0, finalizados = 0, bloqueados = 0,x=0;
	int prior;
	char op = 0;
	int qtdpronto = 0, qtdmt = 0, qtdhd = 0, qtdespera = 0, qtdfilho = 0;
	filaPCB espera, pronto, mouse, hd, concluido;
	PCB *processo_atual, *processo_pai, *processo_executando, *aux;
	int pid_atual = 1, ppid_atual = 0, tempo = 0, total_tempo_bloqueio = 0;
	float media_espera=0;
	int quantum = 0;

	init(&espera, 'e');
	init(&pronto, 'p');
	init(&mouse, 'e');
	init(&hd, 'e');
	init(&concluido, 'p');
	
	PCB *proc1 = criaPCB(pid_atual++, 0, 2,unidadeTempo);
	PCB *proc2 = criaPCB(pid_atual++, 0, 1,unidadeTempo);
	PCB *proc3 = criaPCB(pid_atual++, 0, 3,unidadeTempo);
	PCB *proc4 = criaPCB(pid_atual++, 0, 4,unidadeTempo);

	enqueue(&pronto, proc1, unidadeTempo);
	enqueue(&pronto, proc2, unidadeTempo);
	enqueue(&pronto, proc3, unidadeTempo);
	enqueue(&pronto, proc4, unidadeTempo);

	qtdpronto = lF(pronto);
	qtdmt = lF(mouse);
	qtdhd = lF(hd);
	qtdespera = lF(espera);

	menuInicial();
	clrscr();
	layout();
	processo_executando = dequeue(&pronto, unidadeTempo);
	atualizaProcesso(processo_executando);
	atualizaPagina(lF(pronto), lF(mouse), lF(hd), lF(espera), qtdeFilhos(processo_executando, pronto, mouse, hd, espera), processo_executando, pronto, mouse, hd, espera);
	printFila(pronto, 1);

	do
	{
		if (processo_executando != NULL)
		{
			if (processo_executando->tempo_executado == processo_executando->tempo_total)
			{
				// Processo foi finalizado
				aux = processo_executando;
				
				if (!isEmpty(&pronto))
					processo_executando = dequeue(&pronto, unidadeTempo);
				else
					processo_executando = setatualnulo(); // nao tem mais processos
				
				total_tempo_bloqueio += quantum;
				quantum = 0;
				
				if(!qtdeFilhos(aux, pronto, mouse, hd, espera)) { // se ele nao tem filho
				    finalizados++;
				    enqueue(&concluido, aux, unidadeTempo);
				} else { // se tem filho
				    enqueue(&espera, aux, unidadeTempo);
				}
				
			}
		}
		
		if(!isEmpty(&espera)){
			if(qtdeFilhos(espera.inicio, pronto, mouse, hd, espera)){
				if (espera.inicio->tempo_executado == espera.inicio->tempo_total){
					dequeue(&espera,unidadeTempo);
					finalizados++;
				}//se ele nao tem filho pode encerrar
				else if(unidadeTempo % 10 == 0){
					enqueue(&pronto,dequeue(&espera,unidadeTempo),unidadeTempo);
				}
			}
		}
		
		if (processo_executando != NULL && quantum > 0 && quantum % QUANTUM == 0) {
		    aux = processo_executando;
		    enqueue(&pronto, processo_executando, unidadeTempo);
		    processo_executando = dequeue(&pronto, unidadeTempo);
		    quantum = 0;
		    if (aux == processo_executando)
		        processo_executando->bloc = 1;
		}

		// Decide se o processo vai ser bloqueado
		if (rand() % 100 < 12) 
		{
		    if (processo_executando != NULL)
		    {
		        // Marca o processo como já bloqueado ao menos uma vez
		        if (!processo_executando->ja_bloqueou)
		        {
		            processo_executando->ja_bloqueou = 1;
		            bloqueados++;
		        }
		
		        // Define o tempo de bloqueio aleatório e acumula no total
		        processo_executando->tempo_bloqueio = rand() % 11 + 5;
		        processo_executando->tempo_bloqueado_total += processo_executando->tempo_bloqueio;
		
		        // Envia o processo para a fila de bloqueio (mouse ou hd)
		        if (rand() % 2 == 0)
		            enqueue(&mouse, processo_executando, unidadeTempo);
		        else
		            enqueue(&hd, processo_executando, unidadeTempo);
		
		        // Tira o processo da CPU e pega outro, se houver
		        if (!isEmpty(&pronto))
		            processo_executando = dequeue(&pronto, unidadeTempo);
		        else
		            processo_executando = setatualnulo();
		
		        quantum = 0;
		    }
		}
		if(processo_executando!= NULL)
			if (rand() % 100 < 10) {
				PCB *filho = criaPCB(++pid_atual, processo_executando->pid, rand() % 3 + 1,unidadeTempo);
				enqueue(&espera, processo_executando, unidadeTempo);
				processo_executando = filho;
			}
		// Desbloqueia processos do mouse
		if (!isEmpty(&mouse))
		    if (mouse.inicio->tempo_bloqueio <= 0)
		    {
		        aux = dequeue(&mouse, unidadeTempo);
		
		        // Calcula tempo real de bloqueio
		        int tempo_bloqueado = unidadeTempo - aux->tempob_inicial;
		        aux->tempo_bloqueado_total += tempo_bloqueado;
		
		        total_tempo_bloqueio += tempo_bloqueado;
		
		        enqueue(&pronto, aux, unidadeTempo);
		    }
		
		// Desbloqueia processos do HD,
		if (!isEmpty(&hd))
		    if (hd.inicio->tempo_bloqueio <= 0)
		    {
		        aux = dequeue(&hd, unidadeTempo);
		
		        int tempo_bloqueado = unidadeTempo - aux->tempob_inicial;
		        aux->tempo_bloqueado_total += tempo_bloqueado;
		        total_tempo_bloqueio += tempo_bloqueado;
		
		        enqueue(&pronto, aux, unidadeTempo);
		    }

		decrementaTempoBloqueio(&mouse);
		decrementaTempoBloqueio(&hd);

		if (processo_executando != NULL)
		{
			quantum++;
			processo_executando->tempo_executado++;
		}

		
		exibeUnidadeTempo(unidadeTempo);
		atualizaUnidadeTempo(++unidadeTempo);

		if (op == 'C')
		{
			prior = digitaprior();
			printprecione();
			enqueue(&pronto, criaPCB(++pid_atual, 0, prior,unidadeTempo), unidadeTempo);
			op = ' ';
		}

		if (processo_executando != NULL)
		{
			atualizaPagina(lF(pronto), lF(mouse), lF(hd), lF(espera), qtdeFilhos(processo_executando, pronto, mouse, hd, espera), processo_executando, pronto, mouse, hd, espera);
			atualizaProcesso(processo_executando);
		}
		else
		{
			setatualnulo();
		}

		Sleep(1000);
		if (kbhit())
		{
			op = toupper(getch());
		}
		
	} while (op != 27);

	float soma_tempos = 0;
	aux = concluido.inicio;
	if (aux != NULL) {
	    do {
	        soma_tempos += aux->tempo_bloqueado_total;
	        aux = aux->prox;
	    } while (aux != concluido.inicio);
	}
	
	if (bloqueados > 0)
	    media_espera = soma_tempos / (float) bloqueados;
	else
	    media_espera = 0.0f;

	media_espera = total_tempo_bloqueio / (float) bloqueados;

	
	if (processo_executando != NULL)
    enqueue(&pronto, processo_executando, unidadeTempo);
	finalizaFilasBloqueio(&pronto, &mouse, &hd, &espera, unidadeTempo);
	moveParaConcluido(&pronto, &concluido, unidadeTempo);
	clrscr();
	x = qtdPCBExePronto(pronto,concluido);
	relatorio(finalizados, bloqueados, media_espera,x);
	
	printf("\n\nProcessos e filhos:\n\n");
    if(concluido.inicio != NULL){
        aux = concluido.inicio;
        while(aux != concluido.fim){
            printf("pai: %d   tempo exec total: %d\n",aux->pid,unidadeTempo - aux->tempo_inic);
            qtdeFilhosfinal(concluido,aux->pid);
            aux = aux->prox;
        }
        printf("pai: %d   tempo exec total: %d\n",aux->pid,unidadeTempo - aux->tempo_inic);
        qtdeFilhosfinal(concluido,aux->pid);
        printf("\n");
    }
    getch();
	
	
	
	printf("\n\nAperte qualquer tecla para sair da simulacao.\n");
	getch();
}

int main()
{
	executar();
	
	return 0;
}
