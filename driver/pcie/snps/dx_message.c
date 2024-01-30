// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022-2023 DeepX, Inc. and/or its affiliates.
 * DeepX eDMA PCIe driver
 *
 * Author: Taegyun An <atg@deepx.ai>
 */
#include <linux/mutex.h>
#include <linux/delay.h>

#include "dx_cdev_ctrl.h"
#include "dw-edma-core.h"
#include "dx_lib.h"
#include "version.h"
#include "dx_util.h"
#include "dw-edma-v0-debugfs.h"
#include "dx_pcie_api.h"

#define DX_MSG_ARG_SIZE			(1000)
#define DX_MSG_OFFS 			0x00000

#define DX_NPU0_RESPONSE_OFFS	(DX_MSG_OFFS + 0xA0)
#define DX_NPU1_RESPONSE_OFFS	(DX_MSG_OFFS + 0xD0)
#define DX_RT_REQUEST_QUEUE_OFFS    MSGRAM_REQUEST_OFFSET
#define DX_RT_RESPONSE_QUEUE_OFFS (DX_MSG_OFFS + 0x2000)

typedef struct _dx_msg_t {
	u32 cmd;	/* command */
	u32 dir;	/* Direction : Send - 1, Recv - 2 */
	u32 ack;	/* Response from device */
	u32 size;	/* Data Size */
	u32 data[DX_MSG_ARG_SIZE];
} dx_msg_t;

#define DX_GET_MSG_SIZE(msg) (sizeof(dx_msg_t) - sizeof(msg->data) + msg->size)

static int dx_get_message(struct dw_edma *dw, u32 *dx_msg, u32 offs, u32 size)
{
	void __iomem *reg = dw->npu_region[0].vaddr;
	int i, ret = 0;
	if (!reg) {
		pr_err("[%s] reg address is null\n", __func__);
		return -EBADSLT;
	}
	reg += offs;

	for (i = 0; i<size/4; i++) {
		dx_msg[i] = readl(reg + i*4);
		if (dx_msg[i] == -EFAULT) { /* TODO - This condition should be considered */
			pr_err("[%s] PCIe message error\n", __func__);
			ret = -1;
			break;
		}
		// pr_err("%s(@%p, pos=%d) value = 0x%08x\n", __func__, reg + i*4, i, dx_msg[i]);
	}
	return ret;
}

static ssize_t dx_read_messages(int dev_id, dx_msg_t *dx_msg)
{
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	int ret = dx_get_message(dw, (u32 *)dx_msg, DX_MSG_OFFS, DX_GET_MSG_SIZE(dx_msg));
	return ret;
}

static int dx_set_message(struct dw_edma *dw, u32 *dx_msg, u32 offs, u32 size)
{
	void __iomem *reg = dw->npu_region[0].vaddr;
	int i;
	if (!reg) {
		pr_err("[%s] reg address is null\n", __func__);
		return -EBADSLT;
	}
	reg += offs;

	for (i = 0; i<size/4; i++) {
		writel(dx_msg[i], reg + i*4);
		// pr_err("%s(@%p, pos=%d) value = 0x%08x\n", __func__, reg + i*4, i, dx_msg[i]);
	}
	return 0;
}

static ssize_t dx_send_messages(int dev_id, dx_msg_t *dx_msg)
{
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	int ret = dx_set_message(dw, (u32 *)dx_msg, DX_MSG_OFFS, DX_GET_MSG_SIZE(dx_msg));
	return ret;
}

// static spinlock_t temp_lock;
int dx_pcie_send_message(u32 cmd, u32 dir, u32 size, u32 dev_id, void *data)
{
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	dx_msg_t dx_msg = {0x00,};
	int ret = 0;
	unsigned long flags;

	if (dw) {
		// spin_lock_irqsave(&temp_lock, flags);
		mutex_lock(&dw->message_lock);
		dx_msg.cmd = cmd;
		dx_msg.dir = dir;
		dx_msg.size = size;

		if (dir == DX_MSG_SEND) {
			memcpy(dx_msg.data, data, size);
			ret = dx_send_messages(dev_id, &dx_msg);
		} else if (dir == DX_MSG_RECV) {
			if (dx_send_messages(dev_id, &dx_msg) >= 0) {
				int fail_cnt = 0;
				while(true) { /* TODO - need to check timeout */
					msleep(10);
					if (fail_cnt>10 || dx_read_messages(dev_id, &dx_msg) < 0) {
						ret = -1;
						break;
					}
					if (dx_msg.ack == 1) {
						memcpy(data, dx_msg.data, size);
						ret = 0;
						break;
					}
					pr_info("%s: wait for ack. %d\n", __func__, fail_cnt);					
					fail_cnt++;
				}
				pr_debug("%s: got ack.\n", __func__);
			}
		} else {
			pr_err("Check PCIe message direction : %d\n", dir);
			ret = -1;
		}
		mutex_unlock(&dw->message_lock);
		// spin_unlock_irqrestore(&temp_lock, flags);
	}
	return ret;
}
EXPORT_SYMBOL_GPL(dx_pcie_send_message);

/* This function only get response datas from a ddr of device which is seprated from message section. */
int dx_pcie_get_response(u32 dev_id, u32 npu_id, void *info, u32 size)
{
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	void __iomem *reg = dw->npu_region[0].vaddr;
	int ret = 0, i;

	if (!reg)
		return -EBADSLT;
	if (npu_id == 0) {
		reg += DX_NPU0_RESPONSE_OFFS;
	} else if (npu_id == 1) {
		reg += DX_NPU1_RESPONSE_OFFS;
	} else {
		pr_err("[%s] undefined id (%d)", __func__, npu_id);
		return -1;
	}
	for (i=0; i<size/4; i++) {
		((u32 *)info)[i] = ioread32(reg + i*4);
	}
	return ret;
}
EXPORT_SYMBOL_GPL(dx_pcie_get_response);

