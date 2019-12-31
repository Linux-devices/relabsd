#pragma once

#include <relabsd/device/virtual_device_types.h>

/*
 * - Clones the (real) input device.
 * - Adds ABS event support to the clone.
 * - Adds the ABS axis described in 'config' to the clone (overwriting if
 *   needed).
 * - Disables the associated REL events from the clone.
 *
 * Returns -1 on (fatal) error,
 *         0  on success.
 *
 * 'dev' does not need to be initialized, as the function will to that for you
 * (on success).
 * On success, 'dev' will need to be closed.
 * This opens the (real) input device in read only mode to copy its properties.
 */
int relabsd_device_create
(
   struct relabsd_device * const dev,
   const struct relabsd_config * const config
);

void relabsd_device_destroy (const struct relabsd_device * const dev);

/*
 * Write an event to 'dev'. At this time, every event written to 'dev' is
 * followed by an EV_SYN event.
 *
 * Returns 0 if both the event and the EV_SYN were successful,
 *         -1 if either failed.
 */
int relabsd_device_write_evdev_event
(
   const struct relabsd_device * const dev,
   unsigned int const type,
   unsigned int const code,
   int const value
);

/*
 * Send an event for each enabled axis, setting it to zero.
 * An EV_SYN event is sent afterwards.
 */
void relabsd_device_set_axes_to_zero
(
   const struct relabsd_device * const dev,
   const struct relabsd_config * const config
);
