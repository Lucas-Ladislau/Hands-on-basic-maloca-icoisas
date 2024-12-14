#include <Wire.h>               // Inclui a biblioteca Wire para comunicação I2C
#include <Adafruit_PN532.h>     // Inclui a biblioteca Adafruit para o PN532

// Definição dos pinos I2C no ESP32
#define SDA_PIN 21              // Define o pino 21 do ESP32 como SDA
#define SCL_PIN 22              // Define o pino 22 do ESP32 como SCL
#define LED_SUCESS 18

// Inicializa o objeto para o módulo NFC PN532 usando I2C
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup(void) {
  Serial.begin(115200);         // Inicia a comunicação serial a 115200 bps
  pinMode(LED_SUCESS, OUTPUT);
  Serial.println("Inicializando sistema de controle de acessos...");

  // Inicializa a comunicação I2C com o módulo NFC
  nfc.begin();

  // Obtém a versão do firmware do PN532 para garantir que está funcionando corretamente
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Não foi possível encontrar o leitor NFC.");
    while (1); // Trava o código se o leitor não for encontrado
  }

  
  Serial.print("Firmware do leitor NFC: 0x");
  Serial.println(versiondata, HEX); // Exibe a versão do firmware em hexadecimal

  // Configura o leitor NFC para modo passivo (para ler tags RFID/NFC)
  nfc.SAMConfig(); // Necessário para inicializar o modo de leitura passiva

  Serial.println("Monitorando acessos...");
}

void loop(void) {
  uint8_t success;         
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; 
  uint8_t uidLength;             // Variável para armazenar o comprimento do UID

  // Tenta detectar uma tag NFC próxima ao leitor
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    digitalWrite(LED_SUCESS, HIGH);
    // Se uma tag for detectada, exibe o UID da tag no monitor serial
    Serial.println("Tag NFC detectada!");
    Serial.print("UID da tag: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x"); Serial.print(uid[i], HEX); // Exibe o UID da tag em hexadecimal
    }
    Serial.println();
    delay(1000);  // Pequena pausa antes de continuar a leitura
    digitalWrite(LED_SUCESS, LOW);
  }
}
