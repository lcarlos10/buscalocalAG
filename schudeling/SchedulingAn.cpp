/********************************************************************************
*   Researchers involved:													    *
*   Fabio Henrique Pereira													    *
*   Flavio Grassi															    *
*   Marilda Fatima de Souza da Silva										    *
*   																		    *
*   Contributors:															    *
*   Pedro Henrique Triguis													    *
*   Valdemar Modolo Junior													    *
*   Valmir Ferreira da Cruz													    *
*   																		    *
*   Affiliation:															    *
*   Post-Graduation Program of Industrial Engineering, 						    *
*   Universidade Nove de Julho, Sao Paulo, SP, Brazil.						    *
*																			    *
*   Short Description:														    *
*   Program that uses Genetic Algorithms to find the optimal schedule 		    *
*   sequence for a job shop environment (JSSP). 							    *
*   It uses a develop methodology named "DSGA - Dynamic Seed Genetic 		    *
*   Algorithm", and runs using GAlib - a Genetic Algorithm library developed    *
*   in C++ by Matthew Wall, from the Massachusetts Institute of Technology.     *
*********************************************************************************
*   Adaptações realizadas por Valmir Ferreira da Cruz:                          *
*																			    *
*       Unificação de todos os programas em apenas um. É preciso compilar uma   *
*   versão para cada grupo de problemas de acordo com a quantidade de jobs e    *
*   máquinas. Para isso é preciso deixar declarado, em Scheduling_fact.h apenas *
*   uma das diretivas:                                                          *
*                                                                               *
*   Grupo     Problema                  Diretiva                                *
*   ------- -------------------------   -----------                             *
*   01 a 05 ( 05 máquinas X 10 jobs )   PROGRAMA_01                             *
*   06 a 10 ( 05 máquinas X 15 jobs )   PROGRAMA_06                             *
*   11 a 15 ( 05 máquinas X 20 jobs )   PROGRAMA_11                             *
*   16 a 20 ( 10 máquinas X 10 jobs )   PROGRAMA_16                             *
*   21 a 25 ( 10 máquinas X 15 jobs )   PROGRAMA_21                             *
*                                                                               *
*   Sintaxe para utilização a partir da linha de comando (prompt):              *
*                                                                               *
*   Scheduling <Abordagem> <Problema> <Qtde Iterações> <Num. Execução>          *
*                                                                               *
*   A abordagem pode ser:                                                       *
*       1=Não Factível;                                                         *
*       2=FIFO;                                                                 *
*       3=FIFO + SPT;                                                           *
*       4=FIFO + LPT;                                                           *
*       5=NEH                                                                   *
*                                                                               *
*       O <Problema> pode ser entre 1 e 25, porém, depende de como foi compi-   *
*   lado. Veja quadro acima com os problemas contemplados em cada tipo de compi-*
*   lação.                                                                      *
*                                                                               *
*       A <Qtde Iterações> se refere ao número de laços externos.               *
*                                                                               *
*       O <Num. Execução> é apenas um número sequencial para gravar no histórico*
*                                                                               *
*   Exemplo de chamada no arquivo de lote (.bat) :                              *
*       @for /L %%i in (1,1,20) do Scheduling 5 1 200 %%i                       *
*                                                                               *
*   O exemplo acima cria um laço de 20 execuções passando como parâmetros:      *
*       Abordagem 5 (NEH); Problema LA01; 200 laços externos; sequencial gerado *
*   pelo DOS.
********************************************************************************/

   // 1o parametro indica a abordagem a ser explorada
    if (argc > 1) {
        intAbordagem = atoi(argv[1]);
    } else {
        intAbordagem = 2; // 1=Não Factível; 2=FIFO; 3=FIFO + SPT; 4=FIFO + LPT; 5=NEH
    }

    // 2o parametro indica o problema LAxx
    if (argc > 2) {
        strProblema = argv[2];
        intProblema = atoi(argv[2]);

    } else {
        #ifdef PROGRAMA_01
            intProblema = 1; // LA01
            strProblema = "1";

        #endif

        #ifdef PROGRAMA_06
            intProblema = 6;
            strProblema = "6";

        #endif

        #ifdef PROGRAMA_11
            intProblema = 11;
            strProblema = "11";

        #endif

        #ifdef PROGRAMA_16
            intProblema = 16;
            strProblema = "16";

        #endif

        #ifdef PROGRAMA_21
            intProblema = 21;
            strProblema = "21";

        #endif
    }

    // 3o parametro indica a quantidade de iterações
    if (argc > 3) {
        outIter = atoi(argv[3]);
    }

    if (argc > 4 ){
        intInstancia = atoi(argv[4]);
        strInstancia = argv[4];

    }


//Includes
#include "scheduling_fact.h"
#include <time.h>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <ga.h>
#include <ga/GASimpleGA.h>  // we're going to use the simple GA
#include <ga/GA2DBinStrGenome.h> // and the 2D binary string genome
#include <ga/GASStateGA.h> // and Steady State GA
#include <shlobj.h>//for SHGetFolderPath function


// #include <stdio.h>      // optional
// #include <stdlib.h>     // optional
// #include <vector>       // optional
// #include <string>       // optional
// #include <cstring>      // optional
// #include <fstream>      // optional
// #include <math.h>       // optional
// #include <iostream>     // optional
// #include <cstdlib>      // optional
// #include <std_stream.h>  // optional
// #include <stdio.h>       // optional
// #include <iostream>      // optional
// #include <fstream>       // optional
// #include <math.h>         // optional
// #include <windows.h>// for SHGetFolderPath function
// #include <garandom.h>     // optional


//Defines
// #define cout STD_COUT     // optional
// #define ostream STD_OSTREAM      // optional

using namespace std;

//User-defined function declarations
int factivel(int *vPriorities, int *vSequences, int iPrioMode);
void carregadados();
void checkmachines(int timestep, int iter, int iPrioMode);
void checkprocesses(int timestep, int iter, int iPrioMode);
double geraVarNormal(double media, double desvio);
float Objective(GAGenome &);
int * gera_sequencia();
void setup();
void localSearch();
void localSearch(const GAStatistics &);
void cpBasedSeedGA();
void findCriticalPath(vector<int> s);
void  changeSeed(const GAStatistics &);
//void changeSeed(const GAStatistics &);
string desktopFolder ();
string ProximoArquivo( string pNomeArq, string pExtArq );

int projectDuration(vector<int> S);
//Global variable declarations

std::stringstream ssResult;

int R[MACHINE*JOB];
int T[JOB*MACHINE];
int vetBestSequence[JOB*MACHINE];

string Con;
string Bsi;
string Csv="\\Route";
string His;
string Sch;

string PastaDesktop=desktopFolder() + "\\Scheduling" ;
string ArqCon = PastaDesktop + Con;
string ArqBsi = PastaDesktop + Bsi;
string ArqCsv = PastaDesktop + Csv;
string ArqHis;
string ArqSch = PastaDesktop + Sch;

int intMakeSpanOtimo;
int intProblema; // LA?
int intAbordagem; // 1 = Semente não-factível; 2 = FIFO; 3 = FIFO + SPT; 4 = FIFO + LPT
int intInstancia = 1; // Número da execução
int outIter = 200; // quantidade de iterações externas
string strSemente;

int score_ant = ITER;
bool primeira_geracao_s=true, primeira_geracao;
int *s;
double start = clock();
int horas, minutos, segundos, horas_seg=3600;
GAGenome g_temp;
int cont;
int nBestIndividuals = 1;
int newBestIndividuals = 15;
vector <int> score_rodada_anterior(nBestIndividuals);
vector <int> bestSeedsIndex(nBestIndividuals);
vector <int> scoreTopList(nBestIndividuals);
std::vector<int>::iterator it;
int bestCurrent = ITER;
int seedIndexCurrent = 0;
//vector<int> firstSeed(s, s+MACHINE*JOB);
vector <vector <int> > S(nBestIndividuals);//, vector<int>(MACHINE*JOB));
vector<int> currentCriticalPath;
bool STOP = false;
bool cpBasedSeed = false;

// VARIÁVEIS DO HISTÓRICO
int intSolucoesFactiveis;
int intSolucoesNaoFactiveis;
// string strSementeInicial = "NÃO FACTÍVEL";

//User-defined function
int ArquivoExiste( const char*);
int * OrderVectorFIFO( int pVetor[], int pOrder);
int * OrderVectorNEH_CP( int pVetor[], int pOrder);

