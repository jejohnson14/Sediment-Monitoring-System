# Sediment-Monitoring-System
# Arduino Scripts
  MedianReading10M.ino and MedianReading5M.ino files are the scripts that are to be uploaded to arduino for the operation of the 10 or 5 meter ultrasonic sensor (whichever is used).
The scripts are easily uploaded using the Adruino GUI interface for verifying and compiling arduino codes.

# Packages 
Certain packages are required for the use of the MedianReading codes and are included as zipped folders.
    Package files: DS3232RTC-master.zip, Low-Power-master.zip and Time-master.zip
    
# Port/ COMM Drivers 
There are issues with PORTS/ COMMs while connecting computer to arduino, the following drivers can help solve those issues.
    Drivers: ActiveUSBCOM_E2104

# SDCardtest 
SDCardtest.ino is used to test if the SD card module is connected propoerly and able to save data.

# TimeRTCSet 
TimeRTCSet.ino is used to update the Date and Time of the RTC if it is inaccurate (use Unix Epoch Time).
