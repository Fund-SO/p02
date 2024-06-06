

#include <stdio.h>		/* incloure definicions de funcions estandard */
#include <stdlib.h>
#include "winsuport.h"		/* incloure definicions de funcions propies */
#include <pthread.h> /*Invocar librearía hilos*/
#include <stdint.h>

#define MIN_FIL 7		/* definir limits de variables globals */
#define MAX_FIL 25
#define MIN_COL 10
#define MAX_COL 80
#define MIN_PAL 3
#define MIN_VEL -1.0
#define MAX_VEL 1.0
#define MIN_RET 0.0
#define MAX_RET 5.0

#define MAX_PAL 9

typedef struct s_paleta {
	char id;  /* ID paleta */
	int ipo_pf;
	int ipo_pc;      	/* posicio del la paleta de l'ordinador */
	float po_pf;	/* pos. vertical de la paleta de l'ordinador, en valor real */
	float v_pal;	/* velocitat de la paleta del programa */
	float pal_ret;	/* percentatge de retard de la paleta */
}		t_paleta;

extern int n_fil, n_col, m_por;	/* dimensions del taulell i porteries */
extern int l_pal;			/* longitud de les paletes */

extern int ipil_pf, ipil_pc;		/* posicio de la pilota, en valor enter */
extern float pil_pf, pil_pc;		/* posicio de la pilota, en valor real */
extern float pil_vf, pil_vc;		/* velocitat de la pilota, en valor real*/
extern float pil_ret;			/* percentatge de retard de la pilota */

extern int ipu_pf, ipu_pc;      	/* posicio del la paleta d'usuari */

extern int retard;		/* valor del retard de moviment, en mil.lisegons */
extern int moviments;		/* numero max de moviments paletes per acabar el joc */

extern t_paleta paletes[MAX_PAL];

extern int num_pal;
extern int cont;
extern int tecla;
extern int final_juego;

void	*mou_paleta_ordinador_control(void *index);
void	*mou_pilota_control();
void	*mou_paleta_usuario_control();
