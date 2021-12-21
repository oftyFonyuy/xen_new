#include "private.h"

#include <xen/lib/x86/cpu-policy.h>

int x86_cpu_policies_are_compatible(const struct cpu_policy *host,
                                    const struct cpu_policy *guest,
                                    struct cpu_policy_errors *err)
{
    struct cpu_policy_errors e = INIT_CPU_POLICY_ERRORS;
    int ret = -EINVAL;

#define NA XEN_CPUID_NO_SUBLEAF
#define FAIL_CPUID(l, s) \
    do { e.leaf = (l); e.subleaf = (s); goto out; } while ( 0 )
#define FAIL_MSR(m) \
    do { e.msr = (m); goto out; } while ( 0 )

    /* Migration log stub */
    // DPRINTF("Checking basic leaves\n");
    DPRINTF("guest basic max leaf: %d\n", guest->cpuid->basic.max_leaf);
    DPRINTF("host basic max leaf: %d\n", host->cpuid->basic.max_leaf);
    if ( guest->cpuid->basic.max_leaf > host->cpuid->basic.max_leaf )
        FAIL_CPUID(0, NA);

    /* Migration log stub */
    // DPRINTF("Checking feat leaves\n");
    DPRINTF("guest feat max sub leaf: %d\n", guest->cpuid->feat.max_subleaf);
    DPRINTF("host feat max sub leaf: %d\n", host->cpuid->feat.max_subleaf);
    if ( guest->cpuid->feat.max_subleaf > host->cpuid->feat.max_subleaf )
        FAIL_CPUID(7, 0);

    /* Migration log stub */
    // DPRINTF("Checking extd leaves\n");
    DPRINTF("guest extd max leaf: %d\n", guest->cpuid->extd.max_leaf);
    DPRINTF("host extd max leaf: %d\n", host->cpuid->extd.max_leaf);
    if ( guest->cpuid->extd.max_leaf > host->cpuid->extd.max_leaf )
        FAIL_CPUID(0x80000000, NA);

    /* TODO: Audit more CPUID data. */

    /* Migration log stub */
    // DPRINTF("Checking msr info\n");
    if ( ~host->msr->platform_info.raw & guest->msr->platform_info.raw )
        FAIL_MSR(MSR_INTEL_PLATFORM_INFO);

#undef FAIL_MSR
#undef FAIL_CPUID
#undef NA

    /* Success. */
    ret = 0;

 out:
    if ( err )
        *err = e;

    return ret;
}

/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
