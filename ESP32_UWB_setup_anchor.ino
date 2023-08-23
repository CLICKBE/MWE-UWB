// Configuration de l'ancre #4

// Assurez-vous de modifier anchor_addr et de sélectionner le délai d'antenne précédemment calibré
// Ma convention de nommage est que les ancres 1, 2, 3, ... ont le byte de poids faible de l'adresse MAC défini sur 81, 82, 83, ...

#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000.h"
#include <Adafruit_SSD1306.h>

// Déclaration de l'écran OLED 
// Adafruit_SSD1306 display(4);

// Adresse de l'ancre, les deux premiers bytes déterminent l'adresse courte
char anchor_addr[] = "82:00:5B:D5:A9:9A:E2:9C";

// Délai d'antenne calibré pour cette ancre
uint16_t Adelay = 16575;

// Résultats de calibration précédemment déterminés pour le délai d'antenne
// #1 16630
// #2 16610
// #3 16607
// #4 16580

#define SPI_SCK 5
#define SPI_MISO 19
#define SPI_MOSI 18
#define DW_CS 14

// Pins de connexion
const uint8_t PIN_RST = 32;  // Pin de réinitialisation
const uint8_t PIN_IRQ = 33;  // Pin d'interruption
const uint8_t PIN_SS = 14;   // Pin de sélection SPI

void setup() {
  Serial.begin(115200);
  delay(1000); // Attend que le moniteur série soit connecté

  // Initialisation de l'affichage 
  // display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  // display.display();

  Serial.println("Configuration et démarrage de l'ancre");
  Serial.print("Délai d'antenne ");
  Serial.println(Adelay);

  // Initialisation de la configuration
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);

  // Configuration du délai d'antenne pour les ancres uniquement. Le tag est par défaut (16384)
  DW1000.setAntennaDelay(Adelay);

  // Attache les gestionnaires d'événements
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  // Démarre le module en tant qu'ancre, sans assigner une adresse courte aléatoire
  DW1000Ranging.startAsAnchor(anchor_addr, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
}

void loop() {
  DW1000Ranging.loop(); // Traite les opérations de localisation
}

void newRange() {
  // Affiche l'adresse courte de l'appareil distant et sa distance
  Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print(", ");
  Serial.println(DW1000Ranging.getDistantDevice()->getRange());

  /* Affichage sur l'écran OLED 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  display.println(DW1000Ranging.getDistantDevice()->getRange());
  display.display(); */
}

/*
void newDevice(DW1000Device *device) {
  // Affiche l'adresse du nouvel appareil ajouté
  Serial.print("Appareil ajouté : ");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device) {
  // Affiche l'adresse de l'appareil devenu inactif
  Serial.print("Suppression de l'appareil inactif : ");
  Serial.println(device->getShortAddress(), HEX);
}
*/
