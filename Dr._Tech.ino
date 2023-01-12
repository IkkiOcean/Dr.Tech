#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
 
#define WIFISSID "Hotspot"
#define PASSWORD "whitedown"
//0x5A
//0x27

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27, 20,4);
float rc[6],rf[6]; 
int laser = 18;
void setup() {
  
 pinMode(laser,OUTPUT);
 WiFi.begin(WIFISSID, PASSWORD);
 while(WiFi.status() != WL_CONNECTED){
        delay(100);
    }
    if (WiFi.status() == WL_CONNECTED) {
  HTTPClient http;

 lcd.begin();
 lcd.backlight();
 lcd.setCursor(7,1);
 lcd.print("Dr.");
 lcd.setCursor(6,2);
 lcd.print("Tech");
// Serial.println(digitalRead(Switch)); 
 mlx.begin(); 
 String api = "http://192.168.188.114/docto/api/appointments.php";
 http.begin(api);
 int t = 0;
 int userid;
 int id;
 while(t != 1){
 http.GET();
 
 String payload = http.getString();
  char json[500];
  payload.replace(" ","");
  payload.replace("\n","");
  payload.trim();
  payload.remove(0,1);
  payload.toCharArray(json,500);
  StaticJsonDocument <200>doc;
  deserializeJson(doc,json);
  int statuss = doc["status"];
  userid = doc["user_id"];
  t = statuss;
  id = doc["id"];
 }
http.end();
 lcd.clear();
 lcd.setCursor(8,0);
 lcd.print("Get Ready....");
 digitalWrite(laser,HIGH);
 delay(3000);

 float ecg[500];    
 lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("STARTING IN 3");
    delay(1000);
    lcd.clear();
    lcd.print("STARTING IN 2");
    delay(1000);
    lcd.clear();
    lcd.print("STARTING IN 1");
    delay(1000);
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Measuring");
    lcd.setCursor(4,1);
    lcd.print("Temperature");
    for(int i = 0; i < 6; i++){
      rc[i] = mlx.readObjectTempC();
      rf[i] = mlx.readObjectTempF();
      delay(400);
      
      }
      for (int i = 0; i < 6; i++){
      if (rc[0] < rc[i]){
        rc[0] = rc[i];  
      }
      if (rf[0] < rf[i]){
        rf[0] = rf[i]; 
      }
    }
    
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Temperature ;");
    lcd.setCursor(1,1);
    lcd.print(String(rc[0]) + "°C / " + String(rf[0]) + "°F" );
    lcd.setCursor(4,2);
    lcd.print("Taking ECG...");
    for(int b = 0; b < 500; b++){
      ecg[b] = analogRead(A0);
    }
     
//    StaticJsonDocument<200> doc;
//    doc["tempc"] = String(rc[0]);
//    doc["tempf"] = String(rf[0]);
//    serializeJson(doc, Serial);
//    JsonArray data = doc.createNestedArray("data");
//  for(int d = 0; d < 500; d++){ 
    
    //data.add(ecg[d]);
  
  String dataa = "";
    for(int d = 0; d < 500; d++)  {
      dataa += "," + String(ecg[d]);
    }
    
    
    
    String tempc = String(rc[0]);
    String tempf = String(rf[0]);
    String serverPath = ("http://192.168.188.114/docto/api/get.php?data=" + dataa + "&temp=" + tempc + "&tempf=" + tempf + "&user=" + userid + "&id=" +  String(id) );                                                                                                                                                                                                                                                                                                                                                                                                                                    
    http.begin(serverPath.c_str());  
    http.addHeader("Content-Type", "application/json");  
     
    int httpResponseCode = http.GET();
    if(httpResponseCode>0){
       
      String response = http.getString(); 
     
    }
    else {
       
    }
lcd.clear();
lcd.setCursor(1,0);
lcd.print("Process Completed");
digitalWrite(laser,LOW);
delay(3000);
}
}


void loop(){
}
