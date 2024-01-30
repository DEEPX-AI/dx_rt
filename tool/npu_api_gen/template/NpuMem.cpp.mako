/* Automatically generated from ${input_excel}
 * by Deepx Neural Network Generator v1.0
 */

#include <stdint.h>
#include "dxrt/npuif/npuif.h"

namespace dxrt{
% for module in modules:
  % for mem in module:
#define ${mem.name}_WIDTH     ${mem.width}  /* unit : bit */
#define ${mem.name}_DEPTH     ${mem.depth}
#define ${mem.name}_SIZE      ${int(mem.width * mem.depth / 8)}  /* ${mem.size}KB [<= ${mem.width} * ${mem.depth} / 8] */
#define ${mem.name}_BASE      ${hex(mem.start)}
#define ${mem.name}_END       ${hex(mem.end)}

  % endfor
% endfor

std::vector<NpuMemMap> gNpuMemMaps_${npu_variant}[${len(modules)}] = {
% for module in modules:
  [${module[0].id}] = {
    /* name, group, cpu_mem, in-place copy, type, addr_start, width, size, addr_end */
  % for mem in module:
    {"${mem.name}", ${mem.group_id}, ${mem.cpu_mem}, ${mem.user_dma_cp}, ${mem.mem_type}, ${mem.name}_BASE, ${mem.name}_WIDTH/8, ${mem.name}_SIZE, ${mem.name}_END,},
  % endfor
  },
% endfor
};

} /* namespace dxrt */
