#include <esp_now.h>
#include <WiFi.h>

 int recibido;


void onReceiveData(const uint8_t *mac, const uint8_t *data, int len) {

  // Serial.print("Received from MAC: ");

  /*for (int i = 0; i < 6; i++) {

    // Serial.printf("%02X", mac[i]);
    //if (i < 5)Serial.print(":");
    }*/

  int * messagePointer = (int*)data;

 // Serial.println();
 // Serial.println(*messagePointer);
  recibido = *messagePointer;

}

void setup() {
  Serial.begin(115200);


  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }


}

void loop() {

  esp_now_register_recv_cb(onReceiveData);

  91
  if (recibido > 0){
  Serial.println(recibido);
  recibido = 0;
  }


}
