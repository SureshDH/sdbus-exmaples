#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-bus.h>

int main(int argc, char *argv[]) {
        sd_bus_error error = SD_BUS_ERROR_NULL;
        sd_bus_message *m = NULL;
        sd_bus *bus = NULL;
        char *sresp;
        // int sresp;
        int resp;
        int r;

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
        r = sd_bus_call_method(bus,
                               "net.poettering.Calculator",           /* service to contact */
                               "/net/poettering/Calculator",          /* object path */
                               "net.poettering.Calculator",   /* interface name */
                               "Array",                          /* method name */
                               &error,                               /* object to return error in */
                               &m,                                   /* return message on success */
                               "ai",                                 /* input signature */
							   2,
                               514,
							   515);                           /* second argument */
		printf("Finished function call\n");
        if (r < 0) {
                fprintf(stderr, "Failed to issue method call: %s\n", error.message);
                goto finish;
        }

        /* Parse the response message */
        r = sd_bus_message_read(m, "s", &sresp);
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
