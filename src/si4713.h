/*  Full option SI4713 library with advanced RDS and MPX settings
 *   
 *  v1.0 - Sjef Verhoeven PE5PVB
 * 
 *  Website: https://www.pe5pvb.nl/
 *  Github:  https://github.com/PE5PVB
 *  
*/

#include <Wire.h>
uint8_t buf[10];
uint16_t component;
uint16_t acomp;
uint16_t misc;
int addr;
uint8_t GPO;

class SI4713
{
  public:
    void Init(uint8_t RST, uint16_t clk, uint8_t address);
    void Output(uint8_t level, uint8_t cap);
    void Freq(uint16_t freq);
    void RDS_PI(uint16_t RDSPI);
    void RDS_PTY(uint8_t RDSPTY);
    void RDS_COMP(bool ONOFF);
    void RDS_ART(bool ONOFF);
    void RDS_MS(bool ONOFF);
    void RDS_TP(bool ONOFF);
    void RDS_TA(bool ONOFF);
    void RDS_AF(uint16_t AF);
    void RDS_PS(String PS, uint8_t number);
    void RDS_RT(String RT);
    void RDS_MUSP(bool ONOFF);
    void RDS_PSCOUNT(uint8_t count, uint8_t speed);
    void RDS_Enable(bool ONOFF);
    void MPX_Enable(bool ONOFF);
    void Audio_Mute(bool ONOFF);
    void MPX_Deviation(uint16_t dev);
    void RDS_Deviation(uint16_t dev);
    void Audio_PreEmphasis(uint16_t emp);
    void MPX_Freq(uint16_t freq);
    void Audio_Limiter(bool ONOFF);
    void Audio_AGC(bool ONOFF);
    void Audio_Comp_Threshold(int16_t threshold);
    void Audio_Comp_Attack(uint16_t attack);
    void Audio_Comp_Release(uint16_t rel);
    void Audio_Comp_Gain(uint16_t gain);
    void Audio_Limiter_Release(uint16_t rel);
    void Audio_Deviation(uint16_t dev);
    void ASQ(bool &overmod, int8_t &inlevel);
    void Rev(uint8_t &pn, uint8_t &chiprev);
    void GPO(bool GPO1, bool GPO2, bool GPO3);

  private:
    bool WriteBuffer(uint8_t len);
    bool ReadBuffer(uint8_t len);
    bool Set_Property(uint16_t arg1, uint16_t arg2);
};


bool SI4713::ReadBuffer(uint8_t len)
{


}

bool SI4713::WriteBuffer(uint8_t len)
{
  Wire.beginTransmission(addr);
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(buf[i]);
    if (buf[i] < 0x10) {
    }
  }
  Wire.endTransmission();
  delay(54);
  Wire.requestFrom(addr, 1);
  while (bitRead(Wire.read(), 7) != 1) {}
}

bool SI4713::Set_Property(uint16_t arg1, uint16_t arg2)
{
  buf[0] = 0x12;
  buf[1] = 0x00;
  buf[2] = highByte(arg1);
  buf[3] = lowByte(arg1);
  buf[4] = highByte(arg2);
  buf[5] = lowByte(arg2);
  WriteBuffer(6);
}

void SI4713::Output(uint8_t level, uint8_t cap)
{
  buf[0] = 0x31;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = level;
  buf[4] = cap;
  WriteBuffer(5);
}

void SI4713::Freq(uint16_t freq)
{
  buf[0] = 0x30;
  buf[1] = 0x00;
  buf[2] = highByte(freq);
  buf[3] = lowByte(freq);
  WriteBuffer(4);
}

void SI4713::RDS_PI(uint16_t RDSPI)
{
  Set_Property(0x2c01, RDSPI);
}

void SI4713::RDS_PSCOUNT(uint8_t count, uint8_t speed)
{
  Set_Property(0x2c05, count);
  Set_Property(0x2c04, speed);
}

void SI4713::RDS_PTY(uint8_t RDSPTY)
{
  bitWrite(misc, 5, bitRead(RDSPTY, 0));
  bitWrite(misc, 6, bitRead(RDSPTY, 1));
  bitWrite(misc, 7, bitRead(RDSPTY, 2));
  bitWrite(misc, 8, bitRead(RDSPTY, 3));
  bitWrite(misc, 9, bitRead(RDSPTY, 4));
  Set_Property(0x2c03, misc);
}

