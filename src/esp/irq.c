// Definitions for irq enable/disable on ESP

#include "board/irq.h"
#include "compiler.h" // barrier
#include "internal.h" // delay_us

inline void irq_disable(void) {
    intr_lock();
    barrier();
}

inline void irq_enable(void) {
    barrier();
    intr_unlock();
}

inline irqstatus_t irq_save(void) {
    return 0;
}

inline void irq_restore(irqstatus_t flag) {

}

inline void irq_wait(void) {
    delay_us(1);
}

inline void irq_poll(void) {
}