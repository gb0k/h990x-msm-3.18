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

#ifndef _DIRTYSANTA_FIXUP_H_
#define _DIRTYSANTA_FIXUP_H_
#ifdef CONFIG_DIRTYSANTA_FIXUP

/* used for loading from command-line, likely to be __init and void in future */
bool dirtysanta_fixup_loadcfg(void);

/* overwrites the Qualcomm "smem" area, fixing the modem binary driver */
void dirtysanta_fixup_msm_modem(const char *calling_function);

#define DIRTYSANTA_FIXUP_MSM_MODEM() dirtysanta_fixup_msm_modem(__func__)

#else

#define DIRTYSANTA_FIXUP_MSM_MODEM() do { } while(0)

#endif
#endif