void * OrderVectorNEH( int pVetor[])
{
    if (intProblema == 1) {
		pVetor[  0]= 10; pVetor[  1]=  7; pVetor[  2]=  9; pVetor[  3]= 10; pVetor[  4]=  3;
		pVetor[  5]=  7; pVetor[  6]=  4; pVetor[  7]=  7; pVetor[  8]=  4; pVetor[  9]=  1;
		pVetor[ 10]=  6; pVetor[ 11]= 10; pVetor[ 12]=  4; pVetor[ 13]=  7; pVetor[ 14]=  8;
		pVetor[ 15]=  5; pVetor[ 16]=  2; pVetor[ 17]=  5; pVetor[ 18]=  6; pVetor[ 19]=  4;
		pVetor[ 20]=  8; pVetor[ 21]=  6; pVetor[ 22]=  6; pVetor[ 23]=  1; pVetor[ 24]=  6;
		pVetor[ 25]=  4; pVetor[ 26]=  9; pVetor[ 27]= 10; pVetor[ 28]=  2; pVetor[ 29]=  7;
		pVetor[ 30]=  1; pVetor[ 31]=  3; pVetor[ 32]=  8; pVetor[ 33]=  3; pVetor[ 34]= 10;
		pVetor[ 35]=  9; pVetor[ 36]=  8; pVetor[ 37]=  1; pVetor[ 38]=  5; pVetor[ 39]=  5;
		pVetor[ 40]=  2; pVetor[ 41]=  1; pVetor[ 42]=  3; pVetor[ 43]=  8; pVetor[ 44]=  9;
		pVetor[ 45]=  3; pVetor[ 46]=  5; pVetor[ 47]=  2; pVetor[ 48]=  9; pVetor[ 49]=  2;

    } else if (intProblema == 2) {
		pVetor[  0]=  6; pVetor[  1]=  4; pVetor[  2]=  8; pVetor[  3]=  3; pVetor[  4]=  4;
		pVetor[  5]=  8; pVetor[  6]=  3; pVetor[  7]=  4; pVetor[  8]=  4; pVetor[  9]=  8;
		pVetor[ 10]=  7; pVetor[ 11]=  6; pVetor[ 12]=  6; pVetor[ 13]=  1; pVetor[ 14]=  1;
		pVetor[ 15]=  9; pVetor[ 16]=  8; pVetor[ 17]=  7; pVetor[ 18]=  2; pVetor[ 19]= 10;
		pVetor[ 20]= 10; pVetor[ 21]=  5; pVetor[ 22]=  9; pVetor[ 23]= 10; pVetor[ 24]=  6;
		pVetor[ 25]=  3; pVetor[ 26]=  9; pVetor[ 27]=  2; pVetor[ 28]=  9; pVetor[ 29]=  3;
		pVetor[ 30]=  4; pVetor[ 31]= 10; pVetor[ 32]=  3; pVetor[ 33]=  8; pVetor[ 34]=  7;
		pVetor[ 35]=  5; pVetor[ 36]=  1; pVetor[ 37]= 10; pVetor[ 38]=  5; pVetor[ 39]=  5;
		pVetor[ 40]=  2; pVetor[ 41]=  2; pVetor[ 42]=  1; pVetor[ 43]=  6; pVetor[ 44]=  2;
		pVetor[ 45]=  1; pVetor[ 46]=  7; pVetor[ 47]=  5; pVetor[ 48]=  7; pVetor[ 49]=  9;

    } else if (intProblema == 3) {
		pVetor[  0]=  7; pVetor[  1]=  6; pVetor[  2]=  6; pVetor[  3]=  5; pVetor[  4]=  1;
		pVetor[  5]=  1; pVetor[  6]=  8; pVetor[  7]=  4; pVetor[  8]=  4; pVetor[  9]=  6;
		pVetor[ 10]=  5; pVetor[ 11]= 10; pVetor[ 12]=  9; pVetor[ 13]=  3; pVetor[ 14]=  5;
		pVetor[ 15]=  6; pVetor[ 16]=  7; pVetor[ 17]=  8; pVetor[ 18]=  9; pVetor[ 19]=  9;
		pVetor[ 20]=  4; pVetor[ 21]=  4; pVetor[ 22]=  1; pVetor[ 23]=  2; pVetor[ 24]=  2;
		pVetor[ 25]=  3; pVetor[ 26]=  5; pVetor[ 27]=  3; pVetor[ 28]=  1; pVetor[ 29]= 10;
		pVetor[ 30]=  8; pVetor[ 31]=  3; pVetor[ 32]=  5; pVetor[ 33]=  7; pVetor[ 34]=  4;
		pVetor[ 35]= 10; pVetor[ 36]=  9; pVetor[ 37]=  2; pVetor[ 38]=  6; pVetor[ 39]=  8;
		pVetor[ 40]=  2; pVetor[ 41]=  2; pVetor[ 42]=  7; pVetor[ 43]=  8; pVetor[ 44]=  7;
		pVetor[ 45]=  9; pVetor[ 46]=  1; pVetor[ 47]= 10; pVetor[ 48]= 10; pVetor[ 49]=  3;

    } else if (intProblema == 4) {
		pVetor[  0]= 10; pVetor[  1]=  7; pVetor[  2]=  4; pVetor[  3]=  1; pVetor[  4]=  8;
		pVetor[  5]=  2; pVetor[  6]=  8; pVetor[  7]=  1; pVetor[  8]=  9; pVetor[  9]=  1;
		pVetor[ 10]=  6; pVetor[ 11]=  4; pVetor[ 12]=  7; pVetor[ 13]=  6; pVetor[ 14]=  5;
		pVetor[ 15]=  3; pVetor[ 16]=  6; pVetor[ 17]=  9; pVetor[ 18]=  8; pVetor[ 19]=  6;
		pVetor[ 20]=  8; pVetor[ 21]=  9; pVetor[ 22]= 10; pVetor[ 23]= 10; pVetor[ 24]=  2;
		pVetor[ 25]=  9; pVetor[ 26]=  5; pVetor[ 27]=  8; pVetor[ 28]=  7; pVetor[ 29]=  4;
		pVetor[ 30]=  7; pVetor[ 31]=  2; pVetor[ 32]=  5; pVetor[ 33]=  3; pVetor[ 34]=  3;
		pVetor[ 35]=  5; pVetor[ 36]= 10; pVetor[ 37]=  2; pVetor[ 38]=  2; pVetor[ 39]=  7;
		pVetor[ 40]=  1; pVetor[ 41]=  3; pVetor[ 42]=  3; pVetor[ 43]=  4; pVetor[ 44]=  9;
		pVetor[ 45]=  4; pVetor[ 46]=  1; pVetor[ 47]=  6; pVetor[ 48]=  5; pVetor[ 49]= 10;

    } else if (intProblema == 5) {
		pVetor[  0]=  3; pVetor[  1]=  6; pVetor[  2]=  8; pVetor[  3]=  9; pVetor[  4]=  1;
		pVetor[  5]= 10; pVetor[  6]=  1; pVetor[  7]=  6; pVetor[  8]=  7; pVetor[  9]=  3;
		pVetor[ 10]=  1; pVetor[ 11]=  8; pVetor[ 12]=  5; pVetor[ 13]=  1; pVetor[ 14]=  4;
		pVetor[ 15]=  4; pVetor[ 16]=  2; pVetor[ 17]=  1; pVetor[ 18]=  2; pVetor[ 19]=  7;
		pVetor[ 20]=  7; pVetor[ 21]=  3; pVetor[ 22]= 10; pVetor[ 23]=  8; pVetor[ 24]= 10;
		pVetor[ 25]=  2; pVetor[ 26]=  9; pVetor[ 27]=  9; pVetor[ 28]=  6; pVetor[ 29]=  5;
		pVetor[ 30]=  9; pVetor[ 31]=  7; pVetor[ 32]=  2; pVetor[ 33]=  5; pVetor[ 34]=  8;
		pVetor[ 35]=  6; pVetor[ 36]=  4; pVetor[ 37]=  4; pVetor[ 38]=  3; pVetor[ 39]=  9;
		pVetor[ 40]=  8; pVetor[ 41]=  5; pVetor[ 42]=  3; pVetor[ 43]=  4; pVetor[ 44]=  2;
		pVetor[ 45]=  5; pVetor[ 46]= 10; pVetor[ 47]=  7; pVetor[ 48]= 10; pVetor[ 49]=  6;

    } else if (intProblema == 6) {
		pVetor[  0]= 10; pVetor[  1]= 13; pVetor[  2]=  9; pVetor[  3]= 10; pVetor[  4]=  3;
		pVetor[  5]= 11; pVetor[  6]=  7; pVetor[  7]= 12; pVetor[  8]=  4; pVetor[  9]=  1;
		pVetor[ 10]=  7; pVetor[ 11]=  4; pVetor[ 12]=  7; pVetor[ 13]= 11; pVetor[ 14]=  8;
		pVetor[ 15]=  6; pVetor[ 16]= 10; pVetor[ 17]=  4; pVetor[ 18]=  7; pVetor[ 19]=  4;
		pVetor[ 20]= 15; pVetor[ 21]=  2; pVetor[ 22]=  5; pVetor[ 23]=  6; pVetor[ 24]=  6;
		pVetor[ 25]=  5; pVetor[ 26]=  6; pVetor[ 27]= 13; pVetor[ 28]=  1; pVetor[ 29]=  7;
		pVetor[ 30]= 12; pVetor[ 31]= 14; pVetor[ 32]=  6; pVetor[ 33]=  2; pVetor[ 34]= 10;
		pVetor[ 35]=  8; pVetor[ 36]=  9; pVetor[ 37]= 10; pVetor[ 38]= 14; pVetor[ 39]= 13;
		pVetor[ 40]=  4; pVetor[ 41]= 15; pVetor[ 42]= 15; pVetor[ 43]= 15; pVetor[ 44]= 14;
		pVetor[ 45]=  1; pVetor[ 46]=  3; pVetor[ 47]=  8; pVetor[ 48]=  3; pVetor[ 49]=  5;
		pVetor[ 50]= 13; pVetor[ 51]=  8; pVetor[ 52]= 11; pVetor[ 53]= 12; pVetor[ 54]= 11;
		pVetor[ 55]=  9; pVetor[ 56]=  1; pVetor[ 57]=  1; pVetor[ 58]=  5; pVetor[ 59]=  9;
		pVetor[ 60]= 14; pVetor[ 61]=  5; pVetor[ 62]=  3; pVetor[ 63]=  8; pVetor[ 64]=  2;
		pVetor[ 65]=  2; pVetor[ 66]= 12; pVetor[ 67]= 14; pVetor[ 68]=  9; pVetor[ 69]= 12;
		pVetor[ 70]=  3; pVetor[ 71]= 11; pVetor[ 72]=  2; pVetor[ 73]= 13; pVetor[ 74]= 15;

    } else if (intProblema == 7) {
		pVetor[  0]=  6; pVetor[  1]=  9; pVetor[  2]=  9; pVetor[  3]=  1; pVetor[  4]=  8;
		pVetor[  5]=  9; pVetor[  6]=  1; pVetor[  7]= 14; pVetor[  8]=  2; pVetor[  9]= 11;
		pVetor[ 10]= 11; pVetor[ 11]= 10; pVetor[ 12]= 10; pVetor[ 13]= 15; pVetor[ 14]=  3;
		pVetor[ 15]= 15; pVetor[ 16]= 14; pVetor[ 17]=  3; pVetor[ 18]=  6; pVetor[ 19]=  1;
		pVetor[ 20]=  2; pVetor[ 21]= 15; pVetor[ 22]=  2; pVetor[ 23]=  9; pVetor[ 24]= 15;
		pVetor[ 25]=  8; pVetor[ 26]=  2; pVetor[ 27]=  7; pVetor[ 28]=  4; pVetor[ 29]=  9;
		pVetor[ 30]=  5; pVetor[ 31]= 13; pVetor[ 32]=  8; pVetor[ 33]= 14; pVetor[ 34]=  4;
		pVetor[ 35]=  7; pVetor[ 36]=  5; pVetor[ 37]=  4; pVetor[ 38]= 13; pVetor[ 39]= 12;
		pVetor[ 40]= 14; pVetor[ 41]=  7; pVetor[ 42]= 11; pVetor[ 43]=  7; pVetor[ 44]=  6;
		pVetor[ 45]= 13; pVetor[ 46]=  4; pVetor[ 47]=  6; pVetor[ 48]= 12; pVetor[ 49]= 14;
		pVetor[ 50]=  1; pVetor[ 51]=  3; pVetor[ 52]= 13; pVetor[ 53]= 11; pVetor[ 54]=  2;
		pVetor[ 55]=  4; pVetor[ 56]= 12; pVetor[ 57]= 15; pVetor[ 58]=  3; pVetor[ 59]=  7;
		pVetor[ 60]=  3; pVetor[ 61]= 11; pVetor[ 62]= 12; pVetor[ 63]=  8; pVetor[ 64]= 13;
		pVetor[ 65]= 10; pVetor[ 66]=  8; pVetor[ 67]=  5; pVetor[ 68]= 10; pVetor[ 69]= 10;
		pVetor[ 70]= 12; pVetor[ 71]=  6; pVetor[ 72]=  1; pVetor[ 73]=  5; pVetor[ 74]=  5;

    } else if (intProblema == 8) {
		pVetor[  0]= 10; pVetor[  1]=  7; pVetor[  2]=  4; pVetor[  3]= 13; pVetor[  4]=  8;
		pVetor[  5]=  2; pVetor[  6]=  8; pVetor[  7]= 15; pVetor[  8]=  1; pVetor[  9]=  1;
		pVetor[ 10]=  6; pVetor[ 11]=  4; pVetor[ 12]=  1; pVetor[ 13]=  9; pVetor[ 14]=  5;
		pVetor[ 15]=  3; pVetor[ 16]= 12; pVetor[ 17]= 12; pVetor[ 18]=  6; pVetor[ 19]=  6;
		pVetor[ 20]= 15; pVetor[ 21]= 14; pVetor[ 22]=  7; pVetor[ 23]= 15; pVetor[ 24]= 15;
		pVetor[ 25]=  8; pVetor[ 26]=  6; pVetor[ 27]=  9; pVetor[ 28]=  8; pVetor[ 29]= 11;
		pVetor[ 30]=  9; pVetor[ 31]=  9; pVetor[ 32]= 10; pVetor[ 33]= 12; pVetor[ 34]= 12;
		pVetor[ 35]=  7; pVetor[ 36]= 11; pVetor[ 37]=  8; pVetor[ 38]= 10; pVetor[ 39]=  2;
		pVetor[ 40]= 13; pVetor[ 41]=  5; pVetor[ 42]=  5; pVetor[ 43]= 11; pVetor[ 44]=  4;
		pVetor[ 45]= 11; pVetor[ 46]=  2; pVetor[ 47]= 13; pVetor[ 48]= 14; pVetor[ 49]= 13;
		pVetor[ 50]= 12; pVetor[ 51]= 10; pVetor[ 52]= 11; pVetor[ 53]=  7; pVetor[ 54]= 14;
		pVetor[ 55]= 14; pVetor[ 56]=  3; pVetor[ 57]=  2; pVetor[ 58]=  3; pVetor[ 59]=  3;
		pVetor[ 60]=  5; pVetor[ 61]= 15; pVetor[ 62]=  3; pVetor[ 63]=  2; pVetor[ 64]=  7;
		pVetor[ 65]=  1; pVetor[ 66]=  1; pVetor[ 67]=  6; pVetor[ 68]=  4; pVetor[ 69]=  9;
		pVetor[ 70]=  4; pVetor[ 71]= 13; pVetor[ 72]= 14; pVetor[ 73]=  5; pVetor[ 74]= 10;

    } else if (intProblema == 9) {
		pVetor[  0]=  8; pVetor[  1]= 13; pVetor[  2]=  9; pVetor[  3]=  8; pVetor[  4]= 12;
		pVetor[  5]=  5; pVetor[  6]=  8; pVetor[  7]=  1; pVetor[  8]=  9; pVetor[  9]= 14;
		pVetor[ 10]= 13; pVetor[ 11]= 11; pVetor[ 12]= 13; pVetor[ 13]=  1; pVetor[ 14]=  3;
		pVetor[ 15]= 11; pVetor[ 16]= 10; pVetor[ 17]=  8; pVetor[ 18]=  5; pVetor[ 19]= 10;
		pVetor[ 20]=  7; pVetor[ 21]= 15; pVetor[ 22]= 11; pVetor[ 23]=  3; pVetor[ 24]= 15;
		pVetor[ 25]=  6; pVetor[ 26]=  5; pVetor[ 27]=  4; pVetor[ 28]=  4; pVetor[ 29]=  7;
		pVetor[ 30]=  1; pVetor[ 31]=  3; pVetor[ 32]=  3; pVetor[ 33]= 13; pVetor[ 34]=  5;
		pVetor[ 35]= 10; pVetor[ 36]=  4; pVetor[ 37]= 12; pVetor[ 38]= 12; pVetor[ 39]=  6;
		pVetor[ 40]=  9; pVetor[ 41]=  1; pVetor[ 42]=  2; pVetor[ 43]=  2; pVetor[ 44]= 11;
		pVetor[ 45]=  3; pVetor[ 46]=  9; pVetor[ 47]= 10; pVetor[ 48]= 14; pVetor[ 49]=  4;
		pVetor[ 50]= 15; pVetor[ 51]=  2; pVetor[ 52]=  5; pVetor[ 53]= 11; pVetor[ 54]=  9;
		pVetor[ 55]=  2; pVetor[ 56]=  6; pVetor[ 57]=  6; pVetor[ 58]= 15; pVetor[ 59]= 13;
		pVetor[ 60]= 12; pVetor[ 61]= 12; pVetor[ 62]=  7; pVetor[ 63]= 10; pVetor[ 64]=  8;
		pVetor[ 65]=  4; pVetor[ 66]=  7; pVetor[ 67]= 15; pVetor[ 68]=  6; pVetor[ 69]=  1;
		pVetor[ 70]= 14; pVetor[ 71]= 14; pVetor[ 72]= 14; pVetor[ 73]=  7; pVetor[ 74]=  2;

    } else if (intProblema == 10) {
		pVetor[  0]=  2; pVetor[  1]= 12; pVetor[  2]=  9; pVetor[  3]=  3; pVetor[  4]=  2;
		pVetor[  5]= 13; pVetor[  6]=  7; pVetor[  7]=  2; pVetor[  8]=  2; pVetor[  9]= 11;
		pVetor[ 10]=  3; pVetor[ 11]=  9; pVetor[ 12]= 13; pVetor[ 13]= 12; pVetor[ 14]=  6;
		pVetor[ 15]=  9; pVetor[ 16]=  2; pVetor[ 17]=  3; pVetor[ 18]= 15; pVetor[ 19]=  7;
		pVetor[ 20]= 12; pVetor[ 21]=  3; pVetor[ 22]=  6; pVetor[ 23]=  9; pVetor[ 24]=  4;
		pVetor[ 25]=  7; pVetor[ 26]= 14; pVetor[ 27]= 12; pVetor[ 28]=  5; pVetor[ 29]=  5;
		pVetor[ 30]= 11; pVetor[ 31]=  8; pVetor[ 32]= 10; pVetor[ 33]= 11; pVetor[ 34]=  1;
		pVetor[ 35]=  5; pVetor[ 36]= 10; pVetor[ 37]=  8; pVetor[ 38]=  4; pVetor[ 39]=  9;
		pVetor[ 40]= 15; pVetor[ 41]= 13; pVetor[ 42]= 14; pVetor[ 43]= 14; pVetor[ 44]=  3;
		pVetor[ 45]= 14; pVetor[ 46]=  1; pVetor[ 47]=  5; pVetor[ 48]=  8; pVetor[ 49]= 14;
		pVetor[ 50]=  4; pVetor[ 51]= 11; pVetor[ 52]=  1; pVetor[ 53]=  6; pVetor[ 54]=  8;
		pVetor[ 55]= 10; pVetor[ 56]=  5; pVetor[ 57]= 15; pVetor[ 58]= 13; pVetor[ 59]= 10;
		pVetor[ 60]=  8; pVetor[ 61]=  6; pVetor[ 62]=  4; pVetor[ 63]= 10; pVetor[ 64]= 15;
		pVetor[ 65]=  6; pVetor[ 66]=  4; pVetor[ 67]= 11; pVetor[ 68]=  7; pVetor[ 69]= 12;
		pVetor[ 70]=  1; pVetor[ 71]= 15; pVetor[ 72]=  7; pVetor[ 73]=  1; pVetor[ 74]= 13;

    } else if (intProblema == 11) {
		pVetor[  0]=  2; pVetor[  1]= 12; pVetor[  2]=  9; pVetor[  3]=  3; pVetor[  4]=  2;
		pVetor[  5]= 13; pVetor[  6]=  7; pVetor[  7]=  2; pVetor[  8]=  2; pVetor[  9]= 11;
		pVetor[ 10]=  3; pVetor[ 11]=  9; pVetor[ 12]= 13; pVetor[ 13]= 12; pVetor[ 14]= 17;
		pVetor[ 15]= 17; pVetor[ 16]=  2; pVetor[ 17]=  3; pVetor[ 18]= 15; pVetor[ 19]= 20;
		pVetor[ 20]=  9; pVetor[ 21]=  3; pVetor[ 22]= 17; pVetor[ 23]=  9; pVetor[ 24]=  6;
		pVetor[ 25]= 12; pVetor[ 26]= 14; pVetor[ 27]=  6; pVetor[ 28]=  5; pVetor[ 29]= 19;
		pVetor[ 30]=  7; pVetor[ 31]= 16; pVetor[ 32]= 16; pVetor[ 33]= 11; pVetor[ 34]= 16;
		pVetor[ 35]= 18; pVetor[ 36]= 18; pVetor[ 37]= 12; pVetor[ 38]=  4; pVetor[ 39]=  7;
		pVetor[ 40]= 20; pVetor[ 41]= 17; pVetor[ 42]= 10; pVetor[ 43]= 20; pVetor[ 44]=  4;
		pVetor[ 45]= 11; pVetor[ 46]=  8; pVetor[ 47]=  8; pVetor[ 48]= 14; pVetor[ 49]=  5;
		pVetor[ 50]= 19; pVetor[ 51]= 10; pVetor[ 52]= 14; pVetor[ 53]=  8; pVetor[ 54]=  1;
		pVetor[ 55]=  5; pVetor[ 56]= 20; pVetor[ 57]=  5; pVetor[ 58]= 16; pVetor[ 59]=  9;
		pVetor[ 60]= 15; pVetor[ 61]= 13; pVetor[ 62]=  1; pVetor[ 63]=  6; pVetor[ 64]= 18;
		pVetor[ 65]= 16; pVetor[ 66]=  1; pVetor[ 67]= 15; pVetor[ 68]= 18; pVetor[ 69]=  3;
		pVetor[ 70]= 14; pVetor[ 71]= 11; pVetor[ 72]= 20; pVetor[ 73]= 13; pVetor[ 74]= 14;
		pVetor[ 75]=  4; pVetor[ 76]=  5; pVetor[ 77]=  4; pVetor[ 78]= 10; pVetor[ 79]=  8;
		pVetor[ 80]= 10; pVetor[ 81]= 19; pVetor[ 82]= 11; pVetor[ 83]= 17; pVetor[ 84]= 10;
		pVetor[ 85]=  8; pVetor[ 86]=  6; pVetor[ 87]=  7; pVetor[ 88]=  7; pVetor[ 89]= 15;
		pVetor[ 90]=  6; pVetor[ 91]=  4; pVetor[ 92]= 18; pVetor[ 93]= 19; pVetor[ 94]= 12;
		pVetor[ 95]=  1; pVetor[ 96]= 15; pVetor[ 97]= 19; pVetor[ 98]=  1; pVetor[ 99]= 13;

    } else if (intProblema == 12) {
		pVetor[  0]=  7; pVetor[  1]= 19; pVetor[  2]= 12; pVetor[  3]= 17; pVetor[  4]=  1;
		pVetor[  5]= 19; pVetor[  6]=  6; pVetor[  7]=  6; pVetor[  8]= 19; pVetor[  9]=  6;
		pVetor[ 10]=  1; pVetor[ 11]=  8; pVetor[ 12]= 13; pVetor[ 13]= 11; pVetor[ 14]= 19;
		pVetor[ 15]=  5; pVetor[ 16]= 10; pVetor[ 17]=  4; pVetor[ 18]= 15; pVetor[ 19]=  5;
		pVetor[ 20]= 16; pVetor[ 21]= 13; pVetor[ 22]=  9; pVetor[ 23]= 13; pVetor[ 24]=  9;
		pVetor[ 25]=  6; pVetor[ 26]=  7; pVetor[ 27]= 11; pVetor[ 28]=  5; pVetor[ 29]=  2;
		pVetor[ 30]= 14; pVetor[ 31]=  4; pVetor[ 32]= 19; pVetor[ 33]= 12; pVetor[ 34]= 10;
		pVetor[ 35]= 12; pVetor[ 36]=  5; pVetor[ 37]= 17; pVetor[ 38]=  4; pVetor[ 39]= 11;
		pVetor[ 40]= 11; pVetor[ 41]= 16; pVetor[ 42]=  8; pVetor[ 43]=  3; pVetor[ 44]= 17;
		pVetor[ 45]= 17; pVetor[ 46]=  3; pVetor[ 47]=  1; pVetor[ 48]=  9; pVetor[ 49]=  4;
		pVetor[ 50]=  4; pVetor[ 51]= 14; pVetor[ 52]= 16; pVetor[ 53]=  2; pVetor[ 54]=  8;
		pVetor[ 55]=  3; pVetor[ 56]= 12; pVetor[ 57]= 14; pVetor[ 58]=  1; pVetor[ 59]= 20;
		pVetor[ 60]=  8; pVetor[ 61]=  9; pVetor[ 62]=  3; pVetor[ 63]=  7; pVetor[ 64]= 14;
		pVetor[ 65]= 13; pVetor[ 66]= 20; pVetor[ 67]=  5; pVetor[ 68]= 16; pVetor[ 69]= 16;
		pVetor[ 70]= 18; pVetor[ 71]= 17; pVetor[ 72]=  2; pVetor[ 73]= 14; pVetor[ 74]=  7;
		pVetor[ 75]= 10; pVetor[ 76]= 18; pVetor[ 77]=  7; pVetor[ 78]= 18; pVetor[ 79]=  3;
		pVetor[ 80]=  2; pVetor[ 81]=  2; pVetor[ 82]= 15; pVetor[ 83]= 20; pVetor[ 84]= 12;
		pVetor[ 85]= 15; pVetor[ 86]=  1; pVetor[ 87]= 20; pVetor[ 88]=  6; pVetor[ 89]= 15;
		pVetor[ 90]= 20; pVetor[ 91]= 15; pVetor[ 92]= 18; pVetor[ 93]=  8; pVetor[ 94]= 13;
		pVetor[ 95]=  9; pVetor[ 96]= 11; pVetor[ 97]= 10; pVetor[ 98]= 10; pVetor[ 99]= 18;

    } else if (intProblema == 13) {
		pVetor[  0]=  3; pVetor[  1]= 18; pVetor[  2]= 19; pVetor[  3]= 18; pVetor[  4]=  1;
		pVetor[  5]= 10; pVetor[  6]= 20; pVetor[  7]=  8; pVetor[  8]= 19; pVetor[  9]= 13;
		pVetor[ 10]=  1; pVetor[ 11]=  6; pVetor[ 12]= 11; pVetor[ 13]= 11; pVetor[ 14]= 20;
		pVetor[ 15]= 18; pVetor[ 16]= 15; pVetor[ 17]=  6; pVetor[ 18]= 15; pVetor[ 19]= 17;
		pVetor[ 20]= 15; pVetor[ 21]= 13; pVetor[ 22]=  5; pVetor[ 23]=  9; pVetor[ 24]= 15;
		pVetor[ 25]= 17; pVetor[ 26]=  1; pVetor[ 27]= 18; pVetor[ 28]= 13; pVetor[ 29]=  3;
		pVetor[ 30]= 16; pVetor[ 31]= 14; pVetor[ 32]=  1; pVetor[ 33]= 14; pVetor[ 34]= 16;
		pVetor[ 35]= 11; pVetor[ 36]= 11; pVetor[ 37]= 10; pVetor[ 38]=  7; pVetor[ 39]= 14;
		pVetor[ 40]= 20; pVetor[ 41]= 19; pVetor[ 42]= 14; pVetor[ 43]=  1; pVetor[ 44]=  4;
		pVetor[ 45]=  4; pVetor[ 46]= 12; pVetor[ 47]= 13; pVetor[ 48]= 12; pVetor[ 49]= 19;
		pVetor[ 50]= 19; pVetor[ 51]=  8; pVetor[ 52]=  9; pVetor[ 53]=  2; pVetor[ 54]= 18;
		pVetor[ 55]=  7; pVetor[ 56]=  2; pVetor[ 57]= 12; pVetor[ 58]=  8; pVetor[ 59]=  7;
		pVetor[ 60]=  2; pVetor[ 61]=  3; pVetor[ 62]= 20; pVetor[ 63]= 20; pVetor[ 64]= 10;
		pVetor[ 65]=  9; pVetor[ 66]= 16; pVetor[ 67]= 15; pVetor[ 68]=  6; pVetor[ 69]=  5;
		pVetor[ 70]=  6; pVetor[ 71]=  9; pVetor[ 72]=  2; pVetor[ 73]= 16; pVetor[ 74]= 11;
		pVetor[ 75]= 13; pVetor[ 76]=  7; pVetor[ 77]=  4; pVetor[ 78]=  5; pVetor[ 79]= 12;
		pVetor[ 80]=  8; pVetor[ 81]=  4; pVetor[ 82]= 16; pVetor[ 83]=  3; pVetor[ 84]=  8;
		pVetor[ 85]=  5; pVetor[ 86]=  5; pVetor[ 87]=  3; pVetor[ 88]=  4; pVetor[ 89]=  9;
		pVetor[ 90]= 12; pVetor[ 91]= 10; pVetor[ 92]= 17; pVetor[ 93]= 10; pVetor[ 94]=  2;
		pVetor[ 95]= 14; pVetor[ 96]= 17; pVetor[ 97]=  7; pVetor[ 98]= 17; pVetor[ 99]=  6;

    } else if (intProblema == 14) {
		pVetor[  0]=  2; pVetor[  1]= 12; pVetor[  2]=  9; pVetor[  3]=  3; pVetor[  4]=  2;
		pVetor[  5]= 13; pVetor[  6]=  7; pVetor[  7]=  2; pVetor[  8]=  2; pVetor[  9]= 11;
		pVetor[ 10]=  3; pVetor[ 11]=  9; pVetor[ 12]= 13; pVetor[ 13]= 12; pVetor[ 14]= 17;
		pVetor[ 15]= 17; pVetor[ 16]=  2; pVetor[ 17]=  3; pVetor[ 18]= 15; pVetor[ 19]= 20;
		pVetor[ 20]=  9; pVetor[ 21]=  3; pVetor[ 22]= 17; pVetor[ 23]=  9; pVetor[ 24]=  6;
		pVetor[ 25]= 12; pVetor[ 26]= 14; pVetor[ 27]=  6; pVetor[ 28]=  5; pVetor[ 29]= 19;
		pVetor[ 30]=  7; pVetor[ 31]= 16; pVetor[ 32]= 16; pVetor[ 33]= 11; pVetor[ 34]= 16;
		pVetor[ 35]= 18; pVetor[ 36]= 18; pVetor[ 37]= 12; pVetor[ 38]=  4; pVetor[ 39]=  7;
		pVetor[ 40]= 20; pVetor[ 41]= 17; pVetor[ 42]= 10; pVetor[ 43]= 20; pVetor[ 44]=  4;
		pVetor[ 45]= 11; pVetor[ 46]=  8; pVetor[ 47]=  8; pVetor[ 48]= 14; pVetor[ 49]=  5;
		pVetor[ 50]= 19; pVetor[ 51]= 10; pVetor[ 52]= 14; pVetor[ 53]=  8; pVetor[ 54]=  1;
		pVetor[ 55]=  5; pVetor[ 56]= 20; pVetor[ 57]=  5; pVetor[ 58]= 16; pVetor[ 59]=  9;
		pVetor[ 60]= 15; pVetor[ 61]= 13; pVetor[ 62]=  1; pVetor[ 63]=  6; pVetor[ 64]= 18;
		pVetor[ 65]= 16; pVetor[ 66]=  1; pVetor[ 67]= 15; pVetor[ 68]= 18; pVetor[ 69]=  3;
		pVetor[ 70]= 14; pVetor[ 71]= 11; pVetor[ 72]= 20; pVetor[ 73]= 13; pVetor[ 74]= 14;
		pVetor[ 75]=  4; pVetor[ 76]=  5; pVetor[ 77]=  4; pVetor[ 78]= 10; pVetor[ 79]=  8;
		pVetor[ 80]= 10; pVetor[ 81]= 19; pVetor[ 82]= 11; pVetor[ 83]= 17; pVetor[ 84]= 10;
		pVetor[ 85]=  8; pVetor[ 86]=  6; pVetor[ 87]=  7; pVetor[ 88]=  7; pVetor[ 89]= 15;
		pVetor[ 90]=  6; pVetor[ 91]=  4; pVetor[ 92]= 18; pVetor[ 93]= 19; pVetor[ 94]= 12;
		pVetor[ 95]=  1; pVetor[ 96]= 15; pVetor[ 97]= 19; pVetor[ 98]=  1; pVetor[ 99]= 13;

    } else if (intProblema == 15) {
		pVetor[  0]= 20; pVetor[  1]= 14; pVetor[  2]= 16; pVetor[  3]=  8; pVetor[  4]= 14;
		pVetor[  5]= 18; pVetor[  6]= 13; pVetor[  7]=  5; pVetor[  8]= 12; pVetor[  9]= 13;
		pVetor[ 10]=  9; pVetor[ 11]= 15; pVetor[ 12]=  6; pVetor[ 13]= 13; pVetor[ 14]=  2;
		pVetor[ 15]=  6; pVetor[ 16]= 19; pVetor[ 17]= 13; pVetor[ 18]= 14; pVetor[ 19]= 11;
		pVetor[ 20]= 10; pVetor[ 21]=  6; pVetor[ 22]=  9; pVetor[ 23]= 15; pVetor[ 24]= 20;
		pVetor[ 25]= 17; pVetor[ 26]= 20; pVetor[ 27]=  3; pVetor[ 28]= 10; pVetor[ 29]=  3;
		pVetor[ 30]=  8; pVetor[ 31]=  1; pVetor[ 32]= 18; pVetor[ 33]= 19; pVetor[ 34]=  4;
		pVetor[ 35]=  3; pVetor[ 36]= 16; pVetor[ 37]= 19; pVetor[ 38]= 17; pVetor[ 39]=  7;
		pVetor[ 40]= 14; pVetor[ 41]=  7; pVetor[ 42]=  8; pVetor[ 43]= 11; pVetor[ 44]= 10;
		pVetor[ 45]=  4; pVetor[ 46]=  8; pVetor[ 47]= 15; pVetor[ 48]=  3; pVetor[ 49]=  9;
		pVetor[ 50]= 15; pVetor[ 51]=  3; pVetor[ 52]=  1; pVetor[ 53]=  9; pVetor[ 54]= 15;
		pVetor[ 55]=  2; pVetor[ 56]=  5; pVetor[ 57]=  2; pVetor[ 58]= 18; pVetor[ 59]= 18;
		pVetor[ 60]= 12; pVetor[ 61]= 10; pVetor[ 62]= 12; pVetor[ 63]= 20; pVetor[ 64]=  8;
		pVetor[ 65]= 16; pVetor[ 66]= 11; pVetor[ 67]= 20; pVetor[ 68]= 16; pVetor[ 69]=  5;
		pVetor[ 70]= 11; pVetor[ 71]= 18; pVetor[ 72]=  7; pVetor[ 73]=  7; pVetor[ 74]= 16;
		pVetor[ 75]=  5; pVetor[ 76]=  9; pVetor[ 77]= 14; pVetor[ 78]=  1; pVetor[ 79]=  6;
		pVetor[ 80]= 13; pVetor[ 81]=  4; pVetor[ 82]=  4; pVetor[ 83]=  2; pVetor[ 84]= 19;
		pVetor[ 85]= 19; pVetor[ 86]=  2; pVetor[ 87]= 17; pVetor[ 88]=  6; pVetor[ 89]= 17;
		pVetor[ 90]=  7; pVetor[ 91]= 12; pVetor[ 92]= 11; pVetor[ 93]=  4; pVetor[ 94]=  1;
		pVetor[ 95]=  1; pVetor[ 96]= 17; pVetor[ 97]= 10; pVetor[ 98]=  5; pVetor[ 99]= 12;

    } else if (intProblema == 16) {
		pVetor[  0]=  6; pVetor[  1]=  7; pVetor[  2]=  5; pVetor[  3]=  6; pVetor[  4]=  5; pVetor[  5]=  2; pVetor[  6]=  9; pVetor[  7]=  6; pVetor[  8]=  5; pVetor[  9]=  4;
		pVetor[ 20]=  8; pVetor[ 21]= 10; pVetor[ 22]=  4; pVetor[ 23]=  1; pVetor[ 24]=  3; pVetor[ 25]= 10; pVetor[ 26]=  2; pVetor[ 27]=  2; pVetor[ 28]= 10; pVetor[ 29]=  2;
		pVetor[ 10]=  4; pVetor[ 11]=  4; pVetor[ 12]= 10; pVetor[ 13]=  4; pVetor[ 14]= 10; pVetor[ 15]=  1; pVetor[ 16]= 10; pVetor[ 17]=  8; pVetor[ 18]=  2; pVetor[ 19]= 10;
		pVetor[ 30]=  3; pVetor[ 31]=  5; pVetor[ 32]=  3; pVetor[ 33]=  8; pVetor[ 34]=  6; pVetor[ 35]=  4; pVetor[ 36]=  5; pVetor[ 37]= 10; pVetor[ 38]=  9; pVetor[ 39]=  3;
		pVetor[ 40]=  9; pVetor[ 41]=  8; pVetor[ 42]=  7; pVetor[ 43]=  2; pVetor[ 44]=  4; pVetor[ 45]=  7; pVetor[ 46]=  1; pVetor[ 47]=  3; pVetor[ 48]=  3; pVetor[ 49]=  5;
		pVetor[ 50]= 10; pVetor[ 51]=  2; pVetor[ 52]=  8; pVetor[ 53]=  3; pVetor[ 54]=  2; pVetor[ 55]=  3; pVetor[ 56]=  3; pVetor[ 57]=  5; pVetor[ 58]=  1; pVetor[ 59]=  7;
		pVetor[ 60]=  1; pVetor[ 61]=  9; pVetor[ 62]=  9; pVetor[ 63]= 10; pVetor[ 64]=  9; pVetor[ 65]=  9; pVetor[ 66]=  8; pVetor[ 67]=  4; pVetor[ 68]=  7; pVetor[ 69]=  8;
		pVetor[ 70]=  7; pVetor[ 71]=  1; pVetor[ 72]=  6; pVetor[ 73]=  9; pVetor[ 74]=  7; pVetor[ 75]=  6; pVetor[ 76]=  7; pVetor[ 77]=  7; pVetor[ 78]=  6; pVetor[ 79]=  9;
		pVetor[ 80]=  5; pVetor[ 81]=  3; pVetor[ 82]=  1; pVetor[ 83]=  5; pVetor[ 84]=  1; pVetor[ 85]=  5; pVetor[ 86]=  4; pVetor[ 87]=  1; pVetor[ 88]=  4; pVetor[ 89]=  1;
		pVetor[ 90]=  2; pVetor[ 91]=  6; pVetor[ 92]=  2; pVetor[ 93]=  7; pVetor[ 94]=  8; pVetor[ 95]=  8; pVetor[ 96]=  6; pVetor[ 97]=  9; pVetor[ 98]=  8; pVetor[ 99]=  6;

    } else if (intProblema == 17) {
		pVetor[  0]=  4; pVetor[  1]= 10; pVetor[  2]=  7; pVetor[  3]=  9; pVetor[  4]=  8; pVetor[  5]=  1; pVetor[  6]=  3; pVetor[  7]=  6; pVetor[  8]=  6; pVetor[  9]=  3;
		pVetor[ 10]= 10; pVetor[ 11]=  7; pVetor[ 12]=  5; pVetor[ 13]= 10; pVetor[ 14]=  3; pVetor[ 15]= 10; pVetor[ 16]=  4; pVetor[ 17]=  3; pVetor[ 18]=  2; pVetor[ 19]=  1;
		pVetor[ 20]=  1; pVetor[ 21]=  4; pVetor[ 22]=  6; pVetor[ 23]=  6; pVetor[ 24]=  7; pVetor[ 25]=  3; pVetor[ 26]=  5; pVetor[ 27]=  2; pVetor[ 28]=  1; pVetor[ 29]=  5;
		pVetor[ 30]=  3; pVetor[ 31]=  3; pVetor[ 32]= 10; pVetor[ 33]=  8; pVetor[ 34]=  6; pVetor[ 35]=  5; pVetor[ 36]=  2; pVetor[ 37]=  4; pVetor[ 38]=  8; pVetor[ 39]=  2;
		pVetor[ 40]=  6; pVetor[ 41]=  2; pVetor[ 42]=  9; pVetor[ 43]=  7; pVetor[ 44]=  2; pVetor[ 45]=  6; pVetor[ 46]=  8; pVetor[ 47]=  8; pVetor[ 48]=  7; pVetor[ 49]=  4;
		pVetor[ 50]=  9; pVetor[ 51]=  5; pVetor[ 52]=  1; pVetor[ 53]=  3; pVetor[ 54]=  4; pVetor[ 55]=  9; pVetor[ 56]=  7; pVetor[ 57]=  7; pVetor[ 58]=  9; pVetor[ 59]= 10;
		pVetor[ 60]=  2; pVetor[ 61]=  9; pVetor[ 62]=  2; pVetor[ 63]=  2; pVetor[ 64]=  9; pVetor[ 65]=  4; pVetor[ 66]=  6; pVetor[ 67]=  1; pVetor[ 68]= 10; pVetor[ 69]=  7;
		pVetor[ 70]=  7; pVetor[ 71]=  1; pVetor[ 72]=  3; pVetor[ 73]=  5; pVetor[ 74]=  5; pVetor[ 75]=  2; pVetor[ 76]= 10; pVetor[ 77]= 10; pVetor[ 78]=  3; pVetor[ 79]=  8;
		pVetor[ 80]=  8; pVetor[ 81]=  8; pVetor[ 82]=  4; pVetor[ 83]=  1; pVetor[ 84]=  1; pVetor[ 85]=  8; pVetor[ 86]=  9; pVetor[ 87]=  9; pVetor[ 88]=  4; pVetor[ 89]=  6;
		pVetor[ 90]=  5; pVetor[ 91]=  6; pVetor[ 92]=  8; pVetor[ 93]=  4; pVetor[ 94]= 10; pVetor[ 95]=  7; pVetor[ 96]=  1; pVetor[ 97]=  5; pVetor[ 98]=  5; pVetor[ 99]=  9;

    } else if (intProblema == 18) {
		pVetor[  0]=  2; pVetor[  1]=  8; pVetor[  2]= 10; pVetor[  3]= 10; pVetor[  4]=  5; pVetor[  5]=  1; pVetor[  6]=  9; pVetor[  7]=  4; pVetor[  8]=  9; pVetor[  9]= 10;
		pVetor[ 10]=  1; pVetor[ 11]=  7; pVetor[ 12]=  6; pVetor[ 13]=  6; pVetor[ 14]=  9; pVetor[ 15]=  7; pVetor[ 16]= 10; pVetor[ 17]=  3; pVetor[ 18]=  7; pVetor[ 19]=  8;
		pVetor[ 20]=  8; pVetor[ 21]=  1; pVetor[ 22]=  3; pVetor[ 23]=  8; pVetor[ 24]=  8; pVetor[ 25]=  9; pVetor[ 26]=  3; pVetor[ 27]=  9; pVetor[ 28]=  6; pVetor[ 29]=  7;
		pVetor[ 30]=  9; pVetor[ 31]=  6; pVetor[ 32]=  1; pVetor[ 33]=  5; pVetor[ 34]= 10; pVetor[ 35]=  8; pVetor[ 36]=  7; pVetor[ 37]=  5; pVetor[ 38]=  1; pVetor[ 39]=  2;
		pVetor[ 40]=  6; pVetor[ 41]= 10; pVetor[ 42]=  7; pVetor[ 43]=  7; pVetor[ 44]=  2; pVetor[ 45]=  2; pVetor[ 46]=  6; pVetor[ 47]=  7; pVetor[ 48]=  4; pVetor[ 49]=  9;
		pVetor[ 50]= 10; pVetor[ 51]=  2; pVetor[ 52]=  5; pVetor[ 53]=  4; pVetor[ 54]=  1; pVetor[ 55]= 10; pVetor[ 56]=  4; pVetor[ 57]=  6; pVetor[ 58]= 10; pVetor[ 59]=  5;
		pVetor[ 60]=  4; pVetor[ 61]=  5; pVetor[ 62]=  8; pVetor[ 63]=  1; pVetor[ 64]=  3; pVetor[ 65]=  4; pVetor[ 66]=  1; pVetor[ 67]= 10; pVetor[ 68]=  3; pVetor[ 69]=  6;
		pVetor[ 70]=  5; pVetor[ 71]=  4; pVetor[ 72]=  2; pVetor[ 73]=  3; pVetor[ 74]=  4; pVetor[ 75]=  3; pVetor[ 76]=  8; pVetor[ 77]=  1; pVetor[ 78]=  8; pVetor[ 79]=  4;
		pVetor[ 80]=  7; pVetor[ 81]=  3; pVetor[ 82]=  9; pVetor[ 83]=  2; pVetor[ 84]=  6; pVetor[ 85]=  6; pVetor[ 86]=  2; pVetor[ 87]=  2; pVetor[ 88]=  2; pVetor[ 89]=  1;
		pVetor[ 90]=  3; pVetor[ 91]=  9; pVetor[ 92]=  4; pVetor[ 93]=  9; pVetor[ 94]=  7; pVetor[ 95]=  5; pVetor[ 96]=  5; pVetor[ 97]=  8; pVetor[ 98]=  5; pVetor[ 99]=  3;

    } else if (intProblema == 19) {
		pVetor[  0]=  7; pVetor[  1]=  4; pVetor[  2]=  5; pVetor[  3]=  2; pVetor[  4]=  1; pVetor[  5]=  6; pVetor[  6]=  6; pVetor[  7]=  1; pVetor[  8]=  1; pVetor[  9]=  1;
		pVetor[ 10]=  2; pVetor[ 11]=  5; pVetor[ 12]=  7; pVetor[ 13]=  8; pVetor[ 14]=  6; pVetor[ 15]=  9; pVetor[ 16]=  1; pVetor[ 17]=  3; pVetor[ 18]=  6; pVetor[ 19]= 10;
		pVetor[ 20]=  9; pVetor[ 21]=  2; pVetor[ 22]=  2; pVetor[ 23]=  5; pVetor[ 24]=  9; pVetor[ 25]= 10; pVetor[ 26]=  7; pVetor[ 27]=  8; pVetor[ 28]=  2; pVetor[ 29]=  3;
		pVetor[ 30]=  5; pVetor[ 31]=  9; pVetor[ 32]=  9; pVetor[ 33]=  3; pVetor[ 34]= 10; pVetor[ 35]=  4; pVetor[ 36]=  8; pVetor[ 37]=  6; pVetor[ 38]= 10; pVetor[ 39]=  8;
		pVetor[ 40]=  4; pVetor[ 41]=  7; pVetor[ 42]=  3; pVetor[ 43]=  6; pVetor[ 44]=  8; pVetor[ 45]=  3; pVetor[ 46]=  9; pVetor[ 47]=  5; pVetor[ 48]=  7; pVetor[ 49]=  2;
		pVetor[ 50]=  6; pVetor[ 51]=  1; pVetor[ 52]=  1; pVetor[ 53]=  7; pVetor[ 54]=  7; pVetor[ 55]=  1; pVetor[ 56]=  4; pVetor[ 57]=  4; pVetor[ 58]=  4; pVetor[ 59]=  9;
		pVetor[ 60]= 10; pVetor[ 61]=  6; pVetor[ 62]=  8; pVetor[ 63]=  9; pVetor[ 64]=  2; pVetor[ 65]=  5; pVetor[ 66]=  5; pVetor[ 67]=  7; pVetor[ 68]=  8; pVetor[ 69]=  7;
		pVetor[ 70]=  3; pVetor[ 71]=  3; pVetor[ 72]=  6; pVetor[ 73]=  4; pVetor[ 74]=  5; pVetor[ 75]=  2; pVetor[ 76]= 10; pVetor[ 77]= 10; pVetor[ 78]=  3; pVetor[ 79]=  4;
		pVetor[ 80]=  8; pVetor[ 81]= 10; pVetor[ 82]=  4; pVetor[ 83]= 10; pVetor[ 84]=  4; pVetor[ 85]=  8; pVetor[ 86]=  3; pVetor[ 87]=  2; pVetor[ 88]=  9; pVetor[ 89]=  5;
		pVetor[ 90]=  1; pVetor[ 91]=  8; pVetor[ 92]= 10; pVetor[ 93]=  1; pVetor[ 94]=  3; pVetor[ 95]=  7; pVetor[ 96]=  2; pVetor[ 97]=  9; pVetor[ 98]=  5; pVetor[ 99]=  6;

    } else if (intProblema == 20) {
		pVetor[  0]=  5; pVetor[  1]=  7; pVetor[  2]=  3; pVetor[  3]=  7; pVetor[  4]= 10; pVetor[  5]=  7; pVetor[  6]=  7; pVetor[  7]=  8; pVetor[  8]=  4; pVetor[  9]=  7;
		pVetor[ 10]=  9; pVetor[ 11]=  8; pVetor[ 12]=  7; pVetor[ 13]=  8; pVetor[ 14]=  9; pVetor[ 15]=  6; pVetor[ 16]=  3; pVetor[ 17]=  3; pVetor[ 18]=  6; pVetor[ 19]=  1;
		pVetor[ 20]=  2; pVetor[ 21]= 10; pVetor[ 22]=  5; pVetor[ 23]= 10; pVetor[ 24]=  3; pVetor[ 25]=  2; pVetor[ 26]= 10; pVetor[ 27]=  9; pVetor[ 28]=  7; pVetor[ 29]= 10;
		pVetor[ 30]=  8; pVetor[ 31]=  1; pVetor[ 32]=  2; pVetor[ 33]=  9; pVetor[ 34]=  5; pVetor[ 35]=  4; pVetor[ 36]=  8; pVetor[ 37]=  4; pVetor[ 38]=  5; pVetor[ 39]=  2;
		pVetor[ 40]=  6; pVetor[ 41]=  4; pVetor[ 42]= 10; pVetor[ 43]=  6; pVetor[ 44]=  4; pVetor[ 45]=  5; pVetor[ 46]=  4; pVetor[ 47]=  1; pVetor[ 48]=  9; pVetor[ 49]=  5;
		pVetor[ 50]=  3; pVetor[ 51]=  2; pVetor[ 52]=  8; pVetor[ 53]=  2; pVetor[ 54]=  7; pVetor[ 55]=  8; pVetor[ 56]=  5; pVetor[ 57]=  6; pVetor[ 58]= 10; pVetor[ 59]=  9;
		pVetor[ 60]=  7; pVetor[ 61]=  3; pVetor[ 62]=  1; pVetor[ 63]=  5; pVetor[ 64]=  2; pVetor[ 65]=  3; pVetor[ 66]=  9; pVetor[ 67]= 10; pVetor[ 68]=  8; pVetor[ 69]=  4;
		pVetor[ 70]= 10; pVetor[ 71]=  6; pVetor[ 72]=  4; pVetor[ 73]=  4; pVetor[ 74]=  1; pVetor[ 75]= 10; pVetor[ 76]=  2; pVetor[ 77]=  7; pVetor[ 78]=  1; pVetor[ 79]=  8;
		pVetor[ 80]=  4; pVetor[ 81]=  5; pVetor[ 82]=  9; pVetor[ 83]=  1; pVetor[ 84]=  6; pVetor[ 85]=  9; pVetor[ 86]=  1; pVetor[ 87]=  2; pVetor[ 88]=  3; pVetor[ 89]=  3;
		pVetor[ 90]=  1; pVetor[ 91]=  9; pVetor[ 92]=  6; pVetor[ 93]=  3; pVetor[ 94]=  8; pVetor[ 95]=  1; pVetor[ 96]=  6; pVetor[ 97]=  5; pVetor[ 98]=  2; pVetor[ 99]=  6;

    } else if (intProblema == 21) {
		pVetor[  0]=  6; pVetor[  1]=  7; pVetor[  2]=  5; pVetor[  3]= 12; pVetor[  4]= 13; pVetor[  5]=  2; pVetor[  6]=  9; pVetor[  7]= 14; pVetor[  8]= 12; pVetor[  9]= 12;
		pVetor[ 10]=  4; pVetor[ 11]=  4; pVetor[ 12]= 10; pVetor[ 13]= 11; pVetor[ 14]=  5; pVetor[ 15]=  1; pVetor[ 16]= 10; pVetor[ 17]=  6; pVetor[ 18]= 11; pVetor[ 19]=  4;
		pVetor[ 20]=  8; pVetor[ 21]= 10; pVetor[ 22]=  4; pVetor[ 23]=  6; pVetor[ 24]= 10; pVetor[ 25]= 10; pVetor[ 26]=  2; pVetor[ 27]= 12; pVetor[ 28]=  5; pVetor[ 29]= 10;
		pVetor[ 30]=  3; pVetor[ 31]= 12; pVetor[ 32]=  3; pVetor[ 33]=  4; pVetor[ 34]= 14; pVetor[ 35]=  4; pVetor[ 36]= 12; pVetor[ 37]=  8; pVetor[ 38]= 15; pVetor[ 39]= 14;
		pVetor[ 40]= 14; pVetor[ 41]=  5; pVetor[ 42]=  7; pVetor[ 43]=  1; pVetor[ 44]=  3; pVetor[ 45]= 11; pVetor[ 46]=  5; pVetor[ 47]=  2; pVetor[ 48]=  2; pVetor[ 49]=  2;
		pVetor[ 50]=  9; pVetor[ 51]= 13; pVetor[ 52]= 14; pVetor[ 53]= 15; pVetor[ 54]=  6; pVetor[ 55]=  7; pVetor[ 56]=  1; pVetor[ 57]= 13; pVetor[ 58]= 10; pVetor[ 59]=  3;
		pVetor[ 60]= 15; pVetor[ 61]=  8; pVetor[ 62]= 15; pVetor[ 63]= 13; pVetor[ 64]= 15; pVetor[ 65]=  3; pVetor[ 66]= 13; pVetor[ 67]= 10; pVetor[ 68]=  9; pVetor[ 69]=  5;
		pVetor[ 70]= 10; pVetor[ 71]=  2; pVetor[ 72]=  8; pVetor[ 73]=  8; pVetor[ 74]=  4; pVetor[ 75]=  9; pVetor[ 76]= 14; pVetor[ 77]=  3; pVetor[ 78]=  3; pVetor[ 79]= 15;
		pVetor[ 80]= 12; pVetor[ 81]= 15; pVetor[ 82]=  9; pVetor[ 83]=  2; pVetor[ 84]=  2; pVetor[ 85]=  6; pVetor[ 86]=  3; pVetor[ 87]=  5; pVetor[ 88]=  1; pVetor[ 89]= 13;
		pVetor[ 90]=  1; pVetor[ 91]= 11; pVetor[ 92]=  6; pVetor[ 93]=  3; pVetor[ 94]= 12; pVetor[ 95]= 12; pVetor[ 96]=  8; pVetor[ 97]=  4; pVetor[ 98]=  7; pVetor[ 99]=  7;
		pVetor[100]=  7; pVetor[101]=  9; pVetor[102]=  1; pVetor[103]= 10; pVetor[104]=  9; pVetor[105]= 14; pVetor[106]=  7; pVetor[107]= 11; pVetor[108]= 14; pVetor[109]=  8;
		pVetor[110]=  5; pVetor[111]=  1; pVetor[112]=  2; pVetor[113]=  9; pVetor[114]=  7; pVetor[115]= 13; pVetor[116]=  4; pVetor[117]=  7; pVetor[118]=  6; pVetor[119]=  9;
		pVetor[120]= 13; pVetor[121]=  3; pVetor[122]= 12; pVetor[123]= 14; pVetor[124]= 11; pVetor[125]=  5; pVetor[126]= 15; pVetor[127]=  1; pVetor[128]= 13; pVetor[129]= 11;
		pVetor[130]= 11; pVetor[131]= 14; pVetor[132]= 11; pVetor[133]=  5; pVetor[134]=  1; pVetor[135]= 15; pVetor[136]= 11; pVetor[137]= 15; pVetor[138]=  4; pVetor[139]=  1;
		pVetor[140]=  2; pVetor[141]=  6; pVetor[142]= 13; pVetor[143]=  7; pVetor[144]=  8; pVetor[145]=  8; pVetor[146]=  6; pVetor[147]=  9; pVetor[148]=  8; pVetor[149]=  6;

    } else if (intProblema == 22) {
		pVetor[  0]= 12; pVetor[  1]=  4; pVetor[  2]=  8; pVetor[  3]=  7; pVetor[  4]= 15; pVetor[  5]= 11; pVetor[  6]=  4; pVetor[  7]=  2; pVetor[  8]=  9; pVetor[  9]=  4;
		pVetor[ 10]= 11; pVetor[ 11]= 11; pVetor[ 12]=  1; pVetor[ 13]=  1; pVetor[ 14]=  5; pVetor[ 15]=  1; pVetor[ 16]= 10; pVetor[ 17]=  6; pVetor[ 18]=  8; pVetor[ 19]=  3;
		pVetor[ 20]=  2; pVetor[ 21]=  5; pVetor[ 22]=  4; pVetor[ 23]=  5; pVetor[ 24]=  1; pVetor[ 25]=  9; pVetor[ 26]= 13; pVetor[ 27]= 14; pVetor[ 28]= 10; pVetor[ 29]=  6;
		pVetor[ 30]=  9; pVetor[ 31]=  6; pVetor[ 32]= 13; pVetor[ 33]= 15; pVetor[ 34]=  3; pVetor[ 35]=  3; pVetor[ 36]=  2; pVetor[ 37]= 13; pVetor[ 38]=  3; pVetor[ 39]=  1;
		pVetor[ 40]=  8; pVetor[ 41]= 12; pVetor[ 42]= 11; pVetor[ 43]=  8; pVetor[ 44]=  8; pVetor[ 45]=  8; pVetor[ 46]=  6; pVetor[ 47]=  8; pVetor[ 48]=  4; pVetor[ 49]=  2;
		pVetor[ 50]=  5; pVetor[ 51]=  3; pVetor[ 52]=  5; pVetor[ 53]= 14; pVetor[ 54]=  4; pVetor[ 55]=  6; pVetor[ 56]=  7; pVetor[ 57]= 15; pVetor[ 58]=  6; pVetor[ 59]= 12;
		pVetor[ 60]= 14; pVetor[ 61]= 15; pVetor[ 62]= 15; pVetor[ 63]= 11; pVetor[ 64]= 12; pVetor[ 65]= 12; pVetor[ 66]=  9; pVetor[ 67]=  5; pVetor[ 68]=  5; pVetor[ 69]= 10;
		pVetor[ 70]=  4; pVetor[ 71]=  9; pVetor[ 72]=  3; pVetor[ 73]=  6; pVetor[ 74]= 11; pVetor[ 75]=  7; pVetor[ 76]= 14; pVetor[ 77]= 10; pVetor[ 78]= 11; pVetor[ 79]=  9;
		pVetor[ 80]= 15; pVetor[ 81]= 14; pVetor[ 82]=  7; pVetor[ 83]=  9; pVetor[ 84]= 10; pVetor[ 85]= 14; pVetor[ 86]= 11; pVetor[ 87]=  4; pVetor[ 88]= 14; pVetor[ 89]=  7;
		pVetor[ 90]=  7; pVetor[ 91]= 10; pVetor[ 92]=  6; pVetor[ 93]= 10; pVetor[ 94]= 13; pVetor[ 95]= 10; pVetor[ 96]=  3; pVetor[ 97]=  9; pVetor[ 98]=  7; pVetor[ 99]=  8;
		pVetor[100]=  6; pVetor[101]= 13; pVetor[102]= 12; pVetor[103]=  4; pVetor[104]=  6; pVetor[105]= 13; pVetor[106]= 12; pVetor[107]= 11; pVetor[108]= 13; pVetor[109]= 15;
		pVetor[110]= 13; pVetor[111]=  2; pVetor[112]=  2; pVetor[113]= 13; pVetor[114]= 14; pVetor[115]=  2; pVetor[116]=  8; pVetor[117]= 12; pVetor[118]= 12; pVetor[119]= 14;
		pVetor[120]= 10; pVetor[121]=  8; pVetor[122]= 10; pVetor[123]= 12; pVetor[124]=  9; pVetor[125]=  4; pVetor[126]= 15; pVetor[127]=  1; pVetor[128]= 15; pVetor[129]=  5;
		pVetor[130]=  3; pVetor[131]=  1; pVetor[132]= 14; pVetor[133]=  2; pVetor[134]=  7; pVetor[135]=  5; pVetor[136]=  1; pVetor[137]=  3; pVetor[138]=  1; pVetor[139]= 11;
		pVetor[140]=  1; pVetor[141]=  7; pVetor[142]=  9; pVetor[143]=  3; pVetor[144]=  2; pVetor[145]= 15; pVetor[146]=  5; pVetor[147]=  7; pVetor[148]=  2; pVetor[149]= 13;

    } else if (intProblema == 23) {
		pVetor[  0]=  7; pVetor[  1]=  4; pVetor[  2]=  5; pVetor[  3]= 14; pVetor[  4]=  1; pVetor[  5]=  6; pVetor[  6]=  6; pVetor[  7]=  1; pVetor[  8]= 15; pVetor[  9]=  1;
		pVetor[ 10]= 12; pVetor[ 11]=  5; pVetor[ 12]=  7; pVetor[ 13]=  2; pVetor[ 14]= 15; pVetor[ 15]=  9; pVetor[ 16]= 11; pVetor[ 17]= 12; pVetor[ 18]=  1; pVetor[ 19]= 12;
		pVetor[ 20]=  2; pVetor[ 21]=  2; pVetor[ 22]=  2; pVetor[ 23]=  8; pVetor[ 24]= 11; pVetor[ 25]= 12; pVetor[ 26]=  1; pVetor[ 27]=  3; pVetor[ 28]=  6; pVetor[ 29]= 10;
		pVetor[ 30]= 14; pVetor[ 31]=  9; pVetor[ 32]=  9; pVetor[ 33]=  5; pVetor[ 34]= 14; pVetor[ 35]= 10; pVetor[ 36]= 13; pVetor[ 37]=  8; pVetor[ 38]= 12; pVetor[ 39]=  3;
		pVetor[ 40]=  9; pVetor[ 41]= 12; pVetor[ 42]= 12; pVetor[ 43]=  3; pVetor[ 44]=  6; pVetor[ 45]=  4; pVetor[ 46]=  7; pVetor[ 47]=  6; pVetor[ 48]= 11; pVetor[ 49]=  8;
		pVetor[ 50]=  5; pVetor[ 51]= 13; pVetor[ 52]= 11; pVetor[ 53]= 15; pVetor[ 54]=  9; pVetor[ 55]=  3; pVetor[ 56]=  8; pVetor[ 57]= 11; pVetor[ 58]=  2; pVetor[ 59]=  2;
		pVetor[ 60]= 15; pVetor[ 61]=  7; pVetor[ 62]=  3; pVetor[ 63]=  6; pVetor[ 64]= 10; pVetor[ 65]=  1; pVetor[ 66]=  9; pVetor[ 67]= 14; pVetor[ 68]= 10; pVetor[ 69]= 14;
		pVetor[ 70]= 11; pVetor[ 71]=  1; pVetor[ 72]=  1; pVetor[ 73]= 11; pVetor[ 74]=  8; pVetor[ 75]= 15; pVetor[ 76]=  4; pVetor[ 77]=  5; pVetor[ 78]= 13; pVetor[ 79]= 15;
		pVetor[ 80]=  4; pVetor[ 81]=  6; pVetor[ 82]= 15; pVetor[ 83]= 13; pVetor[ 84]= 13; pVetor[ 85]=  5; pVetor[ 86]=  5; pVetor[ 87]=  4; pVetor[ 88]=  7; pVetor[ 89]=  9;
		pVetor[ 90]=  6; pVetor[ 91]=  3; pVetor[ 92]=  8; pVetor[ 93]=  7; pVetor[ 94]=  7; pVetor[ 95]=  2; pVetor[ 96]= 10; pVetor[ 97]= 15; pVetor[ 98]=  4; pVetor[ 99]=  7;
		pVetor[100]= 10; pVetor[101]= 11; pVetor[102]=  6; pVetor[103]=  9; pVetor[104]=  2; pVetor[105]= 14; pVetor[106]= 12; pVetor[107]=  7; pVetor[108]= 14; pVetor[109]=  4;
		pVetor[110]= 13; pVetor[111]= 10; pVetor[112]= 13; pVetor[113]= 12; pVetor[114]=  5; pVetor[115]= 11; pVetor[116]= 14; pVetor[117]= 13; pVetor[118]=  8; pVetor[119]=  5;
		pVetor[120]=  3; pVetor[121]= 15; pVetor[122]=  4; pVetor[123]=  4; pVetor[124]=  4; pVetor[125]= 13; pVetor[126]= 15; pVetor[127]= 10; pVetor[128]=  3; pVetor[129]= 13;
		pVetor[130]=  8; pVetor[131]=  8; pVetor[132]= 14; pVetor[133]= 10; pVetor[134]=  3; pVetor[135]=  8; pVetor[136]=  3; pVetor[137]=  2; pVetor[138]=  9; pVetor[139]=  6;
		pVetor[140]=  1; pVetor[141]= 14; pVetor[142]= 10; pVetor[143]=  1; pVetor[144]= 12; pVetor[145]=  7; pVetor[146]=  2; pVetor[147]=  9; pVetor[148]=  5; pVetor[149]= 11;

    } else if (intProblema == 24) {
		pVetor[  0]= 14; pVetor[  1]=  7; pVetor[  2]= 14; pVetor[  3]=  4; pVetor[  4]=  4; pVetor[  5]=  1; pVetor[  6]= 13; pVetor[  7]=  6; pVetor[  8]= 13; pVetor[  9]=  7;
		pVetor[ 10]=  4; pVetor[ 11]= 10; pVetor[ 12]=  7; pVetor[ 13]=  3; pVetor[ 14]=  6; pVetor[ 15]=  8; pVetor[ 16]= 12; pVetor[ 17]= 11; pVetor[ 18]=  2; pVetor[ 19]= 13;
		pVetor[ 20]= 13; pVetor[ 21]=  2; pVetor[ 22]=  8; pVetor[ 23]= 11; pVetor[ 24]= 10; pVetor[ 25]= 13; pVetor[ 26]= 15; pVetor[ 27]=  8; pVetor[ 28]=  3; pVetor[ 29]=  1;
		pVetor[ 30]=  7; pVetor[ 31]=  9; pVetor[ 32]= 12; pVetor[ 33]= 15; pVetor[ 34]= 13; pVetor[ 35]=  3; pVetor[ 36]=  1; pVetor[ 37]=  5; pVetor[ 38]=  7; pVetor[ 39]=  9;
		pVetor[ 40]=  1; pVetor[ 41]=  5; pVetor[ 42]=  4; pVetor[ 43]= 12; pVetor[ 44]= 15; pVetor[ 45]=  6; pVetor[ 46]=  5; pVetor[ 47]= 13; pVetor[ 48]=  8; pVetor[ 49]=  8;
		pVetor[ 50]=  8; pVetor[ 51]=  6; pVetor[ 52]= 10; pVetor[ 53]= 13; pVetor[ 54]=  2; pVetor[ 55]=  4; pVetor[ 56]=  7; pVetor[ 57]= 14; pVetor[ 58]=  6; pVetor[ 59]= 12;
		pVetor[ 60]= 12; pVetor[ 61]=  4; pVetor[ 62]=  2; pVetor[ 63]=  9; pVetor[ 64]=  8; pVetor[ 65]=  7; pVetor[ 66]=  4; pVetor[ 67]= 10; pVetor[ 68]= 12; pVetor[ 69]=  5;
		pVetor[ 70]=  3; pVetor[ 71]= 13; pVetor[ 72]=  9; pVetor[ 73]= 10; pVetor[ 74]=  9; pVetor[ 75]= 15; pVetor[ 76]=  3; pVetor[ 77]=  7; pVetor[ 78]= 15; pVetor[ 79]=  6;
		pVetor[ 80]=  6; pVetor[ 81]=  3; pVetor[ 82]=  3; pVetor[ 83]=  2; pVetor[ 84]= 11; pVetor[ 85]=  9; pVetor[ 86]=  8; pVetor[ 87]=  9; pVetor[ 88]= 10; pVetor[ 89]= 10;
		pVetor[ 90]= 15; pVetor[ 91]= 11; pVetor[ 92]= 13; pVetor[ 93]=  8; pVetor[ 94]=  1; pVetor[ 95]= 10; pVetor[ 96]= 14; pVetor[ 97]=  3; pVetor[ 98]=  1; pVetor[ 99]= 15;
		pVetor[100]=  5; pVetor[101]= 15; pVetor[102]=  1; pVetor[103]=  6; pVetor[104]=  5; pVetor[105]= 14; pVetor[106]=  6; pVetor[107]=  4; pVetor[108]=  5; pVetor[109]=  3;
		pVetor[110]=  9; pVetor[111]=  1; pVetor[112]=  5; pVetor[113]=  1; pVetor[114]= 12; pVetor[115]= 11; pVetor[116]= 11; pVetor[117]= 12; pVetor[118]= 14; pVetor[119]= 11;
		pVetor[120]= 11; pVetor[121]= 14; pVetor[122]= 15; pVetor[123]=  7; pVetor[124]= 14; pVetor[125]= 12; pVetor[126]=  9; pVetor[127]=  2; pVetor[128]=  9; pVetor[129]= 14;
		pVetor[130]=  2; pVetor[131]= 12; pVetor[132]= 11; pVetor[133]=  5; pVetor[134]=  7; pVetor[135]=  2; pVetor[136]= 10; pVetor[137]=  1; pVetor[138]=  4; pVetor[139]=  2;
		pVetor[140]= 10; pVetor[141]=  8; pVetor[142]=  6; pVetor[143]= 14; pVetor[144]=  3; pVetor[145]=  5; pVetor[146]=  2; pVetor[147]= 15; pVetor[148]= 11; pVetor[149]=  4;

    } else if (intProblema == 25) {
		pVetor[  0]= 14; pVetor[  1]=  7; pVetor[  2]= 14; pVetor[  3]=  4; pVetor[  4]=  4; pVetor[  5]=  1; pVetor[  6]= 13; pVetor[  7]=  6; pVetor[  8]= 13; pVetor[  9]=  7;
		pVetor[ 10]=  4; pVetor[ 11]= 10; pVetor[ 12]=  7; pVetor[ 13]=  3; pVetor[ 14]=  6; pVetor[ 15]=  8; pVetor[ 16]= 12; pVetor[ 17]= 11; pVetor[ 18]=  2; pVetor[ 19]= 13;
		pVetor[ 20]= 13; pVetor[ 21]=  2; pVetor[ 22]=  8; pVetor[ 23]= 11; pVetor[ 24]= 10; pVetor[ 25]= 13; pVetor[ 26]= 15; pVetor[ 27]=  8; pVetor[ 28]=  3; pVetor[ 29]=  1;
		pVetor[ 30]=  7; pVetor[ 31]=  9; pVetor[ 32]= 12; pVetor[ 33]= 15; pVetor[ 34]= 13; pVetor[ 35]=  3; pVetor[ 36]=  1; pVetor[ 37]=  5; pVetor[ 38]=  7; pVetor[ 39]=  9;
		pVetor[ 40]=  1; pVetor[ 41]=  5; pVetor[ 42]=  4; pVetor[ 43]= 12; pVetor[ 44]= 15; pVetor[ 45]=  6; pVetor[ 46]=  5; pVetor[ 47]= 13; pVetor[ 48]=  8; pVetor[ 49]=  8;
		pVetor[ 50]=  8; pVetor[ 51]=  6; pVetor[ 52]= 10; pVetor[ 53]= 13; pVetor[ 54]=  2; pVetor[ 55]=  4; pVetor[ 56]=  7; pVetor[ 57]= 14; pVetor[ 58]=  6; pVetor[ 59]= 12;
		pVetor[ 60]= 12; pVetor[ 61]=  4; pVetor[ 62]=  2; pVetor[ 63]=  9; pVetor[ 64]=  8; pVetor[ 65]=  7; pVetor[ 66]=  4; pVetor[ 67]= 10; pVetor[ 68]= 12; pVetor[ 69]=  5;
		pVetor[ 70]=  3; pVetor[ 71]= 13; pVetor[ 72]=  9; pVetor[ 73]= 10; pVetor[ 74]=  9; pVetor[ 75]= 15; pVetor[ 76]=  3; pVetor[ 77]=  7; pVetor[ 78]= 15; pVetor[ 79]=  6;
		pVetor[ 80]=  6; pVetor[ 81]=  3; pVetor[ 82]=  3; pVetor[ 83]=  2; pVetor[ 84]= 11; pVetor[ 85]=  9; pVetor[ 86]=  8; pVetor[ 87]=  9; pVetor[ 88]= 10; pVetor[ 89]= 10;
		pVetor[ 90]= 15; pVetor[ 91]= 11; pVetor[ 92]= 13; pVetor[ 93]=  8; pVetor[ 94]=  1; pVetor[ 95]= 10; pVetor[ 96]= 14; pVetor[ 97]=  3; pVetor[ 98]=  1; pVetor[ 99]= 15;
		pVetor[100]=  5; pVetor[101]= 15; pVetor[102]=  1; pVetor[103]=  6; pVetor[104]=  5; pVetor[105]= 14; pVetor[106]=  6; pVetor[107]=  4; pVetor[108]=  5; pVetor[109]=  3;
		pVetor[110]=  9; pVetor[111]=  1; pVetor[112]=  5; pVetor[113]=  1; pVetor[114]= 12; pVetor[115]= 11; pVetor[116]= 11; pVetor[117]= 12; pVetor[118]= 14; pVetor[119]= 11;
		pVetor[120]= 11; pVetor[121]= 14; pVetor[122]= 15; pVetor[123]=  7; pVetor[124]= 14; pVetor[125]= 12; pVetor[126]=  9; pVetor[127]=  2; pVetor[128]=  9; pVetor[129]= 14;
		pVetor[130]=  2; pVetor[131]= 12; pVetor[132]= 11; pVetor[133]=  5; pVetor[134]=  7; pVetor[135]=  2; pVetor[136]= 10; pVetor[137]=  1; pVetor[138]=  4; pVetor[139]=  2;
		pVetor[140]= 10; pVetor[141]=  8; pVetor[142]=  6; pVetor[143]= 14; pVetor[144]=  3; pVetor[145]=  5; pVetor[146]=  2; pVetor[147]= 15; pVetor[148]= 11; pVetor[149]=  4;

    }

    return NULL;
}

