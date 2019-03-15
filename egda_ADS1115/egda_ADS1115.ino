#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 _ADS1x15_1_instance(0x48);
float _gtv1;
float _gtv2;
float _gtv3;
float _gtv4;
String _gtv5 = "\t";
String _gtv6 = "\n";
float _ADS1x15_1_Output_1 = 0.00;
bool _ADS1x15_1_isNeedRequest_1 = 0;
bool _ADS1x15_1_OldState_1 = 0;
float _ADS1x15_1_Output_2 = 0.00;
bool _ADS1x15_1_isNeedRequest_2 = 0;
bool _ADS1x15_1_OldState_2 = 0;
float _ADS1x15_1_Output_3 = 0.00;
bool _ADS1x15_1_isNeedRequest_3 = 0;
bool _ADS1x15_1_OldState_3 = 0;
float _ADS1x15_1_Output_4 = 0.00;
bool _ADS1x15_1_isNeedRequest_4 = 0;
bool _ADS1x15_1_OldState_4 = 0;
bool _gen1I = 0;
bool _gen1O = 0;
unsigned long _gen1P = 0UL;
bool _stou1 = 0;
void setup()
{
  Serial.begin(9600);
  _ADS1x15_1_instance.setGain(GAIN_TWOTHIRDS);
  _ADS1x15_1_instance.begin();
}
void loop()
{ int _tempVariable_int;





  //Плата:1
  if (1) {
    if (! _gen1I) {
      _gen1I = 1;
      _gen1O = 1;
      _gen1P = millis();
    }
  } else {
    _gen1I = 0 ;
    _gen1O = 0;
  }
  if (_gen1I) {
    if ( _isTimer ( _gen1P , 200 )) {
      _gen1P = millis();
      _gen1O = ! _gen1O;
    }
  }
  if (_gen1O) {
    if (! _ADS1x15_1_OldState_1) {
      _ADS1x15_1_OldState_1 = 1;
      _ADS1x15_1_isNeedRequest_1 = 1;
    }
  } else {
    _ADS1x15_1_OldState_1 = 0;
  }
  if (_ADS1x15_1_isNeedRequest_1) {
    _ADS1x15_1_isNeedRequest_1 = 0;
    _tempVariable_int = _ADS1x15_1_instance.readADC_SingleEnded(0);
    _ADS1x15_1_Output_1 = _ads1x15ValueConvert(_tempVariable_int, 0, 1);
  }
  if (_gen1O) {
    if (! _ADS1x15_1_OldState_2) {
      _ADS1x15_1_OldState_2 = 1;
      _ADS1x15_1_isNeedRequest_2 = 1;
    }
  } else {
    _ADS1x15_1_OldState_2 = 0;
  }
  if (_ADS1x15_1_isNeedRequest_2) {
    _ADS1x15_1_isNeedRequest_2 = 0;
    _tempVariable_int = _ADS1x15_1_instance.readADC_SingleEnded(1);
    _ADS1x15_1_Output_2 = _ads1x15ValueConvert(_tempVariable_int, 0, 1);
  }
  if (_gen1O) {
    if (! _ADS1x15_1_OldState_3) {
      _ADS1x15_1_OldState_3 = 1;
      _ADS1x15_1_isNeedRequest_3 = 1;
    }
  } else {
    _ADS1x15_1_OldState_3 = 0;
  }
  if (_ADS1x15_1_isNeedRequest_3) {
    _ADS1x15_1_isNeedRequest_3 = 0;
    _tempVariable_int = _ADS1x15_1_instance.readADC_SingleEnded(2);
    _ADS1x15_1_Output_3 = _ads1x15ValueConvert(_tempVariable_int, 0, 1);
  }
  if (_gen1O) {
    if (! _ADS1x15_1_OldState_4) {
      _ADS1x15_1_OldState_4 = 1;
      _ADS1x15_1_isNeedRequest_4 = 1;
    }
  } else {
    _ADS1x15_1_OldState_4 = 0;
  }
  if (_ADS1x15_1_isNeedRequest_4) {
    _ADS1x15_1_isNeedRequest_4 = 0;
    _tempVariable_int = _ADS1x15_1_instance.readADC_SingleEnded(3);
    _ADS1x15_1_Output_4 = _ads1x15ValueConvert(_tempVariable_int, 0, 1);
  }
  _gtv4 = _ADS1x15_1_Output_4;
  _gtv3 = _ADS1x15_1_Output_3;
  _gtv2 = _ADS1x15_1_Output_2;
  _gtv1 = _ADS1x15_1_Output_1;
  if (_gen1O) {
    if (!_stou1) {
      Serial.print(((( _floatToStringWitRaz(_ADS1x15_1_Output_1, 3))) + (_gtv5) + (( _floatToStringWitRaz(_ADS1x15_1_Output_2, 3))) + (_gtv5) + (( _floatToStringWitRaz(_ADS1x15_1_Output_3, 3))) + (_gtv5) + (( _floatToStringWitRaz(_ADS1x15_1_Output_4, 3))) + (_gtv6)));
      _stou1 = 1;
    }
  } else {
    _stou1 = 0;
  }




}
String  _floatToStringWitRaz(float value, int raz)
{

  return String(value, raz);
}
bool _isTimer(unsigned long startTime, unsigned long period )
{
  unsigned long currentTime;
  currentTime = millis();
  if (currentTime >= startTime) {
    return (currentTime >= (startTime + period));
  } else {
    return (currentTime >= (4294967295UL - startTime + period));
  }
}
float _ads1x15ValueConvert(int value, byte gain, bool isVolt)
{ float temp;
  if (gain == 0) {
    temp = 0.187;
  }
  if (gain == 1) {
    temp = 0.125;
  }
  if (gain == 2) {
    temp = 0.0625;
  }
  if (gain == 3) {
    temp = 0.03125;
  }
  if (gain == 4) {
    temp = 0.01562;
  }
  if (gain == 5) {
    temp = 0.0078125;
  }
  temp = value * temp;
  if (isVolt) {
    temp = temp / 1000;
  }
  return temp;
}
