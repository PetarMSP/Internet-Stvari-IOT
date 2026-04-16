volatile int TimerCounter = 0;

int step = 0;
int LedDiode = 13;
char InputSequence[3] = {'1','0','1'};

bool Blink = false;
int mode = 0;

void setup() {
  cli();
  Serial.begin(9600);//Pocinjemo serijsku komunikaciju sa razunarom brzinom 9600 bitova u sekundi
  
  pinMode(LedDiode,OUTPUT);//Postaviti Led diodu na izlazni uredjaj
  //Preskaler 1/64 takta : CS02 = 0,CS01 = 1,CS00 = 1
  TCCR0B|=(1<<CS01);
  TCCR0B|=(1<<CS00);

  TCCR0A=(1<<WGM01); //Tajmer broji dok se ne poklopi sa OCR0A

  OCR0A=0xF9; //Za 1ms: (16*10^6/64)-1 = 1000/4 = 250 - 1 = 249 (F9 hex)

  TIMSK0|=(1<<OCIE0A); // Omoguciti prekid kada se Output poklopi sa OCR0A

  sei(); 

}

void loop() {
  if(Serial.available()>0){
     char c=Serial.read();
     if(c == '\n' || c == '\r') return;
     if(step == 0 && c == InputSequence[0]){
        step = 1;
     }else if (step == 1 && c == InputSequence[1]){
        step = 2;
     }else if (step == 2 && c == InputSequence[2]){
         step = 0;
         mode = !mode;
         Serial.println("Detektovana Sekvenca");
     }else{
        step = 0;
        Serial.println("Pogresna Sekvenca");
     }
      
  }
  if(mode == 0){
      if(TimerCounter>=500){
         Blink=!Blink;
         TimerCounter=0;
         digitalWrite(LedDiode,Blink);
      }
   }else{
      digitalWrite(LedDiode,LOW);
   }
  
}
ISR(TIMER0_COMPA_vect){
   TimerCounter++;
}

