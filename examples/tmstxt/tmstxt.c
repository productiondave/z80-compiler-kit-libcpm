#include <string.h>
#include "tms.h"
#include "tms_patterns.h"

void main() {
  tms_init_text(WHITE, DARK_BLUE);
  tms_load_pat(tms_patterns, TMS_PATTERNS_LEN);
  memset(tms_buf, ' ', tms_n_tbl_len);
  tms_puts_xy(0, 0, "hellorld");
  tms_wait();
  tms_txtflush(tms_buf);
}