void * AtualizarSemente( vector < vector <int> > pSemente, int pRota[] )
{

    for (int num = 0; num < nBestIndividuals; num++){
        for(int sem = 0; sem < JOB*MACHINE; sem++){
            pSemente[num].push_back(pRota[sem]);

        }
    }

    return NULL;

}

// MÉTODO CRIADO PARA REDUZIR REPETIÇÃO DO SEU CÓDIGO
void * AtualizarVetorComVetor( int pVetorP[], int pVetorBase[]){

    int block = -1;
    int intElemento = 0;

    for(int i=0; i<MACHINE*JOB; i++)
    {
        int p = (i%JOB)+1;
        if(p == 1) block++;
        intElemento = (block*JOB) + pVetorBase[i]-1;

        pVetorP[intElemento] = p;
    }

    return NULL;

} // void AtualizarVetorComVetor( * int pvetorP, *int pVetorBase){

// MÉTODO CRIADO PARA REDUZIR REPETIÇÃO DO SEU CÓDIGO
void * AtualizarVetorComArray( int pVetorP[], vector<int> pArrayBase){

    int block = -1;
    int intElemento = 0;

    for(int i=0; i<MACHINE*JOB; i++)
    {
        int p = (i%JOB)+1;
        if(p == 1) block++;
        intElemento = (block*JOB) + pArrayBase[i]-1;

        pVetorP[intElemento ] = p;
    }

    return NULL;

} // void AtualizarVetorComVetor( * int pvetorP, *int pVetorBase){

