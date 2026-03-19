#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

// 🔹 WiFi Details
#define WIFI_SSID "its_rohit"
#define WIFI_PASSWORD "0909090909"

// 🔹 Gmail SMTP
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "rohitguthle56@gmail.com"
#define AUTHOR_PASSWORD "rqqg atwx lkbe repu"
#define RECIPIENT_EMAIL "rohitguthle63@gmail.com"

// 🔹 GPS Pins
SoftwareSerial gpsSerial(D7, D8); // RX, TX
TinyGPSPlus gps;

// 🔹 SMTP
SMTPSession smtp;

void setup()
{
  Serial.begin(115200);
  delay(2000);

  Serial.println("System Starting...");
  gpsSerial.begin(9600);

  // WiFi Connect
  Serial.print("Connecting WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi Connected!");
}

void loop()
{
  // Read GPS data
  while (gpsSerial.available())
  {
    gps.encode(gpsSerial.read());
  }

  // If GPS valid
  if (gps.location.isValid())
  {
    float lat = gps.location.lat();
    float lng = gps.location.lng();

    Serial.println("Location Found!");

    // 📧 Email Message
    String msg = "📍 GPS Location\n\n";
    msg += "Latitude: " + String(lat, 6) + "\n";
    msg += "Longitude: " + String(lng, 6) + "\n\n";
    msg += "Open in Google Maps:\n";
    msg += "https://www.google.com/maps?q=";
    msg += String(lat, 6) + "," + String(lng, 6);

    sendEmail(msg);

    delay(30000); // send every 30 sec
  }
  else
  {
    Serial.println("Searching GPS...");
    delay(2000);
  }
}

void sendEmail(String msg)
{
  SMTP_Message message;

  message.sender.name = "ESP8266 GPS Tracker";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "📍 Live Location";
  message.addRecipient("User", RECIPIENT_EMAIL);

  message.text.content = msg;

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;

  if (!smtp.connect(&session))
  {
    Serial.println("Email connection failed");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message))
  {
    Serial.println("Email sending failed");
  }
  else
  {
    Serial.println("Email Sent Successfully!");
  }
}