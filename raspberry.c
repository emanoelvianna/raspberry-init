#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#define DIRECTION_MAX 35
#define VALUE_MAX 30
#define BUFFER_MAX 3

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define QUANTIDADE_PADROES 4
#define PADRAO_INICIAL 'A'

char padrao_atual = PADRAO_INICIAL;
int contador = 0;

int config_serial(char * device, unsigned int baudrate)
{
	struct termios options;
	int fd;

	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY );
	if (fd < 0)
	{
		//Could not open the port.
		perror("config_serial: Não pode abrir a serial - ");
		return -1;
	}

	fcntl(fd, F_SETFL, 0);

	//Get the current options for the port...
	tcgetattr(fd, &options);

	//Sets the terminal to something like the "raw" mode
	cfmakeraw(&options);

	//Set the baudrate...
	cfsetispeed(&options, baudrate);
	cfsetospeed(&options, baudrate);

	//Enable the receiver and set local mode...
	options.c_cflag |= (CLOCAL | CREAD);

	//No parit, 1 stop bit, size 8
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	//Clear old settings
	options.c_cflag &= ~CRTSCTS;
	options.c_iflag &= ~(IXON | IXOFF | IXANY);

	//non-caninical mode
	options.c_lflag &= ~ICANON; 

	//Set the new options for the port...
	tcsetattr(fd, TCSANOW, &options);

	//configura a tty para escritas e leituras não bloqueantes
	//fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);

	return fd;
}

static int GPIOExport(int pin) 
{

	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) 
	{
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIOUnexport(int pin) 
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) 
	{
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIODirection(int pin, int dir) 
{
	static const char s_directions_str[]  = "in\0out";

	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) 
	{
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) 
	{
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

static int GPIORead(int pin) 
{
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) 
	{
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if (-1 == read(fd, value_str, 3)) 
	{
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

static int GPIOWrite(int pin, int value) 
{
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) 
	{
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) 
	{
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

void escrever_padrao() 
{
	int fd;

	fd = config_serial((char*) "/dev/ttyAMA0", B9600);

	if(fd < 0)
	{
		printf("ERRO AO TENTAR ESCREVER NO SERIAL!!!\n");

		return;
	}

	printf("PADRAO ATUAL = %c\n", padrao_atual);

	write(fd, &padrao_atual, 1);
}

void proximo_padrao()
{
	if(contador < (QUANTIDADE_PADROES - 1))
	{
		padrao_atual++;
		contador++;
	}
	else
	{
		padrao_atual = PADRAO_INICIAL;
		contador = 0;
	}

	escrever_padrao();
}

void padrao_anterior()
{
	if(contador > 0)
	{
		padrao_atual--;
		contador--;
	}
	else
	{
		padrao_atual = PADRAO_INICIAL + QUANTIDADE_PADROES - 1;
		contador = QUANTIDADE_PADROES - 1;
	}

	escrever_padrao();
}

void ler_mudancas_de_cada_padrao() 
{
	int fd;
	int i;

	fd = config_serial((char*) "/dev/ttyAMA0", B9600);

	if(fd < 0)
	{
		printf("ERRO AO TENTAR LER DO SERIAL!!!\n");

		return;
	}

	char valor = '0';

	write(fd, &valor, 1);	

	char padrao_aux = PADRAO_INICIAL;

	printf("MUDANCAS DE CADA PADRAO -> [");
	for(i=0; i<QUANTIDADE_PADROES; i++)
	{
		read(fd, &valor, 1);

		printf(" %c=%d ", padrao_aux, valor);
		padrao_aux++;
	}
	read(fd, &valor, 1);
	printf(" TOTAL=%d ", valor);
	printf("]\n");
}

int main(int argc, char *argv[]) 
{
	//BOTAO DA ESQUERDA
	int pushbutton25 = 25;
	//BOTAO DO MEIO
	int pushbutton24 = 24;
	//BOTAO DA DIREITA
	int pushbutton23 = 23;

	//Enable GPIO pins
	GPIOExport(pushbutton25);
	GPIOExport(pushbutton24);
	GPIOExport(pushbutton23);

	while(1)
	{
		//Set GPIO directions
		if (-1 == GPIODirection(pushbutton25, OUT))
		{
			return(2);
		}

		//Write GPIO value
		if (-1 == GPIOWrite(pushbutton25, 1))
		{
			return(3);
		}

		//Set GPIO directions
		if (-1 == GPIODirection(pushbutton25, IN))
		{
			return(2);
		}

		//23
		if (-1 == GPIODirection(pushbutton23, OUT))
		{
			return(2);
		}
		if (-1 == GPIOWrite(pushbutton23, 1))
		{
			return(3);
		}
		if (-1 == GPIODirection(pushbutton23, IN))
		{
			return(2);
		}

		//24
		if (-1 == GPIODirection(pushbutton24, OUT))
		{
			return(2);
		}
		if (-1 == GPIOWrite(pushbutton24, 1))
		{
			return(3);
		}
		if (-1 == GPIODirection(pushbutton24, IN))
		{
			return(2);
		}

		//CHAMADA DE FUNCAO QUANDO UM BOTAO E PRESSIONADO
		if (GPIORead(pushbutton25) == 0)
		{
			padrao_anterior();
		}
		else if (GPIORead(pushbutton24) == 0)
		{
			ler_mudancas_de_cada_padrao();		
		}
		else if (GPIORead(pushbutton23) == 0)
		{
			proximo_padrao();
		}		

		//Read GPIO value
		usleep(500 * 1000);
	}

	//Disable GPIO pins
	if (-1 == GPIOUnexport(pushbutton25))
	{
		return(4);
	}
	if (-1 == GPIOUnexport(pushbutton24))
	{
		return(4);
	}
	if (-1 == GPIOUnexport(pushbutton23))
	{
		return(4);
	}

	return(0);
}