int SetProblem( int argc, char **argv ){
    int r=0, t=0;
    string strProblema;
    string strInstancia = "1";

    // 1o parametro indica a abordagem a ser explorada
    if (argc > 1) {
        intAbordagem = atoi(argv[1]);
    } else {
        intAbordagem = 2; // 1=Não Factível; 2=FIFO; 3=FIFO + SPT; 4=FIFO + LPT; 5=NEH
    }

    // 2o parametro indica o problema LAxx
    if (argc > 2) {
        strProblema = argv[2];
        intProblema = atoi(argv[2]);

    } else {
        #ifdef PROGRAMA_01
            intProblema = 1; // LA01
            strProblema = "1";

        #endif

        #ifdef PROGRAMA_06
            intProblema = 6;
            strProblema = "6";

        #endif

        #ifdef PROGRAMA_11
            intProblema = 11;
            strProblema = "11";

        #endif

        #ifdef PROGRAMA_16
            intProblema = 16;
            strProblema = "16";

        #endif

        #ifdef PROGRAMA_21
            intProblema = 21;
            strProblema = "21";

        #endif
    }

    // 3o parametro indica a quantidade de iterações
    if (argc > 3) {
        outIter = atoi(argv[3]);
    }

    if (argc > 4 ){
        intInstancia = atoi(argv[4]);
        strInstancia = argv[4];

    }


    switch (intAbordagem) {
    case 1:
        strSemente = "Nao Factivel";
        break;
    case 2:
        strSemente = "FIFO";
        break;
    case 3:
        strSemente = "FIFO+SPT";
        break;
    case 4:
        strSemente = "FIFO+LPT";
        break;
    case 5:
        strSemente = "NEH_CP";
        break;
    }

    Con="\\LA" + strProblema + "_Convergence_" + strSemente + "_" + strInstancia + ".txt";
    His="\\LA" + strProblema + "_History.csv";
    Bsi="\\LA" + strProblema + "_BestSequenceIdentified_" + strSemente + "_" + strInstancia + ".txt";
    Sch="\\LA" + strProblema + "_Scheduling_" + strSemente + "_" + strInstancia + ".csv";

    ArqHis = PastaDesktop + His;
    ArqCon = PastaDesktop + Con;
    ArqBsi = PastaDesktop + Bsi;
    ArqSch = PastaDesktop + Sch;

    #ifdef PROGRAMA_01

        if (intProblema == 1){
            intMakeSpanOtimo = 666;

            R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3;       T[t++]=21; T[t++]=53; T[t++]=95; T[t++]=55; T[t++]=34;
            R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=3; R[r++]=2;       T[t++]=21; T[t++]=52; T[t++]=16; T[t++]=26; T[t++]=71;
            R[r++]=4; R[r++]=5; R[r++]=2; R[r++]=3; R[r++]=1;       T[t++]=39; T[t++]=98; T[t++]=42; T[t++]=31; T[t++]=12;
            R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=4;       T[t++]=77; T[t++]=55; T[t++]=79; T[t++]=66; T[t++]=77;
            R[r++]=1; R[r++]=4; R[r++]=3; R[r++]=2; R[r++]=5;       T[t++]=83; T[t++]=34; T[t++]=64; T[t++]=19; T[t++]=37;
            R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4;       T[t++]=54; T[t++]=43; T[t++]=79; T[t++]=92; T[t++]=62;
            R[r++]=4; R[r++]=5; R[r++]=2; R[r++]=3; R[r++]=1;       T[t++]=69; T[t++]=77; T[t++]=87; T[t++]=87; T[t++]=93;
            R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=4; R[r++]=5;       T[t++]=38; T[t++]=60; T[t++]=41; T[t++]=24; T[t++]=83;
            R[r++]=4; R[r++]=2; R[r++]=5; R[r++]=1; R[r++]=3;       T[t++]=17; T[t++]=49; T[t++]=25; T[t++]=44; T[t++]=98;
            R[r++]=5; R[r++]=4; R[r++]=3; R[r++]=2; R[r++]=1;       T[t++]=77; T[t++]=79; T[t++]=43; T[t++]=75; T[t++]=96;

        } else if(intProblema == 2) {
            intMakeSpanOtimo = 655;

            R[r++]=1; R[r++]=4; R[r++]=2; R[r++]=5; R[r++]=3;       T[t++]=20; T[t++]=87; T[t++]=31; T[t++]=76; T[t++]=17;
            R[r++]=5; R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=4;       T[t++]=25; T[t++]=32; T[t++]=24; T[t++]=18; T[t++]=81;
            R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4;       T[t++]=72; T[t++]=23; T[t++]=28; T[t++]=58; T[t++]=99;
            R[r++]=3; R[r++]=2; R[r++]=5; R[r++]=1; R[r++]=4;       T[t++]=86; T[t++]=76; T[t++]=97; T[t++]=45; T[t++]=90;
            R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=3; R[r++]=2;       T[t++]=27; T[t++]=42; T[t++]=48; T[t++]=17; T[t++]=46;
            R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3;       T[t++]=67; T[t++]=98; T[t++]=48; T[t++]=27; T[t++]=62;
            R[r++]=5; R[r++]=2; R[r++]=4; R[r++]=1; R[r++]=3;       T[t++]=28; T[t++]=12; T[t++]=19; T[t++]=80; T[t++]=50;
            R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=4; R[r++]=5;       T[t++]=63; T[t++]=94; T[t++]=98; T[t++]=50; T[t++]=80;
            R[r++]=5; R[r++]=1; R[r++]=3; R[r++]=2; R[r++]=4;       T[t++]=14; T[t++]=75; T[t++]=50; T[t++]=41; T[t++]=55;
            R[r++]=5; R[r++]=3; R[r++]=2; R[r++]=4; R[r++]=1;       T[t++]=72; T[t++]=18; T[t++]=37; T[t++]=79; T[t++]=61;

        } else if(intProblema == 3) {
            intMakeSpanOtimo = 597;

            R[r++]=2; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=4;       T[t++]=23; T[t++]=45; T[t++]=82; T[t++]=84; T[t++]=38;
            R[r++]=3; R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=4;       T[t++]=21; T[t++]=29; T[t++]=18; T[t++]=41; T[t++]=50;
            R[r++]=3; R[r++]=4; R[r++]=5; R[r++]=1; R[r++]=2;       T[t++]=38; T[t++]=54; T[t++]=16; T[t++]=52; T[t++]=52;
            R[r++]=5; R[r++]=1; R[r++]=3; R[r++]=2; R[r++]=4;       T[t++]=37; T[t++]=54; T[t++]=74; T[t++]=62; T[t++]=57;
            R[r++]=5; R[r++]=1; R[r++]=2; R[r++]=4; R[r++]=3;       T[t++]=57; T[t++]=81; T[t++]=61; T[t++]=68; T[t++]=30;
            R[r++]=5; R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=4;       T[t++]=81; T[t++]=79; T[t++]=89; T[t++]=89; T[t++]=11;
            R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=2;       T[t++]=33; T[t++]=20; T[t++]=91; T[t++]=20; T[t++]=66;
            R[r++]=5; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=4;       T[t++]=24; T[t++]=84; T[t++]=32; T[t++]=55; T[t++]= 8;
            R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=3; R[r++]=2;       T[t++]=56; T[t++]= 7; T[t++]=54; T[t++]=64; T[t++]=39;
            R[r++]=5; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=4;       T[t++]=40; T[t++]=83; T[t++]=19; T[t++]= 8; T[t++]= 7;

        } else if(intProblema == 4) {
            intMakeSpanOtimo = 590;

            R[r++]=1; R[r++]=3; R[r++]=4; R[r++]=5; R[r++]=2;       T[t++]=12; T[t++]=94; T[t++]=92; T[t++]=91; T[t++]= 7;
            R[r++]=2; R[r++]=4; R[r++]=5; R[r++]=3; R[r++]=1;       T[t++]=19; T[t++]=11; T[t++]=66; T[t++]=21; T[t++]=87;
            R[r++]=2; R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=3;       T[t++]=14; T[t++]=75; T[t++]=13; T[t++]=16; T[t++]=20;
            R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=2;       T[t++]=95; T[t++]=66; T[t++]= 7; T[t++]= 7; T[t++]=77;
            R[r++]=2; R[r++]=4; R[r++]=5; R[r++]=1; R[r++]=3;       T[t++]=45; T[t++]= 6; T[t++]=89; T[t++]=15; T[t++]=34;
            R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=2;       T[t++]=77; T[t++]=20; T[t++]=76; T[t++]=88; T[t++]=53;
            R[r++]=3; R[r++]=2; R[r++]=1; R[r++]=4; R[r++]=5;       T[t++]=74; T[t++]=88; T[t++]=52; T[t++]=27; T[t++]= 9;
            R[r++]=2; R[r++]=4; R[r++]=1; R[r++]=5; R[r++]=3;       T[t++]=88; T[t++]=69; T[t++]=62; T[t++]=98; T[t++]=52;
            R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=2; R[r++]=4;       T[t++]=61; T[t++]= 9; T[t++]=62; T[t++]=52; T[t++]=90;
            R[r++]=3; R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=1;       T[t++]=54; T[t++]= 5; T[t++]=59; T[t++]=15; T[t++]=88;

        } else if(intProblema == 5) {
            intMakeSpanOtimo = 593;

            R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=4;       T[t++]=72; T[t++]=87; T[t++]=95; T[t++]=66; T[t++]=60;
            R[r++]=5; R[r++]=4; R[r++]=1; R[r++]=3; R[r++]=2;       T[t++]= 5; T[t++]=35; T[t++]=48; T[t++]=39; T[t++]=54;
            R[r++]=2; R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5;       T[t++]=46; T[t++]=20; T[t++]=21; T[t++]=97; T[t++]=55;
            R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=2; R[r++]=3;       T[t++]=59; T[t++]=19; T[t++]=46; T[t++]=34; T[t++]=37;
            R[r++]=5; R[r++]=3; R[r++]=4; R[r++]=2; R[r++]=1;       T[t++]=23; T[t++]=73; T[t++]=25; T[t++]=24; T[t++]=28;
            R[r++]=4; R[r++]=1; R[r++]=5; R[r++]=2; R[r++]=3;       T[t++]=28; T[t++]=45; T[t++]= 5; T[t++]=78; T[t++]=83;
            R[r++]=1; R[r++]=4; R[r++]=2; R[r++]=5; R[r++]=3;       T[t++]=53; T[t++]=71; T[t++]=37; T[t++]=29; T[t++]=12;
            R[r++]=5; R[r++]=3; R[r++]=4; R[r++]=2; R[r++]=1;       T[t++]=12; T[t++]=87; T[t++]=33; T[t++]=55; T[t++]=38;
            R[r++]=3; R[r++]=4; R[r++]=2; R[r++]=1; R[r++]=5;       T[t++]=49; T[t++]=83; T[t++]=40; T[t++]=48; T[t++]= 7;
            R[r++]=3; R[r++]=4; R[r++]=1; R[r++]=5; R[r++]=2;       T[t++]=65; T[t++]=17; T[t++]=90; T[t++]=27; T[t++]=23;

        } else {
            printf( "Problema deve estar entre LA01 e LA05");
            getchar();
            return 0; // 0 = erro

        }

    #endif

    #ifdef PROGRAMA_06
        if (intProblema == 6){
            intMakeSpanOtimo = 926;

            R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4;       T[t++]=21; T[t++]=34; T[t++]=95; T[t++]=53; T[t++]=55;
            R[r++]=4; R[r++]=5; R[r++]=2; R[r++]=3; R[r++]=1;       T[t++]=52; T[t++]=16; T[t++]=71; T[t++]=26; T[t++]=21;
            R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=4; R[r++]=5;       T[t++]=31; T[t++]=12; T[t++]=42; T[t++]=39; T[t++]=98;
            R[r++]=4; R[r++]=2; R[r++]=5; R[r++]=1; R[r++]=3;       T[t++]=77; T[t++]=77; T[t++]=79; T[t++]=55; T[t++]=66;
            R[r++]=5; R[r++]=4; R[r++]=3; R[r++]=2; R[r++]=1;       T[t++]=37; T[t++]=34; T[t++]=64; T[t++]=19; T[t++]=83;
            R[r++]=3; R[r++]=2; R[r++]=1; R[r++]=4; R[r++]=5;       T[t++]=43; T[t++]=54; T[t++]=92; T[t++]=62; T[t++]=79;
            R[r++]=1; R[r++]=4; R[r++]=2; R[r++]=5; R[r++]=3;       T[t++]=93; T[t++]=69; T[t++]=87; T[t++]=77; T[t++]=87;
            R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=4;       T[t++]=60; T[t++]=41; T[t++]=38; T[t++]=83; T[t++]=24;
            R[r++]=3; R[r++]=4; R[r++]=5; R[r++]=1; R[r++]=2;       T[t++]=98; T[t++]=17; T[t++]=25; T[t++]=44; T[t++]=49;
            R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=3;       T[t++]=96; T[t++]=77; T[t++]=79; T[t++]=75; T[t++]=43;
            R[r++]=5; R[r++]=3; R[r++]=1; R[r++]=4; R[r++]=2;       T[t++]=28; T[t++]=35; T[t++]=95; T[t++]=76; T[t++]= 7;
            R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=2; R[r++]=4;       T[t++]=61; T[t++]=10; T[t++]=95; T[t++]= 9; T[t++]=35;
            R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=1;       T[t++]=59; T[t++]=16; T[t++]=91; T[t++]=59; T[t++]=46;
            R[r++]=5; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=4;       T[t++]=43; T[t++]=52; T[t++]=28; T[t++]=27; T[t++]=50;
            R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=4;       T[t++]=87; T[t++]=45; T[t++]=39; T[t++]= 9; T[t++]=41;

        } else if(intProblema == 7) {
            intMakeSpanOtimo = 890;

            R[r++]=1; R[r++]=5; R[r++]=2; R[r++]=4; R[r++]=3; 		T[t++]=47; T[t++]=57; T[t++]=71; T[t++]=96; T[t++]=14;
            R[r++]=1; R[r++]=2; R[r++]=5; R[r++]=4; R[r++]=3; 		T[t++]=75; T[t++]=60; T[t++]=22; T[t++]=79; T[t++]=65;
            R[r++]=4; R[r++]=1; R[r++]=3; R[r++]=2; R[r++]=5; 		T[t++]=32; T[t++]=33; T[t++]=69; T[t++]=31; T[t++]=58;
            R[r++]=1; R[r++]=2; R[r++]=5; R[r++]=4; R[r++]=3; 		T[t++]=44; T[t++]=34; T[t++]=51; T[t++]=58; T[t++]=47;
            R[r++]=4; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=5; 		T[t++]=29; T[t++]=44; T[t++]=62; T[t++]=17; T[t++]= 8;
            R[r++]=2; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=4; 		T[t++]=15; T[t++]=40; T[t++]=97; T[t++]=38; T[t++]=66;
            R[r++]=3; R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=4; 		T[t++]=58; T[t++]=39; T[t++]=57; T[t++]=20; T[t++]=50;
            R[r++]=3; R[r++]=4; R[r++]=5; R[r++]=1; R[r++]=2; 		T[t++]=57; T[t++]=32; T[t++]=87; T[t++]=63; T[t++]=21;
            R[r++]=5; R[r++]=1; R[r++]=3; R[r++]=2; R[r++]=4; 		T[t++]=56; T[t++]=84; T[t++]=90; T[t++]=85; T[t++]=61;
            R[r++]=5; R[r++]=1; R[r++]=2; R[r++]=4; R[r++]=3; 		T[t++]=15; T[t++]=20; T[t++]=67; T[t++]=30; T[t++]=70;
            R[r++]=5; R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=4; 		T[t++]=84; T[t++]=82; T[t++]=23; T[t++]=45; T[t++]=38;
            R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=2; 		T[t++]=50; T[t++]=21; T[t++]=18; T[t++]=41; T[t++]=29;
            R[r++]=5; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=4; 		T[t++]=16; T[t++]=52; T[t++]=52; T[t++]=38; T[t++]=54;
            R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=3; R[r++]=2; 		T[t++]=37; T[t++]=54; T[t++]=57; T[t++]=74; T[t++]=62;
            R[r++]=5; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=4; 		T[t++]=57; T[t++]=61; T[t++]=81; T[t++]=30; T[t++]=68;

        } else if(intProblema ==8) {
            intMakeSpanOtimo =863;

            R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=2;       T[t++]=92; T[t++]=94; T[t++]=12; T[t++]=91; T[t++]=07;
            R[r++]=3; R[r++]=2; R[r++]=1; R[r++]=4; R[r++]=5;       T[t++]=21; T[t++]=19; T[t++]=87; T[t++]=11; T[t++]=66;
            R[r++]=2; R[r++]=4; R[r++]=1; R[r++]=5; R[r++]=3;       T[t++]=14; T[t++]=13; T[t++]=75; T[t++]=16; T[t++]=20;
            R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=2; R[r++]=4;       T[t++]=95; T[t++]=66; T[t++]=07; T[t++]=77; T[t++]=07;
            R[r++]=3; R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=1;       T[t++]=34; T[t++]=89; T[t++]=06; T[t++]=45; T[t++]=15;
            R[r++]=5; R[r++]=4; R[r++]=3; R[r++]=2; R[r++]=1;       T[t++]=88; T[t++]=77; T[t++]=20; T[t++]=53; T[t++]=76;
            R[r++]=5; R[r++]=4; R[r++]=1; R[r++]=2; R[r++]=3;       T[t++]= 9; T[t++]=27; T[t++]=52; T[t++]=88; T[t++]=74;
            R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=5;       T[t++]=69; T[t++]=52; T[t++]=62; T[t++]=88; T[t++]=98;
            R[r++]=4; R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=2;       T[t++]=90; T[t++]=62; T[t++]= 9; T[t++]=61; T[t++]=52;
            R[r++]=5; R[r++]=3; R[r++]=4; R[r++]=1; R[r++]=2;       T[t++]= 5; T[t++]=54; T[t++]=59; T[t++]=88; T[t++]=15;
            R[r++]=1; R[r++]=2; R[r++]=5; R[r++]=4; R[r++]=3;       T[t++]=41; T[t++]=50; T[t++]=78; T[t++]=53; T[t++]=23;
            R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=4; R[r++]=2;       T[t++]=38; T[t++]=72; T[t++]=91; T[t++]=68; T[t++]=71;
            R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=3; R[r++]=2;       T[t++]=45; T[t++]=95; T[t++]=52; T[t++]=25; T[t++]= 6;
            R[r++]=4; R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=3;       T[t++]=30; T[t++]=66; T[t++]=23; T[t++]=36; T[t++]=17;
            R[r++]=3; R[r++]=1; R[r++]=4; R[r++]=2; R[r++]=5;       T[t++]=95; T[t++]=71; T[t++]=76; T[t++]= 8; T[t++]=88;

        } else if(intProblema ==9) {
            intMakeSpanOtimo =951;

            R[r++]=2; R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5;       T[t++]=66; T[t++]=85; T[t++]=84; T[t++]=62; T[t++]=19;
            R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1;       T[t++]=59; T[t++]=64; T[t++]=46; T[t++]=13; T[t++]=25;
            R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=1;       T[t++]=88; T[t++]=80; T[t++]=73; T[t++]=53; T[t++]=41;
            R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=4; R[r++]=5;       T[t++]=14; T[t++]=67; T[t++]=57; T[t++]=74; T[t++]=47;
            R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=4; R[r++]=2;       T[t++]=84; T[t++]=64; T[t++]=41; T[t++]=84; T[t++]=78;
            R[r++]=1; R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=5;       T[t++]=63; T[t++]=28; T[t++]=46; T[t++]=26; T[t++]=52;
            R[r++]=4; R[r++]=3; R[r++]=5; R[r++]=2; R[r++]=1;       T[t++]=10; T[t++]=17; T[t++]=73; T[t++]=11; T[t++]=64;
            R[r++]=3; R[r++]=2; R[r++]=4; R[r++]=5; R[r++]=1;       T[t++]=67; T[t++]=97; T[t++]=95; T[t++]=38; T[t++]=85;
            R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=2; R[r++]=4;       T[t++]=95; T[t++]=46; T[t++]=59; T[t++]=65; T[t++]=93;
            R[r++]=3; R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=1;       T[t++]=43; T[t++]=85; T[t++]=32; T[t++]=85; T[t++]=60;
            R[r++]=5; R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=2;       T[t++]=49; T[t++]=41; T[t++]=61; T[t++]=66; T[t++]=90;
            R[r++]=2; R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=3;       T[t++]=17; T[t++]=23; T[t++]=70; T[t++]=99; T[t++]=49;
            R[r++]=5; R[r++]=4; R[r++]=1; R[r++]=2; R[r++]=3;       T[t++]=40; T[t++]=73; T[t++]=73; T[t++]=98; T[t++]=68;
            R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=1; R[r++]=5;       T[t++]=57; T[t++]= 9; T[t++]= 7; T[t++]=13; T[t++]=98;
            R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=4;       T[t++]=37; T[t++]=85; T[t++]=17; T[t++]=79; T[t++]=41;

        } else if(intProblema ==10) {
            intMakeSpanOtimo =958;

            R[r++]=2; R[r++]=3; R[r++]=4; R[r++]=1; R[r++]=5;       T[t++]=58; T[t++]=44; T[t++]= 5; T[t++]= 9; T[t++]=58;
            R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3;       T[t++]=89; T[t++]=97; T[t++]=96; T[t++]=77; T[t++]=84;
            R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=4;       T[t++]=77; T[t++]=87; T[t++]=81; T[t++]=39; T[t++]=85;
            R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=1; R[r++]=5;       T[t++]=57; T[t++]=21; T[t++]=31; T[t++]=15; T[t++]=73;
            R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=4; R[r++]=5;       T[t++]=48; T[t++]=40; T[t++]=49; T[t++]=70; T[t++]=71;
            R[r++]=4; R[r++]=5; R[r++]=3; R[r++]=1; R[r++]=2;       T[t++]=34; T[t++]=82; T[t++]=80; T[t++]=10; T[t++]=22;
            R[r++]=2; R[r++]=5; R[r++]=1; R[r++]=3; R[r++]=4;       T[t++]=91; T[t++]=75; T[t++]=55; T[t++]=17; T[t++]= 7;
            R[r++]=3; R[r++]=4; R[r++]=2; R[r++]=5; R[r++]=1;       T[t++]=62; T[t++]=47; T[t++]=72; T[t++]=35; T[t++]=11;
            R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=2; R[r++]=3;       T[t++]=64; T[t++]=75; T[t++]=50; T[t++]=90; T[t++]=94;
            R[r++]=3; R[r++]=5; R[r++]=4; R[r++]=1; R[r++]=2;       T[t++]=67; T[t++]=20; T[t++]=15; T[t++]=12; T[t++]=71;
            R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3; R[r++]=2;       T[t++]=52; T[t++]=93; T[t++]=68; T[t++]=29; T[t++]=57;
            R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=5; R[r++]=4;       T[t++]=70; T[t++]=58; T[t++]=93; T[t++]= 7; T[t++]=77;
            R[r++]=4; R[r++]=3; R[r++]=2; R[r++]=5; R[r++]=1;       T[t++]=27; T[t++]=82; T[t++]=63; T[t++]= 6; T[t++]=95;
            R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4;       T[t++]=87; T[t++]=56; T[t++]=36; T[t++]=26; T[t++]=48;
            R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=2;       T[t++]=76; T[t++]=36; T[t++]=36; T[t++]=15; T[t++]= 8;

        } else {
            printf( "Problema deve estar entre LA06 e LA10");
            getchar();
            return 0; // 0 = erro

        }

    #endif

    #ifdef PROGRAMA_11
        if (intProblema == 11){
            intMakeSpanOtimo = 1222;

			R[r++]=4; R[r++]=5; R[r++]=3; R[r++]=1; R[r++]=2; 		T[t++]= 5; T[t++]=58; T[t++]=44; T[t++]= 9; T[t++]=58;
			R[r++]=2; R[r++]=5; R[r++]=1; R[r++]=3; R[r++]=4; 		T[t++]=89; T[t++]=96; T[t++]=97; T[t++]=84; T[t++]=77;
			R[r++]=3; R[r++]=4; R[r++]=2; R[r++]=5; R[r++]=1; 		T[t++]=81; T[t++]=85; T[t++]=87; T[t++]=39; T[t++]=77;
			R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=2; R[r++]=3; 		T[t++]=15; T[t++]=57; T[t++]=73; T[t++]=21; T[t++]=31;
			R[r++]=3; R[r++]=5; R[r++]=4; R[r++]=1; R[r++]=2; 		T[t++]=48; T[t++]=71; T[t++]=70; T[t++]=40; T[t++]=49;
			R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3; R[r++]=2; 		T[t++]=10; T[t++]=82; T[t++]=34; T[t++]=80; T[t++]=22;
			R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=5; R[r++]=4; 		T[t++]=17; T[t++]=55; T[t++]=91; T[t++]=75; T[t++]= 7;
			R[r++]=4; R[r++]=3; R[r++]=2; R[r++]=5; R[r++]=1; 		T[t++]=47; T[t++]=62; T[t++]=72; T[t++]=35; T[t++]=11;
			R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4; 		T[t++]=90; T[t++]=94; T[t++]=50; T[t++]=64; T[t++]=75;
			R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=2; 		T[t++]=15; T[t++]=67; T[t++]=12; T[t++]=20; T[t++]=71;
			R[r++]=5; R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=4; 		T[t++]=93; T[t++]=29; T[t++]=52; T[t++]=57; T[t++]=68;
			R[r++]=4; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=5; 		T[t++]=77; T[t++]=93; T[t++]=58; T[t++]=70; T[t++]= 7;
			R[r++]=2; R[r++]=4; R[r++]=1; R[r++]=5; R[r++]=3; 		T[t++]=63; T[t++]=27; T[t++]=95; T[t++]= 6; T[t++]=82;
			R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=3; R[r++]=2; 		T[t++]=36; T[t++]=26; T[t++]=48; T[t++]=56; T[t++]=87;
			R[r++]=3; R[r++]=2; R[r++]=5; R[r++]=4; R[r++]=1; 		T[t++]=36; T[t++]= 8; T[t++]=15; T[t++]=76; T[t++]=36;
			R[r++]=5; R[r++]=2; R[r++]=4; R[r++]=1; R[r++]=3; 		T[t++]=78; T[t++]=84; T[t++]=41; T[t++]=30; T[t++]=76;
			R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3; 		T[t++]=78; T[t++]=75; T[t++]=88; T[t++]=13; T[t++]=81;
			R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=2; R[r++]=4; 		T[t++]=54; T[t++]=40; T[t++]=13; T[t++]=82; T[t++]=29;
			R[r++]=2; R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=3; 		T[t++]=26; T[t++]=82; T[t++]=52; T[t++]= 6; T[t++]= 6;
			R[r++]=4; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=5; 		T[t++]=54; T[t++]=64; T[t++]=54; T[t++]=32; T[t++]=88;

        } else if(intProblema == 12) {
            intMakeSpanOtimo = 1039;

			R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=4; 		T[t++]=23; T[t++]=82; T[t++]=84; T[t++]=45; T[t++]=38;
			R[r++]=4; R[r++]=5; R[r++]=2; R[r++]=1; R[r++]=3; 		T[t++]=50; T[t++]=41; T[t++]=29; T[t++]=18; T[t++]=21;
			R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=1; 		T[t++]=16; T[t++]=54; T[t++]=52; T[t++]=38; T[t++]=52;
			R[r++]=2; R[r++]=4; R[r++]=5; R[r++]=3; R[r++]=1; 		T[t++]=62; T[t++]=57; T[t++]=37; T[t++]=74; T[t++]=54;
			R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=1; R[r++]=5; 		T[t++]=68; T[t++]=61; T[t++]=30; T[t++]=81; T[t++]=57;
			R[r++]=2; R[r++]=3; R[r++]=4; R[r++]=1; R[r++]=5; 		T[t++]=89; T[t++]=89; T[t++]=11; T[t++]=79; T[t++]=81;
			R[r++]=2; R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=3; 		T[t++]=66; T[t++]=91; T[t++]=33; T[t++]=20; T[t++]=20;
			R[r++]=4; R[r++]=5; R[r++]=3; R[r++]=1; R[r++]=2; 		T[t++]= 8; T[t++]=24; T[t++]=55; T[t++]=32; T[t++]=84;
			R[r++]=1; R[r++]=3; R[r++]=2; R[r++]=5; R[r++]=4; 		T[t++]= 7; T[t++]=64; T[t++]=39; T[t++]=56; T[t++]=54;
			R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3; R[r++]=2; 		T[t++]=19; T[t++]=40; T[t++]= 7; T[t++]= 8; T[t++]=83;
			R[r++]=1; R[r++]=3; R[r++]=4; R[r++]=5; R[r++]=2; 		T[t++]=63; T[t++]=64; T[t++]=91; T[t++]=40; T[t++]= 6;
			R[r++]=2; R[r++]=4; R[r++]=5; R[r++]=3; R[r++]=1; 		T[t++]=42; T[t++]=61; T[t++]=15; T[t++]=98; T[t++]=74;
			R[r++]=2; R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=3; 		T[t++]=80; T[t++]=26; T[t++]=75; T[t++]= 6; T[t++]=87;
			R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=2; 		T[t++]=39; T[t++]=22; T[t++]=75; T[t++]=24; T[t++]=44;
			R[r++]=2; R[r++]=4; R[r++]=5; R[r++]=1; R[r++]=3; 		T[t++]=15; T[t++]=79; T[t++]= 8; T[t++]=12; T[t++]=20;
			R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=2; 		T[t++]=26; T[t++]=43; T[t++]=80; T[t++]=22; T[t++]=61;
			R[r++]=3; R[r++]=2; R[r++]=1; R[r++]=4; R[r++]=5; 		T[t++]=62; T[t++]=36; T[t++]=63; T[t++]=96; T[t++]=40;
			R[r++]=2; R[r++]=4; R[r++]=1; R[r++]=5; R[r++]=3; 		T[t++]=33; T[t++]=18; T[t++]=22; T[t++]= 5; T[t++]=10;
			R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=2; R[r++]=4; 		T[t++]=64; T[t++]=64; T[t++]=89; T[t++]=96; T[t++]=95;
			R[r++]=3; R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=1; 		T[t++]=18; T[t++]=23; T[t++]=15; T[t++]=38; T[t++]= 8;

        } else if(intProblema ==13) {
            intMakeSpanOtimo =1150;

			R[r++]=4; R[r++]=1; R[r++]=2; R[r++]=5; R[r++]=3; 		T[t++]=60; T[t++]=87; T[t++]=72; T[t++]=95; T[t++]=66;
			R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=4; R[r++]=5; 		T[t++]=54; T[t++]=48; T[t++]=39; T[t++]=35; T[t++]= 5;
			R[r++]=4; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=5; 		T[t++]=20; T[t++]=46; T[t++]=97; T[t++]=21; T[t++]=55;
			R[r++]=3; R[r++]=1; R[r++]=4; R[r++]=2; R[r++]=5; 		T[t++]=37; T[t++]=59; T[t++]=19; T[t++]=34; T[t++]=46;
			R[r++]=3; R[r++]=4; R[r++]=2; R[r++]=1; R[r++]=5; 		T[t++]=73; T[t++]=25; T[t++]=24; T[t++]=28; T[t++]=23;
			R[r++]=2; R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5; 		T[t++]=78; T[t++]=28; T[t++]=83; T[t++]=45; T[t++]= 5;
			R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1; 		T[t++]=71; T[t++]=37; T[t++]=12; T[t++]=29; T[t++]=53;
			R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=1; 		T[t++]=12; T[t++]=33; T[t++]=55; T[t++]=87; T[t++]=38;
			R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=4; R[r++]=5; 		T[t++]=48; T[t++]=40; T[t++]=49; T[t++]=83; T[t++]= 7;
			R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=4; R[r++]=2; 		T[t++]=90; T[t++]=27; T[t++]=65; T[t++]=17; T[t++]=23;
			R[r++]=1; R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=5; 		T[t++]=62; T[t++]=85; T[t++]=66; T[t++]=84; T[t++]=19;
			R[r++]=4; R[r++]=3; R[r++]=5; R[r++]=2; R[r++]=1; 		T[t++]=59; T[t++]=46; T[t++]=13; T[t++]=64; T[t++]=25;
			R[r++]=3; R[r++]=2; R[r++]=4; R[r++]=5; R[r++]=1; 		T[t++]=53; T[t++]=73; T[t++]=80; T[t++]=88; T[t++]=41;
			R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=2; R[r++]=4; 		T[t++]=57; T[t++]=47; T[t++]=14; T[t++]=67; T[t++]=74;
			R[r++]=3; R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=1; 		T[t++]=41; T[t++]=64; T[t++]=84; T[t++]=78; T[t++]=84;
			R[r++]=5; R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=2; 		T[t++]=52; T[t++]=28; T[t++]=26; T[t++]=63; T[t++]=46;
			R[r++]=2; R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=3; 		T[t++]=11; T[t++]=64; T[t++]=10; T[t++]=73; T[t++]=17;
			R[r++]=5; R[r++]=4; R[r++]=1; R[r++]=2; R[r++]=3; 		T[t++]=38; T[t++]=95; T[t++]=85; T[t++]=97; T[t++]=67;
			R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=1; R[r++]=5; 		T[t++]=93; T[t++]=65; T[t++]=95; T[t++]=59; T[t++]=46;
			R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=4; 		T[t++]=60; T[t++]=85; T[t++]=43; T[t++]=85; T[t++]=32;

        } else if(intProblema ==14) {
            intMakeSpanOtimo =1292;

			R[r++]=4; R[r++]=5; R[r++]=3; R[r++]=1; R[r++]=2; 		T[t++]= 5; T[t++]=58; T[t++]=44; T[t++]= 9; T[t++]=58;
			R[r++]=2; R[r++]=5; R[r++]=1; R[r++]=3; R[r++]=4; 		T[t++]=89; T[t++]=96; T[t++]=97; T[t++]=84; T[t++]=77;
			R[r++]=3; R[r++]=4; R[r++]=2; R[r++]=5; R[r++]=1; 		T[t++]=81; T[t++]=85; T[t++]=87; T[t++]=39; T[t++]=77;
			R[r++]=1; R[r++]=4; R[r++]=5; R[r++]=2; R[r++]=3; 		T[t++]=15; T[t++]=57; T[t++]=73; T[t++]=21; T[t++]=31;
			R[r++]=3; R[r++]=5; R[r++]=4; R[r++]=1; R[r++]=2; 		T[t++]=48; T[t++]=71; T[t++]=70; T[t++]=40; T[t++]=49;
			R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3; R[r++]=2; 		T[t++]=10; T[t++]=82; T[t++]=34; T[t++]=80; T[t++]=22;
			R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=5; R[r++]=4; 		T[t++]=17; T[t++]=55; T[t++]=91; T[t++]=75; T[t++]= 7;
			R[r++]=4; R[r++]=3; R[r++]=2; R[r++]=5; R[r++]=1; 		T[t++]=47; T[t++]=62; T[t++]=72; T[t++]=35; T[t++]=11;
			R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4; 		T[t++]=90; T[t++]=94; T[t++]=50; T[t++]=64; T[t++]=75;
			R[r++]=4; R[r++]=3; R[r++]=1; R[r++]=5; R[r++]=2; 		T[t++]=15; T[t++]=67; T[t++]=12; T[t++]=20; T[t++]=71;
			R[r++]=5; R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=4; 		T[t++]=93; T[t++]=29; T[t++]=52; T[t++]=57; T[t++]=68;
			R[r++]=4; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=5; 		T[t++]=77; T[t++]=93; T[t++]=58; T[t++]=70; T[t++]= 7;
			R[r++]=2; R[r++]=4; R[r++]=1; R[r++]=5; R[r++]=3; 		T[t++]=63; T[t++]=27; T[t++]=95; T[t++]= 6; T[t++]=82;
			R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=3; R[r++]=2; 		T[t++]=36; T[t++]=26; T[t++]=48; T[t++]=56; T[t++]=87;
			R[r++]=3; R[r++]=2; R[r++]=5; R[r++]=4; R[r++]=1; 		T[t++]=36; T[t++]= 8; T[t++]=15; T[t++]=76; T[t++]=36;
			R[r++]=5; R[r++]=2; R[r++]=4; R[r++]=1; R[r++]=3; 		T[t++]=78; T[t++]=84; T[t++]=41; T[t++]=30; T[t++]=76;
			R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3; 		T[t++]=78; T[t++]=75; T[t++]=88; T[t++]=13; T[t++]=81;
			R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=2; R[r++]=4; 		T[t++]=54; T[t++]=40; T[t++]=13; T[t++]=82; T[t++]=29;
			R[r++]=2; R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=3; 		T[t++]=26; T[t++]=82; T[t++]=52; T[t++]= 6; T[t++]= 6;
			R[r++]=4; R[r++]=2; R[r++]=1; R[r++]=3; R[r++]=5; 		T[t++]=54; T[t++]=64; T[t++]=54; T[t++]=32; T[t++]=88;

        } else if(intProblema ==15) {
            intMakeSpanOtimo =1207;

			R[r++]=1; R[r++]=3; R[r++]=2; R[r++]=4; R[r++]=5; 		T[t++]= 6; T[t++]=40; T[t++]=81; T[t++]=37; T[t++]=19;
			R[r++]=3; R[r++]=4; R[r++]=1; R[r++]=5; R[r++]=2; 		T[t++]=40; T[t++]=32; T[t++]=55; T[t++]=81; T[t++]= 9;
			R[r++]=2; R[r++]=5; R[r++]=3; R[r++]=4; R[r++]=1; 		T[t++]=46; T[t++]=65; T[t++]=70; T[t++]=55; T[t++]=77;
			R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=2; 		T[t++]=21; T[t++]=65; T[t++]=64; T[t++]=25; T[t++]=15;
			R[r++]=3; R[r++]=1; R[r++]=2; R[r++]=4; R[r++]=5; 		T[t++]=85; T[t++]=40; T[t++]=44; T[t++]=24; T[t++]=37;
			R[r++]=1; R[r++]=5; R[r++]=2; R[r++]=4; R[r++]=3; 		T[t++]=89; T[t++]=29; T[t++]=83; T[t++]=31; T[t++]=84;
			R[r++]=5; R[r++]=4; R[r++]=2; R[r++]=3; R[r++]=1; 		T[t++]=59; T[t++]=38; T[t++]=80; T[t++]=30; T[t++]= 8;
			R[r++]=1; R[r++]=3; R[r++]=2; R[r++]=5; R[r++]=4; 		T[t++]=80; T[t++]=56; T[t++]=77; T[t++]=41; T[t++]=97;
			R[r++]=5; R[r++]=1; R[r++]=4; R[r++]=3; R[r++]=2; 		T[t++]=56; T[t++]=91; T[t++]=50; T[t++]=71; T[t++]=17;
			R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=3; R[r++]=4; 		T[t++]=40; T[t++]=88; T[t++]=59; T[t++]= 7; T[t++]=80;
			R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=4; 		T[t++]=45; T[t++]=29; T[t++]= 8; T[t++]=77; T[t++]=58;
			R[r++]=3; R[r++]=1; R[r++]=4; R[r++]=2; R[r++]=5; 		T[t++]=36; T[t++]=54; T[t++]=96; T[t++]= 9; T[t++]=10;
			R[r++]=1; R[r++]=3; R[r++]=2; R[r++]=4; R[r++]=5; 		T[t++]=28; T[t++]=73; T[t++]=98; T[t++]=92; T[t++]=87;
			R[r++]=1; R[r++]=4; R[r++]=3; R[r++]=2; R[r++]=5; 		T[t++]=70; T[t++]=86; T[t++]=27; T[t++]=99; T[t++]=96;
			R[r++]=2; R[r++]=1; R[r++]=5; R[r++]=4; R[r++]=3; 		T[t++]=95; T[t++]=59; T[t++]=56; T[t++]=85; T[t++]=41;
			R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4; 		T[t++]=81; T[t++]=92; T[t++]=32; T[t++]=52; T[t++]=39;
			R[r++]=2; R[r++]=5; R[r++]=3; R[r++]=1; R[r++]=4; 		T[t++]= 7; T[t++]=22; T[t++]=12; T[t++]=88; T[t++]=60;
			R[r++]=4; R[r++]=1; R[r++]=3; R[r++]=5; R[r++]=2; 		T[t++]=45; T[t++]=93; T[t++]=69; T[t++]=49; T[t++]=27;
			R[r++]=1; R[r++]=2; R[r++]=3; R[r++]=4; R[r++]=5; 		T[t++]=21; T[t++]=84; T[t++]=61; T[t++]=68; T[t++]=26;
			R[r++]=2; R[r++]=3; R[r++]=5; R[r++]=1; R[r++]=4; 		T[t++]=82; T[t++]=33; T[t++]=71; T[t++]=99; T[t++]=44;

        } else {
            printf( "Problema deve estar entre LA11 e LA15");
            getchar();
            return 0; // 0 = erro

        }

    #endif

    #ifdef PROGRAMA_16
        if (intProblema == 16){
            intMakeSpanOtimo = 945;

			R[r++]= 2; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 8; R[r++]= 3; R[r++]= 1; R[r++]= 5; R[r++]= 4; R[r++]= 6; 		T[t++]=21; T[t++]=71; T[t++]=16; T[t++]=52; T[t++]=26; T[t++]=34; T[t++]=53; T[t++]=21; T[t++]=55; T[t++]=95;
			R[r++]= 5; R[r++]= 3; R[r++]= 6; R[r++]=10; R[r++]= 1; R[r++]= 8; R[r++]= 2; R[r++]= 9; R[r++]= 7; R[r++]= 4; 		T[t++]=55; T[t++]=31; T[t++]=98; T[t++]=79; T[t++]=12; T[t++]=66; T[t++]=42; T[t++]=77; T[t++]=77; T[t++]=39;
			R[r++]= 4; R[r++]= 3; R[r++]= 9; R[r++]= 2; R[r++]= 5; R[r++]=10; R[r++]= 8; R[r++]= 7; R[r++]= 1; R[r++]= 6; 		T[t++]=34; T[t++]=64; T[t++]=62; T[t++]=19; T[t++]=92; T[t++]=79; T[t++]=43; T[t++]=54; T[t++]=83; T[t++]=37;
			R[r++]= 2; R[r++]= 4; R[r++]= 3; R[r++]= 8; R[r++]= 9; R[r++]=10; R[r++]= 7; R[r++]= 1; R[r++]= 6; R[r++]= 5; 		T[t++]=87; T[t++]=69; T[t++]=87; T[t++]=38; T[t++]=24; T[t++]=83; T[t++]=41; T[t++]=93; T[t++]=77; T[t++]=60;
			R[r++]= 3; R[r++]= 1; R[r++]= 6; R[r++]= 7; R[r++]= 8; R[r++]= 2; R[r++]= 5; R[r++]=10; R[r++]= 4; R[r++]= 9; 		T[t++]=98; T[t++]=44; T[t++]=25; T[t++]=75; T[t++]=43; T[t++]=49; T[t++]=96; T[t++]=77; T[t++]=17; T[t++]=79;
			R[r++]= 3; R[r++]= 4; R[r++]= 6; R[r++]=10; R[r++]= 5; R[r++]= 7; R[r++]= 1; R[r++]= 9; R[r++]= 2; R[r++]= 8; 		T[t++]=35; T[t++]=76; T[t++]=28; T[t++]=10; T[t++]=61; T[t++]= 9; T[t++]=95; T[t++]=35; T[t++]= 7; T[t++]=95;
			R[r++]= 4; R[r++]= 3; R[r++]= 1; R[r++]= 2; R[r++]=10; R[r++]= 9; R[r++]= 7; R[r++]= 6; R[r++]= 5; R[r++]= 8; 		T[t++]=16; T[t++]=59; T[t++]=46; T[t++]=91; T[t++]=43; T[t++]=50; T[t++]=52; T[t++]=59; T[t++]=28; T[t++]=27;
			R[r++]= 2; R[r++]= 1; R[r++]= 4; R[r++]= 5; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 6; R[r++]= 3; R[r++]= 8; 		T[t++]=45; T[t++]=87; T[t++]=41; T[t++]=20; T[t++]=54; T[t++]=43; T[t++]=14; T[t++]= 9; T[t++]=39; T[t++]=71;
			R[r++]= 5; R[r++]= 3; R[r++]= 9; R[r++]= 6; R[r++]= 4; R[r++]= 8; R[r++]= 2; R[r++]= 7; R[r++]=10; R[r++]= 1; 		T[t++]=33; T[t++]=37; T[t++]=66; T[t++]=33; T[t++]=26; T[t++]= 8; T[t++]=28; T[t++]=89; T[t++]=42; T[t++]=78;
			R[r++]= 9; R[r++]=10; R[r++]= 3; R[r++]= 5; R[r++]= 4; R[r++]= 1; R[r++]= 8; R[r++]= 7; R[r++]= 2; R[r++]= 6; 		T[t++]=69; T[t++]=81; T[t++]=94; T[t++]=96; T[t++]=27; T[t++]=69; T[t++]=45; T[t++]=78; T[t++]=74; T[t++]=84;

        } else if(intProblema == 17) {
            intMakeSpanOtimo = 784;

			R[r++]= 2; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 8; R[r++]= 3; R[r++]= 1; R[r++]= 5; R[r++]= 4; R[r++]= 6; 		T[t++]=21; T[t++]=71; T[t++]=16; T[t++]=52; T[t++]=26; T[t++]=34; T[t++]=53; T[t++]=21; T[t++]=55; T[t++]=95;
			R[r++]= 5; R[r++]= 3; R[r++]= 6; R[r++]=10; R[r++]= 1; R[r++]= 8; R[r++]= 2; R[r++]= 9; R[r++]= 7; R[r++]= 4; 		T[t++]=55; T[t++]=31; T[t++]=98; T[t++]=79; T[t++]=12; T[t++]=66; T[t++]=42; T[t++]=77; T[t++]=77; T[t++]=39;
			R[r++]= 4; R[r++]= 3; R[r++]= 9; R[r++]= 2; R[r++]= 5; R[r++]=10; R[r++]= 8; R[r++]= 7; R[r++]= 1; R[r++]= 6; 		T[t++]=34; T[t++]=64; T[t++]=62; T[t++]=19; T[t++]=92; T[t++]=79; T[t++]=43; T[t++]=54; T[t++]=83; T[t++]=37;
			R[r++]= 2; R[r++]= 4; R[r++]= 3; R[r++]= 8; R[r++]= 9; R[r++]=10; R[r++]= 7; R[r++]= 1; R[r++]= 6; R[r++]= 5; 		T[t++]=87; T[t++]=69; T[t++]=87; T[t++]=38; T[t++]=24; T[t++]=83; T[t++]=41; T[t++]=93; T[t++]=77; T[t++]=60;
			R[r++]= 3; R[r++]= 1; R[r++]= 6; R[r++]= 7; R[r++]= 8; R[r++]= 2; R[r++]= 5; R[r++]=10; R[r++]= 4; R[r++]= 9; 		T[t++]=98; T[t++]=44; T[t++]=25; T[t++]=75; T[t++]=43; T[t++]=49; T[t++]=96; T[t++]=77; T[t++]=17; T[t++]=79;
			R[r++]= 3; R[r++]= 4; R[r++]= 6; R[r++]=10; R[r++]= 5; R[r++]= 7; R[r++]= 1; R[r++]= 9; R[r++]= 2; R[r++]= 8; 		T[t++]=35; T[t++]=76; T[t++]=28; T[t++]=10; T[t++]=61; T[t++]= 9; T[t++]=95; T[t++]=35; T[t++]= 7; T[t++]=95;
			R[r++]= 4; R[r++]= 3; R[r++]= 1; R[r++]= 2; R[r++]=10; R[r++]= 9; R[r++]= 7; R[r++]= 6; R[r++]= 5; R[r++]= 8; 		T[t++]=16; T[t++]=59; T[t++]=46; T[t++]=91; T[t++]=43; T[t++]=50; T[t++]=52; T[t++]=59; T[t++]=28; T[t++]=27;
			R[r++]= 2; R[r++]= 1; R[r++]= 4; R[r++]= 5; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 6; R[r++]= 3; R[r++]= 8; 		T[t++]=45; T[t++]=87; T[t++]=41; T[t++]=20; T[t++]=54; T[t++]=43; T[t++]=14; T[t++]= 9; T[t++]=39; T[t++]=71;
			R[r++]= 5; R[r++]= 3; R[r++]= 9; R[r++]= 6; R[r++]= 4; R[r++]= 8; R[r++]= 2; R[r++]= 7; R[r++]=10; R[r++]= 1; 		T[t++]=33; T[t++]=37; T[t++]=66; T[t++]=33; T[t++]=26; T[t++]= 8; T[t++]=28; T[t++]=89; T[t++]=42; T[t++]=78;
			R[r++]= 9; R[r++]=10; R[r++]= 3; R[r++]= 5; R[r++]= 4; R[r++]= 1; R[r++]= 8; R[r++]= 7; R[r++]= 2; R[r++]= 6; 		T[t++]=69; T[t++]=81; T[t++]=94; T[t++]=96; T[t++]=27; T[t++]=69; T[t++]=45; T[t++]=78; T[t++]=74; T[t++]=84;

        } else if(intProblema ==18) {
            intMakeSpanOtimo = 848;

			R[r++]= 2; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 8; R[r++]= 3; R[r++]= 1; R[r++]= 5; R[r++]= 4; R[r++]= 6; 		T[t++]=21; T[t++]=71; T[t++]=16; T[t++]=52; T[t++]=26; T[t++]=34; T[t++]=53; T[t++]=21; T[t++]=55; T[t++]=95;
			R[r++]= 5; R[r++]= 3; R[r++]= 6; R[r++]=10; R[r++]= 1; R[r++]= 8; R[r++]= 2; R[r++]= 9; R[r++]= 7; R[r++]= 4; 		T[t++]=55; T[t++]=31; T[t++]=98; T[t++]=79; T[t++]=12; T[t++]=66; T[t++]=42; T[t++]=77; T[t++]=77; T[t++]=39;
			R[r++]= 4; R[r++]= 3; R[r++]= 9; R[r++]= 2; R[r++]= 5; R[r++]=10; R[r++]= 8; R[r++]= 7; R[r++]= 1; R[r++]= 6; 		T[t++]=34; T[t++]=64; T[t++]=62; T[t++]=19; T[t++]=92; T[t++]=79; T[t++]=43; T[t++]=54; T[t++]=83; T[t++]=37;
			R[r++]= 2; R[r++]= 4; R[r++]= 3; R[r++]= 8; R[r++]= 9; R[r++]=10; R[r++]= 7; R[r++]= 1; R[r++]= 6; R[r++]= 5; 		T[t++]=87; T[t++]=69; T[t++]=87; T[t++]=38; T[t++]=24; T[t++]=83; T[t++]=41; T[t++]=93; T[t++]=77; T[t++]=60;
			R[r++]= 3; R[r++]= 1; R[r++]= 6; R[r++]= 7; R[r++]= 8; R[r++]= 2; R[r++]= 5; R[r++]=10; R[r++]= 4; R[r++]= 9; 		T[t++]=98; T[t++]=44; T[t++]=25; T[t++]=75; T[t++]=43; T[t++]=49; T[t++]=96; T[t++]=77; T[t++]=17; T[t++]=79;
			R[r++]= 3; R[r++]= 4; R[r++]= 6; R[r++]=10; R[r++]= 5; R[r++]= 7; R[r++]= 1; R[r++]= 9; R[r++]= 2; R[r++]= 8; 		T[t++]=35; T[t++]=76; T[t++]=28; T[t++]=10; T[t++]=61; T[t++]= 9; T[t++]=95; T[t++]=35; T[t++]= 7; T[t++]=95;
			R[r++]= 4; R[r++]= 3; R[r++]= 1; R[r++]= 2; R[r++]=10; R[r++]= 9; R[r++]= 7; R[r++]= 6; R[r++]= 5; R[r++]= 8; 		T[t++]=16; T[t++]=59; T[t++]=46; T[t++]=91; T[t++]=43; T[t++]=50; T[t++]=52; T[t++]=59; T[t++]=28; T[t++]=27;
			R[r++]= 2; R[r++]= 1; R[r++]= 4; R[r++]= 5; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 6; R[r++]= 3; R[r++]= 8; 		T[t++]=45; T[t++]=87; T[t++]=41; T[t++]=20; T[t++]=54; T[t++]=43; T[t++]=14; T[t++]= 9; T[t++]=39; T[t++]=71;
			R[r++]= 5; R[r++]= 3; R[r++]= 9; R[r++]= 6; R[r++]= 4; R[r++]= 8; R[r++]= 2; R[r++]= 7; R[r++]=10; R[r++]= 1; 		T[t++]=33; T[t++]=37; T[t++]=66; T[t++]=33; T[t++]=26; T[t++]= 8; T[t++]=28; T[t++]=89; T[t++]=42; T[t++]=78;
			R[r++]= 9; R[r++]=10; R[r++]= 3; R[r++]= 5; R[r++]= 4; R[r++]= 1; R[r++]= 8; R[r++]= 7; R[r++]= 2; R[r++]= 6; 		T[t++]=69; T[t++]=81; T[t++]=94; T[t++]=96; T[t++]=27; T[t++]=69; T[t++]=45; T[t++]=78; T[t++]=74; T[t++]=84;

        } else if(intProblema ==19) {
            intMakeSpanOtimo = 842;

			R[r++]= 2; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 8; R[r++]= 3; R[r++]= 1; R[r++]= 5; R[r++]= 4; R[r++]= 6; 		T[t++]=21; T[t++]=71; T[t++]=16; T[t++]=52; T[t++]=26; T[t++]=34; T[t++]=53; T[t++]=21; T[t++]=55; T[t++]=95;
			R[r++]= 5; R[r++]= 3; R[r++]= 6; R[r++]=10; R[r++]= 1; R[r++]= 8; R[r++]= 2; R[r++]= 9; R[r++]= 7; R[r++]= 4; 		T[t++]=55; T[t++]=31; T[t++]=98; T[t++]=79; T[t++]=12; T[t++]=66; T[t++]=42; T[t++]=77; T[t++]=77; T[t++]=39;
			R[r++]= 4; R[r++]= 3; R[r++]= 9; R[r++]= 2; R[r++]= 5; R[r++]=10; R[r++]= 8; R[r++]= 7; R[r++]= 1; R[r++]= 6; 		T[t++]=34; T[t++]=64; T[t++]=62; T[t++]=19; T[t++]=92; T[t++]=79; T[t++]=43; T[t++]=54; T[t++]=83; T[t++]=37;
			R[r++]= 2; R[r++]= 4; R[r++]= 3; R[r++]= 8; R[r++]= 9; R[r++]=10; R[r++]= 7; R[r++]= 1; R[r++]= 6; R[r++]= 5; 		T[t++]=87; T[t++]=69; T[t++]=87; T[t++]=38; T[t++]=24; T[t++]=83; T[t++]=41; T[t++]=93; T[t++]=77; T[t++]=60;
			R[r++]= 3; R[r++]= 1; R[r++]= 6; R[r++]= 7; R[r++]= 8; R[r++]= 2; R[r++]= 5; R[r++]=10; R[r++]= 4; R[r++]= 9; 		T[t++]=98; T[t++]=44; T[t++]=25; T[t++]=75; T[t++]=43; T[t++]=49; T[t++]=96; T[t++]=77; T[t++]=17; T[t++]=79;
			R[r++]= 3; R[r++]= 4; R[r++]= 6; R[r++]=10; R[r++]= 5; R[r++]= 7; R[r++]= 1; R[r++]= 9; R[r++]= 2; R[r++]= 8; 		T[t++]=35; T[t++]=76; T[t++]=28; T[t++]=10; T[t++]=61; T[t++]= 9; T[t++]=95; T[t++]=35; T[t++]= 7; T[t++]=95;
			R[r++]= 4; R[r++]= 3; R[r++]= 1; R[r++]= 2; R[r++]=10; R[r++]= 9; R[r++]= 7; R[r++]= 6; R[r++]= 5; R[r++]= 8; 		T[t++]=16; T[t++]=59; T[t++]=46; T[t++]=91; T[t++]=43; T[t++]=50; T[t++]=52; T[t++]=59; T[t++]=28; T[t++]=27;
			R[r++]= 2; R[r++]= 1; R[r++]= 4; R[r++]= 5; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 6; R[r++]= 3; R[r++]= 8; 		T[t++]=45; T[t++]=87; T[t++]=41; T[t++]=20; T[t++]=54; T[t++]=43; T[t++]=14; T[t++]= 9; T[t++]=39; T[t++]=71;
			R[r++]= 5; R[r++]= 3; R[r++]= 9; R[r++]= 6; R[r++]= 4; R[r++]= 8; R[r++]= 2; R[r++]= 7; R[r++]=10; R[r++]= 1; 		T[t++]=33; T[t++]=37; T[t++]=66; T[t++]=33; T[t++]=26; T[t++]= 8; T[t++]=28; T[t++]=89; T[t++]=42; T[t++]=78;
			R[r++]= 9; R[r++]=10; R[r++]= 3; R[r++]= 5; R[r++]= 4; R[r++]= 1; R[r++]= 8; R[r++]= 7; R[r++]= 2; R[r++]= 6; 		T[t++]=69; T[t++]=81; T[t++]=94; T[t++]=96; T[t++]=27; T[t++]=69; T[t++]=45; T[t++]=78; T[t++]=74; T[t++]=84;

        } else if(intProblema ==20) {
            intMakeSpanOtimo = 902;

			R[r++]= 7; R[r++]= 2; R[r++]= 5; R[r++]= 3; R[r++]= 9; R[r++]= 4; R[r++]= 1; R[r++]= 6; R[r++]=10; R[r++]= 8; 		T[t++]= 9; T[t++]=81; T[t++]=55; T[t++]=40; T[t++]=32; T[t++]=37; T[t++]= 6; T[t++]=19; T[t++]=81; T[t++]=40;
			R[r++]= 8; R[r++]= 3; R[r++]=10; R[r++]= 5; R[r++]= 2; R[r++]= 6; R[r++]= 9; R[r++]= 1; R[r++]= 4; R[r++]= 7; 		T[t++]=21; T[t++]=70; T[t++]=65; T[t++]=64; T[t++]=46; T[t++]=65; T[t++]=25; T[t++]=77; T[t++]=55; T[t++]=15;
			R[r++]= 3; R[r++]= 6; R[r++]= 1; R[r++]= 4; R[r++]= 2; R[r++]= 7; R[r++]= 5; R[r++]= 9; R[r++]= 8; R[r++]=10; 		T[t++]=85; T[t++]=37; T[t++]=40; T[t++]=24; T[t++]=44; T[t++]=83; T[t++]=89; T[t++]=31; T[t++]=84; T[t++]=29;
			R[r++]= 5; R[r++]= 7; R[r++]= 8; R[r++]= 1; R[r++]= 3; R[r++]= 6; R[r++]= 4; R[r++]= 2; R[r++]=10; R[r++]= 9; 		T[t++]=80; T[t++]=77; T[t++]=56; T[t++]= 8; T[t++]=30; T[t++]=59; T[t++]=38; T[t++]=80; T[t++]=41; T[t++]=97;
			R[r++]= 1; R[r++]= 7; R[r++]= 5; R[r++]= 2; R[r++]= 3; R[r++]= 4; R[r++]=10; R[r++]= 9; R[r++]= 6; R[r++]= 8; 		T[t++]=91; T[t++]=40; T[t++]=88; T[t++]=17; T[t++]=71; T[t++]=50; T[t++]=59; T[t++]=80; T[t++]=56; T[t++]= 7;
			R[r++]= 3; R[r++]= 7; R[r++]= 4; R[r++]= 6; R[r++]= 2; R[r++]= 9; R[r++]= 1; R[r++]=10; R[r++]= 5; R[r++]= 8; 		T[t++]= 8; T[t++]= 9; T[t++]=58; T[t++]=77; T[t++]=29; T[t++]=96; T[t++]=45; T[t++]=10; T[t++]=54; T[t++]=36;
			R[r++]= 5; R[r++]= 4; R[r++]= 2; R[r++]= 6; R[r++]= 7; R[r++]= 8; R[r++]= 9; R[r++]=10; R[r++]= 1; R[r++]= 3; 		T[t++]=70; T[t++]=92; T[t++]=98; T[t++]=87; T[t++]=99; T[t++]=27; T[t++]=86; T[t++]=96; T[t++]=28; T[t++]=73;
			R[r++]= 2; R[r++]= 8; R[r++]= 4; R[r++]= 5; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 1; R[r++]= 3; R[r++]= 6; 		T[t++]=95; T[t++]=92; T[t++]=85; T[t++]=52; T[t++]=81; T[t++]=32; T[t++]=39; T[t++]=59; T[t++]=41; T[t++]=56;
			R[r++]= 4; R[r++]= 9; R[r++]= 1; R[r++]= 3; R[r++]= 2; R[r++]= 6; R[r++]= 5; R[r++]=10; R[r++]= 8; R[r++]= 7; 		T[t++]=60; T[t++]=45; T[t++]=88; T[t++]=12; T[t++]= 7; T[t++]=22; T[t++]=93; T[t++]=49; T[t++]=69; T[t++]=27;
			R[r++]= 1; R[r++]= 3; R[r++]= 4; R[r++]= 6; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 5; R[r++]= 8; R[r++]= 2; 		T[t++]=21; T[t++]=61; T[t++]=68; T[t++]=26; T[t++]=82; T[t++]=71; T[t++]=44; T[t++]=99; T[t++]=33; T[t++]=84;

        } else {
            printf( "Problema deve estar entre LA16 e LA20");
            getchar();
            return 0; // 0 = erro

        }

    #endif

    #ifdef PROGRAMA_21
        if (intProblema == 21){
            intMakeSpanOtimo = 1046;

			R[r++]= 3; R[r++]= 4; R[r++]= 6; R[r++]=10; R[r++]= 5; R[r++]= 7; R[r++]= 1; R[r++]= 9; R[r++]= 2; R[r++]= 8; 		T[t++]=34; T[t++]=55; T[t++]=95; T[t++]=16; T[t++]=21; T[t++]=71; T[t++]=53; T[t++]=52; T[t++]=21; T[t++]=26;
			R[r++]= 4; R[r++]= 3; R[r++]= 1; R[r++]= 2; R[r++]=10; R[r++]= 9; R[r++]= 7; R[r++]= 6; R[r++]= 5; R[r++]= 8; 		T[t++]=39; T[t++]=31; T[t++]=12; T[t++]=42; T[t++]=79; T[t++]=77; T[t++]=77; T[t++]=98; T[t++]=55; T[t++]=66;
			R[r++]= 2; R[r++]= 1; R[r++]= 4; R[r++]= 5; R[r++]= 7; R[r++]=10; R[r++]= 9; R[r++]= 6; R[r++]= 3; R[r++]= 8; 		T[t++]=19; T[t++]=83; T[t++]=34; T[t++]=92; T[t++]=54; T[t++]=79; T[t++]=62; T[t++]=37; T[t++]=64; T[t++]=43;
			R[r++]= 5; R[r++]= 3; R[r++]= 9; R[r++]= 6; R[r++]= 4; R[r++]= 8; R[r++]= 2; R[r++]= 7; R[r++]=10; R[r++]= 1; 		T[t++]=60; T[t++]=87; T[t++]=24; T[t++]=77; T[t++]=69; T[t++]=38; T[t++]=87; T[t++]=41; T[t++]=83; T[t++]=93;
			R[r++]= 9; R[r++]=10; R[r++]= 3; R[r++]= 5; R[r++]= 4; R[r++]= 1; R[r++]= 8; R[r++]= 7; R[r++]= 2; R[r++]= 6; 		T[t++]=79; T[t++]=77; T[t++]=98; T[t++]=96; T[t++]=17; T[t++]=44; T[t++]=43; T[t++]=75; T[t++]=49; T[t++]=25;
			R[r++]= 9; R[r++]= 8; R[r++]= 7; R[r++]=10; R[r++]= 3; R[r++]= 2; R[r++]= 6; R[r++]= 5; R[r++]= 1; R[r++]= 4; 		T[t++]=35; T[t++]=95; T[t++]= 9; T[t++]=10; T[t++]=35; T[t++]= 7; T[t++]=28; T[t++]=61; T[t++]=95; T[t++]=76;
			R[r++]= 5; R[r++]= 6; R[r++]= 4; R[r++]=10; R[r++]= 1; R[r++]= 9; R[r++]= 7; R[r++]= 8; R[r++]= 3; R[r++]= 2; 		T[t++]=28; T[t++]=59; T[t++]=16; T[t++]=43; T[t++]=46; T[t++]=50; T[t++]=52; T[t++]=27; T[t++]=59; T[t++]=91;
			R[r++]= 6; R[r++]= 5; R[r++]= 3; R[r++]= 7; R[r++]= 2; R[r++]= 8; R[r++]= 1; R[r++]= 4; R[r++]=10; R[r++]= 9; 		T[t++]= 9; T[t++]=20; T[t++]=39; T[t++]=54; T[t++]=45; T[t++]=71; T[t++]=87; T[t++]=41; T[t++]=43; T[t++]=14;
			R[r++]= 2; R[r++]= 6; R[r++]= 1; R[r++]= 4; R[r++]= 3; R[r++]= 8; R[r++]= 9; R[r++]= 7; R[r++]=10; R[r++]= 5; 		T[t++]=28; T[t++]=33; T[t++]=78; T[t++]=26; T[t++]=37; T[t++]= 8; T[t++]=66; T[t++]=89; T[t++]=42; T[t++]=33;
			R[r++]= 3; R[r++]= 6; R[r++]= 7; R[r++]=10; R[r++]= 2; R[r++]= 4; R[r++]= 9; R[r++]= 1; R[r++]= 8; R[r++]= 5; 		T[t++]=94; T[t++]=84; T[t++]=78; T[t++]=81; T[t++]=74; T[t++]=27; T[t++]=69; T[t++]=69; T[t++]=45; T[t++]=96;
			R[r++]= 2; R[r++]= 5; R[r++]= 1; R[r++]= 3; R[r++]=10; R[r++]= 9; R[r++]= 6; R[r++]= 4; R[r++]= 8; R[r++]= 7; 		T[t++]=31; T[t++]=24; T[t++]=20; T[t++]=17; T[t++]=25; T[t++]=81; T[t++]=76; T[t++]=87; T[t++]=32; T[t++]=18;
			R[r++]= 6; R[r++]=10; R[r++]= 1; R[r++]= 5; R[r++]= 7; R[r++]= 4; R[r++]= 3; R[r++]= 2; R[r++]= 9; R[r++]= 8; 		T[t++]=28; T[t++]=97; T[t++]=58; T[t++]=45; T[t++]=76; T[t++]=99; T[t++]=23; T[t++]=72; T[t++]=90; T[t++]=86;
			R[r++]= 6; R[r++]=10; R[r++]= 9; R[r++]= 8; R[r++]= 5; R[r++]= 7; R[r++]= 4; R[r++]= 1; R[r++]= 2; R[r++]= 3; 		T[t++]=27; T[t++]=48; T[t++]=27; T[t++]=62; T[t++]=98; T[t++]=67; T[t++]=48; T[t++]=42; T[t++]=46; T[t++]=17;
			R[r++]= 2; R[r++]= 9; R[r++]= 1; R[r++]= 3; R[r++]=10; R[r++]= 4; R[r++]= 6; R[r++]= 7; R[r++]= 5; R[r++]= 8; 		T[t++]=12; T[t++]=50; T[t++]=80; T[t++]=50; T[t++]=80; T[t++]=19; T[t++]=28; T[t++]=63; T[t++]=94; T[t++]=98;
			R[r++]= 5; R[r++]= 4; R[r++]= 7; R[r++]= 6; R[r++]= 3; R[r++]= 9; R[r++]= 2; R[r++]=10; R[r++]= 8; R[r++]= 1; 		T[t++]=61; T[t++]=55; T[t++]=37; T[t++]=14; T[t++]=50; T[t++]=79; T[t++]=41; T[t++]=72; T[t++]=18; T[t++]=75;

        } else if(intProblema == 22) {
            intMakeSpanOtimo = 927;

			R[r++]=10; R[r++]= 6; R[r++]= 5; R[r++]= 3; R[r++]= 8; R[r++]= 4; R[r++]= 2; R[r++]= 1; R[r++]= 9; R[r++]= 7; 		T[t++]=66; T[t++]=91; T[t++]=87; T[t++]=94; T[t++]=21; T[t++]=92; T[t++]= 7; T[t++]=12; T[t++]=11; T[t++]=19;
			R[r++]= 4; R[r++]= 3; R[r++]= 5; R[r++]= 2; R[r++]=10; R[r++]= 1; R[r++]= 7; R[r++]= 6; R[r++]= 8; R[r++]= 9; 		T[t++]=13; T[t++]=20; T[t++]= 7; T[t++]=14; T[t++]=66; T[t++]=75; T[t++]=77; T[t++]=16; T[t++]=95; T[t++]= 7;
			R[r++]= 9; R[r++]= 8; R[r++]= 3; R[r++]= 1; R[r++]=10; R[r++]= 6; R[r++]= 7; R[r++]= 4; R[r++]= 2; R[r++]= 5; 		T[t++]=77; T[t++]=20; T[t++]=34; T[t++]=15; T[t++]=88; T[t++]=89; T[t++]=53; T[t++]= 6; T[t++]=45; T[t++]=76;
			R[r++]= 4; R[r++]= 3; R[r++]= 7; R[r++]= 5; R[r++]= 8; R[r++]= 9; R[r++]= 6; R[r++]=10; R[r++]= 1; R[r++]= 2; 		T[t++]=27; T[t++]=74; T[t++]=88; T[t++]=62; T[t++]=52; T[t++]=69; T[t++]= 9; T[t++]=98; T[t++]=52; T[t++]=88;
			R[r++]= 5; R[r++]= 7; R[r++]= 2; R[r++]= 3; R[r++]= 8; R[r++]= 1; R[r++]= 9; R[r++]= 6; R[r++]= 4; R[r++]=10; 		T[t++]=88; T[t++]=15; T[t++]=52; T[t++]=61; T[t++]=54; T[t++]=62; T[t++]=59; T[t++]= 9; T[t++]=90; T[t++]= 5;
			R[r++]= 7; R[r++]= 1; R[r++]= 5; R[r++]= 4; R[r++]= 8; R[r++]= 9; R[r++]= 2; R[r++]= 6; R[r++]= 3; R[r++]=10; 		T[t++]=71; T[t++]=41; T[t++]=38; T[t++]=53; T[t++]=91; T[t++]=68; T[t++]=50; T[t++]=78; T[t++]=23; T[t++]=72;
			R[r++]= 4; R[r++]=10; R[r++]= 7; R[r++]= 6; R[r++]= 1; R[r++]= 9; R[r++]= 5; R[r++]= 3; R[r++]= 8; R[r++]= 2; 		T[t++]=95; T[t++]=36; T[t++]=66; T[t++]=52; T[t++]=45; T[t++]=30; T[t++]=23; T[t++]=25; T[t++]=17; T[t++]= 6;
			R[r++]= 5; R[r++]= 2; R[r++]= 9; R[r++]= 1; R[r++]= 8; R[r++]= 7; R[r++]= 6; R[r++]= 4; R[r++]=10; R[r++]= 3; 		T[t++]=65; T[t++]= 8; T[t++]=85; T[t++]=71; T[t++]=65; T[t++]=28; T[t++]=88; T[t++]=76; T[t++]=27; T[t++]=95;
			R[r++]=10; R[r++]= 2; R[r++]= 5; R[r++]= 4; R[r++]= 9; R[r++]= 3; R[r++]= 7; R[r++]= 1; R[r++]= 8; R[r++]= 6; 		T[t++]=37; T[t++]=37; T[t++]=28; T[t++]=51; T[t++]=86; T[t++]= 9; T[t++]=55; T[t++]=73; T[t++]=51; T[t++]=90;
			R[r++]= 4; R[r++]= 3; R[r++]= 7; R[r++]=10; R[r++]= 8; R[r++]= 1; R[r++]= 5; R[r++]= 6; R[r++]= 2; R[r++]= 9; 		T[t++]=39; T[t++]=15; T[t++]=83; T[t++]=44; T[t++]=53; T[t++]=16; T[t++]=46; T[t++]=24; T[t++]=25; T[t++]=82;
			R[r++]= 2; R[r++]= 5; R[r++]= 1; R[r++]= 3; R[r++]=10; R[r++]= 7; R[r++]= 8; R[r++]= 9; R[r++]= 6; R[r++]= 4; 		T[t++]=72; T[t++]=48; T[t++]=87; T[t++]=66; T[t++]= 5; T[t++]=54; T[t++]=39; T[t++]=35; T[t++]=95; T[t++]=60;
			R[r++]= 2; R[r++]= 4; R[r++]= 1; R[r++]= 3; R[r++]=10; R[r++]= 8; R[r++]= 9; R[r++]= 5; R[r++]= 7; R[r++]= 6; 		T[t++]=46; T[t++]=20; T[t++]=97; T[t++]=21; T[t++]=46; T[t++]=37; T[t++]=19; T[t++]=59; T[t++]=34; T[t++]=55;
			R[r++]= 6; R[r++]= 4; R[r++]= 7; R[r++]= 2; R[r++]= 1; R[r++]= 8; R[r++]= 9; R[r++]=10; R[r++]= 3; R[r++]= 5; 		T[t++]=23; T[t++]=25; T[t++]=78; T[t++]=24; T[t++]=28; T[t++]=83; T[t++]=28; T[t++]= 5; T[t++]=73; T[t++]=45;
			R[r++]= 2; R[r++]= 1; R[r++]= 8; R[r++]= 5; R[r++]= 4; R[r++]= 6; R[r++]=10; R[r++]= 9; R[r++]= 7; R[r++]= 3; 		T[t++]=37; T[t++]=53; T[t++]=87; T[t++]=38; T[t++]=71; T[t++]=29; T[t++]=12; T[t++]=33; T[t++]=55; T[t++]=12;
			R[r++]= 5; R[r++]= 9; R[r++]= 3; R[r++]= 4; R[r++]= 2; R[r++]= 7; R[r++]= 8; R[r++]=10; R[r++]= 6; R[r++]= 1; 		T[t++]=90; T[t++]=17; T[t++]=49; T[t++]=83; T[t++]=40; T[t++]=23; T[t++]=65; T[t++]=27; T[t++]= 7; T[t++]=48;

        } else if(intProblema ==23) {
            intMakeSpanOtimo = 1032;

			R[r++]= 8; R[r++]= 6; R[r++]= 9; R[r++]= 3; R[r++]= 5; R[r++]= 7; R[r++]= 4; R[r++]= 2; R[r++]=10; R[r++]= 1; 		T[t++]=84; T[t++]=58; T[t++]=77; T[t++]=44; T[t++]=97; T[t++]=89; T[t++]= 5; T[t++]=58; T[t++]=96; T[t++]= 9;
			R[r++]= 7; R[r++]= 2; R[r++]= 5; R[r++]= 6; R[r++]= 3; R[r++]= 4; R[r++]= 8; R[r++]= 9; R[r++]=10; R[r++]= 1; 		T[t++]=21; T[t++]=87; T[t++]=15; T[t++]=39; T[t++]=81; T[t++]=85; T[t++]=31; T[t++]=57; T[t++]=73; T[t++]=77;
			R[r++]= 1; R[r++]= 6; R[r++]= 9; R[r++]=10; R[r++]= 4; R[r++]= 7; R[r++]= 5; R[r++]= 8; R[r++]= 3; R[r++]= 2; 		T[t++]=40; T[t++]=71; T[t++]=34; T[t++]=82; T[t++]=70; T[t++]=22; T[t++]=10; T[t++]=80; T[t++]=48; T[t++]=49;
			R[r++]= 6; R[r++]= 3; R[r++]= 4; R[r++]= 7; R[r++]= 5; R[r++]= 8; R[r++]= 9; R[r++]=10; R[r++]= 2; R[r++]= 1; 		T[t++]=75; T[t++]=17; T[t++]= 7; T[t++]=72; T[t++]=11; T[t++]=62; T[t++]=47; T[t++]=35; T[t++]=91; T[t++]=55;
			R[r++]=10; R[r++]= 5; R[r++]= 7; R[r++]= 8; R[r++]= 1; R[r++]= 3; R[r++]= 9; R[r++]= 6; R[r++]= 4; R[r++]= 2; 		T[t++]=20; T[t++]=12; T[t++]=71; T[t++]=67; T[t++]=64; T[t++]=94; T[t++]=15; T[t++]=50; T[t++]=75; T[t++]=90;
			R[r++]= 7; R[r++]= 6; R[r++]= 2; R[r++]= 8; R[r++]= 9; R[r++]= 5; R[r++]= 1; R[r++]= 3; R[r++]=10; R[r++]= 4; 		T[t++]=93; T[t++]=93; T[t++]=57; T[t++]=70; T[t++]=77; T[t++]=58; T[t++]=52; T[t++]=29; T[t++]= 7; T[t++]=68;
			R[r++]= 8; R[r++]= 1; R[r++]= 9; R[r++]= 5; R[r++]= 3; R[r++]= 2; R[r++]=10; R[r++]= 4; R[r++]= 7; R[r++]= 6; 		T[t++]=56; T[t++]=95; T[t++]=48; T[t++]=26; T[t++]=82; T[t++]=63; T[t++]=36; T[t++]=27; T[t++]=87; T[t++]= 6;
			R[r++]= 4; R[r++]= 6; R[r++]=10; R[r++]= 2; R[r++]= 9; R[r++]= 3; R[r++]= 5; R[r++]= 7; R[r++]= 1; R[r++]= 8; 		T[t++]=76; T[t++]=15; T[t++]=78; T[t++]= 8; T[t++]=41; T[t++]=36; T[t++]=30; T[t++]=84; T[t++]=36; T[t++]=76;
			R[r++]= 1; R[r++]= 8; R[r++]= 3; R[r++]= 9; R[r++]= 5; R[r++]= 7; R[r++]= 6; R[r++]= 2; R[r++]=10; R[r++]= 4; 		T[t++]=75; T[t++]=13; T[t++]=81; T[t++]=29; T[t++]=54; T[t++]=82; T[t++]=88; T[t++]=78; T[t++]=40; T[t++]=13;
			R[r++]= 3; R[r++]= 2; R[r++]= 8; R[r++]= 7; R[r++]= 5; R[r++]= 1; R[r++]= 6; R[r++]= 4; R[r++]=10; R[r++]= 9; 		T[t++]= 6; T[t++]=26; T[t++]=32; T[t++]=64; T[t++]=54; T[t++]=52; T[t++]=82; T[t++]= 6; T[t++]=88; T[t++]=54;
			R[r++]= 9; R[r++]= 3; R[r++]= 6; R[r++]= 1; R[r++]= 8; R[r++]= 4; R[r++]= 2; R[r++]= 5; R[r++]=10; R[r++]= 7; 		T[t++]=62; T[t++]=67; T[t++]=32; T[t++]=62; T[t++]=69; T[t++]=61; T[t++]=35; T[t++]=72; T[t++]= 5; T[t++]=93;
			R[r++]= 3; R[r++]=10; R[r++]= 1; R[r++]= 2; R[r++]= 9; R[r++]= 7; R[r++]= 4; R[r++]= 8; R[r++]= 6; R[r++]= 5; 		T[t++]=78; T[t++]=90; T[t++]=85; T[t++]=72; T[t++]=64; T[t++]=63; T[t++]=11; T[t++]=82; T[t++]=88; T[t++]= 7;
			R[r++]= 5; R[r++]=10; R[r++]= 8; R[r++]= 7; R[r++]= 1; R[r++]= 6; R[r++]= 3; R[r++]= 2; R[r++]= 9; R[r++]= 4; 		T[t++]=28; T[t++]=11; T[t++]=50; T[t++]=88; T[t++]=44; T[t++]=31; T[t++]=27; T[t++]=66; T[t++]=49; T[t++]=35;
			R[r++]= 3; R[r++]= 6; R[r++]= 7; R[r++]= 5; R[r++]= 4; R[r++]=10; R[r++]= 8; R[r++]= 2; R[r++]= 9; R[r++]= 1; 		T[t++]=14; T[t++]=39; T[t++]=56; T[t++]=62; T[t++]=97; T[t++]=66; T[t++]=69; T[t++]= 7; T[t++]=47; T[t++]=76;
			R[r++]= 2; R[r++]= 9; R[r++]= 8; R[r++]= 7; R[r++]= 4; R[r++]=10; R[r++]= 3; R[r++]= 6; R[r++]= 5; R[r++]= 1; 		T[t++]=18; T[t++]=93; T[t++]=58; T[t++]=47; T[t++]=69; T[t++]=57; T[t++]=41; T[t++]=53; T[t++]=79; T[t++]=64;

        } else if(intProblema ==24) {
            intMakeSpanOtimo = 935;

			R[r++]= 8; R[r++]=10; R[r++]= 1; R[r++]= 7; R[r++]= 5; R[r++]= 9; R[r++]= 3; R[r++]= 6; R[r++]= 2; R[r++]= 4; 		T[t++]= 8; T[t++]=75; T[t++]=72; T[t++]=74; T[t++]=30; T[t++]=43; T[t++]=38; T[t++]=98; T[t++]=26; T[t++]=19;
			R[r++]= 7; R[r++]= 9; R[r++]= 4; R[r++]= 1; R[r++]= 2; R[r++]= 5; R[r++]= 6; R[r++]=10; R[r++]= 3; R[r++]= 8; 		T[t++]=19; T[t++]=73; T[t++]=43; T[t++]=23; T[t++]=85; T[t++]=39; T[t++]=13; T[t++]=26; T[t++]=67; T[t++]= 9;
			R[r++]= 2; R[r++]= 4; R[r++]= 6; R[r++]= 5; R[r++]= 1; R[r++]= 3; R[r++]= 7; R[r++]= 9; R[r++]=10; R[r++]= 8; 		T[t++]=50; T[t++]=93; T[t++]=80; T[t++]= 7; T[t++]=55; T[t++]=61; T[t++]=57; T[t++]=72; T[t++]=42; T[t++]=46;
			R[r++]= 2; R[r++]= 8; R[r++]= 5; R[r++]= 7; R[r++]= 6; R[r++]= 1; R[r++]= 9; R[r++]= 4; R[r++]=10; R[r++]= 3; 		T[t++]=68; T[t++]=43; T[t++]=99; T[t++]=60; T[t++]=68; T[t++]=91; T[t++]=11; T[t++]=96; T[t++]=11; T[t++]=72;
			R[r++]= 8; R[r++]= 3; R[r++]= 9; R[r++]= 6; R[r++]= 2; R[r++]= 7; R[r++]= 4; R[r++]= 1; R[r++]=10; R[r++]= 5; 		T[t++]=84; T[t++]=34; T[t++]=40; T[t++]= 7; T[t++]=70; T[t++]=74; T[t++]=12; T[t++]=43; T[t++]=69; T[t++]=30;
			R[r++]= 9; R[r++]= 1; R[r++]= 5; R[r++]= 6; R[r++]=10; R[r++]= 2; R[r++]= 8; R[r++]= 7; R[r++]= 4; R[r++]= 3; 		T[t++]=60; T[t++]=49; T[t++]=59; T[t++]=72; T[t++]=63; T[t++]=69; T[t++]=99; T[t++]=45; T[t++]=27; T[t++]= 9;
			R[r++]= 7; R[r++]= 3; R[r++]= 9; R[r++]= 2; R[r++]=10; R[r++]= 5; R[r++]= 8; R[r++]= 1; R[r++]= 6; R[r++]= 4; 		T[t++]=71; T[t++]=91; T[t++]=65; T[t++]=90; T[t++]=98; T[t++]= 8; T[t++]=50; T[t++]=75; T[t++]=37; T[t++]=17;
			R[r++]= 9; R[r++]= 8; R[r++]= 6; R[r++]= 4; R[r++]= 3; R[r++]= 5; R[r++]=10; R[r++]= 2; R[r++]= 1; R[r++]= 7; 		T[t++]=62; T[t++]=90; T[t++]=98; T[t++]=31; T[t++]=91; T[t++]=38; T[t++]=72; T[t++]= 9; T[t++]=72; T[t++]=49;
			R[r++]= 5; R[r++]= 1; R[r++]=10; R[r++]= 6; R[r++]= 8; R[r++]= 4; R[r++]= 3; R[r++]= 9; R[r++]= 7; R[r++]= 2; 		T[t++]=35; T[t++]=39; T[t++]=74; T[t++]=25; T[t++]=47; T[t++]=52; T[t++]=63; T[t++]=21; T[t++]=35; T[t++]=80;
			R[r++]=10; R[r++]= 1; R[r++]= 4; R[r++]= 9; R[r++]= 2; R[r++]= 7; R[r++]= 3; R[r++]= 6; R[r++]= 5; R[r++]= 8; 		T[t++]=58; T[t++]= 5; T[t++]=50; T[t++]=52; T[t++]=88; T[t++]=20; T[t++]=68; T[t++]=24; T[t++]=53; T[t++]=57;
			R[r++]= 8; R[r++]= 4; R[r++]= 5; R[r++]= 6; R[r++]= 3; R[r++]= 7; R[r++]= 1; R[r++]=10; R[r++]= 2; R[r++]= 9; 		T[t++]=99; T[t++]=91; T[t++]=33; T[t++]=19; T[t++]=18; T[t++]=38; T[t++]=24; T[t++]=35; T[t++]=49; T[t++]= 9;
			R[r++]= 1; R[r++]= 4; R[r++]= 3; R[r++]= 8; R[r++]= 9; R[r++]= 6; R[r++]=10; R[r++]= 2; R[r++]= 7; R[r++]= 5; 		T[t++]=68; T[t++]=60; T[t++]=77; T[t++]=10; T[t++]=60; T[t++]=15; T[t++]=72; T[t++]=18; T[t++]=90; T[t++]=18;
			R[r++]=10; R[r++]= 2; R[r++]= 4; R[r++]= 7; R[r++]= 3; R[r++]= 9; R[r++]= 8; R[r++]= 1; R[r++]= 6; R[r++]= 5; 		T[t++]=79; T[t++]=60; T[t++]=56; T[t++]=91; T[t++]=40; T[t++]=86; T[t++]=72; T[t++]=80; T[t++]=89; T[t++]=51;
			R[r++]= 5; R[r++]= 3; R[r++]= 6; R[r++]= 7; R[r++]= 9; R[r++]= 8; R[r++]= 4; R[r++]= 2; R[r++]= 1; R[r++]=10; 		T[t++]=10; T[t++]=92; T[t++]=23; T[t++]=46; T[t++]=40; T[t++]=72; T[t++]= 6; T[t++]=23; T[t++]=95; T[t++]=34;
			R[r++]= 3; R[r++]= 6; R[r++]=10; R[r++]= 9; R[r++]= 1; R[r++]= 7; R[r++]= 4; R[r++]= 8; R[r++]= 2; R[r++]= 5; 		T[t++]=24; T[t++]=29; T[t++]=49; T[t++]=55; T[t++]=47; T[t++]=77; T[t++]=77; T[t++]= 8; T[t++]=28; T[t++]=48;

        } else if(intProblema ==25) {
            intMakeSpanOtimo = 977;

			R[r++]= 9; R[r++]= 5; R[r++]= 4; R[r++]= 3; R[r++]= 1; R[r++]= 6; R[r++]=10; R[r++]= 2; R[r++]= 8; R[r++]= 7; 		T[t++]=14; T[t++]=75; T[t++]=12; T[t++]=38; T[t++]=76; T[t++]=97; T[t++]=12; T[t++]=29; T[t++]=44; T[t++]=66;
			R[r++]= 6; R[r++]= 4; R[r++]= 3; R[r++]= 5; R[r++]= 7; R[r++]=10; R[r++]= 1; R[r++]= 2; R[r++]= 8; R[r++]= 9; 		T[t++]=38; T[t++]=82; T[t++]=85; T[t++]=58; T[t++]=87; T[t++]=89; T[t++]=43; T[t++]=80; T[t++]=69; T[t++]=92;
			R[r++]=10; R[r++]= 2; R[r++]= 1; R[r++]= 7; R[r++]= 5; R[r++]= 8; R[r++]= 4; R[r++]= 6; R[r++]= 9; R[r++]= 3; 		T[t++]= 5; T[t++]=84; T[t++]=43; T[t++]=48; T[t++]= 8; T[t++]= 7; T[t++]=41; T[t++]=61; T[t++]=66; T[t++]=14;
			R[r++]= 3; R[r++]= 2; R[r++]= 1; R[r++]= 6; R[r++]= 5; R[r++]= 8; R[r++]=10; R[r++]= 9; R[r++]= 4; R[r++]= 7; 		T[t++]=42; T[t++]= 8; T[t++]=96; T[t++]=19; T[t++]=59; T[t++]=97; T[t++]=73; T[t++]=43; T[t++]=74; T[t++]=41;
			R[r++]= 7; R[r++]= 3; R[r++]= 4; R[r++]= 9; R[r++]= 5; R[r++]= 8; R[r++]= 2; R[r++]= 6; R[r++]=10; R[r++]= 1; 		T[t++]=55; T[t++]=70; T[t++]=75; T[t++]=42; T[t++]=37; T[t++]=23; T[t++]=48; T[t++]= 5; T[t++]=38; T[t++]= 7;
			R[r++]= 9; R[r++]= 3; R[r++]= 8; R[r++]= 1; R[r++]= 6; R[r++]= 4; R[r++]= 5; R[r++]= 7; R[r++]=10; R[r++]= 2; 		T[t++]= 9; T[t++]=72; T[t++]=31; T[t++]=79; T[t++]=73; T[t++]=95; T[t++]=25; T[t++]=43; T[t++]=60; T[t++]=56;
			R[r++]= 1; R[r++]= 3; R[r++]= 4; R[r++]= 6; R[r++]= 5; R[r++]=10; R[r++]= 9; R[r++]= 7; R[r++]= 8; R[r++]= 2; 		T[t++]=97; T[t++]=64; T[t++]=78; T[t++]=21; T[t++]=94; T[t++]=31; T[t++]=53; T[t++]=16; T[t++]=86; T[t++]= 7;
			R[r++]= 4; R[r++]= 8; R[r++]=10; R[r++]= 1; R[r++]= 3; R[r++]= 5; R[r++]= 6; R[r++]= 2; R[r++]= 9; R[r++]= 7; 		T[t++]=86; T[t++]=85; T[t++]=63; T[t++]=61; T[t++]=65; T[t++]=30; T[t++]=32; T[t++]=33; T[t++]=44; T[t++]=59;
			R[r++]= 3; R[r++]= 4; R[r++]= 5; R[r++]= 7; R[r++]= 2; R[r++]=10; R[r++]= 9; R[r++]= 1; R[r++]= 6; R[r++]= 8; 		T[t++]=44; T[t++]=16; T[t++]=11; T[t++]=45; T[t++]=30; T[t++]=84; T[t++]=93; T[t++]=60; T[t++]=61; T[t++]=90;
			R[r++]= 8; R[r++]= 9; R[r++]= 5; R[r++]= 7; R[r++]= 1; R[r++]= 6; R[r++]= 3; R[r++]=10; R[r++]= 4; R[r++]= 2; 		T[t++]=36; T[t++]=31; T[t++]=47; T[t++]=52; T[t++]=32; T[t++]=11; T[t++]=28; T[t++]=35; T[t++]=20; T[t++]=49;
			R[r++]= 9; R[r++]= 7; R[r++]= 8; R[r++]= 5; R[r++]= 6; R[r++]= 4; R[r++]= 1; R[r++]= 3; R[r++]=10; R[r++]= 2; 		T[t++]=20; T[t++]=49; T[t++]=74; T[t++]=10; T[t++]=17; T[t++]=34; T[t++]=85; T[t++]=77; T[t++]=68; T[t++]=84;
			R[r++]= 2; R[r++]= 6; R[r++]= 9; R[r++]= 7; R[r++]= 5; R[r++]= 1; R[r++]= 4; R[r++]= 3; R[r++]= 8; R[r++]=10; 		T[t++]=85; T[t++]= 7; T[t++]=71; T[t++]=59; T[t++]=76; T[t++]=17; T[t++]=29; T[t++]=17; T[t++]=48; T[t++]=13;
			R[r++]= 3; R[r++]= 7; R[r++]= 8; R[r++]= 2; R[r++]= 5; R[r++]= 9; R[r++]= 1; R[r++]= 4; R[r++]=10; R[r++]= 6; 		T[t++]=15; T[t++]=87; T[t++]=11; T[t++]=39; T[t++]=39; T[t++]=43; T[t++]=19; T[t++]=32; T[t++]=16; T[t++]=64;
			R[r++]= 7; R[r++]= 3; R[r++]= 6; R[r++]= 9; R[r++]= 2; R[r++]= 8; R[r++]=10; R[r++]= 5; R[r++]= 4; R[r++]= 1; 		T[t++]=32; T[t++]=92; T[t++]=33; T[t++]=82; T[t++]=83; T[t++]=57; T[t++]=99; T[t++]=91; T[t++]=99; T[t++]= 8;
			R[r++]= 5; R[r++]= 8; R[r++]= 9; R[r++]= 2; R[r++]= 4; R[r++]= 3; R[r++]= 7; R[r++]=10; R[r++]= 6; R[r++]= 1; 		T[t++]=88; T[t++]= 7; T[t++]=27; T[t++]=38; T[t++]=91; T[t++]=69; T[t++]=21; T[t++]=62; T[t++]=39; T[t++]=48;

        } else {
            printf( "Problema deve estar entre LA21 e LA25");
            getchar();
            return 0; // 0 = erro

        }

    #endif

    return 1; // ' = sucesso

} // void SetProblem( int argc, char **argv ){

