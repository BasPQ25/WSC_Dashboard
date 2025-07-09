
#ifndef INC_TASKS_HEADERS_GPS_H_
#define INC_TASKS_HEADERS_GPS_H_

#define MINIMUM_SATELITE_NUMBER 7

void GPS_handler(void);
float nmeaToDecimal(float coordinate);
void gpsParse(char *strParse);
int gpsValidate(char *nmea);

#endif
