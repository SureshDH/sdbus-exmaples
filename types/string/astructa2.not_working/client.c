#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>

struct Book {
	int id;
};

struct Student {
	int marks[6];
	int id;
	char name[1024];
	struct Book b;
};
/*
struct Student s1[5] = {{514, "Dharavath Suresh", {90, 90, 90, 90, 90, 90}},
						{515, "Nirosha Nani", {91, 91, 91, 91, 91, 91}},
						{516, "Sabitha Reddy", {92, 92, 92, 92, 92, 92}},
						{521, "Veena Reddy", {93, 93, 93, 93, 93, 93 }},
						{522, "Upendar Reddy", {94, 94, 94, 94, 94, 94}}};

*/
struct Student s1[5] = {{{90, 90, 90, 90, 90, 90}, 514, "Dharavath Suresh", 600},
						{{91, 91, 91, 91, 91, 91}, 515, "Nirosha Nani", 601},
						{{93, 93, 93, 93, 93, 93}, 521, "Veena Reddy", 602},
						{{92, 92, 92, 92, 92, 92}, 516, "Sabitha Reddy", 603},
						{{94, 94, 94, 94, 94, 94}, 522, "Upendar Reddy", 604}};

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
		r = sd_bus_message_open_container(m, 'a', "(isai(i))"); /*Open outer array*/
		for (i = 0; i < 5; i++) {
			r = sd_bus_message_open_container(m, 'r', "isai(i)"); /*Open structure element*/
			r = sd_bus_message_open_container(m, 'a', "i");
			// r = sd_bus_message_append_array(m, 'i', s1[i].marks, 6);
			for(j=0; j<6; j++) {
				r = sd_bus_message_append(m, "i", s1[i].marks[j]);
			}
			r = sd_bus_message_close_container(m);
			r = sd_bus_message_append(m, "i", s1[i].id);
			r = sd_bus_message_append(m, "s", s1[i].name);
			r = sd_bus_message_close_container(m);

			r = sd_bus_message_open_container(m, 'r', "i"); /*Open structure element*/
			r = sd_bus_message_append(m, "i", s1[i].b.id);
			r = sd_bus_message_close_container(m);
			// for (j=0; j<6; j++) {
			//}
		}
		r = sd_bus_message_close_container(m);
		r = sd_bus_call(bus, m, 0, &error, &reply);
		printf("Finished function call\n");
        if (r < 0) {
                fprintf(stderr, "Failed to issue method call for astructa: %s\n", error.message);
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
}