/******************************************************************************
* 									Main Program							  *
*******************************************************************************/

int main(int argc, char **argv)
{
    // int outIter = 400;

    // ArqCon = ProximoArquivo( Con, "txt" ); // NOME DO ARQUIVO DE CONVERGÊNCIA
    // ArqBsi = ProximoArquivo( Bsi, "txt" ); // NOME DO ARQUIVO DE MELHOR SEQUENCIA
    // ArqCsv = ProximoArquivo( Csv, "csv" ); // NOME DO ARQUIVO DE ROTAS

    // INICIA AS VARIÁVEIS DE ACORDO COM O PARÂMETRO
    if (SetProblem( argc, argv ) == 0){
        return 0;
    }

    system("cls");

    cout << "sequencing rule: " << strSemente << endl;
    cout << "New test for the scheduling problem LA"<< intProblema <<"."<<endl;
    cout << "Execucao numero: " << intInstancia << endl;
    cout << "It tries to found the optimal sequencing of the jobs.\n\n";
    cout.flush();

    // remove(ArqCon.c_str());
    // remove(ArqBsi.c_str());

    int height  = MACHINE;
    int width   = JOB-1;

    GA2DBinaryStringGenome genome(width, height, Objective);

    GASteadyStateGA ga(genome);
    ga.minimize();

    ga.populationSize(10);
    ga.pReplacement(0.9);
    //ga.nReplacement(80);
    ga.nGenerations(49);
    ga.pMutation(0.01);
    ga.pCrossover(0.10);
    ga.scoreFilename(ArqCon.c_str());
    ga.scoreFrequency(1);
    ga.selectScores(GAStatistics::Minimum);
    ga.flushFrequency(1);
    ga.nBestGenomes(newBestIndividuals);

    ga.pConvergence(1);
    ga.nConvergence((int)MACHINE*JOB/2);
    ga.terminator(GAGeneticAlgorithm::TerminateUponConvergence);

    for(int i = 0; i < outIter; i++) {
        primeira_geracao=true;

        if(i == 0){
            //cout << "// Setup: cria as sementes" << endl;
            setup();
            //cout << "// Setup: criou as sementes" << endl;j*MACHINE+m
        }

        ga.initialize();
        //
        while (!ga.done()) {
            cout <<"\rRunning Iteraction " << i+1 << " (of " << outIter <<
            ") and Generation " << ga.generation()+2;
            ++ga;
            //
        }

        if (currentCriticalPath.size() > 0 and intAbordagem != 1)  {
            localSearch(ga.statistics());
        } else {
            intSolucoesFactiveis=intSolucoesFactiveis; // APENAS PARA BREAKPOINT
        }

    } // for(int i = 0; i < outIter; i++) {

    cout << "\n\nComplete!!! Please check the following files in your DESKTOP FOLDER: \n\"BestSequenceIdentified.txt\"\n\"Convergence.txt\"\n\n"<<endl;

    double end = clock();
    double elapsed = ((double)(end-start))/CLOCKS_PER_SEC;
//    elapsed *= -1;
    horas = (elapsed/horas_seg);
    minutos = (elapsed -(horas_seg*horas))/60;
    segundos = (elapsed -(horas_seg*horas)-(minutos*60));
    cout << "-->" << " Elapsed time (in seconds): " << elapsed << "s" << endl;
    printf("--> Elapsed time (human readable): %dh:%dm:%ds",horas,minutos,segundos);

    ofstream fileOut;
    fileOut.open(ArqBsi.c_str(), ios::app);
    fileOut << "Tempo de processamento = "<<elapsed << "s [ "<<horas<<":"<<minutos<<":"<<segundos<<" ]"<<endl;
    fileOut << "Problema testado = LA"<< intProblema << " ("<<JOB<<" jobs x "<<MACHINE<<" máquinas)"<<endl;
    fileOut << "\nParâmetros utilizados:"<< endl;
    fileOut << "\tMakespan para indivíduos não-factíveis: "<< ITER << endl;
    fileOut << "\tNúmero de laços externos: "<< outIter << endl;
    fileOut << "\tNúmero de gerações: "<< ga.nGenerations()+1 << endl;
    fileOut << "\tNúmero mínimo de gerações para convergência: "<< ga.nConvergence()<< endl;
    fileOut << "\tTamanho da população: "<< ga.populationSize() << endl;
    fileOut << "\tPercentual de substituição da população: "<< ga.pReplacement()*100 << "%"<<endl;
    fileOut << "\tProbabilidade de cruzamento: "<< ga.pCrossover()*100 << "%"<<endl;
    fileOut << "\tProbabilidade de mutação: "<< ga.pMutation()*100 << "%"<<endl;
    fileOut << "\tNúmero de melhores indivíduos (para busca local): "<< ga.nBestGenomes() << endl;
    fileOut << "\tNúmero de soluções factíveis: "<< intSolucoesFactiveis << endl;
    fileOut << "\tNúmero de soluções NÃO factíveis: "<< intSolucoesNaoFactiveis << endl;
    fileOut.close();

    std::stringstream ssHeader;
    std::stringstream ssResult;

    char strMakeSpanOtimo[5];
    itoa(intMakeSpanOtimo, strMakeSpanOtimo, 10);

    ssHeader << "Semente;";        ssResult << strSemente                           << ";" ;
    ssHeader << "Lacos Ext;";      ssResult << outIter                              << ";" ;
    ssHeader << "MakeSpan(" << strMakeSpanOtimo << ");";    ssResult << bestCurrent << ";" ;
    ssHeader << "Decorrido;";      ssResult << horas<<":"<<minutos<<":"<<segundos   << ";" ;
    ssHeader << "Factivel;";       ssResult << intSolucoesFactiveis                 << ";" ;
    ssHeader << "Nao-Factivel;";   ssResult << intSolucoesNaoFactiveis              << ";" ;
    ssHeader << "Problema;";       ssResult << "LA" << intProblema << " (" << JOB <<" jobs x "<<MACHINE<<" maquinas)" << ";" ;
    ssHeader << "Num. Geracoes;";  ssResult << ga.nGenerations()+1                  << ";" ;
    ssHeader << "Tam. Populacao;"; ssResult << ga.populationSize()                  << ";" ;
    ssHeader << "Sequencia;";

    for (int i=0; i < JOB * MACHINE; i++){
        ssResult << vetBestSequence[i] << "," ;

    }

    ssResult << ";";

    if(bestCurrent < intMakeSpanOtimo){
        ssResult << "*** RECORD ***"                  << ";" ;
    }

    // VERIFICA SE O ARQUIVO DE HISTÓRICO JÁ EXISTE
    int intArqHisExiste = ArquivoExiste(ArqHis.c_str());

    ofstream fileHst;
    fileHst.open(ArqHis.c_str(), ios::app);

    // SÓ ACRESCENTA O CABEÇALHO SE O ARQUIVO FOR NOVO
    if (intArqHisExiste==0) {
        fileHst << ssHeader.str() << endl;
    }

    fileHst << ssResult.str() << endl;
    fileHst.close();

    // getchar();
    return 0;
} // Main


