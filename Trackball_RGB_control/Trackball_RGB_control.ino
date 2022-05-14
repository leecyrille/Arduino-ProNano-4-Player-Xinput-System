//
//    FILE: RGB2HUE.pde
//  AUTHOR: Rob Tillaart
//    DATE: 2011-09-17 
//
// PUPROSE: color-rainbow
//
// http://www.dipzo.com/wordpress/?p=50
// http://www.easyrgb.com/index.php?X=MATH
//

int Rpin=10;
int Gpin=6;
int Bpin=9;

float H,S,L, Rval,Gval,Bval;

void HSL(float H, float S, float L, float& Rval, float& Gval, float& Bval);
float Hue_2_RGB( float v1, float v2, float vH );

void setup()
{
  Serial.begin(9600);
}


void loop()
{
  S=1;
  L=.5;
  Rval=0;
  Gval=0;
  Bval=0;
  for (int i = 0; i< 360; i++)
  {
    HSL(i/360.0,S,L,Rval,Gval,Bval);

    //common anode configuration
    if(Rval < 0)
    {
      Rval = 0;
    }

    if(Gval < 0)
    {
      Gval = 0;
    }

    if(Bval < 0)
    {
      Bval = 0;
    }
    analogWrite(Rpin, 255-Rval);
    analogWrite(Gpin, 255-Gval);
    analogWrite(Bpin, 255-Bval);

    //common cathode configuration
    //analogWrite(Rpin, Rval);
    //analogWrite(Gpin, Gval);
    //analogWrite(Bpin, Bval);
    //digitalWrite(12,LOW);

    //print statements for debug
    Serial.print("position:");
    Serial.print(H);
    Serial.print(" R:");
    Serial.print(Rval);
    Serial.print(" G:");
    Serial.print(Gval);
    Serial.print(" B:");
    Serial.println(Bval);
    delay(10);
  }
}

void HSL(float H, float S, float L, float& Rval, float& Gval, float& Bval)
{
  float var_1;
  float var_2;
  float Hu=H+.33;
  float Hd=H-.33;
  if ( S == 0 )                       //HSL from 0 to 1
  {
    Rval = L * 255;                      //RGB results from 0 to 255
    Gval = L * 255;
    Bval = L * 255;
  }
  else
  {
    if ( L < 0.5 ) 
      var_2 = L * ( 1 + S );
    else           
      var_2 = ( L + S ) - ( S * L );

    var_1 = 2 * L - var_2;

    Rval = round(255 * Hue_2_RGB( var_1, var_2, Hu ));
    Serial.print("Rval:");
    Serial.println(Hue_2_RGB( var_1, var_2, Hu ));
    Gval = round(255 * Hue_2_RGB( var_1, var_2, H ));
    Bval = round(255 * Hue_2_RGB( var_1, var_2, Hd ));
  }

}
float Hue_2_RGB( float v1, float v2, float vH )             //Function Hue_2_RGB
{
  if ( vH < 0 ) 
    vH += 1;
  if ( vH > 1 ) 
    vH -= 1;
  if ( ( 6 * vH ) < 1 ) 
    return ( v1 + ( v2 - v1 ) * 6 * vH );
  if ( ( 2 * vH ) < 1 ) 
    return ( v2 );
  if ( ( 3 * vH ) < 2 ) 
    return ( v1 + ( v2 - v1 ) * (.66-vH) * 6 );
  return ( v1 );
}
