#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

byte ethernetMACAddress[] = ETHERNET_SHIELD_MAC;
EthernetClient client;

// The number of times to trigger the action if the condition is met
// We limit this so you won't use all of your Temboo calls while testing
int maxCalls = 10;

// The number of times this Choreo has been run so far in this sketch
int calls = 0;

int inputPin = A2;

void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);

  Serial.print("DHCP:");
  if (Ethernet.begin(ethernetMACAddress) == 0) {
    Serial.println("FAIL");
    while(true);
  }
  Serial.println("OK");
  delay(5000);

  // Initialize pins
  pinMode(inputPin, INPUT);

  Serial.println("Setup complete.\n");
}

void loop() {
  int sensorValue = analogRead(inputPin);
  Serial.println("Sensor: " + String(sensorValue));

  if (sensorValue > 300) {
    if (calls < maxCalls) {
      Serial.println("\nTriggered! Calling SendEmail Choreo...");
      runSendEmail(sensorValue);
      calls++;
    } else {
      Serial.println("\nTriggered! Skipping to save Temboo calls. Adjust maxCalls as required.");
    }
  }
  delay(250);
}

void runSendEmail(int sensorValue) {
  TembooChoreo SendEmailChoreo(client);

  // Set Temboo account credentials
  SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
  SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

  // Set Choreo inputs
  String MessageBodyValue = "Alert";
  SendEmailChoreo.addInput("MessageBody", MessageBodyValue);
  String SubjectValue = "Hack";
  SendEmailChoreo.addInput("Subject", SubjectValue);
  String PasswordValue = "yegdyamopcftmkrk";
  SendEmailChoreo.addInput("Password", PasswordValue);
  String UsernameValue = "eebquinn@gmail.com";
  SendEmailChoreo.addInput("Username", UsernameValue);
  String FromAddressValue = "eebquinn@gmail.com";
  SendEmailChoreo.addInput("FromAddress", FromAddressValue);
  String ToAddressValue = "9086166025@vtext.com";
  SendEmailChoreo.addInput("ToAddress", ToAddressValue);

  // Identify the Choreo to run
  SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

  // Run the Choreo
  unsigned int returnCode = SendEmailChoreo.run();

  // Read and print the error message
  while (SendEmailChoreo.available()) {
    char c = SendEmailChoreo.read();
    Serial.print(c);
  }
  Serial.println();
  SendEmailChoreo.close();
}
