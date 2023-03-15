void setup() {
  Serial.begin(9600);

}

double roundToNearest(double v, double base)
{
  return round(v * 1/base) * base;
}

void loop() {
  double v = 2.5;
  double target_v = roundToNearest((analogRead(A0)/1024. * 5.) * 2 - 5, 0.01953125);
  double curr_v = 0;
  
  for(int i = 0; i < 7; i++)
  {
      if(((curr_v + v) - target_v) > -(curr_v + v) * 0 )
      {
          v /= 2;
          Serial.print('0');
          continue;
      }
      
      curr_v += v;
      v /= 2;
      
      Serial.print('1');
  }

  Serial.println();
}
