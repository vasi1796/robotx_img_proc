/*macro definitions*/
#define S0 1
#define S1 2
#define S2 3
#define S3 11
#define OutPut 13

/*declaration of variables */
boolean DEBUG = true;
int red;
int grn;
int blu;
String color ="";
int count = 0;
long startTiming = 0;
long elapsedTime = 0;


#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() 
{
  //set up the LCD's number of columns and rows
  lcd.begin(16,2);

  pinMode(S0,OUTPUT); //LCD keypad shield PIN 1 - S0
  pinMode(S1,OUTPUT); //LCD keypad shield PIN 2 - S1
  pinMode(S2,OUTPUT); //LCD keypad shield PIN 3 - S2
  pinMode(S3,OUTPUT); //LCD keypad shield PIN 11 - S3
  pinMode(OutPut,INPUT); // LCD keypad shield PIN 13 - OUT


  /* Setting frequency-scaling to 20% */
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

void printData(void)
{
  Serial.print("red= ");
  Serial.print(red);
  Serial.print("   green= ");
  Serial.print(grn);
  Serial.print("   blue= ");
  Serial.print(blu);
  Serial.print (" - ");
  Serial.print (color);
  Serial.println (" detected!");
}

void readRGB() 
{
  red = 0;
  grn = 0;
  blu = 0;
  int n = 10;
  for (int i = 0; i < n; ++i)
  {
    //read red component
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red = red + pulseIn(OutPut, LOW);
  
   //read green component
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    grn = grn + pulseIn(OutPut, LOW);
    
   //let's read blue component
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blu = blu + pulseIn(OutPut, LOW);
  }
  red = red/n;
  grn = grn/n;
  blu = blu/n;
}

void getColor()
{  
  readRGB();
  if (red > 100 && red < 210  &&  grn >  240 && grn < 370    &&  blu > 220  && blu < 300)   color = "Orange";
  else if (red > 320 && red < 400  &&  grn > 205 && grn < 345   &&  blu > 110 && blu < 200)  color = "Blue";
  else if (red > 450 && red < 480   &&  grn > 510 && grn < 560    &&  blu > 335 && blu < 375)   color = "Black";
  else if (red > 215 && red < 350   &&  grn > 195  && grn < 360    &&  blu > 205 && blu < 300)   color = "Green";
  else if (red > 95 && red < 215   &&  grn > 145 && grn < 340    &&  blu > 185 && blu < 310)   color = "Yellow";
  else  color = "NO_COLOR";
}

void showDataLCD(void)
{
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("R");
  lcd.setCursor (1,0);
  lcd.print("    ");
  lcd.setCursor (1,0);
  lcd.print(red);
  lcd.setCursor (5,0);
  lcd.print(" G");
  lcd.setCursor (7,0);
  lcd.print("    ");
  lcd.setCursor (7,0);
  lcd.print(grn);
  lcd.setCursor (12,0);
  lcd.print("B");
  lcd.setCursor (13,0);
  lcd.print("    ");
  lcd.setCursor (13,0);
  lcd.print(blu);
  
  lcd.setCursor (0,1);
  lcd.print("Color: ");
  lcd.setCursor (7,1); 
  lcd.print("        ");
  lcd.setCursor (7,1);  
  lcd.print(color);
}

void loop()
{
  getColor();
  if (DEBUG) printData(); 
  elapsedTime = millis()-startTiming; 
  if (elapsedTime > 1000) 
  {
    showDataLCD();
    startTiming = millis();
  } 
}
