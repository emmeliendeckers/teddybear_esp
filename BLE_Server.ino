#include <BLEDevice.h>

#include <BLEServer.h>

#include <BLEUtils.h>

#include <BLE2902.h>

BLECharacteristic *myCharacteristic; //characteristics transmitting  
bool deviceConnected = false; 
int txValue = 0; 

#define SERVICE_UUID   "ab0828b1-198e-4351-b779-901fa0e0371e"
#define CHARACTERISTIC_UUID  "4ac8a682-9736-4e5d-932b-e9b31405049c"


//Custom callback methods 
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
       deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class CharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic * myCharacteristic) {
      std::string rxValue = myCharacteristic->getValue();
      if(rxValue.length() > 0) {
        Serial.println("====== START RECEIVE ======"); 
        Serial.print("Received value = " ); 
        for(int i=0; i<rxValue.length(); i++){
          Serial.print(rxValue[i]); 
        }

        Serial.println(); 
        
        if(rxValue[0] == '1') {
          Serial.println("Turning ON!"); 
          digitalWrite(LED_BUILTIN, HIGH); 
        }
        else if(rxValue[0] == '0') {
          Serial.println("Turning OFF!"); 
          digitalWrite(LED_BUILTIN, LOW); 
        }
     
      }
       
      Serial.println(); 
      Serial.println("====== END RECEIVE ======"); 
    }
};

void setup() {
  Serial.begin(115200); //for monitoring 
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Create the BLE Device
  BLEDevice::init("ESP32-BLE"); //name displayed while advertising 
 
  // Create the BLE Server
  BLEServer *server = BLEDevice::createServer(); 
  server->setCallbacks(new ServerCallbacks()); //use our own callback methods 

  // Create the BLE Service
  BLEService *service = server->createService(SERVICE_UUID);

  // Create a BLE Characterisc for transmitting 
  myCharacteristic = service->createCharacteristic(
                     CHARACTERISTIC_UUID,
                     BLECharacteristic::PROPERTY_READ |
                     BLECharacteristic::PROPERTY_WRITE
                   );
                   
  myCharacteristic->setCallbacks(new CharacteristicCallbacks); 

  // Start the service
  service->start();
  // Start advertising 
  server->getAdvertising()->start();
}

void loop() {
    delay(1000);

}
