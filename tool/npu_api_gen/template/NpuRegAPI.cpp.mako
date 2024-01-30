/* Automatically generated from ${input_excel}
 * by Deepx Neural Network Generator v1.0
 */
#include "dxrt/common.h"
#include "dxrt/npuif/npuif.h"
#include <cassert>
#include <sstream>

using namespace std;

namespace dxrt {
int ${npu_variant}_VerifyGeneratedStruct(void)
{
    int cnt = 0;
% for module in modules+npu_modules:
  % for reg in module.regs:
    if(sizeof(${module.name}_${reg.name}_t)!=4) cnt--;
    //assert(sizeof(${module.name}_${reg.name}_t)==4);
  % endfor
% endfor
    LOG_DBG(cnt);
    return cnt;
}

/* Register access API */
% for module in modules+npu_modules:
  % for reg in module.regs:
/* ${module.name}.${reg.name} (Category: ${reg.category}) */
uint32_t Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}(void* base)
{
    if(logNpuRegAccess)
        std::cout << std::hex << "[R][" << (void*)base << "][+" + std::to_string(REG_${module.name}_OFFSET) + "][+"+std::to_string(${hex(reg.regbits[0].addr)}) + "] ${module.name}.${reg.name} : " << ((dx${module.name}_t*) base)->${reg.name}.U << std::endl;
    return ((dx${module.name}_t*) base)->${reg.name}.U;
}
void Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}(void* base, uint32_t val)
{
    if(logNpuRegAccess)
        std::cout << std::hex << "[W][" << (void*)base << "][+" + std::to_string(REG_${module.name}_OFFSET) + "][+"+std::to_string(${hex(reg.regbits[0].addr)}) + "] ${module.name}.${reg.name} : " << val << std::endl;
    IOMEM_BARRIER();
    ((dx${module.name}_t*) base)->${reg.name}.U = val;
    //*(volatile uint32_t*)&(((dx${module.name}_t*) base)->${reg.name}.U) = val;
    IOMEM_BARRIER();
}
    % for regbits in reg.regbits:
/* ${module.name}.${reg.name}.${regbits.field} (Category: ${reg.category}) */
uint32_t Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field}(void* base)
{
    if(logNpuRegAccess)
        std::cout << std::hex << "[R][" << (void*)base << "][+" + std::to_string(REG_${module.name}_OFFSET) + "][+"+std::to_string(${hex(reg.regbits[0].addr)}) + "] ${module.name}.${reg.name}.${regbits.field} : " << ((dx${module.name}_t*) base)->${reg.name}.${regbits.field} << std::endl;
    return ((dx${module.name}_t*) base)->${reg.name}.${regbits.field};
}
void Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field}(void* base, uint32_t val)
{
    if(logNpuRegAccess)
        std::cout << std::hex << "[W][" << (void*)base << "][+" + std::to_string(REG_${module.name}_OFFSET) + "][+"+std::to_string(${hex(reg.regbits[0].addr)}) + "] ${module.name}.${reg.name}.${regbits.field} : " << val << std::endl;
    IOMEM_BARRIER();
    ((dx${module.name}_t*) base)->${reg.name}.${regbits.field} = val;
    IOMEM_BARRIER();
}

    % endfor
  % endfor
%endfor

/* Register info */
vector<NpuReg> gNpuRegs_${npu_variant}[${len(npu_modules)}] = {
%for npu_module in npu_modules:
<% idx = 0 %>
  [${npu_module.id}] = {
    /* type, baseaddr, addr, len, mask, offset, reset_val, read, write, enable, name */
% for module in modules+[npu_module]:
  % for reg in module.regs:
    { /* ${idx} : INDEX_${module.name}_${reg.name} */
      DXRT_NPU_REG_${reg.category}, REG_${module.name}_OFFSET, REG_${module.name}_OFFSET + ${hex(reg.regbits[0].addr)}, 32, 0xffffffff, 0x0, -1, ${reg.regbits[0].read}, ${reg.regbits[0].write}, ${reg.regbits[0].enable}, ${reg.has_off_bit},
      "${module.name}.${reg.name}", 
      Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()},
      Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()},
    },<% idx = idx+1 %>
    % for regbits in reg.regbits:
    { /* ${idx} : INDEX_${module.name}_${reg.name}_${regbits.field} */
      DXRT_NPU_REG_${reg.category}, REG_${module.name}_OFFSET, REG_${module.name}_OFFSET + ${hex(regbits.addr)}, ${module.name}_${reg.name}_${regbits.field}_LEN, ${module.name}_${reg.name}_${regbits.field}_MASK, ${module.name}_${reg.name}_${regbits.field}_OFFSET, ${hex(regbits.reset)}, ${regbits.read}, ${regbits.write}, ${regbits.enable}, false,
      "${module.name}.${reg.name}.${regbits.field}", 
      Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field},
      Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field},
    },<% idx = idx+1 %>
    % endfor
  % endfor
% endfor
  },
%endfor
};

vector<NpuReg> gNpuRmapRegs_${npu_variant}[${len(npu_modules)}] = {
%for npu_module in npu_modules:
<% idx = 0 %>
  [${npu_module.id}] = {
    /* type, baseaddr, addr, len, mask, offset, reset_val, read, write, enable, name */
% for module in [npu_module]:
  % for reg in module.regs:
    { /* ${idx} */
      DXRT_NPU_REG_${reg.category}, REG_${module.name}_OFFSET, REG_${module.name}_OFFSET + ${hex(reg.regbits[0].addr)}, 32, 0xffffffff, 0x0, ${hex(reg.regbits[0].reset)}, ${reg.regbits[0].read}, ${reg.regbits[0].write}, ${reg.regbits[0].enable}, ${reg.has_off_bit},
      "${module.name}.${reg.name}", 
      Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()},
      Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()},
    },<% idx = idx+1 %>
  % endfor
% endfor
  },
%endfor
};

} /* namespace dxrt */