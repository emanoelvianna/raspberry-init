
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
int quantidade_padroes = 4;
int mudancas_de_cada_padrao[4];
int mudancas_de_padrao = 0;
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

	int i;
	for (i = 0; i < quantidade_padroes; i = i + 1) 
	{
		mudancas_de_cada_padrao[i] = 0;
	}
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

void padrao_c() 
{
	digitalWrite(led4, HIGH);
	delay(1000);
	digitalWrite(led4, LOW);
	delay(1000);
}

void padrao_d() 
{
	digitalWrite(led4, HIGH);
	delay(100);
	digitalWrite(led4, LOW);
	digitalWrite(led5, HIGH);
	delay(100);
	digitalWrite(led5, LOW);
	digitalWrite(led6, HIGH);
	delay(100);
	digitalWrite(led6, LOW);
	digitalWrite(led7, HIGH);
	delay(100);
	digitalWrite(led7, LOW);
	digitalWrite(led8, HIGH);
	delay(100);
	digitalWrite(led8, LOW);
	digitalWrite(led7, HIGH);
	delay(100);
	digitalWrite(led7, LOW);
	digitalWrite(led6, HIGH);
	delay(100);
	digitalWrite(led6, LOW);
	digitalWrite(led5, HIGH);
	delay(100);
	digitalWrite(led5, LOW);
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
	if(padrao_atual == 'C')
	{
		padrao_c();
	}
	else if(padrao_atual == 'D')
	{
		padrao_d();
	}
}

void escrever_mudancas_de_cada_padrao()
{
	int i;
	for (i = 0; i < quantidade_padroes; i = i + 1) 
	{
		Serial.write(mudancas_de_cada_padrao[i]);
	} 

	Serial.write(mudancas_de_padrao);  
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
				mudancas_de_cada_padrao[0] = mudancas_de_cada_padrao[0] + 1;
				mudancas_de_padrao = mudancas_de_padrao + 1;
			}	
			else if(valor_recebido == 'B')
			{
				padrao_atual = 'B';
				mudancas_de_cada_padrao[1] = mudancas_de_cada_padrao[1] + 1;
				mudancas_de_padrao = mudancas_de_padrao + 1;
			}
			if(valor_recebido == 'C')
			{
				padrao_atual = 'C';
				mudancas_de_cada_padrao[2] = mudancas_de_cada_padrao[2] + 1;
				mudancas_de_padrao = mudancas_de_padrao + 1;
			}	
			else if(valor_recebido == 'D')
			{
				padrao_atual = 'D';
				mudancas_de_cada_padrao[3] = mudancas_de_cada_padrao[3] + 1;
				mudancas_de_padrao = mudancas_de_padrao + 1;
			}
			else if(valor_recebido == '0')
			{
				escrever_mudancas_de_cada_padrao();
			}
		}
	}
	else
	{
		piscar_leds();
	}
}
