#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define initial_position_M1 170
#define initial_position_M2 180
#define initial_position_M3 150

#define take_cube_position_M1 95
#define take_cube_position_M2 80
#define take_cube_position_M3 145

#define deposit_cube_position_M1 155
#define deposit_cube_position_M2 170
#define deposit_cube_position_M3 80

#define position_for_press_button_M1 90 
#define position_for_press_button_M2 95
#define position_for_press_button_M3 150
#define position_for_press_button_preparation_M3 120

#define position_for_throwing_balls_M1 40
#define position_for_throwing_balls_preparation_M1 90
#define position_for_throwing_balls_M2 140
#define position_for_throwing_balls_M3 180

#define delay_value 6
#define exchange_value 1

#define pomp 4
#define valv 2

#define command_for_normal_position         '0'
#define command_for_take_and_deposit_cube   '1'
#define command_for_throwing_balls          '2'
#define command_for_press_button            '3'
#define command_for_drop_cubs               '4'

char myCommand = 0;

short current_value_M1;
short current_value_M2;
short current_value_M3;

short ok = 0;

boolean newData = false;

short nr_of_cubs = 0;

boolean M1_status;
boolean M2_status;
boolean M3_status;

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(&Wire, 0x40);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN_MID  500 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_MID  2500 // this is the 'maximum' pulse length count (out of 4096)
//#define SERVOMIN_HIGH  800 // this is the 'minimum' pulse length count (out of 4096)
//#define SERVOMAX_HIGH  2200 // this is the 'maximum' pulse length count (out of 4096)

void setup() {
  Serial.begin(9600);
  Serial.println("<Arduino is ready, bitch!>");
  
  pwm.begin();
  
  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates

  delay(10);
  
  pinMode(pomp, OUTPUT);
  pinMode(valv, OUTPUT); 

  pwm.setPWM(7, 0, map(145, 0, 180, 0, 410));
  pwm.setPWM(5, 0, map(120, 0, 180, 0, 410));

  pwm.setPWM(1, 0, map(initial_position_M1, 0, 180, 0, 410));
  pwm.setPWM(2, 0, map(initial_position_M2, 0, 180, 0, 410));
  pwm.setPWM(3, 0, map(initial_position_M3, 0, 180, 0, 410));

  current_value_M1 = initial_position_M1;
  current_value_M2 = initial_position_M2;
  current_value_M3 = initial_position_M3;

}

void loop() {
    recvOneCommand();
    
    if (newData == true) 
    {
      
      if(myCommand == command_for_normal_position)
      {
        Serial.println("Este pe pozitie normala");
        pwm.setPWM(1, 0, map(initial_position_M1, 0, 180, 0, 410));
        pwm.setPWM(2, 0, map(initial_position_M2, 0, 180, 0, 410));
        pwm.setPWM(3, 0, map(initial_position_M3, 0, 180, 0, 410));

        current_value_M1 = initial_position_M1;
        current_value_M2 = initial_position_M2;
        current_value_M3 = initial_position_M3;
      }
      else if(myCommand == command_for_take_and_deposit_cube)
      {
        Serial.println("Vezi ca ia si depoziteaza cubul");
        take_cube_position(current_value_M1, current_value_M2, current_value_M3);
        current_value_M1 = take_cube_position_M1;
        current_value_M2 = take_cube_position_M2;
        current_value_M3 = take_cube_position_M3;

        digitalWrite(pomp,HIGH);
        digitalWrite(valv,LOW);
   
        delay(1000); 
   
        deposit_cube_position(current_value_M1, current_value_M2, current_value_M3);
        current_value_M1 = deposit_cube_position_M1;
        current_value_M2 = deposit_cube_position_M2;
        current_value_M3 = deposit_cube_position_M3;
    
        delay(1000);
        digitalWrite(pomp,LOW);
        digitalWrite(valv,HIGH);
        delay(1000);
        
      }
      else if(myCommand == command_for_throwing_balls)
      {
        Serial.println("Iti da drumul la mingi");
        drop_balls();
      }
      else if(myCommand == command_for_press_button)
      {
        Serial.println("Iti apasa pe buton");
        switch_button();
      }
      else if(myCommand == command_for_drop_cubs)
      {
        Serial.println("Iti lasa cuburile");
        drop_cubs();
      }
      else
      {
        /*do nothing*/
        Serial.println("Nu face nimic");
      }
      
      newData = false;
    }
   
}

void recvOneCommand() 
{
 if (Serial.available() > 0) 
 {
  myCommand = Serial.read();
  newData = true;
 }
}

void initial_position(short value_M1, short value_M2, short value_M3)
{
  M1_status = 1;
  M2_status = 1;
  M3_status = 1;
  
  while(M1_status || M2_status  || M3_status )
  {
    if(value_M1 > initial_position_M1)
    {
      value_M1 -= exchange_value;
      pwm.setPWM(1, 0, map(value_M1, 0, 180, 0, 410));
      delay(delay_value);
    }
    else if(value_M1 < initial_position_M1)
    {
      value_M1 += exchange_value;
      pwm.setPWM(1, 0, map(value_M1, 0, 180, 0, 410));
      delay(delay_value);
    }
    else
    {
      M1_status = 0;
    }

    if(value_M2 > initial_position_M2)
    {
      value_M2 -= exchange_value;
      pwm.setPWM(2, 0, map(value_M2, 0, 180, 0, 410));
      delay(delay_value);
    }
    else if(value_M2 < initial_position_M2)
    {
      value_M2 += exchange_value;
      pwm.setPWM(2, 0, map(value_M2, 0, 180, 0, 410));
      delay(delay_value);
    }
    else
    {
      M2_status = 0;
    }

    if(value_M3 > initial_position_M3)
    {
      value_M3 -= exchange_value;
      pwm.setPWM(3, 0, map(value_M3, 0, 180, 0, 410));
      delay(delay_value);
    }
    else if(value_M3 < initial_position_M3)
    {
      value_M3 += exchange_value;
      pwm.setPWM(3, 0, map(value_M3, 0, 180, 0, 410));
      delay(delay_value);
    }
    else
    {
      M3_status = 0;
    }
  }
}

