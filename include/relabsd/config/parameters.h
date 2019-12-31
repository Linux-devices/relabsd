#pragma once

#include <relabsd/config/parameters_types.h>

/**** Utility *****************************************************************/
int relabsd_parameters_parse_execution_mode
(
   const int argc,
   const char * const argv [const restrict static argc],
   struct relabsd_parameters parameters [const restrict static 1]
);

int relabsd_parameters_parse_options
(
   const int argc,
   const char * const argv [const restrict static argc],
   struct relabsd_parameters parameters [const restrict static 1]
);

int relabsd_parameters_argument_count_for
(
   const char option [const restrict static 1],
   int result [const restrict static 1]
);

void relabsd_parameters_print_usage (const char exec [const restrict static 1]);

/**** Accessors ***************************************************************/
void relabsd_parameters_initialize_options
(
   const struct relabsd_parameters parameters [const restrict static 1]
);

int relabsd_parameters_get_run_as_daemon
(
   const struct relabsd_parameters parameters [const restrict static 1]
);

const char * relabsd_parameters_get_communication_node_name
(
   const struct relabsd_parameters parameters [const restrict static 1]
);

const char * relabsd_parameters_get_device_name
(
   const struct relabsd_parameters parameters [const restrict static 1]
);

const char * relabsd_parameters_get_physical_device_file_name
(
   const struct relabsd_parameters parameters [const restrict static 1]
);

enum relabsd_parameters_run_mode relabsd_parameters_get_execution_mode
(
   const struct relabsd_parameters parameters [const restrict static 1]
);
