
#include "tennis3.h"

/* funcio per moure la paleta de l'usuari en funcio de la tecla premuda */
void mou_paleta_usuari(int tecla)
{
	waitS(id_ventana_sem);//pthread_mutex_lock(&ventana_control);
	if ((tecla == TEC_AVALL) && (win_quincar(ipu_pf+l_pal,ipu_pc) == ' '))
	{
		win_escricar(ipu_pf,ipu_pc,' ',NO_INV);	   /* esborra primer bloc */
		ipu_pf++;					   /* actualitza posicio */
		win_escricar(ipu_pf+l_pal-1,ipu_pc,'0',INVERS); /* impri. ultim bloc */
		signalS(id_ventana_sem);//pthread_mutex_unlock(&ventana_control);
		waitS(id_moviments_sem);//pthread_mutex_lock(&moviments_control);
		if (*moviments > 0) (*moviments)--;    /* he fet un moviment de la paleta */
		signalS(id_moviments_sem);//pthread_mutex_unlock(&moviments_control);
	}
	else
		signalS(id_ventana_sem);//pthread_mutex_unlock(&ventana_control);
	waitS(id_ventana_sem);//pthread_mutex_lock(&ventana_control);
	if ((tecla == TEC_AMUNT) && (win_quincar(ipu_pf-1,ipu_pc) == ' '))
	{
		win_escricar(ipu_pf+l_pal-1,ipu_pc,' ',NO_INV); /* esborra ultim bloc */
		ipu_pf--;					    /* actualitza posicio */
		win_escricar(ipu_pf,ipu_pc,'0',INVERS);	    /* imprimeix primer bloc */
		signalS(id_ventana_sem);//pthread_mutex_unlock(&ventana_control);
		waitS(id_moviments_sem);//pthread_mutex_lock(&moviments_control);
		if (*moviments > 0) (*moviments)--;
		signalS(id_moviments_sem);//pthread_mutex_unlock(&moviments_control);
	}
	else
		signalS(id_ventana_sem);//pthread_mutex_unlock(&ventana_control);
	//if (tecla == TEC_ESPAI) 
		//win_escristr("ARA HAURIA D'ATURAR ELS ELEMENTS DEL JOC");
}

void *mou_paleta_usuario_control()
{
	int	pausa;

	pausa = 0;
	do
	{
		waitS(id_ventana_sem);//pthread_mutex_lock(&ventana_control);
		tecla = win_gettec(); // w, s
		signalS(id_ventana_sem);//pthread_mutex_unlock(&ventana_control);
		if (tecla == TEC_ESPAI && !pausa) //En caso que la tecla sea ESPACIO y no estemos en pausa: Pausamos
		{
			pthread_mutex_lock(&pause_control);
			pausa = 1;
		}
		else if (tecla == TEC_ESPAI && pausa) //En caso que la tecla sea ESPACIO y estemos en pausa: Reaunudamos el juego
		{
			pthread_mutex_unlock(&pause_control);
			pausa = 0;
		}
		if (tecla == TEC_RETURN && pausa)
			pthread_mutex_unlock(&pause_control);
		else if (!pausa) //Si no estemos en pausa: No pausamos, y además dejamos mover la paleta.
			mou_paleta_usuari(tecla);
		win_retard(retard);
	} while(!*final_juego);
	pthread_exit(0);
}