/*

*/

//Libaries
#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


char muster[4][6] ={ 
                  {'M','i','t','t','a','g'},
                  {'R','i','s','i','k','o'},
                  {'&','n','b','s','p',';'}
                 };

char temp_zahl[3];
int regenwahrscheinlickeit = 0;

int tag        = 1; // 0 = heute 1 = morgen
int nr_zeichen = 0;
int nr_muster  = 0;
int ausgabe    = 0;
int zaehler    = 0;


boolean muster_ende = false;

boolean debug_on = false;

IPAddress server(217,118,169,182); //wetter.com 

// Initialize the Ethernet client library
EthernetClient client;
Servo myservo;

// ---------------------- Hilfsprogramme ----------------
void suche_muster()
{
  //Hilffunktion zum füllen der Variable für die Regenwahrscheinlichkeit
   
 if (client.connect(server, 80)) {
    Serial.println("connected ");
    Serial.println(tag);
    // Make a HTTP request:
    switch(tag)
    {
      case 0:
        client.println("GET /wettervorhersage/49-2661-39/wetter-sankt-augustin/wetterbericht-aktuell.html");
      break;
      case 1:
        client.println("GET /wettervorhersage/49-2661-39/wetter-sankt-augustin/wetterbericht-morgen.html");
      break;
      default:
        client.println("GET /wettervorhersage/49-2661-39/wetter-sankt-augustin/wetterbericht-morgen.html");
      break;
    }
    
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }   
    while(client.available() && ausgabe < 7)
    {
      char c = client.read();
      if( c == muster[nr_muster][nr_zeichen]) // Muster zeichenweise vergleichen
      {
        if (nr_zeichen < 5 ) 
          {nr_zeichen++;}
        else // Muster gefunden
          {
            if (debug_on) {Serial.print("Muster gefunden ");Serial.print(nr_muster);}
            ausgabe++;
            nr_zeichen =0;
            nr_muster++;
                    
          }
      
      }
      else
      {
        nr_zeichen=0;
      }
      if (ausgabe == 2) 
      {
        if (debug_on) Serial.print(c);
        // Übernehme Zahlen in Variable                  
        switch (c) {
          case '1':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
          case '2':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
          case '3':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
          case '4':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
          case '5':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
          case '6':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
          case '7':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
          case '8':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
          case '9':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
          case '0':
            temp_zahl[zaehler] = c;
            zaehler++;
          break;
        } // end Case
        
      } // 
    } // end  while loop
    if (ausgabe==3)
    {
      
      ausgabe = 9;
      int regen = atoi(temp_zahl);
      myservo.write(map(regen,0,100,150,30));
    }
  }


// ---------------------- Setup ------------------------
void setup() {
  // start the serial library:
  Serial.begin(57600);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
  digitalWrite(5, HIGH);
  myservo.attach(9);
  myservo.write(90);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:

}

// ------------------ Hauptprogramm ---------------------------------------

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  tag = digitalRead(6);
  suche_muster(); 
  delay(15000);
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    //for(;;)
      ;
   
  }
}

