#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

WiFiClient client;

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);

  int wifiStatus = WL_IDLE_STATUS;

  // Determine if the WiFi Shield is present
  Serial.print("\n\nShield:");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("FAIL");

    // If there's no WiFi shield, stop here
    while(true);
  }

  Serial.println("OK");

  // Try to connect to the local WiFi network
  while(wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi:");
    wifiStatus = WiFi.begin(WIFI_SSID);
    if (wifiStatus == WL_CONNECTED) {
      Serial.println("OK");
    } else {
      Serial.println("FAIL");
    }
    delay(5000);
  }

  Serial.println("Setup complete.\n");
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running Get - Run #" + String(numRuns++));

    TembooChoreo GetChoreo(client);

    // Invoke the Temboo client
    GetChoreo.begin();

    // Set Temboo account credentials
    GetChoreo.setAccountName(TEMBOO_ACCOUNT);
    GetChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    GetChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set profile to use for execution
    GetChoreo.setProfile("HA1");

    // Identify the Choreo to run
    GetChoreo.setChoreo("/Library/Utilities/HTTP/Get");

    // Run the Choreo; when results are available, print them to serial
    GetChoreo.run();

    while(GetChoreo.available()) {
      char c = GetChoreo.read();
      Serial.print(c);
    }
    GetChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between Get calls
}