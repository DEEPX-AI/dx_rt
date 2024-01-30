/* Automatically generated from ${input_excel}
 * by Deepx Neural Network Generator v1.0
 */
#pragma once

#include <stdint.h>

using namespace dxrt;
namespace dxrt{
##/* (For debug : Will be removed)
## * Raw data from Excel file
##${str(module)}
## */

/* Address */
extern void *Reg${module.name[0].upper()+module.name[1:].lower()};
#define REG_${module.name}_OFFSET ${hex(module.baseaddr)}
% for reg in module.regs:
#define REG_${module.name}_${reg.name}    ${format(reg.addr, '#010x')} /* ${reg.addr} */
% endfor

/* Register Definition */
% for reg in module.regs:
typedef union
{
    uint32_t U;  /* Register-Access : ${module.name}.${reg.name}*/
    struct {     /* Register-Bitfields-Access */
  % for regbits in reg.regbits:
        uint32_t ${regbits.field}:${regbits.width};  /* ${module.name}.${reg.name}.${regbits.field} ${regbits.bits_pos} (${regbits.type}, ${hex(regbits.reset)}) ${regbits.desc} */
  % endfor
    };
} ${module.name}_${reg.name}_t;
% endfor

/* ${module.name} Module Definition */
typedef volatile struct
{
% for reg in module.regs:
    ${module.name}_${reg.name}_t ${reg.name};  /* ${reg.category} ${hex(reg.addr)} */
% endfor
} dx${module.name}_t;

/* Alias Macro */
#define dx${module.name} ((*(dx${module.name}_t*)Reg${module.name[0].upper()+module.name[1:].lower()}))  /* Module instance */
#define p${module.name} ((dx${module.name}_t*)Reg${module.name[0].upper()+module.name[1:].lower()})  /* Module pointer instance */

/* Register access API */
% for reg in module.regs:
/* ${module.name}.${reg.name} (Category: ${reg.category}) */
#define READ_${module.name}_${reg.name}() readl(Reg${module.name[0].upper()+module.name[1:].lower()}, REG_${module.name}_${reg.name})
#define WRITE_${module.name}_${reg.name}(val) writel(Reg${module.name[0].upper()+module.name[1:].lower()}, REG_${module.name}_${reg.name}, val)
static inline uint32_t Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}(void)
{
    return dx${module.name}.${reg.name}.U;
}
static inline uint32_t Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}(uint32_t val)
{
    dx${module.name}.${reg.name}.U = val;
}
  % for regbits in reg.regbits:
/* ${module.name}.${reg.name}.${regbits.field} (Category: ${reg.category}) */
#define ${module.name}_${reg.name}_${regbits.field}_LEN        ${regbits.width}
#define ${module.name}_${reg.name}_${regbits.field}_OFFSET     ${regbits.end_bit}
#define ${module.name}_${reg.name}_${regbits.field}_MASK_BITS  ${format(regbits.mask_bits, '#010x')} /* ${regbits.bits} ${format(regbits.mask_bits, '#034b')} */
#define ${module.name}_${reg.name}_${regbits.field}_MASK       ${format(regbits.mask, '#010x')} /* ${regbits.bits_pos} ${format(regbits.mask, '#034b')} */
#define READ_MASK_${module.name}_${reg.name}_${regbits.field}() \\

    readl_mask(Reg${module.name[0].upper()+module.name[1:].lower()}, REG_${module.name}_${reg.name}, ${module.name}_${reg.name}_${regbits.field}_MASK)
#define READ_${module.name}_${reg.name}_${regbits.field}() \\

    readb(Reg${module.name[0].upper()+module.name[1:].lower()}, REG_${module.name}_${reg.name}, ${module.name}_${reg.name}_${regbits.field}_MASK, ${module.name}_${reg.name}_${regbits.field}_OFFSET)
#define WRITE_${module.name}_${reg.name}_${regbits.field}(bits) \\

    writeb(Reg${module.name[0].upper()+module.name[1:].lower()}, REG_${module.name}_${reg.name}, bits, ${module.name}_${reg.name}_${regbits.field}_MASK, ${module.name}_${reg.name}_${regbits.field}_OFFSET)
static inline uint32_t Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field}(void)
{
    return dx${module.name}.${reg.name}.${regbits.field};
}
static inline uint32_t Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field}(uint32_t val)
{
    dx${module.name}.${reg.name}.${regbits.field} = val;
}

  % endfor
% endfor

} /* namespace dxrt */
