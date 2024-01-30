#ifndef __NPU_SFR_DEF_H
#define __NPU_SFR_DEF_H

#include <stdint.h>

##debug msg
##% for sfr in sfrlist:
##${sfr.type} ${sfr.addr} ${sfr.name} ${sfr.width} ${sfr.rw}
##% endfor
/* CONV_TYPE */
#define STANDARD 0
#define DEPTHWISE 1
#define POINTWISE 2
#define FC 3
/* NONLIN_TYPE */
#define RELU 0
#define RELU6 1
#define H_SIGMOID 2
#define H_SWISH 3
#define LEAKY_RELU 4
#define RRELU 5

% for sfr in sfrlist:
 % if sfr.width<8:
typedef union
{
    uint32_t    U;
    uint32_t    reg_${sfr.name}:${sfr.width};
} bits_${sfr.type}_${sfr.name};

 % endif
% endfor

<%prev = -1 %>\
<%reserved = 0 %>\
typedef volatile struct _npu_sfr_t
{
  % for sfr in sfrlist:
      % if prev!=-1 and sfr.addr!=prev.addr+4:
    ${'uint32_t'.ljust(42, ' ')} ${('reserved'+str(reserved)+'['+str(int((sfr.addr-prev.addr-4)/4))+']').ljust(40, ' ')}; /* ${'0x{:04x}'.format(prev.addr+4)}*/
<%reserved = reserved + 1 %>\
      %endif
      % if sfr.width>=8:
    ${('uint'+str(sfr.width)+'_t').ljust(42, ' ')} ${sfr.name.lower().ljust(40, ' ')}; /* ${'0x{:04x}'.format(sfr.addr)} , ${sfr.rw} (${sfr.reset}), ${sfr.type} */
      % else:
    ${('bits_'+sfr.type+'_'+sfr.name).ljust(42, ' ')} ${sfr.name.lower().ljust(40, ' ')}; /* ${'0x{:04x}'.format(sfr.addr)} , ${sfr.rw} (${sfr.reset}), ${sfr.type} */
      % endif
<%prev = sfr  %>\
  % endfor
} npu_sfr_t;

  % for sfr in sfrlist:
##${('#define REG_'+sfr.name).ljust(50, ' ')} ${'0x{:04x}'.format(sfr.addr)}
${'#define '+ ('REG_'+sfr.name).rjust(40, ' ')}  ${'0x{:04x}'.format(sfr.addr)}
  % endfor

#endif