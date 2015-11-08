#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

byte mac [] = { 0x02, 0xA2, 0xDA, 0x00, 0x43, 0xC9 };
byte ip [] = {128,235,8,156};
EthernetClient client;

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 1;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  // For debugging, wait until the serial console is connected
  IPAddress(ip);
  Serial.print("ip initialized \n");
  delay(4000);
  while(!Serial); {

    Serial.print("DHCP:");
    Ethernet.begin(mac, ip);
    Serial.print("mac address initialized");
    /*  if (Ethernet.begin( mac ) == 0) {
        Serial.println("FAIL");  
    }
    else {
      Serial.print("not fail?");
    }  
    while(true);{
      Serial.print("OK \n");
    
    }   */
    delay(5000);
    Serial.print("Setup complete.\n");
  }
}

void loop() {
  Serial.print("loop");
 
  if (numRuns <= maxRuns) {
    Serial.println("Running SendEmail - Run #" + String(numRuns++));

    TembooChoreo SendEmailChoreo(client);

    // Invoke the Temboo client
    SendEmailChoreo.begin();

    // Set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String MessageBodyValue = "TEST";
    SendEmailChoreo.addInput("MessageBody", MessageBodyValue);
    String SubjectValue = "TESTING";
    SendEmailChoreo.addInput("Subject", SubjectValue);
    String PasswordValue = "fsldizsumttrgnmq";
    SendEmailChoreo.addInput("Password", PasswordValue);
    String UsernameValue = "aharmon112@gmail.com";
    SendEmailChoreo.addInput("Username", UsernameValue);
    String FromAddressValue = "\"Aaron\"<aharmon112@gmail.com>";
    SendEmailChoreo.addInput("FromAddress", FromAddressValue);
    String ToAddressValue = "8609666184@vtext.com";
    SendEmailChoreo.addInput("ToAddress", ToAddressValue);

    // Identify the Choreo to run
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

    // Run the Choreo; when results are available, print them to serial
    SendEmailChoreo.run();

    while(SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Serial.print(c);
    }
    SendEmailChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between SendEmail calls
 
}

