#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <syscall.h>
#include <sys/prctl.h>
#include <sys/file.h>
#include <stdlib.h>
#include <stdarg.h>
#include<time.h>//necessário p/ função time()
#include <stdlib.h>// necessário p/ as funções rand() e srand()

/*
Artigos que me ajudaram:
https://www.includehelp.com/articles/threading-in-c-programming-language-with-gcc-linux.aspx

*/

#define MAXITEMS 10 /*Nro maximo de elementos*/

int fila[MAXITEMS]; /*Elementos da fila*/
int executada[MAXITEMS]; /*Elementos da fila*/
int       subthread_lwp;
pthread_mutex_t    mutex = PTHREAD_MUTEX_INITIALIZER;
bool flgTerminou = false;
bool flgTerminouRecepcao = false;
bool flgTerminouExecucao = false;

#define UNIQUE_NAME "thread"

bool thread_exists (pthread_t thread_id)
{
    char path[100];
    char thread_name[16];
    FILE *fp;
    bool  thread_exists = false;

    // If the /proc/<pid>/task/<lwp>/comm file exists and it's contents match the "unique name" the
    // thread exists, and it's the original thread (TID has NOT been reused).

    sprintf(path, "/proc/%d/task/%d/comm", getpid(), subthread_lwp);

    fp = fopen(path, "r");

    if( fp != NULL ) {

        fgets(thread_name, 16, fp);
        fclose(fp);

        // Need to trim off the newline
        thread_name[strlen(thread_name)-1] = '\0';

        if( strcmp(UNIQUE_NAME, thread_name) == 0 ) {
            thread_exists = true;
        }
    }

    if( thread_exists ) {
        printf("thread exists\n");
    } else {
        printf("thread does NOT exist\n");
    }

    return thread_exists;
}

int IniciaSessaoCritica()
{
	int  rc;
	int cont = 0;
	while ( (rc = pthread_mutex_lock(&mutex))!=0)
	{	
		//usleep(10); /*Aguarda um pouco*/
		for (int cont1=0;cont1<=1000;cont1++);
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
		//usleep(100); /*Aguarda um pouco*/
		cont++;
		printf("Sessao critica não liberada\n");			
		if (cont>3) break;
	}
	return rc;
	
	
}


/*thread function definition*/
void* threadRecepcao(void* args)
{
	for(int cont = 0;cont<=MAXITEMS-1;cont++)
	{	
		
		//printf("Criando pacote nro %d\n",cont);	
		if(IniciaSessaoCritica()==0)
		{
			int Valor = (rand()% 100);
			if (Valor == 0) Valor = (rand()% 100);
			fila[cont] = Valor; /*Grava Valor na fila*/
			printf("Registrou na fila[%d] = %d\n",cont,Valor);
			TerminaSessaoCritica();
			//usleep(1000);		 
		} else {
				printf("Falha na Recepcao nro:\n",cont);
		}
			
	}
	printf("Recepcao terminou atendimento\n");
	flgTerminouRecepcao = true; /*Indica que terminou*/
   
}

void ordenacao()
{
int i, x;
bool flgordenado = false;
while((!flgordenado)&&(!flgTerminou))
{
	//flgordenado = true;
	for (i=0; i<=MAXITEMS-1; i++)
	{
		if ((fila[i]!=0)&&(fila[i+1]!=0))
		{
		  if(fila[i]>fila[i+1]) 
		  {
		    x = fila[i];		  
            fila[i] = fila[i+1];
		    fila[i+1] = x;
		    flgordenado = false;
		    //printf("Ordenando os pacote nro %d\n",i);
		  }
		}	else {
			//printf("Posicao vazia %d\n",i);
			flgordenado = false;
		}					
    }
	
    
  }
}

/*thread function definition*/
void* threadControlador(void* args)
{
  int oldValue, newValue;
  int flgOrdenado = false;
  printf("Iniciou o controlador\n");
  while(!flgTerminou) /*Faz enquanto nao terminar e nao ordenao*/
  { 
	if(IniciaSessaoCritica()==0)
	{
		ordenacao();
		
		TerminaSessaoCritica();
		//usleep(200);
		
	}
  }
  	
  printf("Terminou ordenação de todos os itens\n");
}

/*thread function definition*/
void* threadExecutor(void* args)
{
  int oldValue, newValue;
  bool flgExecutado = false;
  printf("Iniciou o controlador\n");
  int posicao = 0;
  //while(!flgExecutado)
  while((!flgExecutado)&&(!flgTerminou))
  {
    //printf("Pesquisando Posicao %d\n",posicao);	

	//printf("Entrou na sessao critica\n");  
	if  (fila[posicao]==0) /*Fila nao foi preenchida*/
	{
		printf("posicao vazia %d\n",posicao);
		//posicao = posicao;
	
	} else 
	{
	   executada[posicao] = fila[posicao];
	   printf("Executou[%d] = %d\n",posicao,fila[posicao]);
	   if(posicao==MAXITEMS-1)
	   {
		   printf("Chegou ao fim\n ");
		   flgExecutado= true; /*Finaliza executor*/
	   } else 
	   {
		   posicao ++;
	   }
		   
	}
	//printf("terminou while\n");  
  }
  
  flgTerminouExecucao = true;
  	
  printf("Terminou ordenação de todos os itens\n");

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
	ret=pthread_create(&pid,NULL,&threadControlador,NULL);
	
	return pid;
}

pthread_t  Start_Executor()
{
	pthread_t  pid	= 0;
	int ret;
	ret=pthread_create(&pid,NULL,&threadExecutor,NULL);
	
	return pid;
}

void StartVetor(){
		for (int cont = 0;cont<=MAXITEMS-1;cont++)
		{
			fila[cont] = 0;
			executada[cont]=0;
			//printf("Posicao %i zerada\n",cont);
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
	//bool flag = ((pidRecepcao!=0)||(pidControlador!=0)||(pidExecutor!=0));
	flgTerminou = false;
	while(!flgTerminou){
		printf("Status:");

		if(rstatus != 0)
		{
			printf ("Erro ao aguardar finalização do thread A.\n");

		}

		flgTerminou = flgTerminouRecepcao && flgTerminouExecucao;
		sleep(1);
	}
	printf("\n\nFila ordenada:");
	for(int cont = 0;cont<=MAXITEMS-1;cont++)
	{
	  printf("%i ",fila[cont]);
	}
	printf("\n\n");
	
	printf("\n\nFila executada:");
	for(int cont = 0;cont<=MAXITEMS-1;cont++)
	{
	  printf("%i ",executada[cont]);
	}
	printf("\n\n");
	
}

