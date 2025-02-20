from flask import Flask, request, jsonify, send_from_directory
import cv2
import numpy as np
import os
from datetime import datetime
import pytesseract
import telegram
import logging
import asyncio

# Inicializa la app Flask
app = Flask(__name__)

pytesseract.pytesseract.tesseract_cmd = r'C:/Program Files/Tesseract-OCR/tesseract.exe'
# Configurar Telegram
TELEGRAM_TOKEN = '*****************' #Completar con tus datos
CHAT_ID = '*********'                #Completar con lo que te devuelve script.py
bot = telegram.Bot(token=TELEGRAM_TOKEN)

# Ruta donde se guardarán las imágenes
UPLOAD_FOLDER = 'uploads'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

# Dirección IP y puerto del servidor
SERVER_IP = "http://192.168.18.212" #Cambiar para poner tu ip
SERVER_PORT = 5001  # Puerto del servidor Flask

# Configurar ruta estática para acceder a los archivos
@app.route('/uploads/<filename>', methods=['GET'])
def serve_uploaded_file(filename):
    return send_from_directory(UPLOAD_FOLDER, filename)

@app.route('/upload', methods=['GET', 'POST'])
def upload_image():
    if request.method == 'GET':
        send_telegram_message("Servidor iniciado.")
        return "Este endpoint está configurado para recibir imágenes por POST.", 200
    elif request.method == 'POST':
        if 'file' not in request.files and not request.data:
            send_telegram_message("Error: No se recibió imagen.")
            return jsonify({"error": "No se recibió imagen"}), 400

        # Procesar imagen enviada como datos binarios
        if request.data:
            img_data = request.data
            filename = f"{datetime.now().strftime('%Y%m%d%H%M%S')}.jpg"
            filepath = os.path.join(UPLOAD_FOLDER, filename)

            # Decodifica los datos de la imagen
            np_img = np.frombuffer(img_data, np.uint8)
            img = cv2.imdecode(np_img, cv2.IMREAD_COLOR)

            # Guarda la imagen
            cv2.imwrite(filepath, img)

            # Construir la URL
            file_url = f"{SERVER_IP}:{SERVER_PORT}/uploads/{filename}"
            send_telegram_message(f"Imagen recibida y guardada: {file_url}")

            # Procesar la imagen con OCR
            license_plate = recognize_text(filepath)
            if license_plate:
                send_telegram_message(f"Texto detectado: {license_plate}")
                return jsonify({"message": "Texto detectado", "matricula": license_plate}), 200
            else:
                send_telegram_message("No se detectó texto válido.")
                return jsonify({"message": "No se detectó texto válido"}), 200

        # Procesar imagen enviada como archivo
        file = request.files['file']
        if file:
            filename = f"{datetime.now().strftime('%Y%m%d%H%M%S')}.jpg"
            filepath = os.path.join(UPLOAD_FOLDER, filename)
            file.save(filepath)

            # Construir la URL
            file_url = f"{SERVER_IP}:{SERVER_PORT}/uploads/{filename}"
            send_telegram_message(f"Imagen recibida y guardada: {file_url}")

            # Procesar la imagen con OCR
            license_plate = recognize_text(filepath)
            if license_plate:
                send_telegram_message(f"Texto detectado: {license_plate}")
                return jsonify({"message": "Texto detectado", "matricula": license_plate}), 200
            else:
                send_telegram_message("No se detectó texto válido.")
                return jsonify({"message": "No se detectó texto válido"}), 200

# Inicializa un único loop para el bot
loop = asyncio.get_event_loop()

def send_telegram_message(message):
    """
    Función para enviar un mensaje al chat de Telegram usando un loop persistente.
    """
    try:
        loop.run_until_complete(bot.send_message(chat_id=CHAT_ID, text=message))
    except Exception as e:
        logging.error(f"Error al enviar mensaje a Telegram: {e}")

def recognize_text(image_path):
    """
    Reconoce texto en una imagen usando OCR.
    """
    try:
        # Leer la imagen
        img = cv2.imread(image_path)

        # Convertir a escala de grises
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Opcional: Aplicar un filtro para mejorar el contraste
        gray = cv2.bilateralFilter(gray, 11, 17, 17)

        # Aplicar umbral para convertir a imagen binaria
        threshold_img = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)[1]

        # Pasar la imagen a través de pytesseract
        text = pytesseract.image_to_string(threshold_img)

        # Filtrar texto potencialmente válido como matrícula
        license_plate = ''.join(filter(str.isalnum, text)).upper()
        print(f"Texto detectado: {license_plate}")

        return license_plate if license_plate else None
    except Exception as e:
        print(f"Error en reconocimiento OCR: {e}")
        return None

# Ejecutar el servidor en modo debug
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=True)
