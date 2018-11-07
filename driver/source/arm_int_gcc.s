
/*
 * int irq_lock ();
 */
    .global irq_lock
    .type irq_lock, %function
irq_lock:
    mrs r1, cpsr
    mov r0, r1
    and r0, r0, #(1 << 7)
    orr r1, r1, #(1 << 7)
    msr cpsr, r1
    bx  lr

/*
 * void irq_unlock (int level);
 */
    .global irq_unlock
    .type irq_unlock, %function
irq_unlock:
    mrs r1, cpsr
    orr r1, r0, #(1 << 7)
    msr cpsr, r1
    bx  lr

/*
 * void irq_enable (void);
 */
    .global irq_enable
    .type irq_enable, %function
irq_enable:
    mrs r0, cpsr
    bic r0, r0, #(1 << 7)
    msr cpsr, r0
    bx  lr

/*
 * void irq_disable (void);
 */
    .global irq_disable
    .type irq_disable, %function
irq_disable:
    mrs r0, cpsr
    orr r0, r0, #(1 << 7)
    msr cpsr, r0
    bx  lr
