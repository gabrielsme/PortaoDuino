#include <MeetAndroid.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal.h> 

#define Luz_Fundo  23

#define pinTrigOut  52
#define pinEchoin  53

#define SERVO 6 // Porta Digital 6 PWM

MeetAndroid meetAndroid;
int onboardLed = 13;

Servo s; // Variável Servo
int pos; // Posição Servo

int senhaAdmin = 1234;
int senhaAndroid = 0;
boolean portaoAberto = false;

long distance;
long cm;

LiquidCrystal   lcd(22, 2,  3, 24, 4, 25);

void ConfigLCD(){

    lcd.begin(16, 2); 
  
    pinMode(Luz_Fundo,OUTPUT); 
    digitalWrite(Luz_Fundo,HIGH); 

}

void ConfigUltraSon(){

  pinMode(pinTrigOut, OUTPUT);
  pinMode(pinEchoin,INPUT);
}

void setup()  
{
  Serial.begin(9600); 
  
  meetAndroid.registerFunction(abrir, 'A');
  meetAndroid.registerFunction(fechar, 'F');

  pinMode(onboardLed, OUTPUT);
  
  s.attach(SERVO);
  Serial.begin(9600);
  s.write(0);
  
  ConfigLCD();  
  ConfigUltraSon();
  
  meetAndroid.send("Digite a senha");
}

void loop()
{
  meetAndroid.receive();
  
  lcd.clear();
  
  CalcDistancia();
}

void abrir(byte flag, byte numOfValues)
{
  if(cm < 50){
    senhaAndroid = meetAndroid.getInt();
  
    if(senhaAndroid == senhaAdmin){
      meetAndroid.send("Abrindo o portao");
      lcd.setCursor(0,0); 
      lcd.print("Abrindo o portao");
      digitalWrite(onboardLed, HIGH);
    
      for(pos = 0; pos < 180; pos++){
        s.write(pos);
        delay(10);
      }
      
      portaoAberto = true;
      
      meetAndroid.send("Portao aberto");
      lcd.setCursor(0,0); 
      lcd.print("Portao aberto");
    } else {
      lcd.setCursor(0,0); 
      lcd.print("Senha invalida");
      meetAndroid.send("Senha invalida");  
    }
  }
}

void fechar(byte flag, byte numOfValues)
{
  if(portaoAberto){
    meetAndroid.send("Fechando portao");
    lcd.setCursor(0,0); 
    lcd.print("Fechando portao");
    
    digitalWrite(onboardLed, LOW);
    
    for(pos = 180; pos > 0; pos--){
      s.write(pos);
      delay(10);
    }
    
    portaoAberto = false;
    
    meetAndroid.send("Portao fechado");
    lcd.setCursor(0,0); 
    lcd.print("Portao fechado");
  }
}

void CalcDistancia(){
  digitalWrite(pinTrigOut, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrigOut, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigOut, LOW);   
  distance = pulseIn(pinEchoin, HIGH);
  cm = microsecondsToCentimeters(distance);
}

long microsecondsToCentimeters(long valor){
  return (valor / 29) / 2;
}
