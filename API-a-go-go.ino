// Here we present a sketch that connects our ESP8266 processor to the internet via wifi. Once connected to the server, we can make HTTP requests to obtain 
//information through a myriad of APIs available online. For this specific example, I chose to get information from sites that do not require an API key.
//The following sketch first requests an obtains data regarding the International Space Station's current location. Next in a difference HTTP request 
//we obtain data regarding the time of sunset, sunrise, sun noon, and total daylight for today. It is important to note that both of these requests 
//return data in Json format. More specifically, the information we receive is Json text within Json (i.e. {key:{key:value}} ). Hence, we must parse the 
//data we receive twice. This is done for both requests and then reported in our serial monitor. 


#include <ESP8266WiFi.h> // Include the library for our ESP866 wifi microchip processor
#include <ESP8266HTTPClient.h> //Include library required to setup the HTTP server 
#include <ArduinoJson.h> //Include library that let's us parse and construct JSON objects 

const char* ssid = "STC2"; //Declare a constant and save our wifi name onto it (saves memory) 
const char* pass = "bahtsang";// Declare constant and save our wifi network password in it 

typedef struct { //Define new data type that we will hold all of the incoming information in strings
  String ln; //Save longitude of International Space Station (ISS)
  String lt; //Latitude of ISS 
  String ts; //Time stamp of reported location 
} ISSData; //Assign this data structure a name for later use 
 
ISSData theLocation; //Create an instance of the ISSData type with a new variable 


typedef struct { // Define new data type that we will hold all of the incoming information in strings
  String sr; ////Save sunrise info here 
  String ss; //Sunset
  String sn; //Solar noon 
  String dl; //Day length 
} DayData; //Assign this data structure a name for later use 

DayData today; //Create an instance of the DayData type with a new variable 
  

void setup() { //Call the setup function to start our sketch 
  Serial.begin(115200); //Set the desired data for serial communication with computer
  delay(10); //Pause for 10 milliseconds 
  Serial.print("Connecting to "); Serial.println(ssid); //Print this in serial monitor and lets us know that our processor is starting to connect to Wifi    
  WiFi.mode(WIFI_STA); //Initialize the actual connection to Wifi by calling Wifi.mode from the library
  WiFi.begin(ssid, pass); //Input wifi name and password 

  while (WiFi.status() != WL_CONNECTED) { // Loop to check our connection status during the connection process 
    delay(500); // Delay so that we check every 500 milliseconds 
    Serial.print("."); //Print dots until we are connected to the wifi network
  } // The loop will continue until our Wifi.status is not equal to WL_connected 

  Serial.println("WiFi connected");  //Confirm in our serial monitor that we have connected to Wifi 
  Serial.println(WiFi.localIP()); // Print the IP address assigned to our ESP 
  Serial.println(); //Skip a space 
  Serial.println("The current whereabouts of the International Space Station (ISS) is as follows: "); //Print this 
  
  getISSData(); //Call this function 
  Serial.println("The latitude of the ISS is " + theLocation.lt); //Print this text + latitude info saved in lt string
  Serial.println("The longitude of the ISS is " + theLocation.ln); //Print this text + longitude info saved in ln string 
  Serial.println("Time stamp of location: " + theLocation.ts); //Print this text + time stamp saved in ts string 
  Serial.println(); //Skip a line 
  Serial.println("But good luck catching it because its orbital speed is 17,100 mph!!"); // Fun fact of the day 
  Serial.println();// Skip a line 
  
  Serial.println("The following is the sunrise and sunset data for today."); //Print this 
  Serial.println(); //Skip a line 
  
  getDayData(); //Call getDayData function 
  Serial.println("Sunrise today is at: " + today.sr); //Print this text + sunrise time saved in sr string 
  Serial.println("Sunset today is at: " + today.ss); // Print this text + sunset time saved in ss string 
  Serial.println("The sun will be at its highest point today at: " + today.sn); //Print this text + sun noon time saved in sn string
  Serial.println("Total hours of daylight today is: " + today.dl); //Print this text + total daylight time saved in dl string 
}

void loop() {
}

String getISSData() { //Create a function to get the ISS location from the site 
  HTTPClient theClient; // Creating mini HTTP from the HTTP library 
  String ISSlocation; // String that stores data we get back from request 

  theClient.begin("http://api.open-notify.org/iss-now.json"); //Specify website address where we will make our request 
  int httpCode = theClient.GET(); //Make the http request 
  if (httpCode > 0) { //Check value of returned htttp value to see if successful or not 
    if (httpCode == 200) { // 200 is identified as the working code number for the request 
  }
  DynamicJsonBuffer jsonBuffer; // jsonBuffer can be static or dynamic, if we know how long the result will be we can use a static buffer to save memory
  String payload = theClient.getString();  // String that contains the json data 
  JsonObject& root = jsonBuffer.parse(payload); // Parse the json data 
  String parse1 = root["iss_position"].as<String>(); //
  JsonObject& loc = jsonBuffer.parse(parse1); // Parse again       

      if (!root.success()) {
       Serial.println("parseObject() failed"); // Print this to let us know we failed to parse 
       Serial.println(payload);
    }
    theLocation.lt = loc["latitude"].as<String>(); //Match key containing latitude value to that in the Json data 
    theLocation.ln = loc["longitude"].as<String>(); //Match ket containing longitude value to that in the Json data 
    theLocation.ts = root["timestamp"].as<String>(); //Match key containing timestamp value to that in the Json data
  } 
    else {
      Serial.println("Something went wrong with connecting to the endpoint in getISS()."); //Let us know we could not connect to website 
    }
  
    return ISSlocation; //Return our string that has the ISS location info 
  }

  String getDayData() { //Function that will get our sunrise and sunset info 
    HTTPClient theClient; // Creating mini HTTP from the HTTP library
    String allDay; // String that stores data we get back from request

    theClient.begin("http://api.sunrise-sunset.org/json?lat=-43.1399&lng=-23.2671"); //Specify site where we will make our request 
     int httpCode = theClient.GET(); //Make the request 
     if (httpCode > 0) { //Check value of returned htttp value to see if successful or not 
       if (httpCode == 200) { // 200 is identified as the working code number for the request 
  }
  DynamicJsonBuffer jsonBuffer;
  String payload2 = theClient.getString(); // String that contains the json data
  JsonObject& root = jsonBuffer.parse(payload2); // Parse the json data
  String parse1 = root["results"].as<String>(); //
  JsonObject& dayInfo = jsonBuffer.parse(parse1); // Parse again       

      if (!root.success()) { //Check to see if parsing was successful 
       Serial.println("parseObject() failed"); // Print this to let us know we failed to parse 
       Serial.println(payload2);
    }
    today.sr = dayInfo["sunrise"].as<String>(); //Match key containing sunrise value to that in the Json data 
    today.ss = dayInfo["sunset"].as<String>(); //Match key containing sunset value to that in the Json data
    today.sn = dayInfo["solar_noon"].as<String>(); //Match key containing solar noon value to that in the Json data
    today.dl= dayInfo["day_length"].as<String>(); //Match key containing total daylight value to that in the Json data
  }
  else{
    Serial.println("Something went wrong with connecting to the endpoint in getDayData(). ");
  }
  return allDay; //Return our string with sunrise and sunset info 
  
  }
  
