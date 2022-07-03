#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include<time.h>//necessário p/ função time()
#include <stdlib.h>// necessário p/ as funções rand() e srand()

/*
Artigos que me ajudaram:
https://www.includehelp.com/articles/threading-in-c-programming-language-with-gcc-linux.aspx

*/

#define MAXITEMS 10 /*Nro maximo de elementos*/

int fila[MAXITEMS]; /*Elementos da fila*/
pthread_mutex_t    mutex = PTHREAD_MUTEX_INITIALIZER;


int IniciaSessaoCritica()
{
	int  rc;
	int cont = 0;
	while ( (rc = pthread_mutex_lock(&mutex))!=0)
	{	
		usleep(100); /*Aguarda um pouco*/
		cont++;
		printf("Sessao critica não conseguida\n");			
		if (cont>3) break;
	}
	return rc;
	
	
}

int TerminaSessaoCritica()
{
	int  rc;
	int cont = 0;
	while ( (rc = pthread_mutex_unlock(&mutex))!=0)
	{	
		usleep(100); /*Aguarda um pouco*/
		cont++;
		printf("Sessao critica não liberada\n");			
		if (cont>3) break;
	}
	return rc;
	
	
}


/*thread function definition*/
void* threadRecepcao(void* args)
{
	for(int cont = 0;cont<=MAXITEMS;cont++)
	{	
		
		printf("Criando pacote nro %d\n",cont);	
		if(IniciaSessaoCritica()==0)
		{
			int Valor = (rand()% 100);
			fila[cont] = Valor; /*Grava Valor na fila*/
			printf("Registrou na fila[%d] = %d\n",cont,Valor);
			TerminaSessaoCritica();
			usleep(1000);		
		} else {
				printf("Falha na Recepcao nro:\n",cont);
		}
			
	}
	printf("Recepcao terminou atendimento\n");
   
}

pthread_t  Start_Recepcao()
{
	pthread_t  pid	= 0;
	int ret;
	ret=pthread_create(&pid,NULL,&threadRecepcao,NULL);
	
	return pid;
}

pthread_t  Start_Controlador()
{
	pthread_t  pid	= 0;
	int ret;
	
	return pid;
}

pthread_t  Start_Executor()
{
	pthread_t  pid	= 0;
	int ret;
	
	return pid;
}

void StartVetor(){
		for (int cont = 0;cont<=MAXITEMS;cont++)
		{
			fila[cont] = 0;
			printf("Posicao %i zerada\n",cont);
		}
}


void main(void){
	int rstatus = 0;
	srand(time(NULL));
	pthread_t  pidRecepcao = 0;
	pthread_t  pidControlador = 0;
	pthread_t  pidExecutor = 0;
	//mutex = PTHREAD_MUTEX_INITIALIZER;
	
	printf("\nBem vindo ao programa das threads\n");
	printf("Este programa faz parte do artigo:\n");
	printf("http://maurinsoft.com.br/index.php/2022/07/02/threads-para-raiz-parte-1/\n\n");
	printf("Inicializando vetor\n\n");
	StartVetor();
	printf("Iniciando Recepcao\n");
	pidRecepcao = Start_Recepcao();
	printf("Iniciando Controlador\n");
	pidControlador = Start_Controlador();
	printf("Iniciando Executor\n");
	pidExecutor = Start_Executor();
	bool flag = ((pidRecepcao!=0)||(pidControlador!=0)||(pidExecutor!=0));
	while(flag==true){
		printf("Status:");
		//rstatus = pthread_join (thread_idA, &thread_res);

		if(rstatus != 0)
		{
			printf ("Erro ao aguardar finalização do thread A.\n");
			//exit(EXIT_FAILURE);
			//exit(1);
		}
		
		sleep(1);
	}
	
}

