
#include "tennis3.h"

void *reloj_control()
{
	do
	{
		sleep(1);
		sec++;
		if (sec == 60)
		{
			sec = 0;
			min++;
		}
		pthread_mutex_lock(&pause_control);
		pthread_mutex_unlock(&pause_control);

	} while(!*final_juego);

	pthread_exit(0);
}
