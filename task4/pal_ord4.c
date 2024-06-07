#include "tennis4.h"

int n_fil, n_col;    /* dimensions del taulell i porteries */
int l_pal;           /* longitud de les paletes */

int retard;      /* valor del retard de moviment, en mil.lisegons */

t_paleta paletes[MAX_PAL];

int final_id;
int *final_juego;
int moviments_id;
int *moviments;      /* numero max de moviments paletes per acabar el joc */
int camp_id;

int mida_camp;
void *camp_ptr;

int id_ventana_sem;
int id_moviments_sem;

static void mou_paleta_ordinador(t_paleta *paleta)
{
    int f_h;

    f_h = paleta->po_pf + paleta->v_pal;  /* posicio hipotetica de la paleta */
    if (f_h != paleta->ipo_pf)    /* si pos. hipotetica no coincideix amb pos. actual */
    {
        if (paleta->v_pal > 0.0)  /* verificar moviment cap avall */
        {
            waitS(id_ventana_sem);
            if (win_quincar(f_h + l_pal - 1, paleta->ipo_pc) == ' ')  /* si no hi ha obstacle */
            {
                win_escricar(paleta->ipo_pf, paleta->ipo_pc, ' ', NO_INV);  /* esborra primer bloc */
                paleta->po_pf += paleta->v_pal; 
                paleta->ipo_pf = paleta->po_pf;  /* actualitza posicio */
                win_escricar(paleta->ipo_pf + l_pal - 1, paleta->ipo_pc, paleta->id, INVERS);  /* impr. ultim bloc */
                signalS(id_ventana_sem);
                waitS(id_moviments_sem);
                if (*moviments > 0) (*moviments)--;  /* he fet un moviment de la paleta */
                signalS(id_moviments_sem);
            }
            else  /* si hi ha obstacle, canvia el sentit del moviment */
            {
                signalS(id_ventana_sem);
                paleta->v_pal = -paleta->v_pal;
            }
        }
        else  /* verificar moviment cap amunt */
        {
            waitS(id_ventana_sem);
            if (win_quincar(f_h, paleta->ipo_pc) == ' ')  /* si no hi ha obstacle */
            {
                win_escricar(paleta->ipo_pf + l_pal - 1, paleta->ipo_pc, ' ', NO_INV);  /* esbo. ultim bloc */
                paleta->po_pf += paleta->v_pal; 
                paleta->ipo_pf = paleta->po_pf;  /* actualitza posicio */
                win_escricar(paleta->ipo_pf, paleta->ipo_pc, paleta->id, INVERS);  /* impr. primer bloc */
                signalS(id_ventana_sem);
                waitS(id_moviments_sem);
                if (*moviments > 0) (*moviments)--;  /* he fet un moviment de la paleta */
                signalS(id_moviments_sem);
            }
            else  /* si hi ha obstacle, canvia el sentit del moviment */
            {
                signalS(id_ventana_sem);
                paleta->v_pal = -paleta->v_pal;
            }
        }
    }
    else 
    {
        paleta->po_pf += paleta->v_pal;  /* actualitza posicio vertical real de la paleta */
    }
}

static t_paleta convert_paleta(char **argv)
{
    t_paleta paleta;

    paleta.ipo_pf = atoi(argv[1]);
    paleta.ipo_pc = atoi(argv[2]);
    paleta.po_pf = atof(argv[3]);
    paleta.v_pal = atof(argv[4]);
    paleta.pal_ret = atof(argv[5]);
    paleta.id = atoi(argv[6]);

    return paleta;
}

int main(int argc, char **argv)
{
    t_paleta paleta;
    int final_id, moviments_id, camp_id;
    void *camp_ptr;
    int n_col, n_fil;

    (void)argc;

    paleta = convert_paleta(argv);
    final_id = atoi(argv[7]);
    moviments_id = atoi(argv[8]);
    camp_id = atoi(argv[9]);

    camp_ptr = map_mem(camp_id);

    final_juego = map_mem(final_id);
    moviments = map_mem(moviments_id);

    n_fil = atoi(argv[10]);
    n_col = atoi(argv[11]);

    win_set(camp_ptr, n_fil, n_col);  /* crea acces a finestra oberta pel proces pare */

    id_ventana_sem = atoi(argv[12]);
    id_moviments_sem = atoi(argv[13]);

    l_pal = atoi(argv[14]);
	retard = atoi(argv[15]);
    do
    {
        mou_paleta_ordinador(&paleta);  /* passem el punter a la paleta */
        win_retard(retard);
        
    } while(!(*final_juego));

    return (0);
}
