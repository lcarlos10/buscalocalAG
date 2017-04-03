/*****************************************************************************
Código: fact.c
Autor: Pedro Schimit
Data: 17/06/2013
Revisões
Data        Observações
17/06/13    Primeira versão
18/08/13    Adaptado para problemas não quadráticos - Flávio Grassi
*****************************************************************************/

/* includes */
#include "Scheduling_fact.h"
#include <stdio.h>
#include <stdlib.h>

#define _DEBUG_FACT_

/* global variables */
int priority[MACHINE][JOB];
int tempo[JOB][MACHINE];
int sequence[SEQUENCE][MACHINE];

int machines[MACHINE];
int priomachines[MACHINE];

enum{
	START=0,
	END,
};

enum{
	NOPRIORITY=0,
	MACHPRIORITY,
};

struct sequence {
	int iStep;
	int iWaiting;
	int iWaitingFor;
	int itempo;
	int iMach;
	int isActive;
	int isDead;
	int iSequences;
};

struct sequence sSequence[SEQUENCE];

/* functions */

void carregadados();
void checkmachines(int tempostep, int iter, int iPrioMode);
void checkprocesses(int tempostep, int iter, int iPrioMode);

int factivel(int *vPriorities, int *vSequences, int iPrioMode, int *iTEMPO){
//Matriz de tempos nos jobs, NA ORDEM DE MÁQUINAS DE ACORDO COM A MATRIZ DE ROTAS A SER PASSADA NO MAIN

    int JT[JOB][MACHINE];

    int intElemento = 0;
    int iJob, iMachine;

    // ALIMENTA MATRIZ JT COM OS TEMPOS DO VETOR T
    for(iJob=0; iJob < JOB; iJob++) {
        for(iMachine=0; iMachine < MACHINE; iMachine++) {
            JT[iJob][iMachine]=iTEMPO[intElemento++];

        }
    }

	int i,j; //,k;
	int iter; //, mach;

    for(i=0;i<JOB;i++)
		for(j=0;j<MACHINE;j++)
			tempo[i][j] = JT[i][j];

    for(i=0;i<MACHINE;i++)
		for(j=0;j<JOB;j++)
			priority[i][j] = vPriorities[i*JOB+j];

    for(i=0;i<SEQUENCE;i++)
		for(j=0;j<MACHINE;j++)
			sequence[i][j]=vSequences[i*MACHINE+j];


#ifdef _DEBUG_FACT_
/*	printf("\n tempos\n");
	for(i=0;i<JOB;i++){
		for(j=0;j<MACHINE;j++)
		printf("%d\t", tempo[i][j]);
		printf("\n");
	}

	printf("\n\n Priorities\n");
	for(i=0;i<MACHINE;i++){
		for(j=0;j<JOB;j++)
			printf("%d\t", priority[i][j]);
		printf("\n");
	}

	printf("\n\n Sequences\n");
	for(i=0;i<SEQUENCE;i++){
		for(j=0;j<MACHINE;j++)
			printf("%d\t", sequence[i][j]);
		printf("\n");
	}*/
#endif //_DEBUG_FACT_

	for(i=0;i<MACHINE;i++){
		machines[i]=0;
		priomachines[i]=1;
	}

	for(i=0;i<SEQUENCE;i++){
		sSequence[i].itempo = 0;
		sSequence[i].iStep = 0;
		sSequence[i].iWaiting = 1;
		sSequence[i].iWaitingFor = sequence[i][sSequence[i].iStep]-1;
		sSequence[i].isActive = 0;
		sSequence[i].isDead = 0;
		sSequence[i].iSequences = MACHINE;
	}

//    ofstream fileSch;
//    fileSch.open(ArqSch.c_str(), ios::app);

	for(iter=0;iter<ITER;iter++){

		checkmachines(START, iter, iPrioMode);
		checkprocesses(START, iter, iPrioMode);
		checkmachines(END, iter, iPrioMode);
		checkprocesses(END, iter, iPrioMode);
      j=0;
      for(i=0;i<SEQUENCE;i++)
         if(!sSequence[i].isDead) j=1;
	   if(!j) break;
	}

  //  fileHst.close();

	if(iter==ITER) {
        #ifdef _DEBUG_FACT_
         //   printf("\n *** NAO FACTIVEL ***");
            // getchar();
        #endif //_DEBUG_FACT_

        return iter;
	} else {
        #ifdef _DEBUG_FACT_
            printf("\n FACTIVEL ");
			printf("%d\t", iter);
        #endif //_DEBUG_FACT_

	    return iter+1;
	}

}

void checkmachines(int tempostep, int iter, int iPrioMode){

	int i, j;

	for(i=0;i<MACHINE;i++){

		if(iPrioMode==NOPRIORITY){
			if(!machines[i]){
				for(j=0;j<SEQUENCE;j++){
					if(!machines[i] && sSequence[j].isActive==0
						&& sSequence[j].iWaitingFor==i && !sSequence[j].isDead){
						sSequence[j].itempo = 0;
						sSequence[j].iMach = i;
						sSequence[j].isActive = 1;
#ifdef _DEBUG_FACT_
                        //Luis
						//printf("\n Sequence %d started in machine %d at %d", j+1, i+1, iter);
#endif //_DEBUG_FACT_
						machines[i]=1;
					}
				}
			}
		}

		if(iPrioMode==MACHPRIORITY){
			if(!machines[i]){
				for(j=0;j<JOB;j++) if(priority[i][j]==priomachines[i]) break;

				if(!machines[i] && sSequence[j].isActive==0
					&& sSequence[j].iWaitingFor==i && !sSequence[j].isDead){

					priomachines[i]=priomachines[i]+1;
					sSequence[j].itempo = 0;
					sSequence[j].iMach = i;
					sSequence[j].isActive = 1;
#ifdef _DEBUG_FACT_
                    //Luis
					//printf("\n Sequence %d started in machine %d at %d", j+1, i+1, iter);
#endif //_DEBUG_FACT_
					machines[i]=1;
				}

			}
		}

	}
}

void checkprocesses(int tempostep, int iter, int iPrioMode){

	int j;

	for(j=0;j<SEQUENCE;j++){
		if(sSequence[j].isActive){
			if(sSequence[j].itempo>=tempo[j][sSequence[j].iStep]){
				sSequence[j].isActive=0;
#ifdef _DEBUG_FACT_
				//printf("\n Sequence %d finished in machine %d at %d", j+1, sSequence[j].iMach+1, iter+1);
#endif //_DEBUG_FACT_
				sSequence[j].iStep++;
				sSequence[j].iWaiting=1;
				sSequence[j].iWaitingFor=sequence[j][sSequence[j].iStep]-1;
#ifdef _DEBUG_FACT_
            //printf("\n Sequence %d is at step %d waiting for %d", j+1, sSequence[j].iStep, sequence[j][sSequence[j].iStep]);
#endif //_DEBUG_FACT_
				machines[sSequence[j].iMach]=0;
				//}
			}
			if(sSequence[j].iStep>=sSequence[j].iSequences){
				sSequence[j].isDead=1;
#ifdef _DEBUG_FACT_
				//printf("\n Sequence %d is dead", j+1);
#endif //_DEBUG_FACT_
				machines[sSequence[j].iMach]=0;
			}
			if(tempostep==START)
				sSequence[j].itempo++;

		}
	}

}
