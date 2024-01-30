/* Automatically generated from ${input_excel}
 * by Deepx Neural Network Generator v1.0
 */
#pragma once

#include <stdint.h>
#include "${npu_variant}/NpuRegSystem.h"
#include "${npu_variant}/NpuRegDma.h"
% for module in npu_modules:
#include "${npu_variant}/NpuRegNpu${module.id}.h"
% endfor
#include "${npu_variant}/NpuRegDebug.h"
#include "dxrt/common.h"

namespace dxrt {
struct NpuReg;
struct NpuMemMap;

/* Register index */
<% idx = 0 %>
enum NpuRegIndex_${npu_variant}
{
% for module in modules:
  % for reg in module.regs:
    INDEX_${module.name}_${reg.name} = ${idx},<% idx = idx+1 %>
    % for regbits in reg.regbits:
    INDEX_${module.name}_${reg.name}_${regbits.field} = ${idx},<% idx = idx+1 %>
    % endfor
  % endfor
% endfor
<% prev_idx = idx %>
% for module in npu_modules:
  <% idx = prev_idx %>
  % for reg in module.regs:
    INDEX_${module.name}_${reg.name} = ${idx},<% idx = idx+1 %>
    % for regbits in reg.regbits:
    INDEX_${module.name}_${reg.name}_${regbits.field} = ${idx},<% idx = idx+1 %>
    % endfor
  % endfor
% endfor
};

/* Register access API */
% for module in modules+npu_modules:
  % for reg in module.regs:
/* ${module.name}.${reg.name} (Category: ${reg.category}) */
uint32_t Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}(void* base);
void Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}(void* base, uint32_t val);
    % for regbits in reg.regbits:
/* ${module.name}.${reg.name}.${regbits.field} (Category: ${reg.category}) */
uint32_t Read_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field}(void* base);
void Write_${module.name[0]+module.name[1:].lower()}_${reg.name[0]+reg.name[1:].lower()}_${regbits.field}(void* base, uint32_t val);

    % endfor
  % endfor
%endfor

extern std::vector<NpuReg> gNpuRegs_${npu_variant}[${len(npu_modules)}];
extern std::vector<NpuReg> gNpuRmapRegs_${npu_variant}[${len(npu_modules)}];
extern std::vector<NpuMemMap> gNpuMemMaps_${npu_variant}[${len(npu_modules)}];

int ${npu_variant}_VerifyGeneratedStruct(void);

} /* namespace dxrt */