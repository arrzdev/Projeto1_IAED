#include "main.h"
#include "errors.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>


int main(){
	database db = {0};

	/* set initial args */
	db.date.day = 1;
	db.date.month = 1;
	db.date.year = 2022;

	/* set up counters */
	db.n_airports = 0;
	db.n_flights = 0;

	/* execute till 'q' command is inserted */  
	while (handle_command(&db)){
	}

	return 0;
}

int handle_command(database *db){
	char c = getchar();	
	switch (c){
		case 'q':
			return 0;  
		
		case 'a':
			handle_add_airport_command(db);
			return 1;
			
		case 'l':
			handle_list_airports_command(db);
			return 1;
		
		case 'v':
			handle_add_list_fligth_command(db);
			return 1;

		case 'p':
			handle_list_origin_command(db);
			return 1;

		case 'c':
			handle_list_destination_command(db);
			return 1;

		case 't':
			handle_date_forward(db);
			return 1;
		
		default:
			return 1;
	}
}

void handle_add_airport_command(database *db){
	int i, j;

	/* create new airport */
	airport NewAirport;

	/* get things */
	scanf("%s %s %[^\n]", NewAirport.id, NewAirport.country, NewAirport.city); 

	/* invalid airport ID */
	for (i=0; i < (int)strlen(NewAirport.id); i++){
		if (!isupper(NewAirport.id[i])){
			printf(INVALID_AIRPORT_ID);
			return;
		}
	}
	
	/* to many airports */
	if (db->n_airports == MAX_AIRPORTS){
		printf(TOO_MANY_AIRPORTS);
		return;
	}

	/* duplicate airport*/
	for (i=0; i < db->n_airports; i++){
		if (!strcmp(db->airports[i].id, NewAirport.id)){
			printf(DUPLICATE_AIRPORTS);
			return;	
		}
	}
	
	/* success */
	/* find the insertion index */
	j = db->n_airports-1;
	
	while (j>=0 && strcmp(db->airports[j].id, NewAirport.id) > 0){
		db->airports[j+1] = db->airports[j];
		j--;
	}
	/* insert it */
	db->airports[j+1] = NewAirport;

	db->n_airports++;
	
	printf(ADDED_AIRPORT, NewAirport.id);
	return;

	/* get city usinhg custom method*/
}

void handle_list_airports_command(database *db){
	int c, i, sep, found, n_origin_flights;
	char id[MAX_AIRPORT_ID_SIZE+1];

	/* no arguments */
	if ((c = getchar()) == '\n'){
		for (i=0; i < db->n_airports; i++){
			airport iteration_airport = db->airports[i];

			n_origin_flights = count_flights(db, id);
					
			printf(LIST_AIRPORT, iteration_airport.id, iteration_airport.city, iteration_airport.country, n_origin_flights);
		}
	}
	else {
		do{
			found = 0;
			scanf("%s", id);

			for (i=0; i < db->n_airports; i++){
				airport iteration_airport = db->airports[i];
				
				if (!strcmp(iteration_airport.id, id)){
					n_origin_flights = count_flights(db, id);
					
					printf(LIST_AIRPORT, iteration_airport.id, iteration_airport.city, iteration_airport.country, n_origin_flights);
					
					found = 1;
					break;
				}
			}

			if (!found)
				printf(NO_SUCH_AIRPORT, id);

			sep = getchar();
		} while(sep != '\n');		
	}
}

