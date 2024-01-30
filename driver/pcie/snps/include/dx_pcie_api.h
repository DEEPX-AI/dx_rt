// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022-2023 DeepX, Inc. and/or its affiliates.
 * DeepX eDMA PCIe driver
 *
 * Author: Taegyun An <atg@deepx.ai>
 */

#ifndef _DX_PCIE_API_H
#define _DX_PCIE_API_H

#include <linux/poll.h>

#include "dw-edma-core.h"

enum mem_type {
	USER_SPACE_BUF   = 0,
	KERNEL_SPACE_BUF = 1,
};

typedef enum _dx_msg_dir_t {
	DX_MSG_SEND = 1,
	DX_MSG_RECV = 2,
} dx_msg_dir_t;

/* MSG : DX_PCIE_CMD_IDENTIFY_DEVICE */
typedef struct _msg_mem_info {
    uint64_t memAddr;
    uint64_t memSize;
} msg_mem_info;

enum pcie_msg_type {
	DX_PCIE_CMD_IDENTIFY_DEVICE,	/* Dir : Recv */
	DX_PCIE_CMD_NPU_RUN,			/* Dir : Send */
	DX_PCIE_CMD_MAX,
};

/* Response datas from device */
typedef struct pcie_output_info {
	u32 req_id;
    uint64_t data;
    uint64_t base;
    uint32_t offset;
    uint32_t size;
} __packed pcie_output_info ;

/* PCIe EXternal API */
extern void dx_sgdma_init(int dev_id);
extern void dx_sgdma_deinit(int dev_id);
extern ssize_t dx_sgdma_write(char *dest, u64 src, size_t count, int dev_id, int dma_ch, bool npu_run);
extern ssize_t dx_sgdma_read(char *src, u64 dest, size_t count, int dev_id, int dma_ch);
extern unsigned int dx_pcie_interrupt(int dev_id, int irq_id);
extern unsigned int dx_pcie_interrupt_wakeup(int dev_id, int irq_id);
extern int dx_pcie_send_message(u32 cmd, u32 dir, u32 size, u32 dev_id, void *data);
extern int dx_pcie_get_response(u32 dev_id, u32 npu_id, void *info, u32 size);
extern void __iomem *dx_pcie_get_message_area(u32 dev_id);
extern void __iomem *dx_pcie_get_log_area(u32 dev_id);
extern void __iomem *dx_pcie_get_request_queue(u32 dev_id);
extern void __iomem *dx_pcie_get_response_queue(u32 dev_id, int dma_ch);
extern int dx_pcie_clear_response_queue(u32 dev_id);
extern int dx_pcie_is_response_queue_empty(u32 dev_id, int dma_ch);
extern int dx_pcie_dequeue_response(u32 dev_id, int dma_ch, dx_pcie_response_t* response);
extern uint32_t dx_pcie_get_dev_num(void);

#endif /*_DX_PCIE_API_H*/
