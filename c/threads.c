#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/*
Artigos que me ajudaram:
https://www.includehelp.com/articles/threading-in-c-programming-language-with-gcc-linux.aspx

*/

#define MAXITEMS 10 /*Nro maximo de elementos*/

int file[1000]; /*Elementos da fila*/


/*thread function definition*/
void* threadRecepcao(void* args)
{
	for(int cont = 0;cont<=MAXITEMS;cont++)
	{	
		
		printf("Criando pacote nro %d",cont);	
		sleep(1);		
	}
   
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


void main(void){
	
	pthread_t  pidRecepcao = 0;
	pthread_t  pidControlador = 0;
	pthread_t  pidExecutor = 0;
	
	printf("\nBem vindo ao programa das threads\n");
	printf("Este programa faz parte do artigo:\n");
	printf("http://maurinsoft.com.br/index.php/2022/07/02/threads-para-raiz-parte-1/\n\n");
	printf("Iniciando Recepcao\n");
	pidRecepcao = Start_Recepcao();
	printf("Iniciando Controlador\n");
	pidControlador = Start_Controlador();
	printf("Iniciando Executor\n");
	pidExecutor = Start_Executor();
	bool flag = ((pidRecepcao!=0)||(pidControlador!=0)||(pidExecutor!=0));
	while(flag){
		printf("Status:");
		
		sleep(1);
	}
}
