#include <stdio.h>
#include <conio2.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#include "CodFontTADs/TADfrontend.h"

#define CPU_BURST 10

void exec(void)
{
	srand(time(NULL));
	/////////////////////////////////////////////////////////////
	// DECLARACOES
	int ut = 0, finalizados = 0, bloqueados = 0,x=0;
	int prior;
	char op = 0;
	int qtdpronto = 0, qtdmt = 0, qtdhd = 0, qtdespera = 0, qtdfilho = 0;
	filaPCB espera, pronto, mouse, hd, concluido;
	PCB *processo_atual, *processo_pai, *processo_executando, *aux;
	int pid_atual = 1, ppid_atual = 0, tempo = 0, total_tempo_bloqueio = 0;
	float media_espera=0;
	system("mode con: cols=131 lines=42");
	int cpuburstat = 0; // Flag para rastrear burst atual

	/////////////////////////////////////////////////////////////
	// INICIALIZACAO

	init(&espera, 'e');
	init(&pronto, 'p');
	init(&mouse, 'e');
	init(&hd, 'e');
	init(&concluido, 'p');
	
	PCB *proc1 = criaPCB(pid_atual++, 11, 2,ut);
	PCB *proc2 = criaPCB(pid_atual++, 11, 1,ut);
	PCB *proc3 = criaPCB(pid_atual++, 11, 3,ut);
	PCB *proc4 = criaPCB(pid_atual++, 11, 4,ut);

	enqueue(&pronto, proc1, ut);
	enqueue(&pronto, proc2, ut);
	enqueue(&pronto, proc3, ut);
	enqueue(&pronto, proc4, ut);

	qtdpronto = lF(pronto);
	qtdmt = lF(mouse);
	qtdhd = lF(hd);
	qtdespera = lF(espera);
	textcolor(2);
	printf("        -=LEIA-ME=-\n");
	textcolor(3);
	printf("1- nao utilize o mouse para controlar o simulador, caso clique, pressione qualquer botao para continuar\n");
	printf("2- a tela e definida automaticamente, caso alterada, causa problema na exibicao\n\n");
	printf("Pressione qualquer botao pra comecar a simulacao\n");
	textcolor(15);
	getch();
	clrscr();

	layout();
	processo_executando = dequeue(&pronto, ut); // Pega o primeiro processo
	atualizaProcesso(processo_executando);
	atualizaPagina(lF(pronto), lF(mouse), lF(hd), lF(espera), qtdeFilhos(processo_executando, pronto, mouse, hd, espera), processo_executando, pronto, mouse, hd, espera);
	printFila(pronto, 1);

	////////////////////////////////////////////////////////////
	do
	{
		// Verifica se o processo foi finalizado
		if (processo_executando != NULL)
		{
			if (processo_executando->tempo_executado == processo_executando->tempo_total)
			{
				// Processo foi finalizado
				aux = processo_executando;
				
				if (!isEmpty(&pronto))
					processo_executando = dequeue(&pronto, ut);
				else
					processo_executando = setatualnulo(); // Não há mais processos
				
				total_tempo_bloqueio += cpuburstat;
				cpuburstat = 0;
				
				if(!qtdeFilhos(aux, pronto, mouse, hd, espera)){//se ele nao tem filho
					free(aux);
					finalizados++;
					enqueue(&concluido,aux,ut);
				}else{//se tem filho
					enqueue(&espera,aux,ut);
				}
				
			}
		}
		
		if(!isEmpty(&espera)){
			if(qtdeFilhos(espera.inicio, pronto, mouse, hd, espera)){
				if (espera.inicio->tempo_executado == espera.inicio->tempo_total){
					dequeue(&espera,ut);
					finalizados++;
				}//se ele nao tem filho pode encerrar
				else if(ut % 10 == 0){
					enqueue(&pronto,dequeue(&espera,ut),ut);
				}
			}
		}
		
		if (cpuburstat % CPU_BURST == 0){
				aux = processo_executando; 
				// Reinsere o processo atual na fila e pega o próximo
				enqueue(&pronto, processo_executando, ut);
				processo_executando = dequeue(&pronto, ut);
				cpuburstat = 0; // Reseta o contador de burst ao trocar de processo
				if(aux == processo_executando)
					processo_executando->bloc = 1;
		}

		// Decide se o processo vai ser bloqueado ou nao
		if (rand() % 100 < 12)
		{ // Ajuste da probabilidade de bloqueio
			if (processo_executando != NULL)
			{	
				bloqueados++;
				if (rand() % 2 == 0)
					enqueue(&mouse, processo_executando, ut);
				else
					enqueue(&hd, processo_executando, ut);
				
				if (!isEmpty(&pronto))
					processo_executando = dequeue(&pronto, ut);
				else
					processo_executando = setatualnulo();
				cpuburstat = 0; // Reseta o burst ao ser bloqueado
			}
		}
		if(processo_executando!= NULL)
			if (rand() % 100 < 10) {
				PCB *filho = criaPCB(++pid_atual, processo_executando->pid, rand() % 3 + 1,ut);
				enqueue(&espera, processo_executando, ut);
				processo_executando = filho;
			}

		// Desbloqueia processos das filas de mouse e HD
		if (!isEmpty(&mouse))
			if (mouse.inicio->tempo_bloqueio <= 0)
			{
				bloqueados++;
				aux = dequeue(&mouse, ut);
				total_tempo_bloqueio += aux->total_espera;
				enqueue(&pronto, aux, ut);
			}
		if (!isEmpty(&hd))
			if (hd.inicio->tempo_bloqueio <= 0)
			{
				bloqueados++;
				aux = dequeue(&hd, ut);
				total_tempo_bloqueio += aux->total_espera;
				enqueue(&pronto, aux, ut);
			}

		decrementaTempoBloqueio(&mouse);
		decrementaTempoBloqueio(&hd);

		if (processo_executando != NULL)
		{
			cpuburstat++;
			processo_executando->tempo_executado++;
		}

		setunidadetempo(++ut);
		printunidadetempo(ut);

		if (op == 'C')
		{
			prior = digitaprior();
			printprecione();
			enqueue(&pronto, criaPCB(++pid_atual, 11, prior,ut), ut);
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
	media_espera = bloqueados / (float)total_tempo_bloqueio;
	
	enqueue(&pronto, processo_executando, ut);
	finalizaFilasBloqueio(&pronto, &mouse, &hd, &espera, ut);
	moveParaConcluido(&pronto, &concluido, ut);
	clrscr();
	x = countbloc(concluido);
	relatorio(finalizados, bloqueados, media_espera,x);
	
	printf("\n-------------------------------------\n\nProcessos e seus filhos:\n");
	if(concluido.inicio != NULL){
		aux = concluido.inicio;
		while(aux != concluido.fim){
			printf("pai: %d | tempo exec total: %d\n",aux->pid,ut - aux->tempo_inic);
			qtdeFilhosfinal(concluido,aux->pid);
			aux = aux->prox;
		}
		printf("pai: %d | tempo exec total: %d\n",aux->pid,ut - aux->tempo_inic);
		qtdeFilhosfinal(concluido,aux->pid);
		printf("\n");
	}
	
	
	
	printf("\n\nAperte qualquer tecla para sair da simulacao.");
	getch();
}

int main()
{
	exec();
}