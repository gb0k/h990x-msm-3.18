/* Copyright (c) 2017, Elliott Mitchell. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * version 3, or any later versions as published by the Free Software
 * Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <linux/string.h>
#include <linux/printk.h>
#include <linux/err.h>
#include <soc/qcom/smem.h>

#include "dirtysanta_fixup.h"


struct lge_smem_vendor0 {
	// following 2 fields are common to all devices
	int hw_rev;
	char model_name[10]; // "MSM8996_H1"
	// following fields depend on the device
	char nt_code[2048];
	char lg_model_name[22];
	int sim_num;
	int flag_gpio;

	// the structure in memory is 2096 bytes, likely for alignment
};


static char last[32]="";

static const char dev_name[]=CONFIG_DIRTYSANTA_FIXUP_DEVICENAME;
static const char sim_num=CONFIG_DIRTYSANTA_FIXUP_SIMCOUNT;


void dirtysanta_fixup_msm_modem(const char *caller)
{
	struct lge_smem_vendor0 *ptr;

	char *msg;

	unsigned size;

	if(IS_ERR_OR_NULL(ptr=smem_get_entry(SMEM_ID_VENDOR0, &size, 0, SMEM_ANY_HOST_FLAG))) {
		pr_info("Qualcomm smem not initialized as of \"%s()\"", caller);
		return;
	}

	if(size<sizeof(struct lge_smem_vendor0)) {
		pr_err("Memory area returned by smem_get_entry() too small, when called by \"%s()\"", caller);
		return;
	}

	if(!strcmp(ptr->lg_model_name, dev_name)) {
		if(ptr->sim_num==sim_num) {
			pr_info("Previous overwrite of VENDOR0 (\"%s()\") still effective.", last);
			return;
		}
	}

	if(last[0]) msg=KERN_INFO "Needing to overwrite VENDOR0 a second time from \"%s()\" (last was \"%s()\")";
	else msg=KERN_INFO "Overwriting VENDOR0 model name first time from \"%s()\"";

	printk(msg, caller, last);

	strlcpy(last, caller, sizeof(last));

	strcpy(ptr->lg_model_name, dev_name);
	ptr->sim_num=sim_num;
}
EXPORT_SYMBOL(dirtysanta_fixup_msm_modem);

