#include"main.h"

uint8_t rxBuffer[128] = {0};
uint8_t rxIndex = 0;
uint8_t rxData;
float nmeaLong;
float nmeaLat;
float utcTime;
char northsouth;
char eastwest;
char posStatus;
union reinterpret_cast decimalLong;
union reinterpret_cast decimalLat;
uint8_t satelliteNumber;
float speedKnots; // Viteza în noduri

float nmeaToDecimal(float coordinate) {
    int degree = (int)(coordinate/100);
    float minutes = coordinate - degree * 100;
    float decimalDegree = minutes / 60;
    float decimal = degree + decimalDegree;
    return decimal;
}

void gpsParse(char *strParse) {
  // Acceptă atât GPGGA cât și GNGGA
  if (strstr(strParse, "GGA")) {
    // $GPGGA / $GNGGA: timp, lat, N/S, lon, E/W, fix, nr sateliți
    int fixQuality;
    sscanf(strParse, "$%*[^,],%f,%f,%c,%f,%c,%d,%hhu",
           &utcTime, &nmeaLat, &northsouth, &nmeaLong, &eastwest,
           &fixQuality, &satelliteNumber);
    decimalLat.Float32 = nmeaToDecimal(nmeaLat);
    decimalLong.Float32 = nmeaToDecimal(nmeaLong);
  }
  // Acceptă GPRMC + GNRMC
  else if (strstr(strParse, "RMC")) {
    // $GPRMC / $GNRMC: timp, status, lat, N/S, lon, E/W, viteză
    sscanf(strParse, "$%*[^,],%f,%c,%f,%c,%f,%c,%f",
           &utcTime, &posStatus, &nmeaLat, &northsouth, &nmeaLong, &eastwest, &speedKnots);
    decimalLat.Float32 = nmeaToDecimal(nmeaLat);
    decimalLong.Float32 = nmeaToDecimal(nmeaLong);
  }
  // Acceptă GPGLL + GNGLL
  else if (strstr(strParse, "GLL")) {
    // $GPGLL / $GNGLL: lat, N/S, lon, E/W, timp
    sscanf(strParse, "$%*[^,],%f,%c,%f,%c,%f",
           &nmeaLat, &northsouth, &nmeaLong, &eastwest, &utcTime);
    decimalLat.Float32 = nmeaToDecimal(nmeaLat);
    decimalLong.Float32 = nmeaToDecimal(nmeaLong);
  }
}

int gpsValidate(char *nmea){
    char check[3];
    char calculatedString[3];
    int index;
    int calculatedCheck;

    index=0;
    calculatedCheck=0;

    // Ensure that the string starts with a "$"
    if(nmea[index] == '$')
        index++;
    else
        return 0;

    //No NULL reached, 75 char largest possible NMEA message, no '*' reached
    while((nmea[index] != 0) && (nmea[index] != '*') && (index < 75)){
        calculatedCheck ^= nmea[index];// calculate the checksum
        index++;
    }

    if(index >= 75){
        return 0;// the string is too long so return an error
    }

    if (nmea[index] == '*'){
        check[0] = nmea[index+1];    //put hex chars in check string
        check[1] = nmea[index+2];
        check[2] = 0;
    }
    else
        return 0;// no checksum separator found therefore invalid data

    sprintf(calculatedString,"%02X",calculatedCheck);
    return((calculatedString[0] == check[0])
        && (calculatedString[1] == check[1])) ? 1 : 0 ;
}
