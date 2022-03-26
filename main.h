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
  int hours;
  int minutes;
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
  int n_flights;
  airport airports[MAX_AIRPORTS];
  flight flights[MAX_FLIGHTS];
  date date;
} database;

/* prototypes */
void input(char string[], char separator);


int handle_command(database *system);

void handle_add_airport_command(database *system);
void handle_list_airports_command(database *system);
void handle_add_list_fligth_command(database *system);
void handle_list_origin_command(database *db);
void handle_list_destination_command(database *db);

double date_time_to_days(date date, time time);
int date_to_days(date date);
int valid_date(date db_date, date new_date);
int count_flights(database *system, char id[MAX_AIRPORT_ID_SIZE+1]);
int found_airport_id(database *system, char id[MAX_AIRPORT_ID_SIZE+1]);
void sort_flights(flight arr[], int size);
flight get_destination_info(flight Flight);
void handle_date_forward(database *db);

#endif