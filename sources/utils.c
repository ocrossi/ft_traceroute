#include "../includes/ft_traceroute.h"

int get_milisec_precision() {
  int digit = 0;
  // double time = data->time;
  double time = 0;

  for (int i = 0; i < 3; i++) {
    if (time / 10 > 0.1) {
      digit++;
      time /= 10;
    }
  }
  return 3 - digit;
}

double convert_to_milliseconds(t_val time, t_val base) {
  double fMicro =
      (time.tv_usec - base.tv_usec) + (time.tv_sec - base.tv_sec) * 1000000;
  double fMilli = fMicro / 1000;

  return fMilli;
}
