//#define DEBUG_SOCKET
#define DEBUG_IP "192.168.2.2"
#define DEBUG_PORT 9023

#include "ps4.h"
#include <unistd.h>

int kpayload_enter_idu(struct thread *td, struct kpayload_firmware_args *args) {
  UNUSED(td);
  void *kernel_base;

  uint64_t (*icc_nvs_write)(uint32_t block, uint32_t offset, uint32_t size, void *value);

  uint16_t fw_version = args->kpayload_firmware_info->fw_version;

  // NOTE: This is a C preprocessor macro
  build_kpayload(fw_version, icc_nvs_write_macro);

  char flag = 1; //change this to 1????
  icc_nvs_write(4, 0x1600, 1, &flag);

  return 0;
}

int enter_idu() {
  struct kpayload_firmware_info kpayload_firmware_info;
  kpayload_firmware_info.fw_version = get_firmware();
  return kexec(&kpayload_enter_idu, &kpayload_firmware_info);
}

int _main(struct thread *td) {
  UNUSED(td);

  initKernel();
  initLibc();

#ifdef DEBUG_SOCKET
  initNetwork();
  DEBUG_SOCK = SckConnect(DEBUG_IP, DEBUG_PORT);
#endif

  jailbreak();
  enter_idu();

  initSysUtil();
#ifdef DEBUG_SOCKET
  printf_debug("Closing socket...\n");
  SckClose(DEBUG_SOCK);
#endif
  printf_notification("Enabling Custom Firmware...");
  //printf_notification("Entered IDU mode, restarting in 5 seconds...");

  sleep(5);

  reboot();



  

  return 0;
}
