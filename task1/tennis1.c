#include "tennis1.h"

/* variables globals */
int n_fil, n_col, m_por;	/* dimensions del taulell i porteries */
int l_pal;			/* longitud de les paletes */

int ipu_pf, ipu_pc;      	/* posicio del la paleta d'usuari */

int ipil_pf, ipil_pc;		/* posicio de la pilota, en valor enter */
float pil_pf, pil_pc;		/* posicio de la pilota, en valor real */
float pil_vf, pil_vc;		/* velocitat de la pilota, en valor real*/
float pil_ret;			/* percentatge de retard de la pilota */

int retard;		/* valor del retard de moviment, en mil.lisegons */
int moviments;		/* numero max de moviments paletes per acabar el joc */

t_paleta paletes[MAX_PAL];

int num_pal;
int cont;
int tecla;

int final_juego;

/* funcio per realitzar la carrega dels parametres de joc emmagatzemats */
/* dins un fitxer de text, el nom del qual es passa per referencia en   */
/* 'nom_fit'; si es detecta algun problema, la funcio avorta l'execucio */
/* enviant un missatge per la sortida d'error i retornant el codi per-	*/
/* tinent al SO (segons comentaris del principi del programa).		*/
void carrega_parametres(const char *nom_fit)
{
	FILE *fit;

	fit = fopen(nom_fit,"rt");		/* intenta obrir fitxer */
	if (fit == NULL)
	{
		fprintf(stderr,"No s'ha pogut obrir el fitxer \'%s\'\n",nom_fit);
		exit(2);
	}

	// Carga campo:
	if (!feof(fit))
		fscanf(fit,"%d %d %d %d\n",&n_fil,&n_col,&m_por,&l_pal);
	if ((n_fil < MIN_FIL) || (n_fil > MAX_FIL) ||
		(n_col < MIN_COL) || (n_col > MAX_COL) || (m_por < 0) ||
		(m_por > n_fil-3) || (l_pal < MIN_PAL) || (l_pal > n_fil-3))
	{
		fprintf(stderr,"Error: dimensions del camp de joc incorrectes:\n");
		fprintf(stderr,"\t%d =< n_fil (%d) =< %d\n",MIN_FIL,n_fil,MAX_FIL);
		fprintf(stderr,"\t%d =< n_col (%d) =< %d\n",MIN_COL,n_col,MAX_COL);
		fprintf(stderr,"\t0 =< m_por (%d) =< n_fil-3 (%d)\n",m_por,(n_fil-3));
		fprintf(stderr,"\t%d =< l_pal (%d) =< n_fil-3 (%d)\n",MIN_PAL,l_pal,(n_fil-3));
		fclose(fit);
		exit(3);
	}

	// Carga pelota:
	if (!feof(fit))
		fscanf(fit,"%d %d %f %f %f\n",&ipil_pf,&ipil_pc,&pil_vf,&pil_vc,&pil_ret);
	if ((ipil_pf < 1) || (ipil_pf > n_fil-3) ||
		(ipil_pc < 1) || (ipil_pc > n_col-2) ||
		(pil_vf < MIN_VEL) || (pil_vf > MAX_VEL) ||
		(pil_vc < MIN_VEL) || (pil_vc > MAX_VEL) ||
		(pil_ret < MIN_RET) || (pil_ret > MAX_RET))
	{
		fprintf(stderr,"Error: parametre pilota incorrectes:\n");
		fprintf(stderr,"\t1 =< ipil_pf (%d) =< n_fil-3 (%d)\n",ipil_pf,(n_fil-3));
		fprintf(stderr,"\t1 =< ipil_pc (%d) =< n_col-2 (%d)\n",ipil_pc,(n_col-2));
		fprintf(stderr,"\t%.1f =< pil_vf (%.1f) =< %.1f\n",MIN_VEL,pil_vf,MAX_VEL);
		fprintf(stderr,"\t%.1f =< pil_vc (%.1f) =< %.1f\n",MIN_VEL,pil_vc,MAX_VEL);
		fprintf(stderr,"\t%.1f =< pil_ret (%.1f) =< %.1f\n",MIN_RET,pil_ret,MAX_RET);
		fclose(fit);
		exit(4);
	}

	num_pal = 0;
	// Carga paletas ordenador:
	while(!feof(fit) && (num_pal < MAX_PAL))
	{
		fscanf(fit,"%d %d %f %f\n", &paletes[num_pal].ipo_pf, &paletes[num_pal].ipo_pc, &paletes[num_pal].v_pal, &paletes[num_pal].pal_ret);

		if ((paletes[num_pal].ipo_pf < 1) || (paletes[num_pal].ipo_pf+l_pal > n_fil-2) || 
		(paletes[num_pal].ipo_pc < 5) || (paletes[num_pal].ipo_pc > n_col-2) || 
		(paletes[num_pal].v_pal < MIN_VEL) || (paletes[num_pal].v_pal > MAX_VEL) || 
		(paletes[num_pal].pal_ret < MIN_RET) || (paletes[num_pal].pal_ret > MAX_RET))
		{
			fprintf(stderr,"Error: parametres paleta ordinador incorrectes:\n");
			paletes[num_pal].id = num_pal;
			fprintf(stderr,"\t1 =< ipo_pf (%d) =< n_fil-l_pal-3 (%d)\n",paletes[num_pal].ipo_pf,(n_fil-l_pal-3));
			fprintf(stderr,"\t5 =< ipo_pc (%d) =< n_col-2 (%d)\n",paletes[num_pal].ipo_pc,(n_col-2));
			fprintf(stderr,"\t%.1f =< v_pal (%.1f) =< %.1f\n",MIN_VEL,paletes[num_pal].v_pal,MAX_VEL);
			fprintf(stderr,"\t%.1f =< pal_ret (%.1f) =< %.1f\n",MIN_RET,paletes[num_pal].pal_ret,MAX_RET);

			fclose(fit);
			exit(5);
		}
		num_pal++;
	}
	/*
	if (!feof(fit)) fscanf(fit,"%d %d %f %f\n",&ipo_pf,&ipo_pc,&v_pal,&pal_ret);
	if ((ipo_pf < 1) || (ipo_pf+l_pal > n_fil-2) ||
		(ipo_pc < 5) || (ipo_pc > n_col-2) ||
		(v_pal < MIN_VEL) || (v_pal > MAX_VEL) ||
		(pal_ret < MIN_RET) || (pal_ret > MAX_RET))
		{
		fprintf(stderr,"Error: parametres paleta ordinador incorrectes:\n");
		fprintf(stderr,"\t1 =< ipo_pf (%d) =< n_fil-l_pal-3 (%d)\n",ipo_pf,(n_fil-l_pal-3));
		fprintf(stderr,"\t5 =< ipo_pc (%d) =< n_col-2 (%d)\n",ipo_pc,(n_col-2));
		fprintf(stderr,"\t%.1f =< v_pal (%.1f) =< %.1f\n",MIN_VEL,v_pal,MAX_VEL);
		fprintf(stderr,"\t%.1f =< pal_ret (%.1f) =< %.1f\n",MIN_RET,pal_ret,MAX_RET);
		fclose(fit);
		exit(5);
		}
	*/
	fclose(fit);			/* fitxer carregat: tot OK! */
}


