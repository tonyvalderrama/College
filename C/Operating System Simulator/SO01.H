/* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
						 INGENIERIA EN SISTEMAS COMPUTACIONALES
										Sistemas Operativos
M¢dulo     : Definiciones.
Archivo    : SO01.H
Objetivo   : Definiciones para el uso de funciones principales.
Dise¤o     : Jorge Eduardo Aparicio Mayorga  -  90120511-3,
				 Jos‚ Antonio Valderrama Ramos  -  90120509-9.
Implemento : Los mismos.
Versi¢n    : Marzo de 1994.
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ */

/* ------------------------------------------------------------------------
Definici¢n de macros
------------------------------------------------------------------------- */

#define MAXCODIGOS	 5
#define NUM_RECS		 5
#define NUM_REGS		 5
#define INIC_TIMER	 1
#define INIC_PRI		 3
#define MAXREC			 100
#define INTRBLOK_TIME 10
#define DESPERTAR_TIME 10

#define LISTO     	0                  /* Estados posibles de un proceso */
#define SLISTO       1
#define EJECUCION    2
#define BLOQUEADO    3
#define SBLOQUEADO   4
#define DORMIDO		5

#define CLEAR_ZF		ZF = 0	              /* Limpiar y prender banderas */
#define CLEAR_SF		SF = 0
#define SET_ZF			ZF = 1
#define SET_SF			SF = 1

#define FUE_BLOQ		2

#define ADD				0                                       /* Nem¢nicos */
#define BEGIN			1
#define CALL			2
#define CALLM        3
#define CMP          4
#define DEC          5
#define EMPTY        6
#define END          7
#define ENDM         8
#define ENDP         9
#define FINMO       10
#define FRE         11
#define GET         12
#define INC         13
#define INIMO       14
#define JEQ         15
#define JGE         16
#define JLE         17
#define JMP         18
#define JNE         19
#define MOV         20
#define MOVR        21
#define MOVRV       22
#define MOVV        23
#define NOP         24
#define P           25
#define PARBEGIN    26
#define PAREND      27
#define PROC        28
#define PROCM       29
#define SET         30
#define SIGNAL      31
#define SUB		     32
#define V           33
#define WAIT        34
				 /* Rutas a operadores e instrucci¢n en el segmento de c¢digo */
#define INSTR pcb_act->CS[pcb_act->IP].id_op
#define OP1 pcb_act->CS[pcb_act->IP].op1
#define OP2 pcb_act->CS[pcb_act->IP].op2
#define CODE pcb_act->PRG[pcb_act->IP].line
#define SEMAFOROS pcb_act->sems
#define VARCON pcb_act->var_con

/* ------------------------------------------------------------------------
Definici¢n de tipos
------------------------------------------------------------------------- */
typedef struct									     /* Formato de una instrucci¢n */
{  unsigned int id_op;					   /* Identificador de la instrucci¢n */
	unsigned int op1;       								    /* Primer operador */
	unsigned int op2;											   /* Segundo operador */
} instruccion;

typedef struct
{	char line[18];
} code_line;

typedef struct
{	int edo;
	hls_ptr l_sem;
} semaforo;

struct pcb								                /* Estructura de un PCB. */
{	HEADSLS;
	char name_proc[13];	                            /* Nombre del proceso */
	unsigned int id_proc;			            /* Identificador del proceso */
	unsigned int edo_proc;			                   /* Estado del proceso */
	unsigned char rec[NUM_RECS];                    /* Arreglo de recursos */
	unsigned int num_solic;					/* # de veces de solicitar recurso */
	unsigned char rec_solic;            /* Recurso del que se ha solicitado*/
	struct pcb *pcb_padre;			              /* Puntero a proceso padre */
	unsigned int hijos;								        /* Contador de hijos */
	instruccion *CS;					         /* Puntero a segmento de c¢digo */
	unsigned int *DS;					          /* Puntero a segmento de datos */
	semaforo *sems;							    /* Puntero a zona de sem foros */
	code_line *PRG;							   /* Puntero al programa en texto */
	unsigned int IP;				                     /* Instruction Pointer */
	unsigned int SS;					             /* Retorno de procedimiento */
	unsigned int prio;				                /* Prioridad del proceso */
	int reg_maq[NUM_REGS];			                 /* Arreglo de registros */
	unsigned char ZF, SF;			                             /* Banderas */
	hls_ptr *var_con;        /* Puntero de Variable de Condici¢n (Monitor) */
	unsigned int prim_inst;          /* Posici¢n de la primera instrucci¢n */
};
typedef struct pcb PCB;

typedef struct
{	HEADSLS;
	PCB *pcb;
} PCTRL;