void __iomem *dx_pcie_get_message_area(u32 dev_id)
{
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	void __iomem *reg = dw->npu_region[0].vaddr;
	return reg + MSGRAM_MSG_OFFSET;
}
EXPORT_SYMBOL_GPL(dx_pcie_get_message_area);
void __iomem *dx_pcie_get_log_area(u32 dev_id)
{
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	void __iomem *reg = dw->npu_region[0].vaddr;
	return reg + MSGRAM_LOG_OFFSET;
}
EXPORT_SYMBOL_GPL(dx_pcie_get_log_area);
void __iomem *dx_pcie_get_request_queue(u32 dev_id)
{
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	void __iomem *reg = dw->npu_region[0].vaddr;
	return reg + DX_RT_REQUEST_QUEUE_OFFS;
}
EXPORT_SYMBOL_GPL(dx_pcie_get_request_queue);

void __iomem *dx_pcie_get_response_queue(u32 dev_id, int dma_ch)
{
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	void __iomem *reg = dw->npu_region[0].vaddr;
	return reg + DX_RT_RESPONSE_QUEUE_OFFS + dma_ch*0x1000;
}
EXPORT_SYMBOL_GPL(dx_pcie_get_response_queue);

int dx_pcie_clear_response_queue(u32 dev_id)
{
	int i;
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	for(i=0;i<2;i++)
    {
        dx_pcie_response_list_t *entry, *tmp;
        spin_lock(&dw->responses_lock[i]);
        list_for_each_entry_safe(entry, tmp, &dw->responses[i].list, list) {
            list_del(&entry->list);
            kfree(entry);
        }
        spin_unlock(&dw->responses_lock[i]);
    }
	return 0;
}
EXPORT_SYMBOL_GPL(dx_pcie_clear_response_queue);

int dx_pcie_is_response_queue_empty(u32 dev_id, int dma_ch)
{
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	if(list_empty(&dw->responses[dma_ch].list))
		return 1;
	else
		return 0;
}
EXPORT_SYMBOL_GPL(dx_pcie_is_response_queue_empty);

int dx_pcie_enqueue_response(u32 dev_id, int dma_ch, dx_pcie_response_t* response)
{
	int ret;
	unsigned long flags;
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	spin_lock_irqsave(&dw->responses_lock[dma_ch], flags);
	/* TODO */
	spin_unlock_irqrestore(&dw->responses_lock[dma_ch], flags);
	return ret;
}
EXPORT_SYMBOL_GPL(dx_pcie_enqueue_response);

int dx_pcie_dequeue_response(u32 dev_id, int dma_ch, dx_pcie_response_t* response)
{
	int ret;
	unsigned long flags;
	struct dw_edma *dw = dx_dev_list_get(dev_id);
	spin_lock_irqsave(&dw->responses_lock[dma_ch], flags);
	if(!list_empty(&dw->responses[dma_ch].list))
	{
		dx_pcie_response_list_t *entry = list_first_entry(&dw->responses[dma_ch].list, dx_pcie_response_list_t, list);
		dx_pcie_response_t *src = &entry->response;
		pr_debug("%s: dev_id %d, ch %d, %d", __func__, dw->idx, dma_ch, src->req_id);
		memcpy(response, src, sizeof(dx_pcie_response_t));
		list_del(&entry->list);
		kfree(entry);
		ret = 0;
	}
	else
	{
		pr_debug("%s: dev_id %d, ch %d, empty", __func__, dw->idx, dma_ch);
		ret = -1;
	}
	spin_unlock_irqrestore(&dw->responses_lock[dma_ch], flags);
	return ret;
}
EXPORT_SYMBOL_GPL(dx_pcie_dequeue_response);

/* Debugging */
static u32 msg_cmd_debugfs = 0;
static ssize_t dx_pcie_message_read(struct file *file,
	char __user *buffer, size_t count, loff_t *ppos)
{
	switch (msg_cmd_debugfs)
	{
		case DX_PCIE_CMD_IDENTIFY_DEVICE:
			{
				msg_mem_info info;
				dx_pcie_send_message(msg_cmd_debugfs, DX_MSG_RECV, sizeof(msg_mem_info), 0, &info);
				printk("[CMD:%d] datas:%llx %llx\n",
					msg_cmd_debugfs, info.memAddr, info.memSize);
				break;
			}
		default:
				printk("[E] unknown command!\n");
			break;
	}
	return 0;
}

static ssize_t dx_pcie_message_write(struct file *file,
		 const char __user *user_buf, size_t count, loff_t *ppos)
{
	char buf[32];
	ssize_t len;
	int value;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	buf[len] = '\0';
	if (kstrtoint(buf, 0, &value))
		return -EINVAL;

	if (value < 0)
		return -EINVAL;

	msg_cmd_debugfs = value;
	return count;
}

static const struct file_operations dx_pcie_message_fops = {
    .read = dx_pcie_message_read,
    .write = dx_pcie_message_write,
};


void dw_edma_message_debugfs_on(struct dw_edma_chip *chip)
{
	struct dw_edma *dw = chip->dw;
	debugfs_create_file("message", 0666, dw->debugfs, NULL, &dx_pcie_message_fops);
}
