#include "TADpcb.h"

void desenhaTabela(int x, int y, int largura, int altura)
{
    int i, j;
    gotoxy(x, y); printf("+");
    for(i = 0; i < largura; i++) printf("-");
    printf("+");
    
    for(j = 0; j < altura; j++)
    {
        gotoxy(x, y + j + 1); printf("|");
        gotoxy(x + largura + 1, y + j + 1); printf("|");
    }
    
    gotoxy(x, y + altura + 1); printf("+");
    for(i = 0; i < largura; i++) printf("-");
    printf("+");
}

char menuInicial()
{
    clrscr();
    textcolor(14);
    desenhaTabela(41,10,32,7);
    gotoxy(42, 9); printf("### Ciclo de vida do Processo ###");
    textcolor(11);
    gotoxy(42, 11); printf("Gabriel Hideki ");  
    gotoxy(42, 12); printf("Henrique dos Santos");
    gotoxy(42, 13); printf("Thomas Henschel");
    gotoxy(42, 14); printf("Yuri Gabriel Rocha Lanuti");
    gotoxy(42, 17); printf("[qualquer tecla] para continuar");
    return getch();
}

void desenhaFilaPronto(int linhas)
{
	textcolor(14);
	desenhaTabela(3, 5, 22, linhas);
}

void desenhaFilaDispExt(int linhas)
{
	textcolor(14);
	desenhaTabela(29, 5, 21, linhas);
}

void desenhaFilaFilhos(int linhas)
{
	textcolor(14);
	desenhaTabela(99, 12, 14, linhas);
}

void desenhaFilaHD(int linhas)
{
	textcolor(14);
	desenhaTabela(54, 5, 19, linhas);
}

void desenhaFilaBlocFilho(int linhas)
{
	textcolor(14);
	desenhaTabela(77, 5, 18, linhas);
}

void limpaespaco(int x, int y, int variacaox, int variacaoy)
{
	int i;
	int largespaco = variacaox - x;
	for (i = y; i < variacaoy; i++)
	{
		gotoxy(x, i);
		printf("%*s", largespaco, "");
	}
}

void limpaquadrante1(void)
{
	limpaespaco(3, 5, 27, 36);
}

void limpaquadrante2(void)
{
	limpaespaco(29, 5, 52, 36);
}

void limpaquadrante3(void)
{
	limpaespaco(54, 5, 75, 36);
}

void limpaquadrante4(void)
{
	limpaespaco(77, 5, 97, 36);
}

void limpaquadrante5(void)
{
	limpaespaco(99, 12, 115, 36);
}

void limpastatusatual(void)
{
	limpaespaco(100, 6, 114, 11);
}

void limpaquadrantes(void)
{
	limpaquadrante1();
	limpaquadrante2();
	limpaquadrante3();
	limpaquadrante4();
	limpaquadrante5();
}

int digitaprior(void)
{
	int prioridade;
	
	limpaespaco(2, 37, 131, 40);
	textcolor(15);
	gotoxy(3, 38);
	printf("Digite a prioridade do processo: ");
		
	scanf("%d", &prioridade);
	
	return prioridade;
}

void printprecione(void)
{
	limpaespaco(2, 37, 131, 40);
	textcolor(15);
	gotoxy(3, 38);
	printf("Pressione (c) para criar um processo");
	gotoxy(3, 39);
	printf("Pressione (ESC) para sair da simulacao");
	textcolor(15);
}

void exibeUnidadeTempo()
{
	textcolor(4);
	desenhaTabela(120, 36, 6, 1);
	textcolor(15);
	gotoxy(68, 39);
}

void layout(void)
{

	int i, x, y;
	textcolor(15);
	gotoxy(47, 2);
	printf("SIMULADOR DE CICLO DE VIDA DE PROCESSOS");
	gotoxy(8, 4);
	printf("FILA DE PRONTO");
	gotoxy(30, 4);
	printf("FILA DE MOUSE/TECLADO");
	gotoxy(59, 4);
	printf("FILA DE HD");
	gotoxy(78, 4);
	printf("BLOQUEIO POR FILHO");

	textcolor(14);
	for (i = 2; i < 40; i++) //desenha moldura externa
	{
		gotoxy(1, i);
		printf("%c", 186);
		gotoxy(131, i);
		printf("%c", 186);
	}
	for (i = 2; i <= 131; i++)
	{
		gotoxy(i, 1);
		printf("%c", 205);
		gotoxy(i, 40);
		printf("%c", 205);
	}

	gotoxy(131, 1);
	printf("%c", 187);
	gotoxy(1, 1);
	printf("%c", 201);
	gotoxy(1, 40);
	printf("%c", 200);
	gotoxy(131, 40);
	printf("%c", 188);

	// desenha caixa status
	textcolor(14);
	desenhaTabela(99, 5, 14, 5);

	textcolor(15);
	gotoxy(100, 4);
	printf("PROCESSO ATUAL");
	textcolor(15);
	gotoxy(116, 6);
	printf("PID");
	gotoxy(116, 7);
	printf("PPID");
	gotoxy(116, 8);
	printf("Tempo Total");
	gotoxy(116, 9);
	printf("Tempo Executado");
	gotoxy(116, 10);
	printf("Prioridade");
	gotoxy(116, 13);
	printf("PID filhos");
	printprecione();
	exibeUnidadeTempo();
	gotoxy(68, 39);
}

void atualizaProcesso(PCB *teste)
{
	if(teste!= NULL){
		limpastatusatual();
		textcolor(9);
		gotoxy(101, 6);
		printf("%d", teste->pid);
		gotoxy(101, 7);
		printf("%d", teste->ppid);
		gotoxy(101, 8);
		printf("%d", teste->tempo_total);
		gotoxy(101, 9);
		printf("%d", teste->tempo_executado);
		gotoxy(101, 10);
		printf("%d", teste->prioridade);
		gotoxy(68, 39);
		textcolor(15);
	}
	
}

