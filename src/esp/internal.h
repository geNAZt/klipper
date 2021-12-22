#ifndef __ESP_INTERNAL_H
#define __ESP_INTERNAL_H

#include "internal/bits.h"
#include "internal/io_mux.h"
#include "internal/gpio.h"
#include "internal/time.h"

//write value to register
#define REG_WRITE(_r, _v) ({                                                                                           \
            (*(volatile uint32_t *)(_r)) = (_v);                                                                       \
        })

//read value from register
#define REG_READ(_r) ({                                                                                                \
            (*(volatile uint32_t *)(_r));                                                                              \
        })

// flip a bit in the given register
#define REG_XOR_BIT(_r, _b)  ({                                                                                        \
            (*(volatile uint32_t*)(_r) ^= (_b));                                                                       \
        })

//set bit or set bits to register
#define REG_SET_BIT(_r, _b)  ({                                                                                        \
            (*(volatile uint32_t*)(_r) |= (_b));                                                                       \
        })

//clear bit or clear bits of register
#define REG_CLR_BIT(_r, _b)  ({                                                                                        \
            (*(volatile uint32_t*)(_r) &= ~(_b));                                                                      \
        })

#define MODIFY_U32_REG_FIELD(base_reg, reg_field, field_val)    \
{                                                           \
    uint32_t temp_val = base_reg.val;                       \
    typeof(base_reg) temp_reg;                              \
    temp_reg.val = temp_val;                                \
    temp_reg.reg_field = (field_val);                       \
    (base_reg).val = temp_reg.val;                          \
}

//set field of a register from variable, uses field _S & _V to determine mask
#define REG_SET_FIELD(_r, _f, _v) ({                                                                                   \
            (REG_WRITE((_r),((REG_READ(_r) & ~((_f##_V) << (_f##_S)))|(((_v) & (_f##_V))<<(_f##_S)))));                \
        })

//get field from register, uses field _S & _V to determine mask
#define REG_GET_FIELD(_r, _f) ({                                                                                       \
            ((REG_READ(_r) >> (_f##_S)) & (_f##_V));                                                                   \
        })

//get bit or get bits from register
#define REG_GET_BIT(_r, _b)  ({                                                                                        \
            (*(volatile uint32_t*)(_r) & (_b));                                                                        \
        })

//set bits of register controlled by mask
#define REG_RW_SET_MASK(reg, mask)   REG_WRITE((reg), (REG_READ(reg) | (mask)))
#define REG_RW_CLR_MASK(reg, mask) REG_WRITE((reg), (REG_READ(reg)&(~(mask))))

// Time control
void delay_us(uint32_t); 

// Interrupt control
void intr_lock(void);
void intr_unlock(void);

#endif // internal.h