void SI4713::RDS_COMP(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(misc, 14, 1);
  } else {
    bitWrite(misc, 14, 0);
  }
  Set_Property(0x2c03, misc);
}

void SI4713::RDS_ART(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(misc, 13, 1);
  } else {
    bitWrite(misc, 13, 0);
  }
  Set_Property(0x2c03, misc);
}

void SI4713::RDS_MS(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(misc, 12, 1);
  } else {
    bitWrite(misc, 12, 0);
  }
  Set_Property(0x2c03, misc);
}

void SI4713::RDS_TP(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(misc, 10, 1);
  } else {
    bitWrite(misc, 10, 0);
  }
  Set_Property(0x2c03, misc);
}

void SI4713::RDS_TA(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(misc, 4, 1);
  } else {
    bitWrite(misc, 4, 0);
  }
  Set_Property(0x2c03, misc);
}

void SI4713::RDS_AF(uint16_t AF)
{
  if (AF == 0) {
    Set_Property(0x2c06, 0xe0e0);
  } else {
    Set_Property(0x2c06, 0xdd95 + AF);
  }
}

void SI4713::RDS_MUSP(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(misc, 3, 1);
  } else {
    bitWrite(misc, 3, 0);
  }
  Set_Property(0x2c03, misc);
}

void SI4713::RDS_Enable(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(component, 2, 1);
  } else {
    bitWrite(component, 2, 0);
  }
  Set_Property(0x2100, component);
}

void SI4713::Audio_Comp_Threshold(int16_t threshold)
{
  Set_Property(0x2201, threshold);
}

void SI4713::Audio_Comp_Attack(uint16_t attack)
{
  Set_Property(0x2202, attack);
}

void SI4713::Audio_Comp_Release(uint16_t rel)
{
  Set_Property(0x2203, rel);
}

void SI4713::Audio_Comp_Gain(uint16_t gain)
{
  Set_Property(0x2204, gain);
}

void SI4713::Audio_Limiter_Release(uint16_t rel)
{
  Set_Property(0x2205, rel);
}

void SI4713::Audio_Limiter(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(acomp, 1, 1);
  } else {
    bitWrite(acomp, 1, 0);
  }
  Set_Property(0x2200, acomp);
}

void SI4713::Audio_AGC(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(acomp, 0, 1);
  } else {
    bitWrite(acomp, 0, 0);
  }
  Set_Property(0x2200, acomp);
}

void SI4713::MPX_Enable(bool ONOFF)
{
  if (ONOFF == true) {
    bitWrite(component, 0, 1);
    bitWrite(component, 1, 1);
  } else {
    bitWrite(component, 0, 0);
    bitWrite(component, 1, 0);
  }
  Set_Property(0x2100, component);
}

void SI4713::Audio_Mute(bool ONOFF)
{
  if (ONOFF == false) {
    Set_Property(0x2105, 0x0000);
  } else {
    Set_Property(0x2105, 0x0003);
  }
}

void SI4713::Audio_PreEmphasis(uint16_t emp)
{
  if (emp == 0) {
    Set_Property(0x2106, 0x0002);
  } else if (emp == 75) {
    Set_Property(0x2106, 0x0000);
  } else if (emp == 50) {
    Set_Property(0x2106, 0x0001);
  }
}

void SI4713::MPX_Freq(uint16_t freq)
{
  Set_Property(0x2107, freq);
}

void SI4713::MPX_Deviation(uint16_t dev)
{
  Set_Property(0x2102, dev);
}

void SI4713::RDS_Deviation(uint16_t dev)
{
  Set_Property(0x2103, dev);
}

void SI4713::Audio_Deviation(uint16_t dev)
{
  Set_Property(0x2101, dev);
}