/* funcio per inicialitar les variables i visualitzar l'estat inicial del joc */
int inicialitza_joc(void)
{
	int i, i_port, f_port, retwin;
	char strin[51];

	retwin = win_ini(&n_fil,&n_col,'+',INVERS);   /* intenta crear taulell */

	if (retwin < 0)       /* si no pot crear l'entorn de joc amb les curses */
	{
		fprintf(stderr,"Error en la creacio del taulell de joc:\t");
		switch (retwin)
		{
			case -1: fprintf(stderr,"camp de joc ja creat!\n");
				break;
			case -2: fprintf(stderr,"no s'ha pogut inicialitzar l'entorn de curses!\n");
				break;
			case -3: fprintf(stderr,"les mides del camp demanades son massa grans!\n");
				break;
			case -4: fprintf(stderr,"no s'ha pogut crear la finestra!\n");
				break;
		}
		return(retwin);
	}

	i_port = n_fil/2 - m_por/2;	    /* crea els forats de la porteria */
	if (n_fil%2 == 0) i_port--;
	if (i_port == 0) i_port=1;
	f_port = i_port + m_por -1;
	for (i = i_port; i <= f_port; i++)
	{
		win_escricar(i,0,' ',NO_INV);
		win_escricar(i,n_col-1,' ',NO_INV);
	}


	ipu_pf = n_fil/2; ipu_pc = 3;		/* inicialitzar pos. paletes */
	if (ipu_pf+l_pal >= n_fil-3) ipu_pf = 1;
	for (i=0; i< l_pal; i++)	    /* dibuixar paleta inicialment */
	{
		win_escricar(ipu_pf + i, ipu_pc, '0', INVERS);
		for (int j = 0; j < num_pal; j++)
			win_escricar(paletes[j].ipo_pf + i, paletes[j].ipo_pc, paletes[i].id, INVERS); 
	}
	paletes[i].po_pf = paletes[i].ipo_pf;		/* fixar valor real paleta ordinador */

	pil_pf = ipil_pf; pil_pc = ipil_pc;	/* fixar valor real posicio pilota */
	win_escricar(ipil_pf, ipil_pc, '.',INVERS);	/* dibuix inicial pilota */

	sprintf(strin,"Tecles: \'%c\'-> amunt, \'%c\'-> avall, RETURN-> sortir.",
			TEC_AMUNT, TEC_AVALL);
	win_escristr(strin);
	return(0);
}

