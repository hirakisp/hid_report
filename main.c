#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/hiddev.h>

static int fd;

int main(int argc, char**argv)
{
  char name[128];

  sprintf(name, "/dev/usb/hiddev0");

  fprintf(stdout, "Using device %s \n", name);

  if ((fd = open(name, O_RDWR)) < 0) {
    if (errno == EACCES) {

      fprintf(stderr, "No permission \n"); 
      return -1;
    }
  }

  static char msg[8] = { 0xaa, 0xaa, 0xd0, 0x00, 0x03, 0x01, 0x00, 0x64 };
  char code[8];
  struct hiddev_usage_ref uref;
  uref.report_type = HID_REPORT_TYPE_OUTPUT;
  uref.report_id = 0;
  uref.field_index = 0;
  uref.usage_code = 0x05010906;

  for (int i = 0; i < 8; i++) {
    uref.usage_index = i;
    uref.value = msg[i];
    if (ioctl(fd, HIDIOCSUSAGE, &uref) < 0) {
      perror("HIDIOCSUSAGE");
      exit(1);
    }
  }
  
  struct hiddev_report_info rinfo;

  rinfo.report_type = HID_REPORT_TYPE_OUTPUT;
  rinfo.report_id = 0;
  rinfo.num_fields = 1;
  if (ioctl(fd, HIDIOCSREPORT, &rinfo) < 0) {
    perror("HIDIOCSREPORT");
  }
}