/******************************************************************************
* 	Function: desktopFolder 												  *
*	Short Description: Get the Path to the Current User's Desktop Folder. 	  *
*******************************************************************************/

string desktopFolder()
{
	char path[ MAX_PATH ];

	if (SHGetFolderPath( NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, path ) == S_OK) {
        return path;
    }
    return "Couldn't find the path to the Desktop folder!";
}

string ProximoArquivo( string pNomeArq, string pExtArq )
{
    int iArq = 1;
//    string sRetorno="";
    string sArquivo;
    char buffer[3];

    while(true) {
        itoa(iArq, buffer, 10);
        sArquivo = PastaDesktop + pNomeArq + (string)buffer + "." + pExtArq;

        ofstream arquivo;
        arquivo.open( sArquivo.c_str(),ios_base::in); /* Abre para leitura */

        if(!arquivo) {
            break;
        }
        else {
            arquivo.close();
            iArq++;
        }
    }

    return sArquivo;
} // ProximoArquivo()

/******************************************************************************
* 	Function: changeSeed													  *
*	Short Description: Changes the seed of the next runs based on the best	  *
*					   seed from the previous one.							  *
*******************************************************************************/
void changeSeed(const GAStatistics& g)
{

    cout << endl << "************* Muda as sementes *********** " << endl;

    // Estruturas temporarias para sementes
    vector <vector <int> > Stemp(nBestIndividuals);
    vector<int> SS;
    //
    //Antes de mudar a semente eu avalio os três melhores e identifico a semente que deve mudar
    //
    for(int n=0;n < nBestIndividuals; n++) {
        // cout << "TopList " << n+1 << endl;
        // Pega o n-esimo individuo da lista dos melhores
        GA2DBinaryStringGenome & genome = (GA2DBinaryStringGenome &)g.bestIndividual(n);
        //bestCurrent = ITER;
        int P[MACHINE*JOB];
        vector<int> score(nBestIndividuals);
        //Permuta todas as sementes em S com base no genoma 'genome'
        for (int num = 0; num < nBestIndividuals; num++) {
            // Copia S para uma estrutura temporaria que sofrerá permutação
            SS = S[num];
            //
            for(int i=0; i<genome.width(); i++) {
                for(int j=0; j<genome.height(); j++) {
                    if(genome.gene(i,j) == 1) {
                        int val = SS[(j*JOB)+i];
                        SS[(j*JOB)+i] = SS[(j*JOB)+i+1];
                        SS[(j*JOB)+i+1] = val;
                    } // IF
                } // FOR
            } // FOR

//            int block = -1;
//            for(int i=0; i<MACHINE*JOB; i++) {
//                int p = (i%JOB)+1;
//                if(p == 1) block++;
//                P[(block*JOB)+SS[i]-1] = p;
//            } // FOR

            // Guarda os valores de aptidao
            AtualizarVetorComArray( P, SS );
            score[num] = factivel(P,R,1,T);

        } // FOR

        vector<int> scoreV2(score);
        sort(scoreV2.begin(),scoreV2.end());
        int pos = find(score.begin(),score.end(), scoreV2[0]) - score.begin();
        bestSeedsIndex[n] = pos;
        scoreTopList[n] = score[pos];

    } // for(int n=0;n < nBestIndividuals; n++) {

    for(int n=0;n < nBestIndividuals; n++) {
        // Pega o n-esimo individuo da lista dos melhores
        GA2DBinaryStringGenome & genome = (GA2DBinaryStringGenome &)g.bestIndividual(n);
        //
        // Pega a semente que deve sofrer permutacao
        Stemp[n] = S[bestSeedsIndex[n]];
        // Aplica permutação

        for(int i=0; i<genome.width(); i++) {
            for(int j=0; j<genome.height(); j++) {
                if(genome.gene(i,j) == 1) {
                    int val = Stemp[n][(j*JOB)+i];
                    Stemp[n][(j*JOB)+i] = Stemp[n][(j*JOB)+i+1];
                    Stemp[n][(j*JOB)+i+1] = val;
                } // IF
            } // FOR
        } // FOR
    } // FOR
    // Atualiza a semente original S
    S = Stemp;
    return;
} // void changeSeed(const GAStatistics& g)

