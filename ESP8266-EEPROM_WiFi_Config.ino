#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP_EEPROM.h>

// EEPROM
//  0-31 => ssid
// 32-63 => password
// 64-67 => IP Address
// 68-71 => DNS Address
// 72-75 => Gateway
// 76-79 => Subnet
byte dns[4]; byte gw[4]; byte sn[4]; byte ip[4];

ESP8266WebServer web_server(80);

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512); delay(10);
  EEPROM_readIpConfig();
  start_wifi();
//  web_server.on("/", handle_http_root);
  web_server.on("/config", handle_http_config);
  web_server.on("/wifi", handle_http_wifi);
  web_server.on("/wifisave", handle_http_wifisave);
  web_server.on("/ip", handle_http_ip);
  web_server.on("/ipsave", handle_http_ipsave);
  web_server.begin();
  delay(100);
}

void loop() {
  web_server.handleClient();
}

//void handle_http_root() {
//  web_server.send(200, "text/html", "<form method='get'><table><tr><td><label>Message?: </label></td><td><input name='message' ></td></tr><tr><td><input type='submit' value='Save'></td></tr></table></form></html>");
//  if (web_server.arg("message").length() >0){
//    message=web_server.arg("message");
//  }
//}

void handle_http_config() {
  web_server.send(200, "text/html", get_configPage());
} 
String get_configPage(){
  String htmlPage = "<h2>IP settings</h2>";
  htmlPage += "";
  htmlPage += "<table>";
  htmlPage += "<tr><td><label>DHCP: </label></td><td><input type='checkbox' name='dhcp' disabled ";
  if (byte(EEPROM.read(80))==1){htmlPage += "checked";}
  htmlPage += "></td></tr>";
  if (byte(EEPROM.read(80))==0){
    htmlPage += "<tr><td><label>IP Address: </label></td><td>" + String(ip[0]) +  " . " + String(ip[1]) +  " . " + String(ip[2]) +  " . " + String(ip[3]) +  "</td></tr>";
    htmlPage += "<tr><td><label>Gateway: </label></td><td>" + String(gw[0]) +  " . " + String(gw[1]) +  " . " + String(gw[2]) +  " . " + String(gw[3]) +  "</td></tr>";
    htmlPage += "<tr><td><label>DNS: </label></td><td>" + String(dns[0]) +  " . " + String(dns[1]) +  " . " + String(dns[2]) +  " . " + String(dns[3]) +  "</td></tr>";
    htmlPage += "<tr><td><label>Subnet: </label></td><td>" + String(sn[0]) +  " . " + String(sn[1]) +  " . " + String(sn[2]) +  " . " + String(sn[3]) +  "</td></tr>";
  }
  htmlPage += "</table><br/><hr>";
  htmlPage += "<h2>WiFi settings</h2>";
  htmlPage += "<table>";
  htmlPage += "<tr><td><label>ssid: </label></td><td>" + EEPROM_read(0,32) + "</td></tr>";
  htmlPage += "<tr><td><label>password: </label></td><td>******</td></tr>";
  htmlPage += "</table><br/><hr>";
  htmlPage += "<h2>Device settings</h2>";
  htmlPage += "<table>";
  htmlPage += "<tr><td><label>mac address: </label></td><td>" + WiFi.macAddress() + "</td></tr>";
  htmlPage += "<tr><td><label>name: </label></td><td> </td></tr>";
  htmlPage += "</table><br/><hr>"; 
  htmlPage += "</html>";
  return htmlPage;
}

void handle_http_wifi() {
  web_server.send(200, "text/html", get_wifiPage());
} 
String get_wifiPage(){
  String htmlPage = "Please enter Wlan settings<br/>";
  htmlPage += "<form method='get' action='wifisave'><table><tr><td><label>SSID: </label></td><td><input name='ssid' length=32></td></tr>";
  htmlPage += "<tr><td><label>Password: </label></td><td><input name='pass' length=64></td></tr><tr><td><input type='submit' value='Save'></td></tr></table></form>";
  htmlPage += "</html>";
  return htmlPage;
}

void handle_http_ip() {
  web_server.send(200, "text/html", get_ipPage());
}

