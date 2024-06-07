
#include "tennis2.h"

/* funcio per moure la paleta de l'ordinador autonomament, en funcio de la */
/* velocitat de la paleta (variable global v_pal) */
//void mou_paleta_ordinador(void)
void mou_paleta_ordinador(int index_paleta) {
	int f_h;
	/* char rh,rv,rd; */

	f_h = paletes[index_paleta].po_pf + paletes[index_paleta].v_pal;		/* posicio hipotetica de la paleta */
	if (f_h != paletes[index_paleta].ipo_pf)	/* si pos. hipotetica no coincideix amb pos. actual */
	{
		if (paletes[index_paleta].v_pal > 0.0)			/* verificar moviment cap avall */
		{
			pthread_mutex_lock(&ventana_control);
			if (win_quincar(f_h+l_pal-1,paletes[index_paleta].ipo_pc) == ' ')   /* si no hi ha obstacle */
			{
				win_escricar(paletes[index_paleta].ipo_pf,paletes[index_paleta].ipo_pc,' ',NO_INV);      /* esborra primer bloc */
				paletes[index_paleta].po_pf += paletes[index_paleta].v_pal; paletes[index_paleta].ipo_pf = paletes[index_paleta].po_pf;		/* actualitza posicio */
				win_escricar(paletes[index_paleta].ipo_pf+l_pal-1,paletes[index_paleta].ipo_pc, paletes[index_paleta].id, INVERS); /* impr. ultim bloc */
				pthread_mutex_unlock(&ventana_control);
				pthread_mutex_lock(&moviments_control);
				if (moviments > 0) moviments--;    /* he fet un moviment de la paleta */
				pthread_mutex_unlock(&moviments_control);
			}
			else		/* si hi ha obstacle, canvia el sentit del moviment */
			{
				pthread_mutex_unlock(&ventana_control);
				paletes[index_paleta].v_pal = -paletes[index_paleta].v_pal;
			}
		}
		else			/* verificar moviment cap amunt */
		{
			pthread_mutex_lock(&ventana_control);
			if (win_quincar(f_h,paletes[index_paleta].ipo_pc) == ' ')        /* si no hi ha obstacle */
			{
				win_escricar(paletes[index_paleta].ipo_pf+l_pal-1,paletes[index_paleta].ipo_pc,' ',NO_INV); /* esbo. ultim bloc */
				paletes[index_paleta].po_pf += paletes[index_paleta].v_pal; paletes[index_paleta].ipo_pf = paletes[index_paleta].po_pf;		/* actualitza posicio */
				win_escricar(paletes[index_paleta].ipo_pf,paletes[index_paleta].ipo_pc, paletes[index_paleta].id, INVERS);	/* impr. primer bloc */
				pthread_mutex_unlock(&ventana_control);
				pthread_mutex_lock(&moviments_control);
				if (moviments > 0) moviments--;    /* he fet un moviment de la paleta */
				pthread_mutex_unlock(&moviments_control);
			}
			else		/* si hi ha obstacle, canvia el sentit del moviment */
			{
				pthread_mutex_unlock(&ventana_control);
				paletes[index_paleta].v_pal = -paletes[index_paleta].v_pal;
			}
		}
	}
	else paletes[index_paleta].po_pf += paletes[index_paleta].v_pal; 	/* actualitza posicio vertical real de la paleta */
}

void *mou_paleta_ordinador_control(void* index)
{
	int	index_paleta;

	index_paleta = (intptr_t)index;
	do
	{
		mou_paleta_ordinador(index_paleta);
		win_retard(retard);
		
		pthread_mutex_lock(&pause_control);
		pthread_mutex_unlock(&pause_control);
	} while(!final_juego);
	pthread_exit(0);
}