/****************************************************************************************************
*   Function: OrderVectorFIFO                                                                       *
*   Short Description: Order vector                                                                 *
*   pOrder = 1 (FIFO + SPT)                                                                         *
*          = 2 (FIFO + LPT)                                                                         *
*****************************************************************************************************
*       QUANDO CHEGAREM MAIS DE UM JOB AO MESMO TEMPO NA MÁQUINA SERÁ APLICADO UM SEGUNDO           *
*   CRITÉRIO DE ORDENAÇÃO (LPT OU SPT) APENAS ENTRE OS JOBS CONCORRENTES.                           *
*                                                                                                   *
*       NO EXEMPLO ABAIXO (LA01: 5 x 10), CADA COLUNA REPRESENTA OS JOBS QUE VÃO CHEGAR EM CADA     *
*   MÁQUINA. NA PRIMEIRA COLUNA É POSSÍVEL VERIFICAR QUE OS JOBS 1; 4 E 6 CHEGARÃO AO MESMO TEMPO   *
*   NA MÁQUINA 1. TAMBÉM É POSSÍVEL VERIFICAR QUE OS JOBS 2 E 5 CHEGARÃO AO MESMO TEMPO, ASSIM COMO *
*   OS JOBS 3; 7 E 9.                                                                               *
*                                                                                                   *
*        ROTA                    TEMPOS        SEQUENCIA                                            *
*      -------------           --------------  --------------------------------------               *
* j01  2, 1, 5, 4, 3  Job  1   21,53,95,55,34  Maq 1: {2;5} {1;4;8} {6;9} {3;7;10}                  *
* j02  1, 4, 5, 3, 2  Job  2   21,52,16,26,71  Maq 2: {1;4;6} {9} {3;7;8} {5;10} {2}                *
* j03  4, 5, 2, 3, 1  Job  3   39,98,42,31,12  Maq 3: {8} {6} {5;10} {2;3;4;7} {1;9}                *
* j04  2, 1, 5, 3, 4  Job  4   77,55,79,66,77  Maq 4: {3;7;9} {2;5;10} {1;8} {4;6}                  *
* j05  1, 4, 3, 2, 5  Job  5   83,34,64,19,37  Maq 5: {10} {3;7} {1;2;4;6;9} {5;8}                  *
* j06  2, 3, 5, 1, 4  Job  6   54,43,79,92,62                                                       *
* j07  4, 5, 2, 3, 1  Job  7   69,77,87,87,93                                                       *
* j08  3, 1, 2, 4, 5  Job  8   38,60,41,24,83                                                       *
* j09  4, 2, 5, 1, 3  Job  9   17,49,25,44,98                                                       *
* j10  5, 4, 3, 2, 1  Job 10   77,79,43,75,96                                                       *
*                                                                                                   *
*****************************************************************************************************
*       NO EXEMPLO ACIMA CADA BLOCO COM MAIS DE UM JOB REPRESENTA UMA CONCORRÊNCIA, POIS, OS JOBS   *
*   CHEGARÃO JUNTOS. ENTÃO É NECESSÁRIO VERIFICAR O TEMPO DE PROCESSAMENTO DE CADA JOB NA RESPECTIVA*
*   MÁQUINA E ORDENAR SEGUNDO O CRITÉRIO DESEJADO:                                                  *
*   SPT: SHORTEST PROCESS TIME                                                                      *
*   LPT: LARGEST PROCESS TIME                                                                       *
*                                                                                                   *
*   EXEMPLO ORDENAÇÃO POR SPT:                                                                      *
*                                                                                                   *
*   A MAQUINA 1 APRESENTA O SEGUINTE CENÁRIO:                                                       *
*                                                                                                   *
*   JOBS     TEMPOS       OBSERVAÇÃO                                                                *
*   -------  ------------ -------------------------------------                                     *
*   {2;5}    {21;83}      SEM MUDANÇA, POIS, TEMPOS JÁ OBEDECEM A ORDEM SPT                         *
*   {1;4;8}  {21;77;38}   OS JOBS 4 E 8 DEVERÃO SER PERMUTADOS, FICANDO {1;8;4}                     *
*   {6;9}    {54;17}      OS JOBS 6 E 9 DEVERÃO SER PERMUTADOS, FICANDO {9;6}                       *
*   {3;7;10} {39;69;77}   SEM MUDANÇA, POIS, TEMPOS JÁ OBEDECEM A ORDEM SPT                         *
*                                                                                                   *
*   RESULTADO: A MAQUINA 1 DEVERÁ PROCESSAR OS JOBS NA SEGUINTE ORDEM:                              *
*              2; 5; 1; 8; 4; 9; 6; 3; 7; 10                                                        *
*                                                                                                   *
****************************************************************************************************/
int * OrderVectorFIFO( int pVetor[], int pOrder)
{
    int aux = 0;
    int scoreOrderDepois;
    int P[MACHINE*JOB]; // VETOR PARA AVALIAR FACTIBILIDADE

    boolean blnOrdena;

    int intTempo1, intTempo2, intJob1, intJob2, temp, intElemento1, intElemento2, intColuna = 0;
    //  static int vRetorno[JOB*MACHINE];

    AtualizarVetorComVetor( P, pVetor );
    scoreOrderDepois = factivel(P, R, 1, T);

    // CLASSIFICAÇÃO DOS JOBS DE ACORDO COM A MAQUINA
    for (int iMaq=0; iMaq < MACHINE; iMaq++) { // PERCORRE CADA "LINHA" DA MATRIZ, REPRESENTANDO UMA MÁQUINA

        for (int iJob=0; iJob < JOB; iJob++) { // PERCORRE CADA COLUNA DA LINHA, REPRESENTANDO UM JOB DA MÁQUINA
            aux = iJob;
            intElemento1 = (MACHINE * aux++) + intColuna; // IDENTIFICA QUAL É A CÉLULA DO VETOR
            intJob1 = R[intElemento1]; // IDENTIFICA O NÚMERO DO JOB
            intTempo1 = T[intElemento1]; // IDENTIFICA O TEMPO DO JOB

            // PERCORRE TODOS OS DEMAIS JOBS DA MAQUINA PARA IDENTIFICAR JOBS QUE CHEGARAM AO MESMO TEMPO
            for (int i=1; i < JOB; i++) {
                intElemento2 = (MACHINE * aux++) + intColuna;
                intJob2 = R[intElemento2];
                intTempo2 = T[intElemento2];

                // SE OS NUMEROS DOS JOBS FOREM IGUAIS, QUER DIZER QUE CHEGARAM AO MESMO TEMPO
                if (intJob1 == intJob2) {
                    if (pOrder == 1) {
                        blnOrdena = intTempo1 > intTempo2; // SPT - VAI PERMUTAR SE O TEMPO 1 FOR MAIOR QUE O 2
                    }
                    else {
                        blnOrdena = intTempo1 < intTempo2; // LPT - VAI PERMUTAR SE O TEMPO 1 FOR MENOR QUE O 2

                    } // if (pOrder == 1) {

                    if (blnOrdena){
                        //scoreOrderAntes = factivel(pVetor, R, 1);

                        temp = pVetor[intElemento1];
                        pVetor[intElemento1] = pVetor[intElemento2];
                        pVetor[intElemento2] = temp;

                        //avalia a semente e atualiza valor de aptidão em scoreTopList

                        AtualizarVetorComVetor( P, pVetor );
                        scoreOrderDepois = factivel(P, R, 1, T);

                        // SE A SOLUÇÃO NÃO FOR FACTIVEL, DESFAZ A PERMUTAÇÃO.
                        if (scoreOrderDepois == ITER) {
                            pVetor[intElemento2] = pVetor[intElemento1];
                            pVetor[intElemento1] = temp;

                            AtualizarVetorComVetor( P, pVetor );
                            scoreOrderDepois = factivel(P, R, 1, T);

                        } else {
                            temp=temp; // APENAS PARA DEBUGAR

                        }

                    } // if (blnOrdena){

                } // if (intJob1 == intJob2) {

            } // for (int i=1; i < JOB; i++) {

        } // for (int iJob=0; iJob < JOB; iJob++) {

        intColuna++;
        aux=0;

    } // for (int iMaq=0; iMaq < MACHINE; iMaq++) {

    AtualizarVetorComVetor( P, pVetor );
    scoreOrderDepois = factivel(P, R, 1, T);

    // SE A SOLUÇÃO NÃO FOR FACTIVEL, DESFAZ A PERMUTAÇÃO.
    if (scoreOrderDepois == ITER) {
        pVetor[intElemento2] = pVetor[intElemento1];
        pVetor[intElemento1] = temp;

        AtualizarVetorComVetor( P, pVetor );
        scoreOrderDepois = factivel(P, R, 1, T);

    } else {
        temp=temp; // APENAS PARA DEBUGAR

    }

return pVetor;
} // OrderVectorFIFO

/*
int * OrderVectorNEH_CP( int pVetor[], int pOrder)
{
    int aux = 0;
    int scoreOrderDepois;
    int P[MACHINE*JOB]; // VETOR PARA AVALIAR FACTIBILIDADE

    int intTempo1, temp, intElemento1, intColuna = 0;

    AtualizarVetorComVetor( P, pVetor );
    scoreOrderDepois = factivel(P, R, 1, T);

    for (int i=0; i < JOB * MACHINE; i++){
        P[i] = 0;
    }

    // CLASSIFICAÇÃO DOS JOBS DE ACORDO COM A MAQUINA
    for (int iMaq=0; iMaq < MACHINE; iMaq++) { // PERCORRE CADA "LINHA" DA MATRIZ, REPRESENTANDO UMA MÁQUINA

        int JobMaq[JOB][2];

        for (int iJob=0; iJob < JOB; iJob++) { // PERCORRE CADA COLUNA DA LINHA, REPRESENTANDO UM JOB DA MÁQUINA
            aux = iJob;
            intElemento1 = (MACHINE * aux++) + intColuna; // IDENTIFICA QUAL É A CÉLULA DO VETOR
            intTempo1 = T[intElemento1]; // IDENTIFICA O TEMPO DO JOB

            JobMaq[iJob][0]=R[intElemento1];
            JobMaq[iJob][1]=intTempo1;

        } // for (int iJob=0; iJob < JOB; iJob++) {

        for( int i=0;i<JOB;i++){
            for(int l=0;l<JOB;l++){
                if (JobMaq[i][1] > JobMaq[l][1]){
                    int maqAux = JobMaq[l][0];
                    int tmpAux = JobMaq[l][1];

                    JobMaq[l][0] = JobMaq[i][0];
                    JobMaq[l][1] = JobMaq[i][1];

                    JobMaq[i][0] = maqAux;
                    JobMaq[i][1] = tmpAux;

                }
            }
        }

        for (int iJob=0; iJob < JOB; iJob++) { // PERCORRE CADA COLUNA DA LINHA, REPRESENTANDO UM JOB DA MÁQUINA
            aux = iJob;
            intElemento1 = (MACHINE * aux++) + intColuna; // IDENTIFICA QUAL É A CÉLULA DO VETOR
            intTempo1 = T[intElemento1]; // IDENTIFICA O TEMPO DO JOB

            P[intElemento1] = JobMaq[iJob][0];

        } // for (int iJob=0; iJob < JOB; iJob++) {

        intColuna++;
        aux=0;

    } // for (int iMaq=0; iMaq < MACHINE; iMaq++) {

    scoreOrderDepois = factivel(P, R, 1, T);

    // SE A SOLUÇÃO NÃO FOR FACTIVEL, DESFAZ A PERMUTAÇÃO.
    if (scoreOrderDepois == ITER) {
        AtualizarVetorComVetor( P, pVetor );
        scoreOrderDepois = factivel(P, R, 1, T);

    } else {
        temp=temp; // APENAS PARA DEBUGAR

    }

return pVetor;
} // OrderVectorNEH_CP

*/

