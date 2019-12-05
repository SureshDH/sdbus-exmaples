#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>

struct Student {
	int id;
	char name[1024];
	int marks[6];
};


struct Student s1[5] = {{514, "Dharavath Suresh", {90, 91, 92, 93, 94, 95}},
						{515, "Nirosha Nani", {85, 86, 87, 88, 89, 90}},
						{516, "Sabitha Reddy", {90, 91, 92, 93, 94, 95}},
						{521, "Veena Reddy", {85, 86, 87, 88, 89, 90}},
						{522, "Upendar Reddy", {90, 91, 92, 93, 94, 95}}
						};

/*
struct Student s1[5] = {{514, "Dharavath Suresh"},
						{515, "Nirosha Nani"},
						{516, "Sabitha Reddy"},
						{521, "Veena Reddy"},
						{522, "Upendar Reddy"}};


*/

int main(int argc, char *argv[]) {
        sd_bus_error error = SD_BUS_ERROR_NULL;
        sd_bus_message *m = NULL;
		sd_bus_message *reply = NULL;
        sd_bus *bus = NULL;
        char *sresp;
        // int sresp;
        int resp;
        int r, i, j;

        /* Connect to the system bus */
        r = sd_bus_open_system(&bus);
        if (r < 0) {
                fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
                goto finish;
        }

        /* Issue the method call and store the respons message in m */
        r = sd_bus_call_method(bus,
                               "net.poettering.Calculator",           /* service to contact */
                               "/net/poettering/Calculator",          /* object path */
                               "net.poettering.Calculator",   /* interface name */
                               "Multiply",                          /* method name */
                               &error,                               /* object to return error in */
                               &m,                                   /* return message on success */
                               "xx",                                 /* input signature */
                               2,                       /* first argument */
                               9);                           /* second argument */
        if (r < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n", error.message);
                goto finish;
        }

        /* Parse the response message */
        r = sd_bus_message_read(m, "x", &resp);
        if (r < 0) {
                fprintf(stderr, "Failed to parse response message: %s\n", strerror(-r));
                goto finish;
        }


        printf("Returned output is : %d.\n", resp);

        /* Issue the method call and store the respons message in m */
		/*
        r = sd_bus_call_method(bus,
                               "net.poettering.Calculator",
                               "/net/poettering/Calculator",
                               "net.poettering.Calculator",
                               "Astruct",                 
                               &error,                  
                               &m,                    
                               "a(is)",             
							   2,
							   514,
							   "Dharavath Suresh",
							   515,
							   "Mouse Keyboard"); 
		*/


		r = sd_bus_message_new_method_call(bus,
										   &m,
										   "net.poettering.Calculator",
										   "/net/poettering/Calculator",
										   "net.poettering.Calculator",
										   "Astructa");
		r = sd_bus_message_open_container(m, 'a', "(isai)"); /*Open outer array*/ // containers the type shoudl '' and in message "" 
		// r = sd_bus_message_open_container(m, 'a', "(is)"); /*Open outer array*/ // containers the type shoudl '' and in message "" 
		for (i = 0; i < 5; i++) {
			r = sd_bus_message_open_container(m, 'r', "isai"); /*Open structure element*/ // It should be 'r' not "a"
			// r = sd_bus_message_open_container(m, 'r', "is"); /*Open structure element*/ // It should be 'r' not "a"
			r = sd_bus_message_append(m, "i", s1[i].id);
			r = sd_bus_message_append(m, "s", s1[i].name);
			r = sd_bus_message_close_container(m);

			// Fill inner array
			r = sd_bus_message_open_container(m, 'a', "i"); /*Open structure element*/ // It should be 'a' not "a"
			for (j=0; j<6; j++) {
				r = sd_bus_message_append(m, "i", s1[i].marks[j]); // it should be "i" not 'i'
			}
			r = sd_bus_message_close_container(m);
		}
		r = sd_bus_message_close_container(m);
		r = sd_bus_call(bus, m, 0, &error, &reply);
		printf("Printing contents of the array(struct) container:\n");
		printf("=================================================\n");
		for(i=0; i<5; i++) {
			printf("\t%d -> id: %d\n", i, s1[i].id);
		
			for(j=0; j<6; j++) {
				printf("\t\t%d -> %d  ", j, s1[i].marks[j]);
			}
			
			printf("\n\t%d -> name: %s\n", i, s1[i].name);
		}
		printf("=================================================\n");

		printf("Finished function call\n");
        if (r < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n", error.message);
                goto finish;
        }

        /* Parse the response message */
		printf("Parsding the elements: \n");
        r = sd_bus_message_read(reply, "s", &sresp);
        if (r < 0) {
                fprintf(stderr, "Failed to parse response message: %s\n", strerror(-r));
                goto finish;
        }


        printf("Returned output is : %s.\n", sresp);
finish:
        sd_bus_error_free(&error);
        sd_bus_message_unref(m);
        sd_bus_unref(bus);

        return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
		/*
		r = sd_bus_message_open_container(m, 'a', "(iais)"); // containers the type shoudl '' and in message "" 
		for (i = 0; i < 5; i++) {
			r = sd_bus_message_open_container(m, 'r', "iais"); // It should be 'r' not "a"
			r = sd_bus_message_append(m, "i", 500+i);
			r = sd_bus_message_open_container(m, 'a', "i");  // It should be 'a' not "a"
			for (j=0; j<6; j++) {
				r = sd_bus_message_append(m, "i", 90+j); // it should be "i" not 'i'
			}
			r = sd_bus_message_close_container(m);
			r = sd_bus_message_append(m, "s", "Appeneded string");
			r = sd_bus_message_close_container(m);
		}
		r = sd_bus_message_close_container(m);
		r = sd_bus_call(bus, m, 0, &error, &reply);
		*/
}
