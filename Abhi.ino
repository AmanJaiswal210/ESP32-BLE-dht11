#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <DHT.h>

#define DHTPIN 13                                                           // what digital pin we're connected to
#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

int ldrvalue;
BLECharacteristic *characteristicTX; 
BLECharacteristic *characteristicTX1; 
bool deviceConnected = false;

#define SERVICE_UUID           "ab0828b1-198e-4351-b779-901fa0e0371e" 
#define CHARACTERISTIC_UUID_RX "4ac8a682-9736-4e5d-932b-e9b31405049c"
#define CHARACTERISTIC_UUID_TX "0972EF8C-7613-4075-AD52-756F33D4DA91"

#define CHARACTERISTIC_UUID_RX1 "4ac8a682-9736-4e5d-932b-e9b31405049b"
#define CHARACTERISTIC_UUID_TX1 "0972EF8C-7613-4075-AD52-756F33D4DA9l"

class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  Serial.begin(115200);
dht.begin();
  // Create the BLE Device
  BLEDevice::init("Abhishek FuckOFF");

  BLEServer *server = BLEDevice::createServer(); 
  server->setCallbacks(new ServerCallbacks()); 

  BLEService *service = server->createService(SERVICE_UUID);

  characteristicTX = service->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  characteristicTX->addDescriptor(new BLE2902());
  BLECharacteristic *characteristic = service->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  characteristicTX1 = service->createCharacteristic(
                      CHARACTERISTIC_UUID_TX1,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  characteristicTX1->addDescriptor(new BLE2902());
  BLECharacteristic *characteristic1 = service->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX1,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );


  service->start();
  server->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
 float h = dht.readHumidity();                                              // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature();                                           // Read temperature as Celsius (the default)
  Serial.print("Air Temperature = ");
  Serial.print(t);
  Serial.print("\nHumidity = ");
  Serial.print(h);    
          
          char txString[8]; 
          dtostrf(h, 1, 2, txString); 
          characteristicTX1->setValue(txString);       
          characteristicTX1->notify();
           

          dtostrf(t, 1, 2, txString); 
          characteristicTX->setValue(txString);       
          characteristicTX->notify(); 
          

          delay(1000);

}

