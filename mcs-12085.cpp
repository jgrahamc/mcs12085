// mcs-12085.cpp
//
// Copyright (c) 2012 John Graham-Cumming
//
// Code to interface to MCS-12085 Optical Mouse Chip

#include "mcs-12085.h"

// The digital pins that are connected to SCLK and SDIO on the sensor

int clk_pin = 4;
int dat_pin = 7;

// The time of a clock pulse. This will be used twice to make the clock
// signal: cycle us low and then cycle us high
int cycle = 25;

// mcs12085_init: set up the pins for the clock and data
void mcs12085_init() 
{
  // When not being clocked the clock pin needs to be high
  
  pinMode(clk_pin, OUTPUT);
  digitalWrite(clk_pin, HIGH);

  pinMode(dat_pin, OUTPUT);
  digitalWrite(dat_pin, LOW);
}

// mcs12085_tick: perform a single clock tick of 25us low
void mcs12085_tick()
{
  digitalWrite(clk_pin, LOW);
  delayMicroseconds(cycle);
  digitalWrite(clk_pin, HIGH);  
}

// mcs12085_tock: finish the clock pulse by waiting during the high period
void mcs12085_tock()
{
  delayMicroseconds(cycle);
}

// mcs12085_read_bit: read a single bit from the chip by creating a clock
// pulse and reading the value returned
int mcs12085_read_bit()
{
  mcs12085_tick();
  
  int r = (digitalRead(dat_pin) == HIGH);
  
  mcs12085_tock();
  return r;
}

// mcs12085_read_byte: Reads 8 bits from the sensor MSB first
byte mcs12085_read_byte()
{
  int bits = 8;
  byte value = 0x80;
  byte b = 0;
  
  while (bits > 0) {
    if ( mcs12085_read_bit() ) {
      b |= value;
    }
    
    value >>= 1;
    --bits;
  }
  
  pinMode(dat_pin, OUTPUT);
  digitalWrite(dat_pin, LOW);
  
  return b;
}  
                       
// mcs12085_write_bit: write a single bit to the chip by creating a clock
// pulse and writing the value
void mcs12085_write_bit(byte b)    // 1 or 0
{
  // Set the data pin value ready for the write and then clock
  
  if ( b ) {
    digitalWrite(dat_pin, HIGH);
  } else {
    digitalWrite(dat_pin, LOW);
  }
  
  mcs12085_tick();
  mcs12085_tock();
  digitalWrite(dat_pin, LOW);
}

// mcs12086_write_byte: write a byte to the sensor MSB first
void mcs12085_write_byte(byte w)      // Number to get the bits from
{  
  int bits = 8;
  
  while ( bits > 0 ) {
    mcs12085_write_bit(w & 0x80);
    w <<= 1;
    --bits;
  }
  
  pinMode(dat_pin, INPUT);
}

// mcs12085_wr_pause: pause between a write and a read to the sensor
void mcs12085_wr_pause()
{
  delayMicroseconds(100);
}

// mcs12085_rw_pause: pause between a read and a write to the sensor
void mcs12085_rw_pause()
{
  delayMicroseconds(250);
}

// mcs12085_convert: converts a byte into a signed 8-bit int
int mcs12085_convert(byte b) 
{
  if ( b < 128 ) {
     return int(b);
  } else {
     return -(int(b ^ 0xFF) + 1);
  }
}

// mcs12085_dx: read the change in X position since last read
int mcs12085_dx() 
{
  mcs12085_write_byte(0x02); // 0x02 = Read DX Register
  mcs12085_wr_pause();
  int i = mcs12085_convert(mcs12085_read_byte());
  mcs12085_rw_pause();
  return i;
}

// mcs12085_dy: read the change in Y position since last read
int mcs12085_dy()
{
  mcs12085_write_byte(0x03); // 0x03 = Read DY Register
  mcs12085_wr_pause();
  int i = mcs12085_convert(mcs12085_read_byte());
  mcs12085_rw_pause();
  return i;
}






