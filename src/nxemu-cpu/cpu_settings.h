#pragma once

#include <nxemu-module-spec/cpu.h>
#include <yuzu_common/settings.h>

struct CpuSettings
{
    Settings::Linkage linkage{};

    Settings::SwitchableSetting<CpuBackend, true> cpu_backend{linkage,
#ifdef HAS_NCE
                                                             CpuBackend::Nce,
#else
                                                             CpuBackend::Dynarmic,
#endif
                                                             CpuBackend::Dynarmic,
#ifdef HAS_NCE
                                                             CpuBackend::Nce,
#else
                                                             CpuBackend::Dynarmic,
#endif
                                                             "cpu_backend",
                                                             Settings::Category::Cpu};
    Settings::SwitchableSetting<CpuAccuracy, true> cpu_accuracy{linkage,
                                                                CpuAccuracy::Auto,
                                                                CpuAccuracy::Auto,
                                                                CpuAccuracy::Paranoid,
                                                                "cpu_accuracy",
                                                                Settings::Category::Cpu};
    Settings::SwitchableSetting<bool> cpu_debug_mode{linkage, false, "cpu_debug_mode", Settings::Category::CpuDebug};

    Settings::Setting<bool> cpuopt_page_tables{linkage, true, "cpuopt_page_tables", Settings::Category::CpuDebug};
    Settings::Setting<bool> cpuopt_block_linking{linkage, true, "cpuopt_block_linking", Settings::Category::CpuDebug};
    Settings::Setting<bool> cpuopt_return_stack_buffer{linkage, true, "cpuopt_return_stack_buffer", Settings::Category::CpuDebug};
    Settings::Setting<bool> cpuopt_fast_dispatcher{linkage, true, "cpuopt_fast_dispatcher", Settings::Category::CpuDebug};
    Settings::Setting<bool> cpuopt_context_elimination{linkage, true, "cpuopt_context_elimination", Settings::Category::CpuDebug};
    Settings::Setting<bool> cpuopt_const_prop{linkage, true, "cpuopt_const_prop", Settings::Category::CpuDebug};
    Settings::Setting<bool> cpuopt_misc_ir{linkage, true, "cpuopt_misc_ir", Settings::Category::CpuDebug};
    Settings::Setting<bool> cpuopt_reduce_misalign_checks{linkage, true, "cpuopt_reduce_misalign_checks", Settings::Category::CpuDebug};
    Settings::SwitchableSetting<bool> cpuopt_fastmem{linkage, true, "cpuopt_fastmem", Settings::Category::CpuDebug};
    Settings::SwitchableSetting<bool> cpuopt_fastmem_exclusives{linkage, true, "cpuopt_fastmem_exclusives", Settings::Category::CpuDebug};
    Settings::Setting<bool> cpuopt_recompile_exclusives{linkage, true, "cpuopt_recompile_exclusives", Settings::Category::CpuDebug};
    Settings::Setting<bool> cpuopt_ignore_memory_aborts{linkage, true, "cpuopt_ignore_memory_aborts", Settings::Category::CpuDebug};

    Settings::SwitchableSetting<bool> cpuopt_unsafe_unfuse_fma{linkage, true, "cpuopt_unsafe_unfuse_fma", Settings::Category::CpuUnsafe};
    Settings::SwitchableSetting<bool> cpuopt_unsafe_reduce_fp_error{linkage, true, "cpuopt_unsafe_reduce_fp_error", Settings::Category::CpuUnsafe};
    Settings::SwitchableSetting<bool> cpuopt_unsafe_ignore_standard_fpcr{linkage, true, "cpuopt_unsafe_ignore_standard_fpcr", Settings::Category::CpuUnsafe};
    Settings::SwitchableSetting<bool> cpuopt_unsafe_inaccurate_nan{linkage, true, "cpuopt_unsafe_inaccurate_nan", Settings::Category::CpuUnsafe};
    Settings::SwitchableSetting<bool> cpuopt_unsafe_fastmem_check{linkage, true, "cpuopt_unsafe_fastmem_check", Settings::Category::CpuUnsafe};
    Settings::SwitchableSetting<bool> cpuopt_unsafe_ignore_global_monitor{linkage, true, "cpuopt_unsafe_ignore_global_monitor", Settings::Category::CpuUnsafe};
};

extern CpuSettings cpuSettings;

void SetupCpuSetting(void);
void SaveCpuSettings(void);
