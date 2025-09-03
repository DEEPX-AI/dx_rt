/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include <array>
#include <iostream>
#include <cstdint>
#include <cstring>
#include "dxrt/common.h"
#include "dxrt/device_struct.h"
#include "dxrt/device_struct_operators.h"
#include "dxrt/map_lookup_template.h"


using std::ostream;
using std::endl;
using std::array;
using std::hex;
using std::dec;
using std::string;


namespace dxrt {



bool has_error(const p_corr_err_t& o)
{
    if (o.rx_err_status) return false;
    if (o.bad_tlp_status) return false;
    if (o.bad_dllp_status) return false;
    if (o.replay_no_roleover_status) return false;
    if (o.rpl_timer_timeout_status) return false;
    if (o.advisory_non_fatal_err_status) return false;
    if (o.corrected_int_err_status) return false;
    if (o.header_log_overflow_status) return false;
    return true;
}
bool has_error(const p_fatal_err_t& o)
{
    if (o.dl_protocol_err_status) return false;
    if (o.surprise_down_err_status) return false;
    if (o.fc_protocol_err_status) return false;
    if (o.rec_overflow_err_status) return false;
    if (o.malf_tlp_err_status) return false;
    if (o.internal_err_status) return false;
    return true;
}


bool has_error(const p_nonfatal_err_t& o)
{
    if (o.pois_tlp_err_status) return false;
    if (o.cmplt_timeout_err_status) return false;
    if (o.cmplt_abort_err_status) return false;
    if (o.ecrc_err_status) return false;
    if (o.unsupported_req_err_status) return false;
    if (o.tlp_prfx_blocked_err_status) return false;
    return true;
}
bool has_error(const dxrt_pcie_err_stat_t& e)
{
    if (has_error(e.corr)) return false;
    if (has_error(e.fatal)) return false;
    if (has_error(e.non_fatal)) return false;
    return true;
}

ostream& operator<<(ostream& os, const p_corr_err_t& o)
{
    if (o.rx_err_status) os << " RxErr";
    if (o.bad_tlp_status) os << " BadTLP";
    if (o.bad_dllp_status) os << " BadDLLP";
    if (o.replay_no_roleover_status) os << " Rollover";
    if (o.rpl_timer_timeout_status) os << " Timeout";
    if (o.advisory_non_fatal_err_status) os << " AdvNonFatalErr";
    if (o.corrected_int_err_status) os << " IntErr";
    if (o.header_log_overflow_status) os <<  "HeaderOF";
    return os;
}
ostream& operator<<(ostream& os, const p_fatal_err_t& o)
{
    if (o.dl_protocol_err_status) os << " DLP";
    if (o.surprise_down_err_status) os << " SDES";
    if (o.fc_protocol_err_status) os << " FCP";
    if (o.rec_overflow_err_status) os << " RxOF";
    if (o.malf_tlp_err_status) os << " MalfTLP";
    if (o.internal_err_status) os << " IntErr";
    return os;
}


ostream& operator<<(ostream& os, const p_nonfatal_err_t& o)
{
    if (o.pois_tlp_err_status) os << " TLP";
    if (o.cmplt_timeout_err_status) os << " CmplTO";
    if (o.cmplt_abort_err_status) os << " UnxCmpl";
    if (o.ecrc_err_status) os << " ECRC";
    if (o.unsupported_req_err_status) os << " UnSupReq";
    if (o.tlp_prfx_blocked_err_status) os << " TLPBlock";
    return os;
}

ostream& operator<<(ostream& os, const dxrt_pcie_err_stat_t& o)
{
    if (has_error(o))
        os <<"Errors:" << o.corr << o.fatal << o.non_fatal;
    else
        os <<"Errors: None";
    return os;
}
ostream& operator<<(ostream& os, const p_evt_by_lane& e)
{
    os << hex << "EBUF_OF:0x" << e.ebuf_ovfl << " EBUF_UF:0x" << e.ebuf_unfl << " DecodeErr:0x" << e.decode_err
      << " SKP_PARITY:0x" << e.skp_os_parity_err << " DisparityErr:0x" << e.disparity_err
      << " SyncErr:0x" << e.sync_header_err;
    return os;
}
ostream& operator<<(ostream& os, const p_evt_common& e)
{
    os << hex << "EI:0x" << e.detect_ei << " RxErr:0x" << e.rx_err << " RxRec:0x" << e.rx_recovery_req << " NFTS_TO:0x" << e.n_fts_tout
      << " FramingErr:0x" << e.framing_err << " Deskew:0x" << e.deskew_err << " BadTLP:0x" << e.bad_tlp << " LCRC:0x" << e.lcrc_err
      << " BadDLLP:0x" << e.bad_dllp << " ROLLOVER:0x" << e.replay_num_rollover << endl << " ReplayTO:0x" << e.replay_tout
      << " RxNak:0x" << e.rx_nak_dllp << " TxNak:0x" << e.tx_nak_dllp << " ReTLP:0x" << e.retry_tlp << " FC_TO:0x" << e.fc_tout
      << " PoisonTLP:0x" << e.poisoned_tlp << " ECRC: "<< e.ecrc_err << " UA:0x" << e.ua << " CA:0x" << e.ca << " CmplTo:0x" << e.c_tout;
    return os;
}
ostream& operator<<(ostream& os, const dxrt_pcie_evt_stat_t& e)
{
    for(int i = 0; i < 4;i++)
    {
        os << "Lane "<< i<< ":" << e.lane[i] << endl;
    }
    os << "Common: " << endl << e.common;
    return os;
}

static constexpr std::array<pair_type, 4> pstate_arr = {{{0, "P0"}, {1, "P0s"},{2,"P1"},{3,"P2"}}};
static constexpr std::array<pair_type, 4> dstate_arr = {{{0, "D0"}, {1, "D1"},{2,"D2"},{3,"D3Hot"}}};
static constexpr std::array<pair_type, 23> lstate_arr = {{{0, "IDLE"}, {1, "L0"},{2,"L0S"},{3,"ENTER_L0S"},{4,"EXIT_L0S"},{5,"WAIT_PMCSR_CPL_SEND"},{8,"L1"},
    {9,"L1_BLOCK_TLP"},{10,"L1_WAIT_LAST_TLP_ACK"},{11,"L1_WAIT_PMDLLP_ACK"},{12,"L1_LINK_ENTR_L1"},{13,"L1_EXIT"},{15,"PREP_L1"},
    {16,"L23_BLOCK_TLP"},{17,"L23_WAIT_LAST_TLP_ACK"},{18,"L23_WAIT_PMDLLP_ACK"}, {19,"L23_ENTR_L23"}, {20,"L23_RDY"},
    {21,"PREP_4L23"},{22,"L23RDY_WAIT4ALIVE"},{23,"LOS_BLOCK_TLP"},{24,"WAIT_LAST_PMDLLP"},{25,"WAIT_DSTATE_UPDATE"}}};



ostream& operator<<(ostream& os, const dxrt_pcie_power_stat_t& e)
{
    os << "P-State " << map_lookup(pstate_arr,e.p_state) << " D-state " << map_lookup(dstate_arr, e.d_state) 
    << " L_state " << map_lookup(lstate_arr, e.l_state)  << endl;
    return os;
}



static constexpr std::array<pair_type, 3> cs_arr = {{{1,"RUNNING"},{2,"HALTED"},{3,"STOPPED"}}};


uint64_t combine_uint(uint32_t msb, uint32_t lsb)
{
    uint64_t a = static_cast<uint64_t>(msb);
    uint64_t b = static_cast<uint64_t>(lsb);
    return (a << 32) + b;
}


ostream& operator<<(ostream& os, const dma_ch& c)
{
    os << dec << "CS:" << map_lookup(cs_arr,c.cs) << " CB:" << c.cb << " TCB:"<< c.tcb << " LLP:" << c.llp
      << " LIE:" << c.lie << hex << " Func:"<< c.func_num << " TC:0x" << c.tc_tlp_header << " AT:0x" << c.at_tlp_header << " Size:0x" << c.t_size
      << " SAR:0x" << combine_uint(c.sar_msb, c.sar_lsb) << " DAR:0x" << combine_uint(c.dar_msb,c.dar_lsb) << " LLP:0x" << combine_uint(c.llp_msb,c.llp_lsb);
    return os;

}

static constexpr std::array<pair_type,2> phy_stat_arr = {{{0,"Link Down"},{1,"Link Up"}}};
static constexpr std::array<pair_type,3> link_stat_arr = {{{0,"Not Active"},{1,"FC_INIT"},{3,"Active"}}};

ostream& operator<<(ostream& os, const dxrt_pcie_info_t& e)
{
    
    os << "PHY Status: "<< map_lookup(phy_stat_arr,e.phy_stat) << ", Link Status:"<< map_lookup(link_stat_arr,e.dll_stat) << endl;
    os << "Power Status: " << e.power_stat;
    os << "DMA R/W Channel Status:" << endl;
    for(int i = 0; i < 4;i++)
    {
        os << "r_ch["<< i << "] " << e.dma_stat.r_ch[i] << endl;
        os << "w_ch["<< i << "] " << e.dma_stat.w_ch[i] << endl; 
    }
    os << e.err_stat << endl << "Event count: " << endl << e.evt_stat;
    return os;
}


}  //namespace dxrt
