
#include "tennis1.h"

/* funcio per moure la pilota; retorna un valor amb alguna d'aquestes	*/
/* possibilitats:							*/
/*	-1 ==> la pilota no ha sortit del taulell			*/
/*	 0 ==> la pilota ha sortit per la porteria esquerra		*/
/*	>0 ==> la pilota ha sortit per la porteria dreta		*/
int moure_pilota(void)
{
	int f_h, c_h, result;
	char rh,rv,rd,pd;

	f_h = pil_pf + pil_vf;		/* posicio hipotetica de la pilota */
	c_h = pil_pc + pil_vc;
	result = -1;		/* inicialment suposem que la pilota no surt */
	rh = rv = rd = pd = ' ';
	if ((f_h != ipil_pf) || (c_h != ipil_pc))
	{		/* si posicio hipotetica no coincideix amb la pos. actual */
		if (f_h != ipil_pf)		/* provar rebot vertical */
		{
			rv = win_quincar(f_h,ipil_pc);	/* veure si hi ha algun obstacle */
			if (rv != ' ')			/* si no hi ha res */
			{
				pil_vf = -pil_vf;		/* canvia velocitat vertical */
				f_h = pil_pf+pil_vf;	/* actualitza posicio hipotetica */
			}
		}
		if (c_h != ipil_pc)		/* provar rebot horitzontal */
		{
			rh = win_quincar(ipil_pf,c_h);	/* veure si hi ha algun obstacle */
			if (rh != ' ')			/* si no hi ha res */
			{
				pil_vc = -pil_vc;		/* canvia velocitat horitzontal */
				c_h = pil_pc+pil_vc;	/* actualitza posicio hipotetica */
			}
		}
		if ((f_h != ipil_pf) && (c_h != ipil_pc))	/* provar rebot diagonal */
		{
			rd = win_quincar(f_h,c_h);
			if (rd != ' ')				/* si no hi ha obstacle */
			{
				pil_vf = -pil_vf;
				pil_vc = -pil_vc;	/* canvia velocitats */
				f_h = pil_pf+pil_vf;
				c_h = pil_pc+pil_vc;		/* actualitza posicio entera */
			}
		}
		if (win_quincar(f_h,c_h) == ' ')	/* verificar posicio definitiva */
		{						/* si no hi ha obstacle */
			win_escricar(ipil_pf,ipil_pc,' ',NO_INV);	/* esborra pilota */
			pil_pf += pil_vf; pil_pc += pil_vc;
			ipil_pf = f_h; ipil_pc = c_h;		/* actualitza posicio actual */
			if ((ipil_pc > 0) && (ipil_pc <= n_col))	/* si no surt */
				win_escricar(ipil_pf,ipil_pc,'.',INVERS); /* imprimeix pilota */
			else
				result = ipil_pc;	/* codi de finalitzacio de partida */
		}
	}
	else
	{
		pil_pf += pil_vf;
		pil_pc += pil_vc;
	}
	return (result);
}

void *mou_pilota_control()
{
	do
	{
		cont = moure_pilota();
		win_retard(retard);
	} while(!final_juego);
	pthread_exit(0);
}