#include "mcp_can.h"
#include <SPI.h>
#include <stdio.h>
#include <LiquidCrystal.h>

#define INT8U unsigned char

INT8U can_len = 0;
INT8U can_buf[8];

INT8U ecu_error = 0;
INT8U ecu_throttle = 0;
INT8U ecu_rpm = 0;
INT8U ecu_clt = 0;
INT8U ecu_mat = 0;
int ecu_map = 0;

float ecu_lambda = 0;
float ecu_volt = 0;

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

void setup()
{
  //  pinMode(4, INPUT);
  //  digitalWrite(4, HIGH); 
  //  pinMode(5, INPUT);
  //  digitalWrite(5, HIGH); 
  //  pinMode(7, OUTPUT);
  //  pinMode(8, OUTPUT);
  
  lcd.begin(4,20);
  lcd.setCursor(0,1);
  lcd.print(" trijkt CAN Display ");
  lcd.setCursor(0,2);
  lcd.print("v1.0 AIM2 24.02.2013");
  Serial.begin(57600);
  delay(250);
  CAN.begin(CAN_500KBPS);
  delay(250);
  attachInterrupt(0, MCP2515_ISR, FALLING);
  delay(250);
  lcd.clear();
}

// MAIN LOOP!
void loop() {
  LCD_ISR();
  //BUTTON_ISR();

  delay(100);
}

// Interrupt Buttons
void BUTTON_ISR() {
  digitalWrite(7,digitalRead(4)); 
  digitalWrite(8,digitalRead(5)); 

}
// Interrupt Display Data
void LCD_ISR()
{
  lcd.setCursor(0,0);
  lcd.print("RPM ");
  if(ecu_rpm > 9) {
    lcd.setCursor(4,0);
  } 
  else if (ecu_rpm > 99) {
    lcd.setCursor(5,0);
  } 
  else if (ecu_rpm > 999) {
    lcd.setCursor(6,0);
  }
  lcd.print(ecu_rpm);

  lcd.setCursor(10,0);
  lcd.print("LMD ");
  lcd.print(ecu_lambda);

  lcd.setCursor(0,1);
  lcd.print("THR ");
  lcd.print(ecu_throttle, DEC);

  lcd.setCursor(10,1);
  lcd.print("MAP ");
  lcd.print(ecu_map,DEC);


  lcd.setCursor(0,2);
  lcd.print("CLT ");
  lcd.print(ecu_clt,DEC);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(10,2);
  lcd.print("MAT ");
  lcd.print(ecu_mat,DEC);
  lcd.print((char)223);
  lcd.print("C");  

  lcd.setCursor(0,3);
  lcd.print("BAT ");
  lcd.print(ecu_volt);

  /*
  Serial.print("U:   ");
   Serial.println(ecu_volt);
   Serial.print("CLT: ");
   Serial.println(ecu_clt, DEC);
   Serial.print("MAT: ");
   Serial.println(ecu_mat, DEC);
   Serial.print("INP: ");
   Serial.print(inp1);
   Serial.println(inp2);
   Serial.print("PAG: ");
   Serial.print(page);
   Serial.println("---");
   */
}

// Interrupt CAN Data Reseived
void MCP2515_ISR() {
  CAN.readMsgBuf(&can_len, can_buf);  
  parseData(CAN.getCanId());
}

// Parse data from buffer
void parseData(int id) {

  switch(id) {
  case 0x770:
    ecu_rpm =  ((int)(word(can_buf[0],can_buf[1])));
    break;

  case 0x771:
    ecu_clt = ((int)(word(can_buf[0],can_buf[1]))/10);
    ecu_volt = ((float)(word(can_buf[4],can_buf[5]))*5/1024);
    ecu_throttle = ((float)(word(can_buf[6],can_buf[7]))/10);
    break;

  case 0x772:
    ecu_map = ((int)(word(can_buf[0],can_buf[1])));
    ecu_mat = ((int)(word(can_buf[2],can_buf[3]))/10);
    ecu_lambda = (int)(word(can_buf[6],can_buf[7]));
    break;

  case 0x773:
    ecu_error = (int)(word(can_buf[4],can_buf[5]));
    break;    
  }

  ecu_rpm = 2500;
}

// Print CAN Data formated to Serial1
void printBuf(uint32_t frame_id, byte *frame_data) {
  Serial.print(" Frame:");
  Serial.print(frame_id,HEX);

  Serial.print(" Data:[");
  for (int i=0;i<8;i++) {
    if (i>0) Serial.print(" ");
    Serial.print(frame_data[i],HEX);
  }
  Serial.println("]"); 
}