/******************************************************************************
* 	Function: gera_sequencia												  *
*	Short Description: Generates the initial seed from the routes given by    *
*					   the problem, based on FIFO dispatching rule.		  *
*******************************************************************************/

int * gera_sequencia (int rota[])
{
    int i, aux1=0, aux2=0, conta_job=1;
    static int s_temp[MACHINE*JOB];

    while(conta_job<JOB+1){
        for(i=0; i<JOB; i++) { //  VARRE OS JOBS POR MAQUINA
            // SE O JOB ATUAL DA MÁQUINA = JOB ANALISADO
            if(rota[i*MACHINE+aux1]==conta_job) {
                s_temp[aux2]=i+1;
                aux2++;
                if(aux2%JOB==0) {
                    conta_job++;
                    aux1=0;
                    break;
                }
            }
            if(i==JOB-1) aux1++;
        }
    }

/*
    i = factivel(s_temp, R, 1);

    if (intAbordagem == 1){
        s_temp[0]=s_temp[1]; } // não-factivel

    else if ((intAbordagem == 3) || (intAbordagem == 4)){ // 3=FIFO+SPT; 4=FIFO+LPT
        return OrderVectorFIFO(s_temp, intAbordagem);

    }
*/
    return s_temp;

} // int * gera_sequencia (int rota[])


////////////////////////////////////////////////////////////////////
// Faz setup da semente na primeira geração de todo o processo
////////////////////////////////////////////////////////////////////
void setup()
{
    int * ss = gera_sequencia(R); // GERA SEMENTE INICIAL COM ABORDAGEM FIFO
    int P[MACHINE*JOB];

        S.clear();

        for (int num = 0; num < nBestIndividuals; num++){
            for(int sem = 0; sem < JOB*MACHINE; sem++){
                S[num].push_back(ss[sem]);

            }
        }

        int score; // = factivel(P,R,1,T);

        // Calcula aptidao da semente
        AtualizarVetorComArray(P, S[0]);
        score = factivel(P,R,1,T);

        // Calcula Caminho Critico
        findCriticalPath(S[0]);

        //Faz busca local na semente inicial e atualiza S[0]
        bestCurrent = ITER;
        localSearch();

        // Calcula aptidao da semente
        AtualizarVetorComArray(P, S[0]);
        score = factivel(P,R,1,T);

        if (intAbordagem==1){ // SEMENTE INICIAL NÃO FACTIVEL

            for (int intMaquina=0; intMaquina < MACHINE; intMaquina++){
                // REALIZA UM EMBARALHAMENTO DOS JOBS COM A INTENÇÃO DE PRODUZIR UMA SOLUÇÃO NÃO-FACTIVEL
                std::random_shuffle(S[intMaquina].begin(),S[intMaquina].end());

                // Calcula aptidao da semente
                AtualizarVetorComArray( P, S[0] );
                score = factivel(P,R,1,T);

                // SE A SOLUÇÃO NÃO FOR FACTÍVEL, SAI DO LAÇO FOR
                if (score == ITER) {
                    break;
                }

            }

        } else if (intAbordagem==3){
            OrderVectorFIFO(ss,1); // FIFO + SPT

        } else if (intAbordagem==4){
            OrderVectorFIFO(ss,2); // FIFO + LPT

        } else if (intAbordagem==5){
            // OrderVectorNEH( ss ); // FIFO + LPT
            OrderVectorNEH( ss );

        }

        S.clear();

        for (int num = 0; num < nBestIndividuals; num++){
            for(int sem = 0; sem < JOB*MACHINE; sem++){
                S[num].push_back(ss[sem]);
                // S[num][sem] = ss[sem];

            }
        }

        // Calcula aptidao da semente
        AtualizarVetorComArray( P, S[0] );
        score = factivel(P,R,1,T);

        cout << "-->" << " Score Inicial: " << score << " - MakeSpan: " << intMakeSpanOtimo << endl;
        bestCurrent = score;
        // Inicialmente, todos os  valores são iguais
        for (int num = 0; num < nBestIndividuals; num++)
            scoreTopList[num] = score;

        //cout << endl << "score: " << bestCurrent << endl;
        //getchar();
} // setup()

//
//*****************************************************************************
// calculate the makespan based on the critical path method
int projectDuration(vector<int> Seed)
{
    //
    int *p;
    vector<int> startVec;
    vector<int> endVec;
    vector<int> timeVec;

    // Cria arestas do grafo
    for(int j=0; j<JOB; j++)
    {
            startVec.push_back(0);
            timeVec.push_back(0);

            for(int m=1; m<=MACHINE; m++)
            {
                    //
                    endVec.push_back(j*MACHINE+m); //1-index
                    startVec.push_back(j*MACHINE+m); //1-index
                    timeVec.push_back(T[j*MACHINE+m-1]); //0-index
            }
            endVec.push_back(JOB*MACHINE+1);
    }
    //
    for(int m=0; m<MACHINE; m++)
    {
            for(int j=0; j<JOB; j++)
            {
                    int job = Seed[m*JOB+j] - 1;
                    // Vai na 'linha' de R referente ao job
                    int start = job * MACHINE;
                    int end = job * MACHINE + MACHINE - 1;
                    p = std::find(R+start,R+end, m+1);

                    //Posicao
                    int pos = p - (R+start);
                    int no = job*MACHINE + pos + 1;
                    //
                    // Coloca na lista de arestas
                    if(j == 0)
                    {
                         //seta apenas como nó inicial
                         startVec.push_back(no); //1-index
                         timeVec.push_back(T[no-1]); //0-index
                    }
                    if(j == JOB-1)
                    {
                         //seta apenas como nó final
                         endVec.push_back(no); //1-index
                    }
                    if(j != 0 && j != JOB-1)
                    {
                        //seta como inicial e final
                        startVec.push_back(no); //1-index
                        endVec.push_back(no); //1-index
                        timeVec.push_back(T[no-1]); //0-index
                    }
            }
    }
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    int numberVertex = MACHINE*JOB + 2;   // number  of vertex
    int numberActivities = endVec.size(); // number of edges
//    int i, j;
    int j;

    /*vertices are of the form (v, u) */
    //indegree of each vertex (that is, count the number of edges entering them)
    vector<int> indegree(numberVertex, 0);

    std::vector<int> v(startVec);
    std::vector<int> u(endVec);
    std::vector<int> d(timeVec);

    int project_duration=0;//project duration


    /*# Compute the indegree for each vertex v from the graph:
    for each neighbor u of v: indegree[u] += 1*/
    for (j=0; j<numberActivities; j++){
        indegree[u[j]]++;
    }

    queue<int> Q; //queue Q = empty queue
    int distance [numberVertex];
    memset(distance, 0, sizeof(int) * numberVertex);//distance = array filled with zeroes

    //vetor de conjuntos de nos
    vector< set<int> > vecSetCriticalPath(numberVertex);

    //for each vertex v:
    //if indegree[v] = 0:
    //insert v on Q
    for (j=0; j<numberVertex; j++)
    {
        if (indegree[j]==0)
            Q.push(v[j]);
    }

    int first;

    //printf ("first in the queue=%d\n", Q.front());

    /*for each neighbor u of v:
    d istance[u] = max(distance[u], distance[v] + time(v, u))
    indegree[u] -= 1
    if indegree[u] = 0:
    insert u on Q
    */
    while (!Q.empty()){ //while Q is not empty:

     first= Q.front();  //v = get front element from Q
     Q.pop(); //delete de first from queue

     //Para todo vizinho de first faca
     for(int i = 0;i < numberActivities;i++) //**********MELHORAR ESSA ESTRUTURA **********
     {
        if(v[i] == first) //
        {
                //////////////////////////////////////////////////////////////
                //
                distance[u[i]]=std::max(distance[u[i]], distance[v[i]]+ d[i]);
                indegree[u[i]]-=1;
                ////////////////////////////////////////////////////////////////

                if (indegree[u[i]]==0){
                   Q.push(u[i]);
                }
        }
     }
   }//fecha while

    /*Now, select the vertex x with the largest distance.
    This is the minimum total project_duration.*/
    project_duration = *std::max_element(distance,distance+numberVertex);
    return project_duration;
} // projectDuration

/******************************************************************************
* void findCriticalPath
*******************************************************************************/
void findCriticalPath(vector<int> s)
{
    //
    int *p;
    vector<int> startVec;
    vector<int> endVec;
    vector<int> timeVec;

    // Cria arestas do grafo
    for(int j=0; j<JOB; j++) {
        startVec.push_back(0);
        timeVec.push_back(0);

        for(int m=1; m<=MACHINE; m++) {
            //
            endVec.push_back(j*MACHINE+m); //1-index
            startVec.push_back(j*MACHINE+m); //1-index
            timeVec.push_back(T[j*MACHINE+m-1]); //0-index
        }
        endVec.push_back(JOB*MACHINE+1);
    }
    //
    //    for(int j=0; j<endVec.size(); j++)
    //    {
    //            cout << startVec[j] << "  " << endVec[j] << "  " << timeVec[j] << endl;
    //    }
    //    getchar();
    //
    for(int m=0; m<MACHINE; m++) {
        for(int j=0; j<JOB; j++) {
            int job = s[m*JOB+j] - 1;
            // cout << "Job: " << job+1 << endl;
            // Vai na 'linha' de R referente ao job
            int start = job * MACHINE;
            int end = job * MACHINE + MACHINE - 1;
            //cout << "start = " << start << " end = " << end << endl;
            p = std::find(R+start,R+end, m+1);

            //cout << "Maq: " << m+1 << endl;
            //Posicao
            int pos = p - (R+start);
            //cout << "Operacao: " << pos+1 << endl;
            int no = job*MACHINE + pos + 1;
            //cout << endl << "****No***** " << no << endl;
            //
            // Coloca na lista de arestas

            if(j == 0) {
                //seta apenas como nó inicial
                startVec.push_back(no); //1-index
                timeVec.push_back(T[no-1]); //0-index
            }

            if(j == JOB-1) {
                //seta apenas como nó final
                endVec.push_back(no); //1-index
            }

            if(j != 0 && j != JOB-1) {
                //seta como inicial e final
                startVec.push_back(no); //1-index
                endVec.push_back(no); //1-index
                timeVec.push_back(T[no-1]); //0-index
            }
        }
    //            //
    }
    //    getchar();
    //    for(int j=0; j<endVec.size(); j++)
    //    {
    //            cout << startVec[j] << "  " << endVec[j] << "  " << timeVec[j] << "  " << endVec.size() << endl;
    //    }
    //
    //    getchar();


    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    int numberVertex = MACHINE*JOB + 2;   // number  of vertex
    int numberActivities = endVec.size(); // number of edges
    //    int i, j;
    int j;

    /*vertices are of the form (v, u) */
    //indegree of each vertex (that is, count the number of edges entering them)
    vector<int> indegree(numberVertex, 0);

    std::vector<int> v(startVec);
    std::vector<int> u(endVec);
    std::vector<int> d(timeVec);

    //    for (j=0;j<numberActivities; j++)
    //        cout << "***** " << v[j] << "  " << u[j] << "  " << d[j] << endl;
    //    getchar();

    int project_duration=0;//project duration


    /*# Compute the indegree for each vertex v from the graph:
    for each neighbor u of v: indegree[u] += 1*/
    for (j=0; j<numberActivities; j++){
        indegree[u[j]]++;
    }

    //    for (j=0;j<numberVertex; j++)
    //        printf ("indegree %d= %d\n",j,indegree[j] );
    //getchar();

    queue<int> Q; //queue Q = empty queue
    int distance [numberVertex];
    memset(distance, 0, sizeof(int) * numberVertex);//distance = array filled with zeroes

    //vetor de conjuntos de nos
    vector< set<int> > vecSetCriticalPath(numberVertex);

    //for each vertex v:
    //if indegree[v] = 0:
    //insert v on Q
    for (j=0; j<numberVertex; j++)
    {
        if (indegree[j]==0)
            Q.push(v[j]);
    }

    int first;

    //printf ("first in the queue=%d\n", Q.front());

    /*for each neighbor u of v:
    d istance[u] = max(distance[u], distance[v] + time(v, u))
    indegree[u] -= 1
    if indegree[u] = 0:
    insert u on Q
    */
    while (!Q.empty()){ //while Q is not empty:
        first= Q.front();  //v = get front element from Q
        Q.pop(); //delete de first from queue

        //Para todo vizinho de first faca
        for(int i = 0;i < numberActivities;i++) { //**********MELHORAR ESSA ESTRUTURA **********
            if(v[i] == first) {
                //////////////////////////////////////////////////////////////
                distance[u[i]]=std::max(distance[u[i]], distance[v[i]]+ d[i]);
                indegree[u[i]]-=1;
                ////////////////////////////////////////////////////////////////

                if (indegree[u[i]]==0){
                   Q.push(u[i]);
                }
            } // fecha IF
        } // fecha for
        ////////////////////////////////////////////
        //
        //for(int ii=0; ii<numberVertex; ii++)
        //cout << *std::max_element(distance,distance+numberVertex) << endl;
        //getchar();
        ////////////////////////////////////////////
   }//fecha while


    /*Now, select the vertex x with the largest distance.
    This is the minimum total project_duration.*/
    project_duration = *std::max_element(distance,distance+numberVertex);
    //printf ("Total Project Duration %d\n", project_duration);
    //getchar();
    ////////////////////////////////////////////////////////////////////////////
    // Pega posicao do elemento máximo = primeiro na 'lista critica'
    int criticalNode = std::find(distance, distance+numberVertex, project_duration) - distance;
    vector<int> criticalPath;
    //criticalPath.push_back(criticalNode);
    ////////////////////////////////////////////////////////////////////////////
    while(criticalNode != 0){
        vector<int> neighborhood;
        vector<int> neighDistances;
        //
        // varre recursivamente os vizinhos e pega o máximo
        for(int i = 0;i < numberActivities;i++) {//**********MELHORAR ESSA ESTRUTURA **********
            if(u[i] == criticalNode) {
                neighborhood.push_back(v[i]);
                neighDistances.push_back(distance[v[i]]);
                //cout << v[i] << " " << distance[v[i]] << endl;
                //getchar();
            }
        }

        int max = *std::max_element(neighDistances.begin(),neighDistances.end());
        int pp =  std::find(neighDistances.begin(), neighDistances.end(), max) - neighDistances.begin();
        if(max == 0) pp = neighDistances.size() - 1;
        criticalNode = neighborhood[pp];
        criticalPath.push_back(criticalNode);
        //cout << "criticalNode = " << criticalNode << "Max: " << max << endl;
    }
    //getchar();
    ////////////////////////////////////////////////////////////////////////////
    //    cout << "criticalPath: " << endl;
    //    for (int j=0; j<criticalPath.size(); j++)
    //        cout << criticalPath[j] << endl;
    //intSolucoesFactiveis
    //    cout << '\n';

    currentCriticalPath = criticalPath;

//    system("PAUSE");

return;
} // findCriticalPath

//
/******************************************************************************
* 	Function: Objective														  *
*	Short Description: Function called automatically by the GA, where the     *
*					   chromosome are evaluated (and can be manipulated as	  *
*					   well.
*******************************************************************************/

float Objective(GAGenome& g)
{
    //cout << " Entrou em Objective " << endl;
    //getchar();

    GA2DBinaryStringGenome & genome = (GA2DBinaryStringGenome &)g;
    vector<int> score(nBestIndividuals);
    int val = 0;
    //    int count=0;
    //    unsigned int i, j;

    int P[MACHINE*JOB];
    //    int lin_1, col_1, lin_2, col_2;
    // Na primeira geração apenas adiciona a semente na população
    // A aptidão já foi calculada no setup ou na changeSeed
    if (primeira_geracao==true) {
        //cout << "// Coloca individuo nulo na população" << endl;
        genome.unset(0,0,genome.width(),genome.height());
        //
        primeira_geracao=false;

        //        cout << " \n Primeira geracao em Objective " << endl;
        //        cout << " Semente: " << endl;
        //        for (int num = 0; num < S[0].size(); num++)
        //            cout << S[0][num] << ", ";
        //        cout << endl;
        //        getchar();
        // Retorna o valor de aptidao da melhor semente
        return bestCurrent;

    } else {
        /////////////////////////////////////////////
        /// TESTA O GENOMA
        /////////////////////////////////////////////
        //Permuta todas as sementes em S com base no genoma 'genome'
        vector<int> SS;
        //bestCurrent = ITER;
        for (int num = 0; num < nBestIndividuals; num++) {
            // Copia S para uma estrutura temporaria que sofrerá permutação
            SS = S[num];
            //
            for(int i=0; i<genome.width(); i++) {
                for(int j=0; j<genome.height(); j++) {
                    if(genome.gene(i,j) == 1) {
                        val = SS[(j*JOB)+i];
                        SS[(j*JOB)+i] = SS[(j*JOB)+i+1];
                        SS[(j*JOB)+i+1] = val;
                    }
                }
            }

            // Apenas o menor score interessa.
            AtualizarVetorComArray( P, SS );

            score[num] = factivel(P,R,1,T);

            // Conta as soluções factiveis
            if (score[num] < ITER) {
                intSolucoesFactiveis++;

            } else {
                intSolucoesNaoFactiveis++;

            }

            sort(score.begin(), score.end());
            if(score[0] < bestCurrent) bestCurrent = score[0];
            ///////////////////////////////////////////////////
            // Imprime se melhorou
            ///////////////////////////////////////////////////
            if (score[0] < score_ant) {
                ofstream fileOut;
                fileOut.open(ArqBsi.c_str(), ios::out);
                for (int seq = 0; seq < JOB*MACHINE; seq++){
                    fileOut << P[seq] << endl;
                    vetBestSequence[seq] = P[seq];

                }

                fileOut << "\nMakespan da seqüência prioridades acima = " << bestCurrent <<" (UT)"<< endl;
                fileOut.close();
                score_ant = score[0];
                return (float)score[0];
            }
        }//fecha for
     }//fecha else
     return (float)score[0];
} // Objective(GAGenome& g)

/******************************************************************************
* 	Function: Local Search (multiple seeds)                                    *
*	Short Description:
*******************************************************************************/
void localSearch(const GAStatistics &g)
{
    // Estruturas temporarias para sementes
    vector <vector <int> > Stemp(nBestIndividuals);
    vector<int> SS;
    vector<int> bestSeed;
    int val;
    int P[MACHINE*JOB];
    int bgn, fnl, pos, job, opr, maq, bgn2, fnl2, pos2, job2, opr2, maq2;
    //
    //Faz uma busla local em cada um dos melhores indivíduos
    //
    for(int n=0;n < newBestIndividuals; n++)
    {
        //cout << "\nTopList " << n+1 << endl;
        // Pega o n-esimo individuo da lista dos melhores
        GA2DBinaryStringGenome & genome = (GA2DBinaryStringGenome &)g.bestIndividual(n);
        //bestCurrent = ITER;
        vector<int> score(newBestIndividuals);
        //cout << genome << endl;

        // Copia S para uma estrutura temporaria que sofrerá permutação
        SS = S[0];
        // Permuta os valores de SS baseado no cromossomo gerado pelo AG
        for(int i=0; i<genome.width(); i++)
        {
            for(int j=0; j<genome.height(); j++)
            {
                if(genome.gene(i,j) == 1)
                {
                    val = SS[(j*JOB)+i];
                    SS[(j*JOB)+i] = SS[(j*JOB)+i+1];
                    SS[(j*JOB)+i+1] = val;
                }
            }
        }
        //
        findCriticalPath(SS);
        if(n == 0)
             bestSeed = SS;

        ////////////////////////////////////////////////////////////////////////
        // INICIA BUSCA LOCAL NA SEMENTE PERMUTADA PELO INDIVIDUO
        // BUSCA LOCAL COM BASE NO CAMINHO CRÍTICO DA SEMENTE PERMUTADA
        ////////////////////////////////////////////////////////////////////////
        for(unsigned int i = 0; i < currentCriticalPath.size() - 2; i++) {
            ////////////////////////////////////////////////////////////////////////////
            //identifica a i-ésima operacao do caminho critico
            job = (int) (currentCriticalPath[i]-1) / MACHINE; //0-index
            opr = (int) ((currentCriticalPath[i]-1) - job*MACHINE); //0-index
            // busca na matriz R quem é a maquina
            maq = R[job * MACHINE + opr]; //1-index
            // encontra posicao na semente
            bgn = (maq - 1) * JOB;
            fnl = bgn + (JOB - 1);
            pos = std::find(SS.begin()+bgn, SS.begin()+fnl, (job+1)) - SS.begin();

            //cout << endl;
            //cout << currentCriticalPath[i] << "  " << job+1 << "  " << opr+1 << "  " << maq << endl;
            //cout << currentCriticalPath[i] << "  " << bgn << "  " << fnl << "  " << pos << endl;
            //getchar();
            ////////////////////////////////////////////////////////////////
            // identifica todas as outras operacoes críticas da mesma maq
            for(unsigned int j = i+1; j < currentCriticalPath.size() - 1; j++) {
                //
                job2 = (int) (currentCriticalPath[j]-1) / MACHINE; //0-index
                opr2 = (int) ((currentCriticalPath[j]-1) - job2*MACHINE); //0-index
                // busca na matriz R quem é a maquina

                maq2 = R[job2 * MACHINE + opr2]; //1-index
                // encontra posicao na semente
                bgn2 = (maq2 - 1) * JOB;
                fnl2 = bgn2 + (JOB - 1);
                pos2 = std::find(SS.begin()+bgn2, SS.begin()+fnl2, (job2+1)) - SS.begin();
                //
                if(maq2  == maq){ //permuta
                    //
                    vector<int> newSS = SS;
                    val = newSS[pos];
                    newSS[pos] = newSS[pos2];
                    newSS[pos2] = val;

                    // Apenas o menor score interessa.
                    AtualizarVetorComArray( P, newSS );
                    score[0] = factivel(P,R,1,T);
                    //
                    int fitness = (int) score[0];
                    ////////////////////////////////////////////////////
                    //cout << endl << projectDuration(SS) << " " << fitness << endl;
                    ////////////////////////////////////////////////////
                    if(fitness < bestCurrent) {
                        bestCurrent = fitness;
                        //mudo a semente
                        bestSeed = newSS;
                        //cout << "\nAtualiza solucao em Local Search: " << bestCurrent << endl;
                        //getchar();
                    }

                    //cout << endl;
                    //cout << currentCriticalPath[j] << "  " << job2+1 << "  " << opr2+1 << "  " << maq2 << endl;
                    //cout << currentCriticalPath[j] << "  " << bgn2 << "  " << fnl2 << "  " << pos2 << endl;
                    //getchar();

                }//Fecha if maq == maq2
            }
        }
    }
    //
    //findCriticalPath(S[0]);
    S[0] = bestSeed;
    return;
 } // localSearch(const GAStatistics &g)

/******************************************************************************
* 	Function: Local Search	(single seed)									  *
*	Short Description:
*******************************************************************************/
void localSearch()
{
    //cout << "****  EM LOCAL SEARCH *****" << endl;
    //getchar();
    vector<int> score(currentCriticalPath.size());
    int val = 0;
    //        int count=0;
    //        unsigned int i, j;
    int maq, maq2, pos, pos2;
    vector<int> bestInitialSeed;

    int P[MACHINE*JOB];

    /////////////////////////////////////////////
    /// Permuta semente e calcula aptidao
    /////////////////////////////////////////////
    //Permuta todas as sementes em S
    vector<int> SS;
        //bestCurrent = ITER;
    bool melhora = true;
    while(melhora){
        // Copia S para uma estrutura temporaria que sofrerá permutação
        SS = S[0];
        findCriticalPath(SS);
        melhora = false;

        //cout << "*" << currentCriticalPath.size() << endl;
        //getchar();
        for(unsigned int i = 0; i < currentCriticalPath.size() - 1; i++) {
            ////////////////////////////////////////////////////////////////
            //identifica a i-ésima operacao do caminho critico
            int job = (int) (currentCriticalPath[i]-1) / MACHINE; //0-index
            int opr = (int) ((currentCriticalPath[i]-1) - job*MACHINE); //0-index
            // busca na matriz R quem é a maquina
            maq = R[job * MACHINE + opr]; //1-index
            // encontra posicao na semente
            int bgn = (maq - 1) * JOB;
            int fnl = bgn + (JOB - 1);
            pos = std::find(SS.begin()+bgn, SS.begin()+fnl, (job+1)) - SS.begin();

            //cout << endl;
            //cout << currentCriticalPath[i] << "  " << job+1 << "  " << opr+1 << "  " << maq << endl;
            //cout << currentCriticalPath[i] << "  " << bgn << "  " << fnl << "  " << pos << endl;
            //getchar();
            ////////////////////////////////////////////////////////////////
            // identifica todas as outras operacoes críticas da mesma maq
            for(unsigned int j = i+1; j < currentCriticalPath.size() - 1; j++) {
                int job2 = (int) (currentCriticalPath[j]-1) / MACHINE; //0-index
                int opr2 = (int) ((currentCriticalPath[j]-1) - job2*MACHINE); //0-index
                // busca na matriz R quem é a maquina
                maq2 = R[job2 * MACHINE + opr2]; //1-index
                // encontra posicao na semente
                int bgn2 = (maq2 - 1) * JOB;
                int fnl2 = bgn2 + (JOB - 1);
                pos2 = std::find(SS.begin()+bgn2, SS.begin()+fnl2, (job2+1)) - SS.begin();

                if(maq  == maq2){ //permuta
                    //
                    vector<int> newSS = SS;
                    val = newSS[pos];
                    newSS[pos] = newSS[pos2];
                    newSS[pos2] = val;

                    // Apenas o menor score interessa.
                    AtualizarVetorComArray( P, newSS );
                    score[0] = factivel(P,R,1,T);
                    //
                    int fitness = score[0];
                    //cout << endl << scoreTopList[0] << " " << fitness << endl;
                    if(fitness < bestCurrent) {
                        bestCurrent = fitness;
                        //mudo a semente
                        bestInitialSeed = newSS;
                        melhora = true;
                        //cout << "****Atualiza Semente Local Search: " << fitness << endl;
                        //getchar();
                    }
                } // if(maq  == maq2){ //permuta
            } // for(unsigned int j = i+1; j < currentCriticalPath.size() - 1; j++) {
        } // for(unsigned int i = 0; i < currentCriticalPath.size() - 1; i++) {

        if(melhora == true) {
            S[0] = bestInitialSeed;
            findCriticalPath(S[0]);
        }

    }//fecha while
    return;
} // localSearch()

////////////////////////////////////////////////////////////////////////////////
// cpBasedSeedGA()
////////////////////////////////////////////////////////////////////////////////
void cpBasedSeedGA()
{
        //
        int cp_SIZE = currentCriticalPath.size();
        int height = 1;
        int width  = cp_SIZE - 1;
        int nLocalBestIndividuals = 1;

        GA2DBinaryStringGenome genome(width, height, Objective);

        GASteadyStateGA ga(genome);
        ga.minimize();

        ga.populationSize(100);
        ga.pReplacement(0.80);
        ga.nGenerations(49);
        ga.pMutation(0.01);
        ga.pCrossover(0.90);
        ga.scoreFilename(ArqCon.c_str());
        ga.scoreFrequency(1);
        ga.selectScores(GAStatistics::Minimum);
        ga.flushFrequency(1);

        ga.nBestGenomes(nLocalBestIndividuals);

        ga.pConvergence(1);
        ga.nConvergence((int)MACHINE*JOB/5);
        ga.terminator(GAGeneticAlgorithm::TerminateUponConvergence);

        primeira_geracao=true;
        cpBasedSeed = true;

        ga.initialize();
        //
        while (!ga.done())
        {
            cout <<"\rCritical Path  " << 1 << " (of " << 1 <<
            ") and Generation " << ga.generation()+2;

            ++ga;
            //
        }
        cpBasedSeed = false;
} // cpBasedSeedGA()

int ArquivoExiste(const char *filename)
{
  FILE *arquivo;

  if ((arquivo = fopen(filename, "r"))) {
    fclose(arquivo);
    return 1;
  }
  return 0;
} // ArquivoExiste
