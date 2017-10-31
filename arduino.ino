
int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;
int led5 = 9;
int led6 = 8;
int led7 = 7;
int led8 = 6;
int led9 = 5;
int led10 = 4;
int led11 = 3;
int led12 = 2;
int led13 = 1;

char padrao_atual = 'A';
int contador_a = 0;
int contador_b = 0;
int valor_recebido = 0;

void setup() 
{         
	Serial.begin(9600);  
	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);     
	pinMode(led3, OUTPUT);     
	pinMode(led4, OUTPUT);     
	pinMode(led5, OUTPUT);     
	pinMode(led6, OUTPUT);   
	pinMode(led7, OUTPUT);
	pinMode(led8, OUTPUT);     
	pinMode(led9, OUTPUT);     
	pinMode(led10, OUTPUT);     
	pinMode(led11, OUTPUT);     
	pinMode(led12, OUTPUT); 
        pinMode(led13, OUTPUT); 
}

void padrao_a() 
{
	digitalWrite(led8, HIGH);
	delay(1000);
	digitalWrite(led8, LOW);
	delay(1000);
}

void padrao_b() 
{
	digitalWrite(led8, HIGH);
	delay(1000);
	digitalWrite(led8, LOW);
	digitalWrite(led7, HIGH);
	delay(1000);
	digitalWrite(led7, LOW);
	digitalWrite(led6, HIGH);
	delay(1000);
	digitalWrite(led6, LOW);
	digitalWrite(led5, HIGH);
	delay(1000);
	digitalWrite(led5, LOW);
	digitalWrite(led4, HIGH);
	delay(1000);
	digitalWrite(led4, LOW);
        digitalWrite(led5, HIGH);
        delay(1000);
        digitalWrite(led5, LOW);
        digitalWrite(led6, HIGH);
        delay(1000);
        digitalWrite(led6, LOW);
        digitalWrite(led7, HIGH);
        delay(1000);
        digitalWrite(led7, LOW);
}

void piscar_leds()
{
	if(padrao_atual == 'A')
	{
		padrao_a();
	}
	else if(padrao_atual == 'B')
	{
		padrao_b();
	}
}

void escrever_contador_no_serial()
{
	if(padrao_atual == 'A')
	{
		Serial.write(contador_a);
	}
	else if(padrao_atual == 'B')
	{
		Serial.write(contador_b);
	}
}

void loop() 
{
	if (Serial.available()) 
	{
		valor_recebido = Serial.read();

		if (valor_recebido > -1) 
		{
			if(valor_recebido == 'A')
			{
				padrao_atual = 'A';
				contador_a = contador_a + 1;
			}	
			else if(valor_recebido == 'B')
			{
				padrao_atual = 'B';
				contador_b = contador_b + 1;
			}
			else if(valor_recebido == '0')
			{
				escrever_contador_no_serial();
			}
		}

		piscar_leds();
	}
	else
	{
		piscar_leds();
	}
}
