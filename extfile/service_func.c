#include <stdio.h>
#include <errno.h>
#include <systemd/sd-bus.h>

int method_multiply(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
        int64_t x, y;
        int r;
		const char *sign;
		int complete;

		sign = sd_bus_message_get_signature(m, complete);
		printf("returned signature is: %s\n", sign);

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

int method_divide(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
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