String get_ipPage(){
  String htmlPage = "<h2>Please enter IP settings</h2><br/>";
  htmlPage += "<form method='get' action='ipsave'>";
  htmlPage += "<table>";
  htmlPage += "<tr><td><label>DHCP: </label></td><td><input type='checkbox' name='dhcp' ";
  if (byte(EEPROM.read(80))==1) { htmlPage += "checked";}
  htmlPage += "></td></tr>";
  htmlPage += "<tr><td><label>IP Address: </label></td><td><input name='ip1' value='" + String(ip[0]) +  "' size='3'> - <input name='ip2' value='" + String(ip[1]) +  "' size='3'> - <input name='ip3' value='" + String(ip[2]) +  "' size='3'> - <input name='ip4' value='" + String(ip[3]) +  "' size='3'></td></tr>";
  htmlPage += "<tr><td><label>Gateway: </label></td><td><input name='gw1' value='" + String(gw[0]) +  "' size='3'> - <input name='gw2' value='" + String(gw[1]) +  "' size='3'> - <input name='gw3' value='" + String(gw[2]) +  "' size='3'> - <input name='gw4' value='" + String(gw[3]) +  "' size='3'></td></tr>";
  htmlPage += "<tr><td><label>DNS: </label></td><td><input name='dns1' value='" + String(dns[0]) +  "' size='3'> - <input name='dns2' value='" + String(dns[1]) +  "' size='3'> - <input name='dns3' value='" + String(dns[2]) +  "' size='3'> - <input name='dns4' value='" + String(dns[3]) +  "' size='3'></td></tr>";
  htmlPage += "<tr><td><label>Subnet: </label></td><td><input name='sn1' value='" + String(sn[0]) +  "' size='3'> - <input name='sn2' value='" + String(sn[1]) +  "' size='3'> - <input name='sn3'  value='" + String(sn[2]) +  "'size='3'> - <input name='sn4' value='" + String(sn[3]) +  "' size='3'></td></tr>";
  htmlPage += "<tr><td><input type='submit' value='Save'></td></tr></table></form>";
  htmlPage += "</html>";
  return htmlPage;
}

void handle_http_wifisave() {
  String qsid = web_server.arg("ssid");
  String qpass = web_server.arg("pass");
  EEPROM_write(qsid, 0);
  EEPROM_write(qpass, 32);
  web_server.send(200, "application/json", "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}");
}

void handle_http_ipsave() {
  if (web_server.arg("dhcp")=="on") {
    EEPROM.write(80, 1);
  }
  else {
    EEPROM.write(80, 0);  
  }
  EEPROM.write(64, web_server.arg("ip1").toInt()); EEPROM.write(65, web_server.arg("ip2").toInt()); EEPROM.write(66, web_server.arg("ip3").toInt()); EEPROM.write(67, web_server.arg("ip4").toInt());
  EEPROM.write(68, web_server.arg("dns1").toInt()); EEPROM.write(69, web_server.arg("dns2").toInt()); EEPROM.write(70, web_server.arg("dns3").toInt()); EEPROM.write(71, web_server.arg("dns4").toInt());
  EEPROM.write(72, web_server.arg("gw1").toInt()); EEPROM.write(73, web_server.arg("gw2").toInt()); EEPROM.write(74, web_server.arg("gw3").toInt()); EEPROM.write(75, web_server.arg("gw4").toInt());
  EEPROM.write(76, web_server.arg("sn1").toInt()); EEPROM.write(77, web_server.arg("sn2").toInt()); EEPROM.write(78, web_server.arg("sn3").toInt()); EEPROM.write(79, web_server.arg("sn4").toInt());
  EEPROM.commit();
  web_server.send(200, "application/json", "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}");
}

void start_wifi() {
  // Connect to WiFi network
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  if (byte(EEPROM.read(80))==0) {
    WiFi.config(ip, dns, gw, sn);
  }
  WiFi.begin(EEPROM_read(0,32).c_str(), EEPROM_read(32,64).c_str());
  int trys = 0;
  while ( WiFi.status() != WL_CONNECTED && trys<= 5) {
    trys++;
    delay ( 1000 );
  }
  if(WiFi.isConnected()) {
    Serial.println("Link => " + EEPROM_read(0,32) + " " + WiFi.localIP().toString() + " " + WiFi.macAddress());
  }
  else {
    // Start AP Mode
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP("DotMtrx","");
    Serial.println("AP Mode ( DotMtrx ) " + WiFi.softAPIP().toString() + " " + WiFi.macAddress());
  }
}

void EEPROM_write(String buffer, int N) {
  for (int L = 0; L < 32; ++L) {
    EEPROM.write(N + L, buffer[L]);
  }
  EEPROM.commit();
}

String EEPROM_read(int min, int max) {
String buffer;
  for (int L = min; L < max; ++L)
    if (isAlphaNumeric(EEPROM.read(L)))
      buffer += char(EEPROM.read(L));
  return buffer;
}

void EEPROM_readIpConfig() {
// 64-67 => IP Address
// 68-71 => DNS Address
// 72-75 => Gateway
// 76-79 => Subnet
// 80 => DHCP
  for (int L = 0; L < 4; ++L) {
    ip[L] = byte(EEPROM.read(64 + L));
    dns[L] = byte(EEPROM.read(68 + L));
    gw[L] = byte(EEPROM.read(72 + L));
    sn[L] = byte(EEPROM.read(76 + L));
  }
}








