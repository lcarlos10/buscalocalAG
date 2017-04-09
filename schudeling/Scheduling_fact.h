/******************************************************************************
* 	Author: Pedro Schimit 													  *
*	Date: 17/Jun/2013												  		  *
*	Revisions: 																  *
*	17/Jun/13    First release												  *
*	18/Aug/13    Modified by Flavio Grassi to support "square" problems 	  *
*	29/Mar/14    Modified by Flavio Grassi to support random processing times *
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

int factivel(int *vPriorities, int *vSequences, int iPrioMode, int *iTEMPO);

#ifdef __cplusplus
}
#endif

#ifndef _FACT_

#define _FACT_

//Defines

#define PROGRAMA_01
//#define PROGRAMA_06
//#define PROGRAMA_11
//#define PROGRAMA_16
//#define PROGRAMA_21

#ifdef PROGRAMA_01
    #define MACHINE			5
    #define JOB    			10
#endif

#ifdef PROGRAMA_06
    #define MACHINE			5
    #define JOB    			15
#endif

#ifdef PROGRAMA_11
    #define MACHINE			5
    #define JOB    			20
#endif

#ifdef PROGRAMA_16
    #define MACHINE			10
    #define JOB    			10
#endif

#ifdef PROGRAMA_21
    #define MACHINE			10
    #define JOB    			15
#endif

#define SEQUENCE	    JOB
#define TIME_STEP   	1
#define ITER            1000*JOB*TIME_STEP

/******************************************************************************
* 	Function: int factivel(int *vPriorities, int *vSequences, int iPrioMode)  *
*	Short Description: Simulates logically the makespan value of a given 	  *
*					   sequence of jobs.									  *
*	Parameters:																  *
*	vPriorities -> matrix with priorities - JOB lines x MACHINE rows		  *
*	vSequences  -> matrix with jobs - SEQUENCE lines x MACHINE rows			  *
*	iPrioMode   -> 0 - only executes the sequence, no priority				  *
*				   1 - job x machine priority								  *
*	Return:																	  *
*   Time steps for processing. If returns 1000, the sequence x priority	 	  *
*   is not valid.															  *
*******************************************************************************/

int factivel(int *vPriorities, int *vSequences, int iPrioMode, int *iTEMPO);

#endif /* _FACT_ */
