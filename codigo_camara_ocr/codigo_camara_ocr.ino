#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <ESP32Servo.h>

// ===================
// Selección del modelo de cámara
// ===================
#define CAMERA_MODEL_WROVER_KIT

#include "camera_pins.h"

// Configuración WiFi
const char* ssid = "nombreWifi";                     // Nombre de la red WiFi
const char* password = "*********";                       // Contraseña de la red
const char* serverUrl = "http://192.168.18.212:5001/upload";  // URL del servidor Flask

camera_config_t config;
// Inicializar cámara
void camera_init();

// Pin del servo motor e inicialización del objeto
static const int servoPin = 13;
Servo servo1;

void setup() {
  Wire.begin(33, 32);
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  servo1.attach(servoPin);

  // Inicializar la cámara
  camera_init();
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Opciones adicionales de configuración
  sensor_t* s = esp_camera_sensor_get();
  s->set_vflip(s, 0);        // Voltear vertical: 1=Sí, 0=No
  s->set_hmirror(s, 0);      // Espejo horizontal: 1=Sí, 0=No
  s->set_brightness(s, 1);   // Ajustar brillo (entre -2 y 2)
  s->set_saturation(s, -1);  // Ajustar saturación (entre -2 y 2)

  // Conectar al WiFi
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  Serial.println(String("Connecting to ") + ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
}

void loop() {
  // Capturar una imagen
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Error capturando imagen");
    return;
  }

  // Enviar imagen al servidor Flask
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "image/jpeg");

  int httpResponseCode = http.POST(fb->buf, fb->len);  // Enviar imagen
  Serial.printf("Respuesta del servidor: %d\n", httpResponseCode);
  if (httpResponseCode > 0) {
    // Leer respuesta del servidor
    String response = http.getString();
    Serial.printf("Respuesta del servidor: %s\n", response.c_str());

     // Limpiar la respuesta para eliminar saltos de línea y espacios extra
    response.replace("\n", ""); // Quitar saltos de línea
    response.replace(" ", "");  // Quitar espacios

    // Buscar el campo "matricula" en la respuesta
    int startIndex = response.indexOf("\"matricula\":\""); // Buscar el inicio del campo
    if (startIndex != -1) {
      startIndex += 12; // Mover índice al comienzo del valor de la matrícula
      int endIndex = response.indexOf("\"", startIndex); // Buscar el cierre del valor
      if (endIndex != -1) {
        // Extraer matrícula detectada
        String matricula = response.substring(startIndex, endIndex);
        Serial.printf("Matrícula detectada: %s\n", matricula.c_str());

        // Activar el servo motor
        Serial.println("Abriendo barrera...");
        for (int posDegrees = 0; posDegrees <= 90; posDegrees++) {
          servo1.write(posDegrees);
          delay(20);
        }

        delay(2000);  // Espera con la barrera levantada

        Serial.println("Cerrando barrera...");
        for (int posDegrees = 90; posDegrees >= 0; posDegrees--) {
          servo1.write(posDegrees);
          delay(20);
        }
      }
    } else {
      Serial.println("No se encontró matrícula en la respuesta.");
    }
  } else {
    Serial.printf("Error en la solicitud HTTP: %d\n", httpResponseCode);
  }

  // Liberar el buffer de la cámara
  esp_camera_fb_return(fb);
  http.end();

  delay(5000);  // Intervalo entre capturas (ajustable)
}

void camera_init() {
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;             // Frecuencia del XCLK
  config.pixel_format = PIXFORMAT_JPEG;       // Formato de imagen
  config.frame_size = FRAMESIZE_QVGA;         // Tamaño de imagen (ajustable)
  config.jpeg_quality = 12;                   // Calidad de imagen
  config.fb_count = 1;                        // Frame buffers
  config.fb_location = CAMERA_FB_IN_DRAM;     // Ubicación del frame buffer
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;  // Modo de captura
}
