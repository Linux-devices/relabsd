#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "error.h"
#include "axis.h"
#include "config.h"

#include "relabsd_device.h"

/* LIBEVDEV_UINPUT_OPEN_MANAGED is not defined on my machine. */
#ifndef LIBEVDEV_UINPUT_OPEN_MANAGED
   #warning "libevdev did not define LIBEVDEV_UINPUT_OPEN_MANAGED, "\
            "using value '-2' instead."
   #define LIBEVDEV_UINPUT_OPEN_MANAGED -2
#endif

static void replace_rel_axis
(
   struct relabsd_device * const dev,
   const struct relabsd_config * const config,
   struct input_absinfo * const absinfo,
   unsigned int rel_code
)
{
   enum relabsd_axis rad_code;
   unsigned int abs_code;

   rad_code = relabsd_axis_convert_evdev_rel(rel_code, &abs_code);

   relabsd_config_get_absinfo(config, rad_code, absinfo);
   libevdev_disable_event_code(dev->dev, EV_REL, rel_code);
   libevdev_enable_event_code(dev->dev, EV_ABS, abs_code, absinfo);
}

int relabsd_device_create
(
   struct relabsd_device * const dev,
   const struct relabsd_config * const config
)
{
   struct input_absinfo absinfo;
   int fd;

   fd = open(config->input_file, O_RDONLY);

   if (fd < 0)
   {
      _FATAL
      (
         "Could not open device %s in read only mode:",
         config->input_file,
         strerror(errno)
      );

      return -1;
   }

   if
   (
      libevdev_new_from_fd(fd, &(dev->dev)) < 0
   )
   {
      _FATAL
      (
         "libevdev could not open %s:",
         config->input_file,
         strerror(errno)
      );

      close(fd);

      return -1;
   }

   libevdev_set_name(dev->dev, config->device_name);

   libevdev_enable_event_type(dev->dev, EV_ABS);

   replace_rel_axis(dev, config, &absinfo, REL_X);
   replace_rel_axis(dev, config, &absinfo, REL_Y);
   replace_rel_axis(dev, config, &absinfo, REL_Z);
   replace_rel_axis(dev, config, &absinfo, REL_RX);
   replace_rel_axis(dev, config, &absinfo, REL_RY);
   replace_rel_axis(dev, config, &absinfo, REL_RZ);

   if
   (
       libevdev_uinput_create_from_device
       (
         dev->dev,
         LIBEVDEV_UINPUT_OPEN_MANAGED,
         &(dev->uidev)
       )
       < 0
   )
   {
      _FATAL("Could not create relabsd device: %s.", strerror(errno));

      libevdev_free(dev->dev);

      close(fd);

      return -1;
   }

   close(fd);

   return 0;
}

void relabsd_device_destroy (const struct relabsd_device * const dev)
{
   libevdev_uinput_destroy(dev->uidev);
   libevdev_free(dev->dev);
}

int relabsd_device_write_evdev_event
(
   const struct relabsd_device * const dev,
   unsigned int const type,
   unsigned int const code,
   int const value
)
{
   if
   (
      (libevdev_uinput_write_event(dev->uidev, type, code, value) == 0)
      && (libevdev_uinput_write_event(dev->uidev, EV_SYN, SYN_REPORT, 0) == 0)
   )
   {
      return 0;
   }

   return -1;
}