# License Plate Recognition and Automatic Access Control System

## English

### Project Overview
This project aims to develop an automatic vehicle access control system using an ESP32-Wrover. By integrating hardware and software, it leverages embedded processing capabilities with advanced software technologies like Optical Character Recognition (OCR). The system captures license plate images, recognizes text in real time, and automates access control using a servo motor as a barrier. Additionally, it sends notifications through a Telegram bot and stores images on a local server.

### Problem Statement
Current access control systems face several challenges:
- **Human intervention:** Many systems still require manual supervision.
- **High costs:** Commercial license plate recognition systems are expensive.
- **Lack of instant notifications and record-keeping:** Many systems do not provide real-time alerts or evidence tracking.

### Proposed Solution
This project provides an automated and cost-effective access control system:
- **Automated access:** Uses an ESP32-Wrover camera and a servo motor to control vehicle entry.
- **Real-time OCR recognition:** Utilizes Tesseract OCR to extract license plate numbers.
- **Instant notifications:** Sends alerts via Telegram with images and detected license plates.
- **Local server storage:** Captures and stores images for monitoring and security purposes.

### System Components
#### Hardware:
- ESP32-Wrover with integrated camera
- Servo motor
- H-M and M-M cables
- Protoboard and extension board (optional for better organization)

#### Software:
- **Arduino IDE:** To program the ESP32-Wrover.
- **Telegram API:** For real-time notifications.
- **Python (Flask, OpenCV, NumPy, Pytesseract):** To process images and manage the server.

### Installation and Setup
1. **ESP32-Wrover Setup:**
   - Flash the firmware using Arduino IDE.
   - Connect the ESP32 to a WiFi network.

2. **Server Installation:**
   - Install dependencies:
     ```bash
     pip install flask opencv-python numpy pytesseract
     ```
   - Configure Tesseract OCR.
   - Run the server script `server.py`.

3. **Telegram Bot Configuration:**
   - Obtain a bot token via BotFather.
   - Run `script.py` to retrieve the chat ID.

4. **System Execution:**
   - Start `server.py` and `script.py`.
   - The ESP32 will capture images and send them to the server for processing.

### Future Improvements
- Enhanced OCR accuracy.
- Higher image resolution for better recognition.
- Integration of an authorized license plate database.
- Expansion to recognize other types of information (e.g., QR codes, barcodes, facial recognition).

---

## Usage example:

| Screenshot 1 | Screenshot 2 | 
|--------------|--------------|
| ![Image 1](/img/montaje.png) | ![Image 2](/img/telegram.jpeg) |

- **Serial monitor**:
  ![Serial monitor](/img/serial.png)
---

## Español

### Resumen del Proyecto
Este proyecto desarrolla un sistema automático de control de acceso vehicular utilizando un ESP32-Wrover. Se combina hardware y software para procesar matrículas en tiempo real mediante OCR, automatizar el acceso con un servomotor y enviar notificaciones a través de Telegram. Además, almacena las imágenes capturadas en un servidor local para su monitoreo.

### Problema Detectado
Los sistemas actuales presentan varios problemas:
- **Intervención humana:** Se requiere supervisión manual.
- **Altos costos:** Los sistemas comerciales de reconocimiento de matrículas son costosos.
- **Falta de notificación y registro:** No todos los sistemas ofrecen alertas en tiempo real ni almacenamiento de evidencias.

### Solución Propuesta
Este proyecto ofrece una alternativa automatizada y económica:
- **Automatización del acceso:** Uso de ESP32-Wrover y servomotor.
- **Reconocimiento de matrículas en tiempo real:** Implementación de Tesseract OCR.
- **Notificaciones instantáneas:** Envío de alertas vía Telegram.
- **Almacenamiento en servidor local:** Registro de imágenes capturadas.

### Componentes del Sistema
#### Hardware:
- ESP32-Wrover con cámara integrada
- Servomotor
- Cables H-M y M-M
- Protoboard y extension board (opcional para mejor organización)

#### Software:
- **Arduino IDE:** Para programar el ESP32-Wrover.
- **API de Telegram:** Para notificaciones en tiempo real.
- **Python (Flask, OpenCV, NumPy, Pytesseract):** Para procesamiento de imágenes y gestión del servidor.

### Instalación y Configuración
1. **Configuración del ESP32-Wrover:**
   - Cargar firmware en Arduino IDE.
   - Conectar el ESP32 a una red WiFi.

2. **Instalación del Servidor:**
   - Instalar dependencias:
     ```bash
     pip install flask opencv-python numpy pytesseract
     ```
   - Configurar Tesseract OCR.
   - Ejecutar `server.py`.

3. **Configuración del Bot de Telegram:**
   - Obtener el token del bot con BotFather.
   - Ejecutar `script.py` para obtener el chat ID.

4. **Ejecución del Sistema:**
   - Iniciar `server.py` y `script.py`.
   - El ESP32 capturará imágenes y las enviará al servidor para su procesamiento.

### Mejoras Futuras
- Mayor precisión en el OCR.
- Mejor resolución de imagen.
- Base de datos de matrículas autorizadas.
- Expansión para reconocer otros tipos de información (códigos QR, códigos de barras, reconocimiento facial).



### Autores:
- Blanca Girón
- Raul Martínez