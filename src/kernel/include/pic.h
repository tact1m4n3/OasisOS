#pragma once

#include <types.h>

void pic_remap();
void pic_send_eoi(uint64_t int_no);