/* programa principal				    */
int main(int n_args, const char *ll_args[])
{
	int i;
	pthread_t hilo_usuario, hilo_ordenador[MAX_PAL], hilo_pelota, hilo_reloj;

	if ((n_args != 3) && (n_args !=4))
	{
		fprintf(stderr,"Comanda: tennis0 fit_param moviments [retard]\n");
		exit(1);
	}
	carrega_parametres(ll_args[1]);
	moviments=atoi(ll_args[2]);

	if (n_args == 4) retard = atoi(ll_args[3]);
	else retard = 100;

	if (inicialitza_joc() !=0)    /* intenta crear el taulell de joc */
		exit(4);   /* aborta si hi ha algun problema amb taulell */

	final_juego = 0;
	cont = -1;
	pthread_create(&hilo_usuario, NULL, mou_paleta_usuario_control, NULL); // Hilo paleta usuari
	for (i = 0; i < num_pal; i++)
		pthread_create(&hilo_ordenador[i], NULL, mou_paleta_ordinador_control, (void*)(intptr_t) i); // Hilo paleta ordenador
	pthread_create(&hilo_pelota, NULL, mou_pilota_control, NULL); // Hilo pelota
	//pthread_create(&hilo_reloj, NULL, moure_pilota, NULL); // Hilo pelota

	/********** bucle principal del joc **********/
	while ((tecla != TEC_RETURN) && (cont == -1) && ((moviments > 0) || moviments == -1));

	final_juego = 1;
	pthread_join(hilo_usuario, NULL);
	for (i = 0; i < num_pal; i++)
		pthread_join(hilo_ordenador[i], NULL); // Hilo paleta ordenador
	pthread_join(hilo_pelota, NULL);

	win_fi();

	if (tecla == TEC_RETURN) printf("S'ha aturat el joc amb la tecla RETURN!\n");
	else { if (cont == 0 || moviments == 0) printf("Ha guanyat l'ordinador!\n");
			else printf("Ha guanyat l'usuari!\n"); }
	return(0);
}