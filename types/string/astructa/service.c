#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <systemd/sd-bus.h>

struct Student{
	int marks[6];
	int id;
	char *name;
};

struct Student s1[5];

static int accept_astructa(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
	int r, i, j;
	// struct Student s1;
	int id;
	// char *name;
	// const void* marks[5];
	// int *printMarks;
	size_t length = 6;

	// Read the parameters
	printf("Inside accept string method\n");


	r = sd_bus_message_enter_container(m, 'a', "(aiis)");
	if (r < 0) {
		fprintf(stderr, "Failed enter array container: %s\n", strerror(-r));
	}
	for(i=0; i<5; i++) {
			r = sd_bus_message_enter_container(m, 'r', "aiis");
			if (r < 0) {
				fprintf(stderr, "Failed enter structure container: %s\n", strerror(-r));
			}
			r = sd_bus_message_enter_container(m, 'a', "i");
			if (r < 0) {
				fprintf(stderr, "Failed to parse inner array: %s\n", strerror(-r));
			}
			// r = sd_bus_message_read_array(m, 'i', &marks[i], &length);
			// printMarks = (int *)marks;
			for(j=0; j<6; j++) {
				r = sd_bus_message_read(m, "i", &s1[i].marks[j]);
			}
			// for(j=0; j<6; j++) {
			//	printf("%5d\t", printMarks[j]);
			// }
			printf("\n");
			if (r < 0) {
				fprintf(stderr, "Failed to parse marks: %s\n", strerror(-r));
			}
			r = sd_bus_message_exit_container(m);
			if (r < 0) {
				fprintf(stderr, "Failed to exit inner array: %s\n", strerror(-r));
			}
			r = sd_bus_message_read(m, "i", &s1[i].id);
			if (r < 0) {
				fprintf(stderr, "Failed to parse id param: %s\n", strerror(-r));
			}
			r = sd_bus_message_read(m, "s", &s1[i].name);
			if (r < 0) {
				fprintf(stderr, "Failed to parse name param: %s\n", strerror(-r));
			}

			r = sd_bus_message_exit_container(m);
			if (r < 0) {
				fprintf(stderr, "Failed to exit struct container: %s\n", strerror(-r));
			}

	}
	r = sd_bus_message_exit_container(m);
	if (r < 0) {
		fprintf(stderr, "Failed to exit array container: %s\n", strerror(-r));
	}

	/*
	for(i=0; i<5; i++) {
		s1[i].marks[i] = (int *)marks[i];
	}
	*/
	printf("Printing contents of the array(struct) container:\n");
	printf("=================================================\n");
	for(i=0; i<5; i++) {
		printf("\t%d -> id: %d\n", i, s1[i].id);
		printf("\t%d -> name: %s\n", i, s1[i].name);
		for (j=0; j<6; j++) {
			printf("\t\t%d -> %3d: ", j, s1[i].marks[j]);
		}
		printf("\n");
	}
	printf("=================================================\n");

	/*
	// r = sd_bus_message_read(m, "(a(ss))", 1,  &string1, &string2);
	r = sd_bus_message_read(m, "a(is)", &s1[0].id, &s1[0].name, &s1[1].id, &s1[1].name);
	if (r < 0) {
		fprintf(stderr, "Failed to parse parameters: %s\n", strerror(-r));
	}
	printf("Recieved string1 is: %d\n", s1[0].id);
	printf("Recieved string1 is: %s\n", s1[0].name);
	printf("Recieved string1 is: %d\n", s1[1].id);
	printf("Recieved string1 is: %s\n", s1[1].name);
	// printf("Recieved string2 is: %s\n", string2);
	*/

	/*Reply with the response*/
	printf("Sending response\n");
	return sd_bus_reply_method_return(m, "s", "Welcome, to the world of magics!");
}

static int method_multiply(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
        int64_t x, y;
        int r;

        /* Read the parameters */
        r = sd_bus_message_read(m, "xx", &x, &y);
		printf("Request for multiply\n");
        if (r < 0) {
                fprintf(stderr, "Failed to parse parameters: %s\n", strerror(-r));
                return r;
        }

        /* Reply with the response */
		printf("Sending response\n");
        return sd_bus_reply_method_return(m, "x", x * y);
}

static int method_divide(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
        int64_t x, y;
        int r;

        /* Read the parameters */
        r = sd_bus_message_read(m, "xx", &x, &y);
        if (r < 0) {
                fprintf(stderr, "Failed to parse parameters: %s\n", strerror(-r));
                return r;
        }

        /* Return an error on division by zero */
        if (y == 0) {
                sd_bus_error_set_const(ret_error, "net.poettering.DivisionByZero", "Sorry, can't allow division by zero.");
                return -EINVAL;
        }

        return sd_bus_reply_method_return(m, "x", x / y);
}

/* The vtable of our little object, implements the net.poettering.Calculator interface */
static const sd_bus_vtable calculator_vtable[] = {
        SD_BUS_VTABLE_START(0),
        SD_BUS_METHOD("Multiply", "xx", "x", method_multiply, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_METHOD("Divide",   "xx", "x", method_divide,   SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_METHOD("Astructa",   "a(aiis)", "s", accept_astructa,   SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_VTABLE_END
};

int main(int argc, char *argv[]) {
        sd_bus_slot *slot = NULL;
        sd_bus *bus = NULL;
        int r;

        /* Connect to the user bus this time */
        r = sd_bus_open_system(&bus);
        if (r < 0) {
                fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
                goto finish;
        }

        /* Install the object */
        r = sd_bus_add_object_vtable(bus,
                                     &slot,
                                     "/net/poettering/Calculator",  /* object path */
                                     "net.poettering.Calculator",   /* interface name */
                                     calculator_vtable,
                                     NULL);
        if (r < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n", strerror(-r));
                goto finish;
        }

        /* Take a well-known service name so that clients can find us */
        r = sd_bus_request_name(bus, "net.poettering.Calculator", 0);
        if (r < 0) {
                fprintf(stderr, "Failed to acquire service name: %s\n", strerror(-r));
                goto finish;
        }

        for (;;) {
                /* Process requests */
                r = sd_bus_process(bus, NULL);
                if (r < 0) {
                        fprintf(stderr, "Failed to process bus: %s\n", strerror(-r));
                        goto finish;
                }
                if (r > 0) /* we processed a request, try to process another one, right-away */
                        continue;

                /* Wait for the next request to process */
                r = sd_bus_wait(bus, (uint64_t) -1);
                if (r < 0) {
                        fprintf(stderr, "Failed to wait on bus: %s\n", strerror(-r));
                        goto finish;
                }
        }

finish:
        sd_bus_slot_unref(slot);
        sd_bus_unref(bus);

        return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