void SI4713::GPO(bool GPO1, bool GPO2, bool GPO3)
{
  buf[0] = 0x81;
  if (GPO1 == true)
  {
    bitWrite(buf[1], 1, 1);
  } else {
    bitWrite(buf[1], 1, 0);
  }
  if (GPO2 == true)
  {
    bitWrite(buf[1], 2, 1);
  } else {
    bitWrite(buf[1], 2, 0);
  }
  if (GPO3 == true)
  {
    bitWrite(buf[1], 3, 1);
  } else {
    bitWrite(buf[1], 3, 0);
  }
  WriteBuffer(2);

}

void SI4713::RDS_PS(String PS, uint8_t number)
{
  char PSArray[9];
  for (uint8_t i = 0; i < 9; i++) {
    PSArray[i] = 0x20;
  }
  PS.toCharArray(PSArray, 9);
  for (uint8_t i = 0; i < 8; i++) {
    if (PSArray[i] == 0x00)
    {
      PSArray[i] = 0x20;
    }
  }
  buf[0] = 0x36;
  if (number > 0) {
    buf[1] = number * 2;
  } else {
    buf[1] = 0x00;
  }
  buf[2] = PSArray[0];
  buf[3] = PSArray[1];
  buf[4] = PSArray[2];
  buf[5] = PSArray[3];
  WriteBuffer(6);
  buf[0] = 0x36;
  if (number > 0) {
    buf[1] = (number * 2) + 1;
  } else {
    buf[1] = 0x01;
  }
  buf[2] = PSArray[4];
  buf[3] = PSArray[5];
  buf[4] = PSArray[6];
  buf[5] = PSArray[7];
  WriteBuffer(6);
}

void SI4713::RDS_RT(String RT)
{
  char RTArray[32];
  for (uint8_t i = 0; i < 32; i++) {
    RTArray[i] = 0x20;
  }

  RT.toCharArray(RTArray, 32);
  for (uint8_t i = 0; i < 32; i++) {
    if (RTArray[i] == 0x00)
    {
      RTArray[i] = 0x20;
    }
  }
  uint8_t counter = 0;
  for (uint8_t i = 0; i < 8; i++) {
    buf[0] = 0x35;
    if (i == 0) {
      buf[1] = 0x06;
    } else {
      buf[1] = 0x04;
    }
    buf[2] = 0x20;
    buf[3] = i;
    buf[4] = RTArray[0 + counter];
    buf[5] = RTArray[1 + counter];
    buf[6] = RTArray[2 + counter];
    buf[7] = RTArray[3 + counter];
    WriteBuffer(8);
    counter += 4;
  }
}

void SI4713::Init(uint8_t RST, uint16_t clk, uint8_t address)
{
  addr = address;           // Copy I2C address
  pinMode(RST, OUTPUT);     // Send RST
  digitalWrite(RST, HIGH);
  delay(50);
  digitalWrite(RST, LOW);
  delay(50);
  digitalWrite(RST, HIGH);
  Wire.begin();
  Wire.beginTransmission(addr);
  Wire.write(0x01);
  Wire.write(0x12);
  Wire.write(0x50);
  Wire.endTransmission();
  buf[0] = 0x80;
  buf[1] = 0x0e;
  WriteBuffer(2);
  Set_Property(0x0201, clk);
  Set_Property(0x2300, 0x0007); // Enable ASQ Interrupts
}

void SI4713::ASQ(bool &overmod, int8_t &inlevel)
{
  buf[0] = 0x34;
  buf[1] = 0x00;
  WriteBuffer(2);
  uint8_t resp[5];
  uint8_t len = 5;
  Wire.requestFrom(0x63, 5);
  if (Wire.available() == len) {
    for (uint16_t i = 0; i < len; i++) {
      resp[i] = Wire.read();
    }
    overmod = bitRead(resp[1], 2);
    inlevel = resp[4];
    buf[0] = 0x34;
    buf[1] = 0x01;
    WriteBuffer(2);
    return overmod, inlevel;
  }
}

void SI4713::Rev(uint8_t &pn, uint8_t &chiprev)
{
  buf[0] = 0x10;
  WriteBuffer(1);
  uint8_t resp[9];
  uint8_t len = 9;
  Wire.requestFrom(0x63, 9);
  if (Wire.available() == len) {
    for (uint16_t i = 0; i < len; i++) {
      resp[i] = Wire.read();
    }
    pn = resp[1];
    chiprev = resp[8];
  }
  return pn, chiprev;
}
