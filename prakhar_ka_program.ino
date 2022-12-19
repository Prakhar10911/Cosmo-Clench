
  #include <PS2X_lib.h>  //for v1.6
  #define PS2_DAT        47  //14    
  #define PS2_CMD        49  //15
  #define PS2_SEL        51  //16
  #define PS2_CLK        53  //17
  #define BRAKE 0
  #define CW    1
  #define CCW   2
  #define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").
//MOTOR 1

  #define MOTOR_A1_PIN 8
  #define MOTOR_B1_PIN 7
//MOTOR 2
  #define MOTOR_A2_PIN 4
  #define MOTOR_B2_PIN 9
  #define PWM_MOTOR_1 5
  #define PWM_MOTOR_2 6
  #define CURRENT_SEN_1 A2
  #define CURRENT_SEN_2 A3
  #define EN_PIN_1 A0
  #define EN_PIN_2 A1
  #define MOTOR_1 0
  #define MOTOR_2 1
  PS2X ps2x;
  
  int error = 0;
  byte type = 0;
  byte vibrate = 0;
  short usSpeed = 70;//default motor speed
  short usSpeed1 = 40;
  unsigned short usMotor_Status = BRAKE;;
  int arm4=39;
  int arm3=41;
  int arm2=43;
  int arm1=45; 
  #define pressures   false
//#define rumble      true
  #define rumble      false
void setup() {
  Serial.begin(57600);
 
    //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }

 
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);

  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);

  pinMode(CURRENT_SEN_1, OUTPUT);
  pinMode(CURRENT_SEN_2, OUTPUT);  
 pinMode(EN_PIN_1, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);
 
  
  
  pinMode(arm3,OUTPUT);
  pinMode(arm4,OUTPUT);
  pinMode(arm2,OUTPUT);
  pinMode(arm1,OUTPUT);
}

void loop() 
{
       digitalWrite(EN_PIN_1, HIGH);
       digitalWrite(EN_PIN_2, HIGH); 

  if(error == 1) //skip loop if no controller found
    return; 
    
  else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); 
  
  if(ps2x.Button(PSB_L3))
      {
  Serial.println("stop ");
 digitalWrite(arm2,LOW); 
  digitalWrite(arm1,LOW);
  digitalWrite(arm3,LOW); 
  digitalWrite(arm4,LOW);
}

  if(ps2x.Button(PSB_R3))
      {
  Serial.println("STOP");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
       }
  else if(ps2x.Button(PSB_L2))
       {
  Serial.println("going backwards");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}
  else if(ps2x.Button(PSB_R2))
       {
  Serial.println("going forward ");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}
else if(ps2x.Analog(PSS_RX)>129) 
 {
  Serial.println("GOING RIGHT SIR");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed1); 
  usMotor_Status = CCW;
  motorGo(MOTOR_2, usMotor_Status, usSpeed1);
       
}
  else if(ps2x.Analog(PSS_RX)<127) 
    {
  Serial.println("GOING LEFT SIR ");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed1);
  usMotor_Status = CW;
  motorGo(MOTOR_2, usMotor_Status, usSpeed1);
  
}
 /*else if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      analogWrite(arm1,255); 
  analogWrite(arm2,0);
    }
    else if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.print("DOWN held this hard: ");
      analogWrite(arm1,0); 
  analogWrite(arm2,255);
       }   
    else if(ps2x.Button(PSB_PAD_LEFT))
    {
      Serial.println("ARM GOING LEFT ,COMMANDER");
  analogWrite(arm3,0);
  analogWrite(arm4,255);
    }
    else if(ps2x.Button(PSB_PAD_RIGHT))
    {Serial.println("ARM GOING RIGHT ,COMMANDER");
  analogWrite(arm3,255); 
  analogWrite(arm4,0);
      }*/
 
 else 
 {
  Serial.println("STOP");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
 /* digitalWrite(arm3,LOW); 
  digitalWrite(arm4,LOW);
  digitalWrite(arm2,LOW); 
  digitalWrite(arm1,LOW);*/
 }
 if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      analogWrite(arm1,255); 
  analogWrite(arm2,0);
    }
    else if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.print("DOWN held this hard: ");
      analogWrite(arm1,0); 
  analogWrite(arm2,255);
       }   
    else if(ps2x.Button(PSB_PAD_LEFT))
    {
      Serial.println("ARM GOING LEFT ,COMMANDER");
  analogWrite(arm3,0);
  analogWrite(arm4,255);
    }
     else if(ps2x.Button(PSB_PAD_RIGHT))
    {Serial.println("ARM GOING RIGHT ,COMMANDER");
  analogWrite(arm3,255); 
  analogWrite(arm4,0);
      }
      else{
        digitalWrite(arm3,LOW); 
  digitalWrite(arm4,LOW);
  digitalWrite(arm2,LOW); 
  digitalWrite(arm1,LOW);
  }
 
  if(ps2x.Button(PSB_R1)){
    
  usSpeed = usSpeed + 5;
  if(usSpeed > 255)
  {
    usSpeed = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed); }
  
  if(ps2x.Button(PSB_L1))
  {
  usSpeed = usSpeed - 5;
  if(usSpeed < 0)
  {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);
Serial.println(ps2x.Button(PSB_L1));
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed); } 

    
    if(ps2x.Button(PSB_CIRCLE)){
    
  usSpeed1 = usSpeed1 + 5;
  if(usSpeed1 > 255)
  {
    usSpeed1 = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed1);

  motorGo(MOTOR_1, usMotor_Status, usSpeed1);
  motorGo(MOTOR_2, usMotor_Status, usSpeed1); }
  
  if(ps2x.Button(PSB_SQUARE))
  {
  usSpeed1 = usSpeed1 - 5;
  if(usSpeed1 < 0)
  {
    usSpeed1 = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed1);
Serial.println(ps2x.Button(PSB_L1));
  motorGo(MOTOR_1, usMotor_Status, usSpeed1);
  motorGo(MOTOR_2, usMotor_Status, usSpeed1); 
  } 
}
    


  delay(50);  
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
  if(motor == MOTOR_1)
  {
  if(direct == CW)
    {
  digitalWrite(MOTOR_A1_PIN, LOW); 
  digitalWrite(MOTOR_B1_PIN, HIGH);
    }
  else if(direct == CCW)
    {
  digitalWrite(MOTOR_A1_PIN, HIGH);
  digitalWrite(MOTOR_B1_PIN, LOW); 
    }     
  else
    {
  digitalWrite(MOTOR_A1_PIN, LOW);
  digitalWrite(MOTOR_B1_PIN, LOW);            
    }
   analogWrite(PWM_MOTOR_1, pwm); 
  }
  else if(motor == MOTOR_2)
  {
  if(direct == CW)
    {
  digitalWrite(MOTOR_A2_PIN, LOW);
  digitalWrite(MOTOR_B2_PIN, HIGH);
    }
  else if(direct == CCW)
    {
  digitalWrite(MOTOR_A2_PIN, HIGH);
  digitalWrite(MOTOR_B2_PIN, LOW);      
    }
  else
    {
  digitalWrite(MOTOR_A2_PIN, LOW);
  digitalWrite(MOTOR_B2_PIN, LOW);            
    }  
  analogWrite(PWM_MOTOR_2, pwm);
  }
}
