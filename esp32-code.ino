// Libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

// Constants
#define SS_PIN 5  // Pin para SDA/SS
#define RST_PIN 22 // Pin para RST
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

#define ON_Board_LED 2  // Pin para el LED integrado

// SSID y contraseña de tu red WiFi
const char* ssid = "MotoE32";
const char* password = "mateowifi01";
const char* endpoint = "http://192.168.140.200:8000/api/rfid-tags";

// Variables
int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

// Setup
void setup() {
    Serial.begin(115200); // Inicializa la comunicación serial
    SPI.begin();          // Inicia el bus SPI
    mfrc522.PCD_Init();   // Inicializa el lector MFRC522

    delay(500);

    WiFi.begin(ssid, password); // Conecta al router WiFi
    Serial.println("");

    pinMode(ON_Board_LED, OUTPUT);
    digitalWrite(ON_Board_LED, HIGH); // Apaga el LED al inicio

    // Espera a que se conecte a WiFi
    Serial.print("Conectando");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        digitalWrite(ON_Board_LED, LOW);
        delay(250);
        digitalWrite(ON_Board_LED, HIGH);
        delay(250);
    }
    digitalWrite(ON_Board_LED, HIGH); // Enciende el LED cuando esté conectado
    Serial.println("");
    Serial.print("Conectado a: ");
    Serial.println(ssid);
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());

    Serial.println("Por favor, acerque una tarjeta o llavero para ver el UID!");
    Serial.println("");
}

// Loop
void loop() {
    readsuccess = getid();
 
    if (readsuccess) {  
        digitalWrite(ON_Board_LED, LOW);
        HTTPClient http;  // Crea un objeto de clase HTTPClient
 
        String UIDresultSend, postData;
        UIDresultSend = StrUID;


        // Datos para enviar
        //postData = "{UIDresult:" + UIDresultSend;
        postData = "{\"UIDresult\": \"" + String(UIDresultSend) + "\"}";
        http.begin(endpoint);  // Especifica la URL de destino
        http.addHeader("Content-Type", "application/json"); // Establece el encabezado
        http.addHeader("Accept", "application/json");

        int httpCode = http.POST(postData); // Envía la solicitud
        String payload = http.getString();   // Obtiene la respuesta

        Serial.println(UIDresultSend);
        Serial.println(httpCode);   // Imprime el código de respuesta HTTP
        Serial.println(payload);     // Imprime el contenido de la respuesta

        http.end();  // Cierra la conexión
        delay(1000);
        digitalWrite(ON_Board_LED, HIGH);
    }
}

// Función para leer y obtener un UID de una tarjeta o llavero
int getid() {  
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return 0;
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
        return 0;
    }

    Serial.print("EL UID DE LA TARJETA ESCANEADA ES: ");
  
    for (int i = 0; i < 4; i++) {
        readcard[i] = mfrc522.uid.uidByte[i]; // Almacena el UID
        array_to_string(readcard, 4, str);
        StrUID = str;
    }
    mfrc522.PICC_HaltA();
    return 1;
}

// Función para convertir un array de UID en un string
void array_to_string(byte array[], unsigned int len, char buffer[]) {
    for (unsigned int i = 0; i < len; i++) {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
        buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
    }
    buffer[len * 2] = '\0';
}

