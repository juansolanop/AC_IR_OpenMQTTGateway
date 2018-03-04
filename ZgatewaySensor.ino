#include "config_sensor.h"
#include "DHTesp.h"
DHTesp dht;
float diffTEMP = 0.2;
float tempValue;
float diffHUM = 1;
float humValue;

/********************************** START SETUP*****************************************/
void Sensor_setup() {
  dht.setup(DHTPIN); // data pin 2

}


/********************************** START CHECK SENSOR **********************************/
bool checkBoundSensor(float newValue, float prevValue, float maxDiff) {
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}


void Sensorloop() {

    float newHumValue = dht.getHumidity(); //to use celsius remove the true text inside the parentheses  
    float newTempValue = dht.getTemperature();
    float heatIndex = dht.computeHeatIndex(newTempValue, newHumValue, false);
    delay(dht.getMinimumSamplingPeriod());

    if (checkBoundSensor(newTempValue, tempValue, diffTEMP)) {
      tempValue = newTempValue;
      SensortoMQTT(newHumValue,newTempValue,heatIndex);
    }
     if (checkBoundSensor(newHumValue, humValue, diffHUM)) {
      humValue = newHumValue;
      SensortoMQTT(newHumValue,newTempValue,heatIndex);
    } 


}

/********************************** START SEND STATE*****************************************/


boolean SensortoMQTT(float humidity, float temp, float rfeel){  
  
  trc(F("Rcv. IR"));
  unsigned long MQTTvalue = 0;
  String MQTTvalueHumidity = String(humidity);
  String MQTTvalueTemperature = String(temp);
  String MQTTvalueRFeel = String(rfeel);

  trc(F("Sensor data IRtoMQTT"));
  trc(F("Sending Humidity, Temp and Real Feel"));
  String value = String(MQTTvalue);
  trc(value);
  boolean resultH = client.publish(subjectHumiditytoMQTT,(char *)MQTTvalueHumidity.c_str());
  boolean resultT = client.publish(subjectTemperaturetoMQTT,(char *)MQTTvalueTemperature.c_str());
  boolean resultR = client.publish(subjectRealFeeltoMQTT,(char *)MQTTvalueRFeel.c_str());
  return resultH and resultT and resultR;  
}
