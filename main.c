#include "main.h"
#include "errors.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>


int main(){
	system_data system = {0};

	/* set initial args */
	system.date.day = 1;
	system.date.month = 1;
	system.date.year = 2022;

	/* set up counters */
	system.n_airports = 0;
	system.n_fligths = 0;

	/* execute till 'q' command is inserted */  
	while (handle_command(&system)){
	}

	return 0;
}

int handle_command(system_data *system){
	char c = getchar();	
	switch (c){
		case 'q':
			return 0;
		
		case 'a':
			handle_add_airport_command(system);
			return 1;
			
		case 'l':
			handle_list_airports_command(system);
			return 1;
		
		case 'v':
			handle_add_list_fligth_command(system);
			return 1;

		case 'p':
			/* handle_display_origin_id_flights(); */
			return 1;

		case 'c':
			/* handle_display_destination_id_flights(); */
			return 1;

		case 't':
			/* handle_date_forward(); */
			return 1;
		
		default:
			return 1;
	}
}

void handle_add_airport_command(system_data *system){
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
	if (system->n_airports == MAX_AIRPORTS){
		printf(TOO_MANY_AIRPORTS);
		return;
	}

	/* duplicate airport*/
	for (i=0; i < system->n_airports; i++){
		if (!strcmp(system->airports[i].id, NewAirport.id)){
			printf(DUPLICATE_AIRPORTS);
			return;	
		}
	}
	
	/* success */
	/* find the insertion index */
	j = system->n_airports-1;
	
	while (j>=0 && strcmp(system->airports[j].id, NewAirport.id) > 0){
		system->airports[j+1] = system->airports[j];
		j--;
	}
	/* insert it */
	system->airports[j+1] = NewAirport;

	system->n_airports++;
	
	printf(ADDED_AIRPORT, NewAirport.id);
	return;

	/* get city usinhg custom method*/
}

void handle_list_airports_command(system_data *system){
	int c, i, sep, found, n_origin_flights;
	char id[MAX_AIRPORT_ID_SIZE+1];

	/* no arguments */
	if ((c = getchar()) == '\n'){
		for (i=0; i < system->n_airports; i++){
			airport iteration_airport = system->airports[i];

			n_origin_flights = count_flights(system, id);
					
			printf(LIST_AIRPORT, iteration_airport.id, iteration_airport.city, iteration_airport.country, n_origin_flights);
		}
	}
	else {
		do{
			found = 0;
			scanf("%s", id);

			for (i=0; i < system->n_airports; i++){
				airport iteration_airport = system->airports[i];
				
				if (!strcmp(iteration_airport.id, id)){
					n_origin_flights = count_flights(system, id);
					
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

void handle_add_list_fligth_command(system_data *system){

	int c, i, found_origin_id=0, found_destination_id=0;

	/* no arguments */
	if ((c = getchar()) == '\n'){
		for (i=0; i < system->n_fligths; i++){
			flight iteration_flight = system->flights[i];

			printf(LIST_FLIGHT, iteration_flight.id, iteration_flight.origin_id, iteration_flight.destination_id, iteration_flight.date.day, iteration_flight.date.month, iteration_flight.date.year, iteration_flight.time.hour, iteration_flight.time.minute);
		}
	}
	else {
		/* create new flight*/
		flight NewFlight;

		scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d", NewFlight.id, NewFlight.origin_id, NewFlight.destination_id, &NewFlight.date.day, &NewFlight.date.month, &NewFlight.date.year, &NewFlight.time.hour, &NewFlight.time.minute, &NewFlight.duration.hour, &NewFlight.duration.minute, &NewFlight.capacity);

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
		for (i=0; i<system->n_fligths; i++){
			flight iteration_flight = system->flights[i];

			if (!strcmp(iteration_flight.id, NewFlight.id) && iteration_flight.date.day == NewFlight.date.day && iteration_flight.date.month == NewFlight.date.month && iteration_flight.date.year == NewFlight.date.year){
				printf(FLIGHT_ALREADY_EXISTS);
				return;
			}
		}

		/* no such airport ID*/
		for (i=0; i<system->n_airports; i++){
			airport iteration_airport = system->airports[i];

			if (!strcmp(iteration_airport.id, NewFlight.origin_id)){
				found_origin_id = 1;
				/*printf("%s-%s\n", iteration_airport.id, NewFlight.origin_id);*/
			}
			else if (!strcmp(iteration_airport.id, NewFlight.destination_id))
				found_destination_id = 1;
		}
		
		if (!found_origin_id){
			printf(NO_SUCH_AIRPORT, NewFlight.origin_id);
			return;
		}
		else if (!found_destination_id){
			printf(NO_SUCH_AIRPORT, NewFlight.destination_id);
			return;
		}

		/* too many flights */
		if (system->n_fligths == MAX_FLIGHTS){
			printf(TOO_MANY_FLIGHTS);
			return;
		}

		/* invalid date */
		if (!valid_date(system, NewFlight.date)){
			printf(INVALID_DATE);
			return;
		}

		/* invalid duration */
		if (NewFlight.duration.hour > 12 || (NewFlight.duration.hour == 12 && NewFlight.duration.minute > 0)){
			printf(INVALID_DURATION);
			return;
		}

		/* invalid capacity */
		if (NewFlight.capacity < 10 || NewFlight.capacity > 100){
			printf(INVALID_CAPACITY);
			return;
		}

		/* success*/
		system->flights[system->n_fligths] = NewFlight;
		system->n_fligths++;
	}
}


int valid_date(system_data *system, date new_date){
	/* days from year 0*/
	int system_days = (system->date.day * system->date.month) + (system->date.year - 1) * 365;
	int new_date_days = (new_date.day * new_date.month) + (new_date.year - 1) * 365;
	
	/* future +1 year */
	if (new_date_days - system_days > 365)
		return 0;
	
	/* in the past*/
	else if (new_date_days - system_days < 0)
		return 0;

	return 1;
}

void input(char string[], char separator){
	int i = 0;
	char c;

	while (isspace(c = getchar()));

	while (c != EOF && c != separator ){
		string[i++] = c;
		c = getchar();
	}
	
	/* end vector */
	string[i] = '\0';
}

int count_flights(system_data *system, char id[MAX_AIRPORT_ID_SIZE+1]){
	int i, counter = 0;
	
	for (i=0; i<system->n_fligths; i++){
		if (!strcmp(system->flights[i].origin_id, id))
			counter++;
	}
	
	return counter;
}