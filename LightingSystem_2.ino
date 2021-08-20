#include <Ultrasonic.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#define PIN 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_RGB + NEO_KHZ800);
#define BRIGHTNESS 100

Ultrasonic ultrasonic(9, 8);
int switchPin = 10;

int distance;
int calibratedDistance = 10 ;
int preBrightness = 0;
int stripNumPixels = strip.numPixels();
double voltage = 5.0 ;
double b = 255;

boolean switchLumens = 0;

void setup() {
  Serial.begin(115200);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(100);
  
  pinMode(10,INPUT_PULLUP);
  
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  //lcd.print("Calibrating....");
  lcd.print("LED READY");
  if(digitalRead(switchPin) == 0){
    stripNumPixels = strip.numPixels();
  }else{
    stripNumPixels = strip.numPixels() / 2;
  }
  /*
  for(int i = 0; i<10; i++){
    distance = ultrasonic.read();
    if(calibratedDistance != distance){
      calibratedDistance = distance;
      i = 0;
    }
    
  }

*/
  for(int i = 0 ; i<stripNumPixels; i++){
     strip.setPixelColor(i,strip.Color(100,   100,   100));   
  }
  strip.setBrightness(255);
  strip.show();
  
  preBrightness = 255;

  
  Serial.print("Calibrated Distance in CM: ");
  Serial.println(calibratedDistance);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Voltage: "+String(voltage));
  lcd.setCursor(0,1);
  lcd.print("Distance: "+String(calibratedDistance));

  delay(1000);
}

void loop() {
    
    distance = ultrasonic.read();
    if(distance > calibratedDistance){
      distance = calibratedDistance;
    }
    int brightness = map(distance,0,calibratedDistance,0,255);
    b = (double)brightness;
    
//    Serial.println("Brightness: "+String(brightness));
    Serial.println("Distance: "+String(distance));
    if(distance != 0){
      
    if(preBrightness > brightness){
      for(int pre = preBrightness; pre >= brightness; pre-- ){
        for(int i = 0 ; i<stripNumPixels; i++){
           strip.setPixelColor(i,strip.Color(100,   100,   100));   
        }
        strip.setBrightness(pre);
        strip.show(); 
        
        voltage = mapf(pre,0.00,255.00,0.00,5.00);
        Serial.println("Voltage: "+String(voltage));
        
//        lcd.setCursor(0,0);
//        lcd.print("Voltage: "+String(voltage));
//        lcd.setCursor(0,1);
//        lcd.print("Distance: "+String(distance)+"  ");
        
//        Serial.println("Brightness: "+String(pre));      
//        delay(5);
      }
      
        lcd.setCursor(0,0);
        lcd.print("Voltage: "+String(voltage));
        lcd.setCursor(0,1);
        lcd.print("Distance: "+String(distance)+"  ");
        
      preBrightness = brightness;
    }
    else if(preBrightness < brightness){
      for(int pre = preBrightness; pre <= brightness; pre++ ){
        for(int i = 0 ; i<stripNumPixels; i++){
           strip.setPixelColor(i,strip.Color(100,   100,   100));   
        }
        strip.setBrightness(pre);
        strip.show(); 
        
        voltage = mapf(pre,0.00,255.00,0.00,5.00);
        Serial.println("Voltage: "+String(voltage));
//        lcd.setCursor(0,0);
//        lcd.print("Voltage: "+String(voltage));
//        lcd.setCursor(0,1);
//        lcd.print("Distance: "+String(distance)+"  ");
        
//        Serial.println("Brightness: "+String(pre));
//        delay(5);
      }
      
        lcd.setCursor(0,0);
        lcd.print("Voltage: "+String(voltage));
        lcd.setCursor(0,1);
        lcd.print("Distance: "+String(distance)+"  ");
      preBrightness = brightness;
    }else{
      for(int i = 0 ; i<stripNumPixels; i++){
         strip.setPixelColor(i,strip.Color(100,   100,   100));   
      }
      strip.setBrightness(preBrightness);
      strip.show(); 
      
      voltage = mapf(preBrightness,0.00,255.00,0.00,5.00);
      Serial.println("Voltage: "+String(voltage));
      lcd.setCursor(0,0);
      lcd.print("Voltage: "+String(voltage));
      lcd.setCursor(0,1);
      lcd.print("Distance: "+String(distance)+"  ");
      preBrightness = brightness;
    }

    if(digitalRead(switchPin) == 0){
      if(switchLumens == 1){
        switchLumens = 0;
        for(int i = 0 ; i<stripNumPixels; i++){
           strip.setPixelColor(i,strip.Color(100,   100,   100));   
        }
        strip.setBrightness(0);
        strip.show(); 
        stripNumPixels = strip.numPixels();
        
        lcd.setCursor(0,0);
        lcd.print("Voltage: "+String(0.00));
        lcd.setCursor(0,1);
        lcd.print("Distance: "+String(distance)+"  ");
      }
    }else{
      if(switchLumens == 0){
        switchLumens = 1;
        for(int i = 0 ; i<stripNumPixels; i++){
           strip.setPixelColor(i,strip.Color(100,   100,   100));   
        }
        strip.setBrightness(0);
        strip.show(); 
        stripNumPixels = strip.numPixels() / 2;
        lcd.setCursor(0,0);
        lcd.print("Voltage: "+String(0.00));
        lcd.setCursor(0,1);
        lcd.print("Distance: "+String(distance)+"  ");
       
      }
    }

    
    }
    
//    Serial.print("Calibrated Distance in CM: ");
//    Serial.println(calibratedDistance);

    delay(1000);

}
double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