void take_cube_position(short value_M1, short value_M2, short value_M3)
{
  M1_status = 1;
  M2_status = 1;
  M3_status = 1;
  
  while(M1_status || M2_status  || M3_status )
  {
    if(value_M1 > take_cube_position_M1)
    {
      value_M1 -= exchange_value;
      pwm.setPWM(1, 0, map(value_M1, 0, 180, 0, 410));
      delay(delay_value);
    }
    else if(value_M1 < take_cube_position_M1)
    {
      value_M1 += exchange_value;
      pwm.setPWM(1, 0, map(value_M1, 0, 180, 0, 410));
      delay(delay_value);
    }
    else
    {
      M1_status = 0;
    }

    if(value_M2 > take_cube_position_M2)
    {
      value_M2 -= exchange_value;
      pwm.setPWM(2, 0, map(value_M2, 0, 180, 0, 410));
      delay(delay_value);
    }
    else if(value_M2 < take_cube_position_M2)
    {
      value_M2 += exchange_value;
      pwm.setPWM(2, 0, map(value_M2, 0, 180, 0, 410));
      delay(delay_value);
    }
    else
    {
      M2_status = 0;
    }

    if(value_M3 > take_cube_position_M3)
    {
      value_M3 -= exchange_value;
      pwm.setPWM(3, 0, map(value_M3, 0, 180, 0, 410));
      delay(delay_value);
    }
    else if(value_M3 < take_cube_position_M3)
    {
      value_M3 += exchange_value;
      pwm.setPWM(3, 0, map(value_M3, 0, 180, 0, 410));
      delay(delay_value);
    }
    else
    {
      M3_status = 0;
    }
  }
}

void deposit_cube_position(short value_M1, short value_M2, short value_M3)
{
  M1_status = 1;
  M2_status = 1;
  M3_status = 1;
  
  while(M2_status  || M3_status )
  {
    if(value_M2 > deposit_cube_position_M2)
    {
      value_M2 -= exchange_value;
      pwm.setPWM(2, 0, map(value_M2, 0, 180, 0, 410));
       delay(delay_value);
    }
    else if(value_M2 < deposit_cube_position_M2)
    {
      value_M2 += exchange_value;
      pwm.setPWM(2, 0, map(value_M2, 0, 180, 0, 410));
      delay(delay_value);
    }
    else
    {
      M2_status = 0;
    }

    if(value_M3 > deposit_cube_position_M3)
    {
      value_M3 -= exchange_value;
      pwm.setPWM(3, 0, map(value_M3, 0, 180, 0, 410));
       delay(delay_value);
    }
    else if(value_M3 < deposit_cube_position_M3)
    {
      value_M3 += exchange_value;
      pwm.setPWM(3, 0, map(value_M3, 0, 180, 0, 410));
      delay(delay_value);
    }
    else
    {
      M3_status = 0;
    }
  }
  while(M1_status)
  {
    if(value_M1 > deposit_cube_position_M1)
    {
      value_M1 -= exchange_value;
      pwm.setPWM(1, 0, map(value_M1, 0, 180, 0, 410));
      delay(delay_value);
    }
    else if(value_M1 < deposit_cube_position_M1)
    {
      value_M1 += exchange_value;
      pwm.setPWM(1, 0, map(value_M1, 0, 180, 0, 410));
      delay(delay_value);
    }
    else
    {
      M1_status = 0;
    }
  }
  
}

//functie pentru dat drumu la mingi; la inaltimea de aproximativ 26-27 cm
void drop_balls()
{
  pwm.setPWM(1, 0, map(position_for_throwing_balls_preparation_M1, 0, 180, 0, 410));
  pwm.setPWM(2, 0, map(position_for_throwing_balls_M2, 0, 180, 0, 410));
  pwm.setPWM(3, 0, map(position_for_throwing_balls_M3, 0, 180, 0, 410));
  delay(3000);
  pwm.setPWM(1, 0, map(position_for_throwing_balls_M1, 0, 180, 0, 410));
  delay(3000);
}

//functie pentru apasare switch; la inaltimea de aproximativ de 13 cm;
void switch_button()
{for(int x =1; x <= 2; x++)
  {
  pwm.setPWM(1, 0, map(position_for_press_button_M1, 0, 180, 0, 410));
  pwm.setPWM(2, 0, map(position_for_press_button_M2, 0, 180, 0, 410));
  pwm.setPWM(3, 0, map(position_for_press_button_M3, 0, 180, 0, 410));
  delay(3000);
  pwm.setPWM(3, 0, map(position_for_press_button_preparation_M3, 0, 180, 0, 410));
  delay(3000);  
  }
}

void drop_cubs()
{
      pwm.setPWM(7, 0, map(40, 0, 180, 0, 410));
      delay(1500);
      pwm.setPWM(5, 0, map(180, 0, 180, 0, 410));
}

