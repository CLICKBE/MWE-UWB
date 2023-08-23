// Actuellement, le module est un tag


#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "link.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Largeur de l'affichage OLED en pixels
#define SCREEN_HEIGHT 64 // Hauteur de l'affichage OLED en pixels

#define OLED_RESET     -1 // Numéro de pin de réinitialisation (ou -1 si partageant la pin de réinitialisation Arduino)
#define SCREEN_ADDRESS 0x3D // Adresse de l'écran : 0x3D pour 128x64 et 0x3C pour 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct MyLink2 {
    uint16_t anchor_addr;
    float range;
    float dbm;
};

struct globalLink{
  struct MyLink2 anchor81;
  struct MyLink2 anchor82;
  struct MyLink2 anchor83;
};

#define OLED_RESET 4
#define SPI_SCK 5
#define SPI_MISO 19
#define SPI_MOSI 18
#define DW_CS 14

// Pins de connexion
const uint8_t PIN_RST = 32; // Pin de réinitialisation
const uint8_t PIN_IRQ = 33; // Pin d'interruption
const uint8_t PIN_SS = 14;  // Pin de sélection SPI

// Le délai de l'antenne du TAG est par défaut défini à 16384
// Les deux premiers octets ci-dessous deviendront l'"adresse courte"
char tag_addr[] = "7D:00:22:EA:82:60:3B:9C";

struct globalLink global;

QueueHandle_t xQueue;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Échec de l'allocation SSD1306"));
    for(;;); // Ne pas continuer, boucle indéfiniment
  }

  display.display();

  delay(1000);

  // Initialisation de la configuration
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); // Pins de Reset, CS, IRQ

  DW1000Ranging.attachNewRange(newRange);

  // Démarrage en tant que tag, sans attribuer d'adresse courte aléatoire
  DW1000Ranging.startAsTag(tag_addr, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
  
  delay(1000);
}
// Boucle pouvant être insérer dans un thread pour améliorer le système et diminuer les erreurs
/*
void loopDw1000(void* parameter) {
  while (1) {
    // Boucle dans un autre thread qui ne fait que récupérer les distances 
    DW1000Ranging.loop();
  }
}*/

void loop() {

  struct globalLink receivedLink;

     DW1000Ranging.loop();
}

void newRange() {
  global = fresh_link2(global, DW1000Ranging.getDistantDevice()->getShortAddress(), DW1000Ranging.getDistantDevice()->getRange(), DW1000Ranging.getDistantDevice()->getRXPower());
}

/*

void newDevice(DW1000Device *device) {
  Serial.print("Appareil ajouté : ");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device) {
  Serial.print("Suppression de l'appareil inactif : ");
  Serial.println(device->getShortAddress(), HEX);
}

*/