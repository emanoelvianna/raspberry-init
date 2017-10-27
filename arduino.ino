
int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;
int led5 = 9;
int led6 = 8;

int inByte = 0;

void setup() {         
    Serial.begin(9600);  
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);     
    pinMode(led3, OUTPUT);     
    pinMode(led4, OUTPUT);     
    pinMode(led5, OUTPUT);     
    pinMode(led6, OUTPUT);     
}

void patternA() {
    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);
    delay(1000);
}

void patternB() {
    digitalWrite(led5, HIGH);
    delay(1000);
    digitalWrite(led5, LOW);
    digitalWrite(led4, HIGH);
    delay(750);
    digitalWrite(led4, LOW);
    digitalWrite(led3, HIGH);
    delay(500);
    digitalWrite(led3, LOW);
    digitalWrite(led2, HIGH);
    delay(250);
    digitalWrite(led2, LOW);
    digitalWrite(led1, HIGH);
    delay(125);
    digitalWrite(led5, LOW);
}

void patternC() {
    // digitalWrite(13, HIGH);
    // delay(1000);
    // digitalWrite(13, LOW);
    // delay(1000);
}

void loop() {
    if (Serial.available()) {

        inByte = Serial.read();
   
        if (inByte > -1) {
            switch(inByte) {

                case 'A'  :
                    patternA();
                    break;

                case 'B'  :
                    patternB();
                    break;

                case 'C'  :
                    patternC();
                    break;

                default :
                    break;
            } 
        }

    }
}
