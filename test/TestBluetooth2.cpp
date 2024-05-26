#include <Arduino.h>
#include <NimBLEDevice.h>

// Service name to be broadcasted to the outside world
#define PERIPHERAL_NAME "Robotel"
#define SERVICE_UUID "CD9CFC21-0ECC-42E5-BF22-48AA715CA112"
#define CHARACTERISTIC_INPUT_UUID "66E5FFCE-AA96-4DC9-90C3-C62BBCCD29AC"
#define CHARACTERISTIC_OUTPUT_UUID "142F29DD-B1F0-4FA8-8E55-5A2D5F3E2471"

// Output characteristic is used to send the response back to the connected phone
BLECharacteristic *pOutputChar;

// Class defines methods called when a device connects and disconnects from the service
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
        Serial.println("BLE Client Connected");
    }
    void onDisconnect(BLEServer* pServer) override {
        BLEDevice::startAdvertising();
        Serial.println("BLE Client Disconnected");
    }
};

class InputReceivedCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharWriteState) override {
        std::string inputValue = pCharWriteState->getValue();
        if (inputValue.length() > 0) {
            Serial.print("Received Value: ");
            for (char c : inputValue) {
                Serial.print(c);
            }
            Serial.println();
            // Send data to client
            std::string outputData = "Last received: " + inputValue;
            pOutputChar->setValue(outputData);
            pOutputChar->notify();
        }
    }
};

void setup() {
    Serial.begin(9600);

    // Initialize BLE
    BLEDevice::init(PERIPHERAL_NAME);

    // Create the server
    BLEServer *pServer = BLEDevice::createServer();

    // Create the service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Handle inputs (sent from app)
    BLECharacteristic *pInputChar = pService->createCharacteristic(
        CHARACTERISTIC_INPUT_UUID,
        NIMBLE_PROPERTY::WRITE_NR |
        NIMBLE_PROPERTY::WRITE
    );

    pOutputChar = pService->createCharacteristic(
        CHARACTERISTIC_OUTPUT_UUID,
        NIMBLE_PROPERTY::READ |
        NIMBLE_PROPERTY::NOTIFY
    );

    pServer->setCallbacks(new ServerCallbacks());
    pInputChar->setCallbacks(new InputReceivedCallbacks());

    // Start service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();
}

void loop() {
    // Nothing to do here for now
}
