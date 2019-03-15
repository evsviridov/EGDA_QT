#include <MsTimer2.h>
#include <analogFuncs.h>

int NUMBITS = 16;
unsigned long MAX_VALUE = ((1UL << NUMBITS) - 1);
#define NUM_MEAS  20

typedef unsigned int (*ai_func_type)(char);
ai_func_type ai_func;

ai_func_type setup_aiFunction(int adc_bits)
{
  ai_func_type ai_func_temp;
  switch (adc_bits)
  {
    case 12: ai_func_temp = (unsigned int (*)(char))&analogRead_12bits;
      break;
    case 16: ai_func_temp = (unsigned int (*)(char))&analogRead_16bits;
      break;
    case 10: ai_func_temp = (unsigned int (*)(char))&analogRead;
      break;
    default:  ai_func_temp = (unsigned int (*)(char))&analogRead;
      NUMBITS = 10;
  }
  MAX_VALUE = ((1UL << NUMBITS) - 1);
  return ai_func_temp;
}

void setup() {
  // put your setup code here, to run once:
  ai_func = setup_aiFunction(NUMBITS);
  pinMode(A0,INPUT);
  analogReference(EXTERNAL);

  Serial.begin(115200);
//  Serial.println();
//  Serial.print("10 bit: "); Serial.println((unsigned long)(unsigned int (*)(char))&analogRead);
//  Serial.print("12 bit: "); Serial.println((unsigned long)(unsigned int (*)(char))&analogRead_12bits);
//  Serial.print("16 bit: "); Serial.println((unsigned long)(unsigned int (*)(char))&analogRead_16bits);
//  Serial.print("active: "); Serial.println((unsigned long)ai_func);
//  Serial.println(MAX_VALUE);
}

void loop() {
  unsigned long ai_long;
  double ai;
  int n;
  // put your main code here, to run repeatedly:
  // Serial.print("Adc: ");
  for (int i = A0; i <= A0; i++)
  {
    //    switch (NUMBITS)
    //    {
    //      case 12: ai = (double) analogRead_12bits(i);
    //        break;
    //      case 16: ai = (double) analogRead_16bits(i);
    //        break;
    //      default: ai = (double) analogRead(i);
    //    }
    ai_long = 0;
    n = NUM_MEAS;
    while (n--)
    {
      ai_long = ai_long + ai_func(i);
    }
    ai = ai_long / NUM_MEAS;
    ai = (double) ai / (double) MAX_VALUE * 100;
    Serial.print(ai, 1);
    Serial.print("\t");
  }
  Serial.println();

  delay(100);
}
