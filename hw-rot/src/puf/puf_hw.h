#ifndef PUF_HW_H
#define PUF_HW_H

#include "puf4.h"
#include "../common/intf.h"
#include "../common/extra_func.h"
#include "../common/conf.h"

void puf_gen_enrollment(INTF* puf, unsigned int RC, unsigned int K_bits);
void puf_HelpDataGen(INTF* puf, unsigned int gen_enroll, unsigned int RC, char* key, char* HelpData);
void puf_RetryKey(INTF* puf, unsigned int RC, char* key, char* HelpData);


#endif