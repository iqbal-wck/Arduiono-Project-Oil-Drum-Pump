#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); 

int relay = 12;

char customKey;
const byte ROWS = 4;
const byte COLS = 4;
long setnilai;
int id;
long nilai;
long nilai1;
char keys[ROWS][COLS] = {
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {7,8,9,10}; 
byte colPins[COLS] = {3,4,5,6}; 


Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 


byte sensorInterrupt = 0;  
byte sensorPin       = 2;

float calibrationFactor = 1.7 ; 

volatile byte pulseCount;  

unsigned int frac;
float flowRate;
unsigned int flowMilliLitres;
int totalMilliLitres;

unsigned long oldTime;

void setup() 
{
  
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  
  pinMode(sensorPin, INPUT_PULLUP);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  lcd.backlight();
  lcd.init(); 
  lcd.noCursor(); 
  lcd.clear(); 
  Serial.begin(9600);   


  delay(1000);

}


void loop() 
{

  lcd.setCursor(0,0); 
  lcd.print("POMPA OLI OTOMATIS");
  lcd.setCursor(0,1); 
  lcd.print("-> A : setting awal ");
  lcd.setCursor(0,2);
  lcd.print("-> B : setting tetap");
  
customKey = customKeypad.getKey();

    if(customKey == 'A'){
    pulseCount        = 0;
    flowRate          = 0.0;
    flowMilliLitres   = 0;
    totalMilliLitres  = 0;
    oldTime           = 0;
  
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Loading...");
    delay(500);
    lcd.clear();

    lcd.clear();
    nilai = 0;
    nilai1 = 0;
    lanjut();
    digitalWrite(relay, HIGH);
    jalankan();
    }

     if(customKey == 'B'){
     pulseCount        = 0;
     flowRate          = 0.0;
     flowMilliLitres   = 0;
     totalMilliLitres  = 0;
     oldTime           = 0;
     lcd.clear();
     digitalWrite(relay, HIGH);
     jalankan();
     } 
}


void lanjut(){

  lcd.setCursor(0,0); 
  lcd.print("Setting Value");
  lcd.setCursor(0,3);
  lcd.print("lalu tekan 'D' ");
  
customKey = customKeypad.getKey();

    if(customKey >= '0' && customKey <= '9')
    {
      nilai = nilai * 10 + (customKey - '0');
      lcd.setCursor(0,1);
      lcd.print(nilai);
      
    
      nilai1 = nilai * 1 ;
      lcd.setCursor(0,2);
      lcd.print(nilai1);
      
    }
   
    if(customKey == 'C'){
    lcd.clear();
    delay(200); 
    nilai = 0;
    }
 
if(customKey == 'D'){
    lcd.clear();
    delay(2000); 
    setnilai = setnilai - nilai;

setnilai = 10000;       
    
      if(setnilai >= 0){
    Serial.println(nilai1);
    delay(1000);
    lcd.setCursor(0,0); 
    lcd.print("BERHASIL");
    lcd.setCursor(0,1); 
    lcd.print("Maks Set=");
    lcd.print(setnilai);
    lcd.setCursor(0,3);
    lcd.print("tunggu");
    delay(2000);
    lcd.clear();
 return;
      }
      } 
    
  lanjut();
   }

void jalankan(){

lcd.setCursor(0,0); 
lcd.print("Setting ");
lcd.print(nilai1);
lcd.print(" mL   ");

delay(100);
  
if((millis() - oldTime) > 1000)    
  { 

    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
       
    frac = (flowRate - int(flowRate)) * 10;

    lcd.setCursor(0,1);
    lcd.print("Spd Flow: ");             
    lcd.print(flowMilliLitres);
    lcd.print(" mL/Sec   ");


    lcd.setCursor(0,2); 
    lcd.print("ISI     : ");             
    lcd.print(totalMilliLitres);
    lcd.print(" mL   ");
    

    lcd.setCursor(0,3); 
    lcd.print("Setting :  ");
    lcd.print(nilai);
    lcd.print(" mL ");

    pulseCount = 0;
    
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

  if(totalMilliLitres < nilai1){
    delay(1000);
    digitalWrite(relay, LOW);
  }
 
 if(totalMilliLitres >= nilai1){
    
    digitalWrite(relay, HIGH);
     
    delay(1000);
    
    return;
 }
  }

    
jalankan();

}
void pulseCounter()
{
  pulseCount++;
  
}
