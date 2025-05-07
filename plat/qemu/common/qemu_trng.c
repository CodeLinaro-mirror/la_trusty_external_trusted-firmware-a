/*
 * Copyright (c) 2025, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include <arch_features.h>
#include <arch_helpers.h>
#include <plat/common/platform.h>
#include <services/trng_svc.h>

__attribute__((target("arch=armv8.5-a+rng")))
bool plat_get_entropy(uint64_t *out)
{
	/*
	 * Read entropy from a freshly seeded RNDRRS register.
	 * WARNING!!! You SHOULD NOT use this on any platform other than qemu.
	 * The qemu implementation of RNDRRS pulls entropy from either
	 * getrandom or /dev/urandom which is secure, but actual hardware
	 * might use less secure randomness sources.
	 *
	 * The builtin returns 1 on failure and 0 on success.
	 */
	return !__builtin_arm_rndrrs(out);
}

DEFINE_SVC_UUID2(qemu_trng_uuid,
		 0xaa767875, 0xe700, 0x4d66, 0xbf, 0xb0,
		 0xdb, 0x35, 0xb4, 0xdf, 0x73, 0x25
);
uuid_t plat_trng_uuid;

void plat_entropy_setup(void)
{
	/* Use the RNDRRS instruction if the CPU supports it */
	if (is_feat_rng_supported()) {
		plat_trng_uuid = qemu_trng_uuid;
	}
}
