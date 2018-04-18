 #include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define initial_position_M1 170
#define initial_position_M2 180
#define initial_position_M3 150


#define take_cube_position_M1 95
#define take_cube_position_M2 80
#define take_cube_position_M3 150

#define deposit_cube_position_M1 145
#define deposit_cube_position_M2 170
#define deposit_cube_position_M3 80

#define delay_value 6
#define exchange_value 1

#define pomp 4
#define valv 2

short current_value_M1;
short current_value_M2;
short current_value_M3;

short ok = 0;

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
  Serial.println("8 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates

  delay(10);

   pwm.setPWM(1, 0, map(initial_position_M1, 0, 180, 0, 410));
   pwm.setPWM(2, 0, map(initial_position_M2, 0, 180, 0, 410));
   pwm.setPWM(3, 0, map(initial_position_M3, 0, 180, 0, 410));
   delay(3000);
     
   current_value_M1 = initial_position_M1;
   current_value_M2 = initial_position_M2;
   current_value_M3 = initial_position_M3;

   pinMode(pomp, OUTPUT);
   pinMode(valv, OUTPUT);
  
}

void loop() {
  
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

    nr_of_cubs++;
    
    if(3 == nr_of_cubs)
    {
      
      //pentru suport cuburi
      pwm.setPWM(7, 0, map(40, 0, 180, 0, 410));
      delay(2000);
      //pentru usa
      pwm.setPWM(5, 0, map(180, 0, 180, 0, 410));
      delay(1000);
      while(1)
      {
        ;
      }
    }
        
   /*
    initial_position(current_value_M1, current_value_M2, current_value_M3);
    current_value_M1 = initial_position_M1;
    current_value_M2 = initial_position_M2;
    current_value_M3 = initial_position_M3;
    delay(2000);
  */
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
  pwm.setPWM(1, 0, map(90, 0, 180, 0, 410));
  pwm.setPWM(2, 0, map(140, 0, 180, 0, 410));
  pwm.setPWM(3, 0, map(180, 0, 180, 0, 410));

  delay(5000);
  pwm.setPWM(1, 0, map(40, 0, 180, 0, 410));
  delay(5000);
}

//functie pentru apasare switch; la inaltimea de aproximativ de 13 cm;
void switch_button ()
{
        pwm.setPWM(1, 0, map(90, 0, 180, 0, 410));
        pwm.setPWM(2, 0, map(100, 0, 180, 0, 410));
        pwm.setPWM(3, 0, map(150, 0, 180, 0, 410));

        delay(3000);
       pwm.setPWM(3, 0, map(120, 0, 180, 0, 410));

       delay(3000);
  
}
