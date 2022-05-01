#include <stdio.h>
#include <stdint.h>

typedef struct s_data
{
	uint16_t	tt_die;
	uint16_t	tt_eat;
	uint16_t	tt_sleep;
	uint8_t		min_eat;
	uint8_t		id;
}				t_data;

int main(void)
{
	printf("%ld\n",sizeof(void *));
	// printf("%ld\n",sizeof(int16_t));
	printf("%ld\n",sizeof(t_data));
	printf("%ld\n",sizeof(t_data *));

	return 1;
}