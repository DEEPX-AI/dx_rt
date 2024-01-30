// SPDX-License-Identifier: GPL-2.0
/*
 * Deepx Runtime Driver
 *
 * Copyright (C) 2023 Deepx, Inc.
 *
 */
#ifndef __NPU_REG_${module.name}_${npu_variant}_H
#define __NPU_REG_${module.name}_${npu_variant}_H

#include <linux/types.h>

/* Address */
#define REG_${module.name}_OFFSET ${hex(module.baseaddr)}
% for reg in module.regs:
#define REG_${module.name}_${reg.name}    (REG_${module.name}_OFFSET + ${format(reg.addr, '#010x')}) /* ${reg.addr} */
% endfor

/* Register Definition */
% for reg in module.regs:
typedef volatile union _${module.name}_${reg.name}_t
{
    volatile uint32_t U;  /* Register-Access : ${module.name}.${reg.name}*/
    struct {     /* Register-Bitfields-Access */
  % for regbits in reg.regbits:
        volatile uint32_t ${regbits.field}:${regbits.width};  /* ${module.name}.${reg.name}.${regbits.field} ${regbits.bits_pos} (${regbits.type}, ${hex(regbits.reset)}) ${regbits.desc} */
  % endfor
    };
} ${module.name}_${reg.name}_t;
% endfor

/* ${module.name} Module Definition */
typedef volatile struct _dx${module.name}_t
{
% for reg in module.regs:
    ${module.name}_${reg.name}_t ${reg.name};  /* ${reg.category} ${hex(reg.addr)} */
% endfor
} dx${module.name}_t;

/* Register access API */
% for reg in module.regs:
/* ${module.name}.${reg.name} (Category: ${reg.category}) */
#define READ_${module.name}_${reg.name}(base) npu_reg_read(base, REG_${module.name}_${reg.name})
#define WRITE_${module.name}_${reg.name}(base, val) npu_reg_write(base, REG_${module.name}_${reg.name}, val)
##static inline uint32_t Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}(dx${module.name}_t* base)
##{
##    if(logNpuRegAccess)
##        cout << hex << "[R][" << (void*)base << "] ${module.name}.${reg.name} : " << base->${reg.name}.U << endl;
##    return base->${reg.name}.U;
##}
##static inline void Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}(dx${module.name}_t* base, uint32_t val)
##{
##    if(logNpuRegAccess)
##        cout << hex << "[W][" << (void*)base << "] ${module.name}.${reg.name} : " << val << endl;
##    base->${reg.name}.U = val;
##}
  % for regbits in reg.regbits:
/* ${module.name}.${reg.name}.${regbits.field} (Category: ${reg.category}) */
#define ${module.name}_${reg.name}_${regbits.field}_LEN        ${regbits.width}
#define ${module.name}_${reg.name}_${regbits.field}_OFFSET     ${regbits.end_bit}
#define ${module.name}_${reg.name}_${regbits.field}_MASK_BITS  ${format(regbits.mask_bits, '#010x')} /* ${regbits.bits} ${format(regbits.mask_bits, '#034b')} */
#define ${module.name}_${reg.name}_${regbits.field}_MASK       ${format(regbits.mask, '#010x')} /* ${regbits.bits_pos} ${format(regbits.mask, '#034b')} */
#define READ_${module.name}_${reg.name}_${regbits.field}(base) \\

    npu_reg_read_mask(base, REG_${module.name}_${reg.name}, ${module.name}_${reg.name}_${regbits.field}_MASK, ${module.name}_${reg.name}_${regbits.field}_OFFSET)
#define WRITE_${module.name}_${reg.name}_${regbits.field}(base, val) \\

    npu_reg_write_mask(base, REG_${module.name}_${reg.name}, val, ${module.name}_${reg.name}_${regbits.field}_MASK, ${module.name}_${reg.name}_${regbits.field}_OFFSET)
##static inline uint32_t Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field}(dx${module.name}_t* base)
##{
##    if(logNpuRegAccess)
##        cout << hex << "[R][" << (void*)base << "] ${module.name}.${reg.name}.${regbits.field} : " << base->${reg.name}.${regbits.field} << endl;
##    return base->${reg.name}.${regbits.field};
##}
##static inline void Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field}(dx${module.name}_t* base, uint32_t val)
##{
##    if(logNpuRegAccess)
##        cout << hex << "[W][" << (void*)base << "] ${module.name}.${reg.name}.${regbits.field} : " << val << endl;
##    base->${reg.name}.${regbits.field} = val;
##}
##int Verify_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field}(dx${module.name}_t* base);

  % endfor
% endfor

#endif // __NPU_REG_${module.name}_${npu_variant}_H
