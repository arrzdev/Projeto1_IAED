#ifndef MAIN_H
#define MAIN_H

#define MAX_AIRPORT_ID_SIZE 3
#define MAX_COUNTRY_SIZE 30
#define MAX_CITY_SIZE 50
#define MAX_AIRPORTS 40
#define MAX_FLIGHTS 30000
#define MAX_FLIGHT_ID_SIZE 6


/* structures */
typedef struct{
  int day;
  int month;
  int year;
} date;

typedef struct{
  int hour;
  int minute;
} time;

typedef struct{
  char id[MAX_AIRPORT_ID_SIZE+1];
  char country[MAX_COUNTRY_SIZE+1];
  char city[MAX_CITY_SIZE+1];
} airport;

typedef struct{
  char id[MAX_FLIGHT_ID_SIZE+1];
  char destination_id[MAX_AIRPORT_ID_SIZE+1];
  char origin_id[MAX_AIRPORT_ID_SIZE+1];
  date date;
  time time;
  time duration;
  int capacity;
} flight;

/* 
Global structure
*/

typedef struct{
  int n_airports;
  int n_fligths;
  airport airports[MAX_AIRPORTS];
  flight flights[MAX_FLIGHTS];
  date date;
} system_data;

/* prototypes */
void input(char string[], char separator);


int handle_command(system_data *system);
void handle_add_airport_command(system_data *system);
void handle_list_airports_command(system_data *system);
void handle_add_list_fligth_command(system_data *system);
int valid_date(system_data *system, date);
int count_flights(system_data *system, char id[MAX_AIRPORT_ID_SIZE+1]);

#endif