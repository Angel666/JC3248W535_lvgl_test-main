#include "ble_bridge.h"
#include "bike_data.h"
#include <Arduino.h>

// Внешнее объявление объекта UART (он определён в ft85bd_uart.cpp)
extern HardwareSerial FT85BD;

// --- Конфигурация UUID ---
#define FT85BD_SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define FT85BD_CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define FT85BD_CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// --- Глобальные переменные ---
static NimBLEServer *pServer = nullptr;
static NimBLECharacteristic *pCharacteristicVescTx = nullptr;
static NimBLECharacteristic *pCharacteristicVescRx = nullptr;
static bool deviceConnected = false;
static bool oldDeviceConnected = false;

static int MTU_SIZE = 128;
static int PACKET_SIZE = MTU_SIZE - 3;

// --- Callback для подключений (ПРАВИЛЬНЫЙ ДЛЯ v2.5.0) ---
class MyServerCallbacks : public NimBLEServerCallbacks {
    // В v2.5.0 onConnect: void onConnect(BLEServer* pServer)
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
        Serial.printf("Client connected: %s\n", connInfo.getAddress().toString().c_str());
        deviceConnected = true;
        Serial.println("Client connected!");
        NimBLEDevice::startAdvertising();
    }

    void onDisconnect(NimBLEServer* pServer,  NimBLEConnInfo& connInfo, int reason) override {
        Serial.printf("Client disconnected: %s, reason: %d\n", connInfo.getAddress().toString().c_str(), reason);
        deviceConnected = false;
        NimBLEDevice::startAdvertising();
    }

   void onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo) override {
        Serial.printf("BLE: MTU changed to %d\n", MTU);
        MTU_SIZE = MTU;
        PACKET_SIZE = MTU_SIZE - 3;
    }
};

class MyRxCallbacks : public NimBLECharacteristicCallbacks {
    // Правильная сигнатура для v2.5.0
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
        std::string rxValue = pCharacteristic->getValue();
        if (rxValue.length() > 0) {
            Serial.print("BLE -> FT85BD: ");
            for (int i = 0; i < rxValue.length(); i++) {
                FT85BD.write(rxValue[i]);
                Serial.print(rxValue[i], HEX);
                Serial.print(" ");
            }
            Serial.println();
        }
    }
};

// --- Функции модуля ---

void ble_bridge_init() {
    Serial.println("BLE Bridge Init...");
    
    NimBLEDevice::init("Flipsky_Display");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);

    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    NimBLEService *pService = pServer->createService(FT85BD_SERVICE_UUID);

    pCharacteristicVescTx = pService->createCharacteristic(
        FT85BD_CHARACTERISTIC_UUID_TX,
        NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::READ
    );

    pCharacteristicVescRx = pService->createCharacteristic(
        FT85BD_CHARACTERISTIC_UUID_RX,
        NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR
    );
    pCharacteristicVescRx->setCallbacks(new MyRxCallbacks());

    // В v2.5.0 метод start() у сервиса deprecated, сервис стартует автоматически
    // pService->start(); // Убираем или оставляем закомментированным
    
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(FT85BD_SERVICE_UUID);
    pAdvertising->start();

    Serial.println("BLE Bridge ready, waiting for connection...");
}

bool ble_bridge_is_connected() {
    return deviceConnected;
}

void ble_bridge_update() {
    static std::string vescBuffer;
    
    if (deviceConnected) {
        while (FT85BD.available()) {
            uint8_t byte = FT85BD.read();
            vescBuffer.push_back(byte);

            if (vescBuffer.length() >= PACKET_SIZE) {
                pCharacteristicVescTx->setValue(vescBuffer);
                pCharacteristicVescTx->notify();
                vescBuffer.clear();
                delay(5);
            }
        }

        if (!vescBuffer.empty()) {
            pCharacteristicVescTx->setValue(vescBuffer);
            pCharacteristicVescTx->notify();
            vescBuffer.clear();
        }
    }

    // Обновление статуса подключения для UI
    bikeData.ble_connected = deviceConnected;

    if (!deviceConnected && oldDeviceConnected) {
        delay(500);
        pServer->startAdvertising();
        Serial.println("BLE: restart advertising");
        oldDeviceConnected = deviceConnected;
    }
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
}