void handle_add_list_fligth_command(database *db){

	int c, i;

	/* no arguments */
	if ((c = getchar()) == '\n'){
		for (i=0; i < db->n_flights; i++){
			flight iteration_flight = db->flights[i];

			printf(LIST_FLIGHT, iteration_flight.id, iteration_flight.origin_id, iteration_flight.destination_id, iteration_flight.date.day, iteration_flight.date.month, iteration_flight.date.year, iteration_flight.time.hours, iteration_flight.time.minutes);
		}
	}
	else {
		/* create new flight*/
		flight NewFlight;

		scanf("%s %s %s %02d-%02d-%d %02d:%02d %02d:%02d %02d", NewFlight.id, NewFlight.origin_id, NewFlight.destination_id, &NewFlight.date.day, &NewFlight.date.month, &NewFlight.date.year, &NewFlight.time.hours, &NewFlight.time.minutes, &NewFlight.duration.hours, &NewFlight.duration.minutes, &NewFlight.capacity);

		/* invalid flight code*/
		for (i=0; i < (int)strlen(NewFlight.id); i++){
			if (i == 0 || i == 1){
				if (!isupper(NewFlight.id[i])){
					printf(INVALID_FLIGHT_CODE);
					return;
				}
			}
			else if (!isdigit(NewFlight.id[i])){
				printf(INVALID_FLIGHT_CODE);
				return;
			}
		}

		/* flight already exists */
		for (i=0; i<db->n_flights; i++){
			flight iteration_flight = db->flights[i];

			if (!strcmp(iteration_flight.id, NewFlight.id) && iteration_flight.date.day == NewFlight.date.day && iteration_flight.date.month == NewFlight.date.month && iteration_flight.date.year == NewFlight.date.year){
				printf(FLIGHT_ALREADY_EXISTS);
				return;
			}
		}

		/* no such airport ID*/
		if (!found_airport_id(db, NewFlight.origin_id)){
			printf(NO_SUCH_AIRPORT, NewFlight.origin_id);
			return;
		}
		
		else if (!found_airport_id(db, NewFlight.destination_id)){
			printf(NO_SUCH_AIRPORT, NewFlight.destination_id);
			return;
		}

		/* too many flights */
		if (db->n_flights == MAX_FLIGHTS){
			printf(TOO_MANY_FLIGHTS);
			return;
		}

		/* invalid date */
		if (!valid_date(db->date, NewFlight.date)){
			printf(INVALID_DATE);
			return;
		}

		/* invalid duration */
		if (NewFlight.duration.hours > 12 || (NewFlight.duration.hours == 12 && NewFlight.duration.minutes > 0)){
			printf(INVALID_DURATION);
			return;
		}

		/* invalid capacity */
		if (NewFlight.capacity < 10 || NewFlight.capacity > 100){
			printf(INVALID_CAPACITY);
			return;
		}

		/* success*/
		db->flights[db->n_flights] = NewFlight;
		db->n_flights++;
	}
}

void handle_list_origin_command(database *db){
	int i, size, found_flights;
	char airport_id[MAX_AIRPORT_ID_SIZE+1];
	flight sorted_flights[MAX_FLIGHTS];
	
	/* wut ?*/
	size = db->n_flights;
	found_flights = 0;
	
	scanf("%s", airport_id);
	
	/* check for id */
	if (!found_airport_id(db ,airport_id)){
		printf(NO_SUCH_AIRPORT, airport_id);
		return;
	}
	
	/* create an array with the flights that we want */
  for (i = 0; i < size; i++){    
    flight iteration_flight = db->flights[i];
    
    if (!strcmp(iteration_flight.origin_id, airport_id)){
      sorted_flights[found_flights] = iteration_flight;
      found_flights++;
		}
  }
	
	/* sort them */
	sort_flights(sorted_flights, found_flights);
	
	/* print them */
	for (i = 0; i < found_flights; i++){
		flight iteration_flight = sorted_flights[i];
		
		printf(LIST_FLIGHT_AIRPORT, iteration_flight.id, iteration_flight.destination_id, iteration_flight.date.day, iteration_flight.date.month, iteration_flight.date.year, iteration_flight.time.hours, iteration_flight.time.minutes);
	}
}

