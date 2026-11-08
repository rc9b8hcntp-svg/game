#include <WiFi.h>
#include <WebServer.h>
#include "game.h"   // contient HTML_GAME (la page du jeu)

// --- 1. CONFIGURATION DES RESEAUX WI-FI ---
// Wi-Fi auquel le Pico se connecte (Mode STA - pour l'accès Internet/CDN)
const char* ssid_sta = "YOUR_SSID";
const char* password_sta = "YOUR_MDP";

// Wi-Fi hébergé par le Pico (Mode AP - pour que les joueurs s'y connectent)
const char* ssid_ap = "SSID";
const char* password_ap = "MDP"; // 8 caractères minimum

WebServer server(80);

// Envoie la page directement depuis la flash, par morceaux (évite de copier 300 Ko en RAM)
void handleRoot() {
  const size_t len = strlen(HTML_GAME);
  server.setContentLength(len);
  server.send(200, "text/html", "");
  const size_t CH = 1024;
  for (size_t i = 0; i < len; i += CH) {
    server.sendContent(HTML_GAME + i, min(CH, len - i));
  }
}

void handleNotFound() {
  // Redirige toute autre adresse vers le jeu
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Mode double : Station (STA) + Point d'accès (AP)
  WiFi.mode(WIFI_AP_STA);

  // Connexion au réseau Wi-Fi externe (STA)
  Serial.print("Connexion au Wi-Fi : ");
  Serial.println(ssid_sta);
  WiFi.begin(ssid_sta, password_sta);

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnecté au Wi-Fi STA ! IP : ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nÉchec de connexion STA (fonctionnera en mode hors ligne local).");
  }

  // Création du point d'accès Wi-Fi (AP) du Pico
  Serial.print("Création du point d'accès Wi-Fi : ");
  Serial.println(ssid_ap);
  bool apResult = WiFi.softAP(ssid_ap, password_ap);

  if (apResult) {
    Serial.println("Point d'accès créé avec succès !");
    Serial.print("Adresse IP du serveur Pico (AP) : ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Échec de création du point d'accès.");
  }

  // Routes du serveur web
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Serveur HTTP démarré sur le port 80.");
}

void loop() {
  server.handleClient();
}