void atualizaUnidadeTempo(int ut)
{
	textcolor(12);
	gotoxy(117, 37);
	printf("  ");
	gotoxy(123, 37);
	printf("%d", ut);
	textcolor(15);
	gotoxy(68, 39);
}

int lF(filaPCB f)
{
	PCB *aux = f.inicio;
	if (aux == NULL)
		return 0;
	int x = 1;
	while (aux != f.fim && aux != NULL)
	{
		x++;
		aux = aux->prox;
	}
	return x;
}


int contarFilhos(PCB *pcb, filaPCB f) {
    int qtde = 0;
    if (f.inicio == NULL) 
		return qtde;
    PCB *aux = f.inicio;
    do {
        if (pcb->pid == aux->ppid) {
            qtde++;
        }
        aux = aux->prox;
    } while (aux != f.fim);

    return qtde;
}

int qtdeFilhos(PCB *pcb, filaPCB p, filaPCB mt, filaPCB hd, filaPCB bf) {
    int qtde = 0;
    qtde += contarFilhos(pcb, p);
    qtde += contarFilhos(pcb, mt);
    qtde += contarFilhos(pcb, hd);
    qtde += contarFilhos(pcb, bf);

    return qtde;
}

void printFila(filaPCB f,int id)
{
	int x,y=8,x2;
	PCB *aux = f.inicio;
	switch(id){
		case 1:
			x = 6;  
			break;
		case 2:
			x = 32;   
			break;
		case 3:
			x = 56;  
			break;
		case 4:
			x = 79;    
			break;
	}
	
	if(aux!=NULL){
		textcolor(9);
		while (aux != f.fim && aux != NULL)
		{
			gotoxy(x,y);printf("%d",aux->pid);
			gotoxy(x+6,y);printf("%d",aux->ppid);
			gotoxy(x+13,y);printf("%d",aux->prioridade);
			y++;
			aux = aux->prox;
		}
		gotoxy(x,y);printf("%d",aux->pid);
		gotoxy(x+6,y);printf("%d",aux->ppid);
		gotoxy(x+13,y);printf("%d",aux->prioridade);
		textcolor(15);
	}
	
	
}

void pf(PCB *pcb, filaPCB f,int *y) {
	PCB *aux = f.inicio;
	if(f.inicio!=NULL){
		do {
			if (pcb->pid == aux->ppid){
				gotoxy(101,*y);printf("%d",aux->pid);
				(*y)+=1;
			}
			aux = aux->prox;
		} while (aux != f.inicio);
	}
    
 
}

void printafilhos(PCB *pcb, filaPCB p, filaPCB mt, filaPCB hd, filaPCB bf) {
	int y = 13;
    gotoxy(101,y);
    textcolor(9);
    pf(pcb, p, &y);
    pf(pcb, mt, &y);
    pf(pcb, hd, &y);
    pf(pcb, bf, &y);
  	textcolor(15);

}

void atualizaPagina(int pronto, int qtdmt, int qtdhd, int qtdfb, int qtdfilho,
	PCB *pcb, filaPCB p, filaPCB mt, filaPCB hd, filaPCB bf)
{
	limpaquadrantes();
	if (pronto <= 0)
	{ 
		desenhaFilaPronto(1);
		textcolor(14);
		gotoxy(9, 6);
		printf("lista vazia!");
		textcolor(2);
	}
	else
	{ 
		textcolor(14);
		gotoxy(4, 6);
		printf("  PID   PPID   PRI");
		desenhaFilaPronto(pronto + 2);
		printFila(p,1);
		textcolor(15);
	}

	if (qtdmt <= 0)
	{ 
		desenhaFilaDispExt(1);
		textcolor(14);
		gotoxy(34, 6);
		printf("lista vazia!!");
		textcolor(2);
	}
	else
	{ 
		textcolor(14);
		gotoxy(30, 6);
		printf("  PID   PPID   PRI");
		desenhaFilaDispExt(qtdmt + 2);
		printFila(mt,2);
		textcolor(15);
	}

	if (qtdhd <= 0)
	{ 
		desenhaFilaHD(1);
		textcolor(14);
		gotoxy(58, 6);
		printf("lista vazia!!");
		textcolor(2);
	}
	else
	{ 
		textcolor(14);
		gotoxy(54, 6);
		printf("  PID   PPID   PRI");
		desenhaFilaHD(qtdhd + 2);
		printFila(hd,3);
		textcolor(15);
	}

	if (qtdfb <= 0)
	{ 
		desenhaFilaBlocFilho(1);
		textcolor(14);
		gotoxy(81, 6);
		printf("lista vazia!");
		textcolor(2);
	}
	else
	{ 
		textcolor(14);
		gotoxy(78, 6);
		printf(" PID   PPID   PRI");
		desenhaFilaBlocFilho(qtdfb + 2);
		printFila(bf,4);
		textcolor(15);
	}

	if (qtdfilho <= 0)
	{ 
		desenhaFilaFilhos(1);
		textcolor(14);
		gotoxy(101, 13);
		printf(" Sem filhos");
	}
	else
	{ 
		desenhaFilaFilhos(qtdfilho);
		printafilhos(pcb,p,mt,hd,bf);
		textcolor(15);
	}
	gotoxy(68, 29);
}

PCB *setatualnulo(void){
	limpastatusatual();
	textcolor(15);
	gotoxy(101, 7);
	printf("SEM");
	gotoxy(101, 8);
	printf("PROCESSO");
	gotoxy(101, 9);
	printf("EM EXEC.");
	gotoxy(68, 39);
	textcolor(15);

	return NULL;
}

