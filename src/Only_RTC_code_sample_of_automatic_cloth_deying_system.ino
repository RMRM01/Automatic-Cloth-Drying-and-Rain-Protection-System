// Include the necessary libraries for RTC and I2C communication
#include <Wire.h>     // REQUIRED for I2C communication with hardware RTC (SDA/A4, SCL/A5 on Uno/Nano)
#include <RTClib.h> 


// this store the current second since January 1, 2000.
uint32_t startTimeEpoch; 

// Time need to dry in second
long timeNeedToDry=240;

// This global variable will hold the number of second , cloths are placed in sun
long staingAtSun=0;

// Initialize an RTC object for the hardware DS3231 module.
RTC_DS3231 rtc; 


void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("--- RTC Epoch Time Tracker Demo Initialized ---");

  // 1. Initialize the I2C bus and check for the RTC module
  if (!rtc.begin()) {
    Serial.println("FATAL: Couldn't find RTC. Check wiring (A4/SDA, A5/SCL).");
    while (1) delay(10); // Halt if the RTC is not found
  }

  // 2. Check if the RTC has lost power/time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time to the time of compilation!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  // --- CAPTURE EPOCH TIME IN SETUP() ---
  // Read the current time and store it as seconds since 2000.
  startTimeEpoch = rtc.now().unixtime();
  
  // Print the captured start time in both human-readable and epoch format
  Serial.print("Start Time (Human-Readable): ");
  Serial.println(rtc.now().timestamp(DateTime::TIMESTAMP_FULL));
  Serial.print("Start Time (Epoch Seconds): ");
  Serial.println(startTimeEpoch);
  
  Serial.print("Target: Wait for ");
  Serial.print(TARGET_SECONDS);
  Serial.println(" seconds to elapse...");
  Serial.println("----------------------------------------");
}

bool isDried(){
  if(staingAtSun >= timeNeedToDry){
    return 1;
  }
  else{
    return 0;
  }
}

void updateStaingAtSun(){
  // Read the current time and convert it to epoch seconds
  uint32_t currentTimeEpoch = rtc.now().unixtime();

  // Duration of drying 
  long durationSeconds = currentTimeEpoch - startTimeEpoch;
  
  // updating the staing time 
  staingAtSun += durationSeconds;

}
void loop() {
  // Read the current time and convert it to epoch seconds
  uint32_t currentTimeEpoch = rtc.now().unixtime();
  
  // Find the difference simply by subtracting the two epoch times.
  // The result is the elapsed duration in seconds.
  long durationSeconds = currentTimeEpoch - startTimeEpoch;
  
  // Print progress
  Serial.print("Elapsed Time: ");
  Serial.print(durationSeconds);
  Serial.print("/");
  Serial.print(TARGET_SECONDS);
  Serial.println(" seconds.");

  // Compare the difference to the target duration
  if (durationSeconds >= TARGET_SECONDS) {
    Serial.println("\n*** TARGET REACHED! REQUIRED TIME HAS PASSED ***");
    
    // In a real application, you would put your event logic (e.g., motor code) here.
    
    // Freeze the loop to prevent repeated messages
    while(true) {
      delay(10000); 
    }
  }

  // Only check the time every 1 second
  delay(1000); 
}