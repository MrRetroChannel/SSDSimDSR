#include "page.h"
#include "ssd.h"

int main(void) {
    SSD* ssd = init_ssd("ssd_file");
    free_ssd(ssd);
}