void handle_list_destination_command(database *db){
	int i, size, found_flights;
	char airport_id[MAX_AIRPORT_ID_SIZE+1];
	flight sorted_flights[MAX_FLIGHTS];
	
	/* wut ?*/
	size = db->n_flights;
	found_flights = 0;
	
	scanf("%s", airport_id);
	
	/* check for id */
	if (!found_airport_id(db ,airport_id)){
		printf(NO_SUCH_AIRPORT, airport_id);
		return;
	}
	
	/* create an array with the flights that we want */
  for (i = 0; i < size; i++){    
    flight iteration_flight = db->flights[i];
    
    if (!strcmp(iteration_flight.destination_id, airport_id)){
      /* update date and time */
      flight updated_flight = get_destination_info(iteration_flight);
    
      sorted_flights[found_flights] = updated_flight;
      found_flights++;
		}
  }
  
	/* sort them */
	sort_flights(sorted_flights, found_flights);
	
	
	/* print them */
	for (i = 0; i < found_flights; i++){
		flight iteration_flight = sorted_flights[i];
		
		printf(LIST_FLIGHT_AIRPORT, iteration_flight.id, iteration_flight.origin_id, iteration_flight.date.day, iteration_flight.date.month, iteration_flight.date.year, iteration_flight.time.hours, iteration_flight.time.minutes);
	}
}

void handle_date_forward(database *db){
	date NewDate;
	
	scanf("%02d-%02d-%d", &NewDate.day, &NewDate.month, &NewDate.year);
	
	/* invalid date */
	if (!valid_date(db->date, NewDate)){
		printf(INVALID_DATE);
		return;
	}
	
	db->date = NewDate;
	printf(PRINT_DATE, NewDate.day, NewDate.month, NewDate.year);
}


int get_month_days(int month){
	if (month <= 7){
		if (month == 2)
			return 28;
		else if (month % 2 == 0)
			return 30;
		
		return 31;
	}
	else {
		if (month % 2 == 0)
			return 31;
		else
			return 30;
	}
}

int date_to_days(date date){
	int i, days;
	
	days = date.day + (date.year - 1) * 365;

	/* add the days that passed in the current year */
	for (i=1; i<date.month; i++){
		days += get_month_days(i);
	}
	
	return days;
}

double date_time_to_days(date date, time time){
	double days;
	
	days = date_to_days(date);
	
	/* add the time converted to days */
	days += (double)(time.hours + ((double)time.minutes / 60)) / 24;
	
	return days;
}

int valid_date(date db_date, date new_date){
	/* days from year 0*/
	int db_days = date_to_days(db_date);
	int new_date_days = date_to_days(new_date);

	/* future +1 year */
	if (new_date_days - db_days > 365)
		return 0;
	
	/* in the past*/
	else if (db_days > new_date_days)
		return 0;

	return 1;
}

int count_flights(database *db, char id[MAX_AIRPORT_ID_SIZE+1]){
	int i, counter = 0;
	
	for (i=0; i<db->n_flights; i++){
		if (!strcmp(db->flights[i].origin_id, id))
			counter++;
	}
	
	return counter;
}

int found_airport_id(database *db, char id[MAX_AIRPORT_ID_SIZE+1]){
	int i;

	for (i=0; i<db->n_airports; i++){
			airport iteration_airport = db->airports[i];

			if (!strcmp(iteration_airport.id, id))
				return 1;
	}
	
	return 0;
}

void sort_flights(flight arr[], int size){
  int i, j;

	/* sort the copied array */
	for (i=1; i<size; i++){
		flight key = arr[i];
		j = i - 1;		
		
		while (j>=0 && (date_time_to_days(arr[j].date, arr[j].time) > date_time_to_days(key.date, key.time))){
			arr[j+1] = arr[j];
			j--;
		}
			
		arr[j+1] = key;
	}
}

flight get_destination_info(flight Flight){
	int max_month_days;
	
	/* add minutes */
	Flight.time.minutes += Flight.duration.minutes;
	if (Flight.time.minutes >= 60){
		Flight.time.minutes -= 60;
		Flight.time.hours += 1;
	}
	
	/* add hours */
	Flight.time.hours += Flight.duration.hours;
	if (Flight.time.hours >= 24){
		Flight.time.hours -= 24;
		Flight.date.day += 1;
	}
	
	max_month_days = get_month_days(Flight.date.month);
	if (Flight.date.day > max_month_days){
		Flight.date.day -= max_month_days;
		Flight.date.month += 1;
	}
	
	if (Flight.date.month > 12){
		Flight.date.month -= 12;
		Flight.date.year += 1;
	}
	
	return Flight;
	
}
