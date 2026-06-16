#include "cpu_settings.h"
#include "cpu_enum_strings.h"
#include <nxemu-cpu/cpu_settings_identifiers.h>
#include <nxemu-loader/loader_settings_identifiers.h>
#include <common/json.h>
#include <common/json_util.h>
#include <nxemu-module-spec/base.h>
#include <nxemu-module-spec/cpu.h>
#include <yuzu_common/logging/log.h>
#include <yuzu_common/settings_enums.h>
#include <yuzu_common/settings_setting.h>
#include <yuzu_common/yuzu_assert.h>

extern IModuleSettings * g_settings;

CpuSettings cpuSettings;

namespace Settings
{

#define CPU_SETTING_ENUM_METADATA(EnumType, translation_type, ...)                             \
    template <>                                                                              \
    std::vector<std::pair<std::string, EnumType>> EnumMetadata<EnumType>::Canonicalizations() { \
        return {__VA_ARGS__};                                                                \
    }                                                                                        \
    template <>                                                                              \
    u32 EnumMetadata<EnumType>::Index() {                                                    \
        return static_cast<u32>(translation_type);                                           \
    }

CPU_SETTING_ENUM_METADATA(
    CpuBackend, CpuSettingTranslationType::CpuBackend,
    {CpuBackendToString(CpuBackend::Dynarmic), CpuBackend::Dynarmic},
    {CpuBackendToString(CpuBackend::Nce), CpuBackend::Nce});

CPU_SETTING_ENUM_METADATA(
    CpuAccuracy, CpuSettingTranslationType::CpuAccuracy,
    {CpuAccuracyToString(CpuAccuracy::Auto), CpuAccuracy::Auto},
    {CpuAccuracyToString(CpuAccuracy::Accurate), CpuAccuracy::Accurate},
    {CpuAccuracyToString(CpuAccuracy::Unsafe), CpuAccuracy::Unsafe},
    {CpuAccuracyToString(CpuAccuracy::Paranoid), CpuAccuracy::Paranoid});

#undef CPU_SETTING_ENUM_METADATA

#ifndef CANNOT_EXPLICITLY_INSTANTIATE
#define SETTING(TYPE, RANGED) template class Setting<TYPE, RANGED>
#define SWITCHABLE(TYPE, RANGED) template class SwitchableSetting<TYPE, RANGED>

SWITCHABLE(CpuBackend, true);
SWITCHABLE(CpuAccuracy, true);
SETTING(bool, false);
SWITCHABLE(bool, false);

#undef SETTING
#undef SWITCHABLE
#endif

} // namespace Settings

namespace
{
enum class SettingType
{
    CpuBackend,
    CpuAccuracy,
    BooleanSwitchable,
    BooleanSetting,
};

class CpuSetting
{
public:
    CpuSetting(const char * id, const char * section, const char * key, Settings::SwitchableSetting<CpuBackend, true> * val);
    CpuSetting(const char * id, const char * section, const char * key, Settings::SwitchableSetting<CpuAccuracy, true> * val);
    CpuSetting(const char * id, const char * section, const char * key, Settings::SwitchableSetting<bool> * val);
    CpuSetting(const char * id, const char * section, const char * key, Settings::Setting<bool> * val);

    const char * identifier;
    const char * json_section;
    const char * json_key;
    SettingType settingType;
    union
    {
        Settings::SwitchableSetting<CpuBackend, true> * cpuBackend;
        Settings::SwitchableSetting<CpuAccuracy, true> * cpuAccuracy;
        Settings::SwitchableSetting<bool> * booleanSwitchable;
        Settings::Setting<bool> * booleanSetting;
    } setting;
};

static CpuSetting settings[] = {
    {NXCpuSetting::CpuBackend, "cpu", "cpu_backend", &cpuSettings.cpu_backend},
    {NXCpuSetting::CpuAccuracy, "cpu", "cpu_accuracy", &cpuSettings.cpu_accuracy},
    {NXCpuSetting::CpuDebugMode, "cpu", "cpu_debug_mode", &cpuSettings.cpu_debug_mode},
    {NXCpuSetting::CpuoptPageTables, "cpu", "cpuopt_page_tables", &cpuSettings.cpuopt_page_tables},
    {NXCpuSetting::CpuoptBlockLinking, "cpu", "cpuopt_block_linking", &cpuSettings.cpuopt_block_linking},
    {NXCpuSetting::CpuoptReturnStackBuffer, "cpu", "cpuopt_return_stack_buffer", &cpuSettings.cpuopt_return_stack_buffer},
    {NXCpuSetting::CpuoptFastDispatcher, "cpu", "cpuopt_fast_dispatcher", &cpuSettings.cpuopt_fast_dispatcher},
    {NXCpuSetting::CpuoptContextElimination, "cpu", "cpuopt_context_elimination", &cpuSettings.cpuopt_context_elimination},
    {NXCpuSetting::CpuoptConstProp, "cpu", "cpuopt_const_prop", &cpuSettings.cpuopt_const_prop},
    {NXCpuSetting::CpuoptMiscIr, "cpu", "cpuopt_misc_ir", &cpuSettings.cpuopt_misc_ir},
    {NXCpuSetting::CpuoptReduceMisalignChecks, "cpu", "cpuopt_reduce_misalign_checks", &cpuSettings.cpuopt_reduce_misalign_checks},
    {NXCpuSetting::CpuoptFastmem, "cpu", "cpuopt_fastmem", &cpuSettings.cpuopt_fastmem},
    {NXCpuSetting::CpuoptFastmemExclusives, "cpu", "cpuopt_fastmem_exclusives", &cpuSettings.cpuopt_fastmem_exclusives},
    {NXCpuSetting::CpuoptRecompileExclusives, "cpu", "cpuopt_recompile_exclusives", &cpuSettings.cpuopt_recompile_exclusives},
    {NXCpuSetting::CpuoptIgnoreMemoryAborts, "cpu", "cpuopt_ignore_memory_aborts", &cpuSettings.cpuopt_ignore_memory_aborts},
    {NXCpuSetting::CpuoptUnsafeUnfuseFma, "cpu", "cpuopt_unsafe_unfuse_fma", &cpuSettings.cpuopt_unsafe_unfuse_fma},
    {NXCpuSetting::CpuoptUnsafeReduceFpError, "cpu", "cpuopt_unsafe_reduce_fp_error", &cpuSettings.cpuopt_unsafe_reduce_fp_error},
    {NXCpuSetting::CpuoptUnsafeIgnoreStandardFpcr, "cpu", "cpuopt_unsafe_ignore_standard_fpcr", &cpuSettings.cpuopt_unsafe_ignore_standard_fpcr},
    {NXCpuSetting::CpuoptUnsafeInaccurateNan, "cpu", "cpuopt_unsafe_inaccurate_nan", &cpuSettings.cpuopt_unsafe_inaccurate_nan},
    {NXCpuSetting::CpuoptUnsafeFastmemCheck, "cpu", "cpuopt_unsafe_fastmem_check", &cpuSettings.cpuopt_unsafe_fastmem_check},
    {NXCpuSetting::CpuoptUnsafeIgnoreGlobalMonitor, "cpu", "cpuopt_unsafe_ignore_global_monitor", &cpuSettings.cpuopt_unsafe_ignore_global_monitor},
};

CpuSetting::CpuSetting(const char * id, const char * section, const char * key, Settings::SwitchableSetting<CpuBackend, true> * val) :
    identifier(id), json_section(section), json_key(key), settingType(SettingType::CpuBackend), setting{.cpuBackend = val}
{
}

CpuSetting::CpuSetting(const char * id, const char * section, const char * key, Settings::SwitchableSetting<CpuAccuracy, true> * val) :
    identifier(id), json_section(section), json_key(key), settingType(SettingType::CpuAccuracy), setting{.cpuAccuracy = val}
{
}

CpuSetting::CpuSetting(const char * id, const char * section, const char * key, Settings::SwitchableSetting<bool> * val) :
    identifier(id), json_section(section), json_key(key), settingType(SettingType::BooleanSwitchable), setting{.booleanSwitchable = val}
{
}

CpuSetting::CpuSetting(const char * id, const char * section, const char * key, Settings::Setting<bool> * val) :
    identifier(id), json_section(section), json_key(key), settingType(SettingType::BooleanSetting), setting{.booleanSetting = val}
{
}

} // namespace

bool IsFastmemEnabled()
{
    if (g_settings->GetBool(NXCpuSetting::CpuDebugMode))
    {
        return g_settings->GetBool(NXCpuSetting::CpuoptFastmem);
    }
    return true;
}

void UpdateNceEnabled()
{
    const bool has_39bit = g_settings->GetBool(NXLoaderSetting::Has39BitAddressSpace);
    const bool is_nce =
        g_settings->GetInt(NXCpuSetting::CpuBackend) == static_cast<int32_t>(CpuBackend::Nce);
    if (is_nce && !IsFastmemEnabled())
    {
        LOG_WARNING(Common, "Fastmem is required to natively execute code in a performant manner, "
                            "falling back to Dynarmic");
    }
    if (is_nce && !has_39bit)
    {
        LOG_WARNING(Common,
                    "Program does not utilize 39-bit address space, unable to natively execute code");
    }
    g_settings->SetBool(NXCpuSetting::NceEnabled, IsFastmemEnabled() && is_nce && has_39bit);
}

bool AffectsNceEnabled(const char * setting)
{
    return strcmp(setting, NXCpuSetting::CpuBackend) == 0 ||
           strcmp(setting, NXCpuSetting::CpuDebugMode) == 0 ||
           strcmp(setting, NXCpuSetting::CpuoptFastmem) == 0 ||
           strcmp(setting, NXLoaderSetting::Has39BitAddressSpace) == 0;
}

void CpuSettingChanged(const char * setting, void * /*userData*/)
{
    for (const CpuSetting & cpuSetting : settings)
    {
        if (strcmp(cpuSetting.identifier, setting) != 0)
        {
            continue;
        }
        switch (cpuSetting.settingType)
        {
        case SettingType::CpuBackend:
            cpuSetting.setting.cpuBackend->SetValue(static_cast<CpuBackend>(g_settings->GetInt(setting)));
            break;
        case SettingType::CpuAccuracy:
            cpuSetting.setting.cpuAccuracy->SetValue(static_cast<CpuAccuracy>(g_settings->GetInt(setting)));
            break;
        case SettingType::BooleanSwitchable:
            cpuSetting.setting.booleanSwitchable->SetValue(g_settings->GetBool(setting));
            break;
        case SettingType::BooleanSetting:
            cpuSetting.setting.booleanSetting->SetValue(g_settings->GetBool(setting));
            break;
        default:
            UNIMPLEMENTED();
        }
    }
    if (AffectsNceEnabled(setting))
    {
        UpdateNceEnabled();
    }
}

void NceInputChanged(const char * setting, void * /*userData*/)
{
    if (AffectsNceEnabled(setting))
    {
        UpdateNceEnabled();
    }
}

void SetupCpuSetting(void)
{
    for (const CpuSetting & cpuSetting : settings)
    {
        switch (cpuSetting.settingType)
        {
        case SettingType::CpuBackend:
            cpuSetting.setting.cpuBackend->SetValue(cpuSetting.setting.cpuBackend->GetDefault());
            break;
        case SettingType::CpuAccuracy:
            cpuSetting.setting.cpuAccuracy->SetValue(cpuSetting.setting.cpuAccuracy->GetDefault());
            break;
        case SettingType::BooleanSwitchable:
            cpuSetting.setting.booleanSwitchable->SetValue(cpuSetting.setting.booleanSwitchable->GetDefault());
            break;
        case SettingType::BooleanSetting:
            cpuSetting.setting.booleanSetting->SetValue(cpuSetting.setting.booleanSetting->GetDefault());
            break;
        default:
            UNIMPLEMENTED();
        }
    }

    JsonValue root;
    JsonReader reader;
    const std::string json = g_settings->GetSectionSettings("nxemu-cpu");

    if (!json.empty() && reader.Parse(json.data(), json.data() + json.size(), root))
    {
        for (const CpuSetting & cpuSetting : settings)
        {
            JsonValue section = root[cpuSetting.json_section];
            if (!section.isObject())
            {
                continue;
            }
            JsonValue value = section[cpuSetting.json_key];
            switch (cpuSetting.settingType)
            {
            case SettingType::CpuBackend:
                if (value.isString())
                {
                    cpuSetting.setting.cpuBackend->SetValue(CpuBackendFromString(value.asString()));
                }
                break;
            case SettingType::CpuAccuracy:
                if (value.isString())
                {
                    cpuSetting.setting.cpuAccuracy->SetValue(CpuAccuracyFromString(value.asString()));
                }
                break;
            case SettingType::BooleanSwitchable:
                if (value.isBool())
                {
                    cpuSetting.setting.booleanSwitchable->SetValue(value.asBool());
                }
                break;
            case SettingType::BooleanSetting:
                if (value.isBool())
                {
                    cpuSetting.setting.booleanSetting->SetValue(value.asBool());
                }
                break;
            default:
                UNIMPLEMENTED();
            }
        }
    }

    for (const CpuSetting & cpuSetting : settings)
    {
        switch (cpuSetting.settingType)
        {
        case SettingType::CpuBackend:
            g_settings->SetDefaultInt(cpuSetting.identifier, static_cast<int32_t>(cpuSetting.setting.cpuBackend->GetDefault()));
            g_settings->SetInt(cpuSetting.identifier, static_cast<int32_t>(cpuSetting.setting.cpuBackend->GetValue()));
            break;
        case SettingType::CpuAccuracy:
            g_settings->SetDefaultInt(cpuSetting.identifier, static_cast<int32_t>(cpuSetting.setting.cpuAccuracy->GetDefault()));
            g_settings->SetInt(cpuSetting.identifier, static_cast<int32_t>(cpuSetting.setting.cpuAccuracy->GetValue()));
            break;
        case SettingType::BooleanSwitchable:
            g_settings->SetDefaultBool(cpuSetting.identifier, cpuSetting.setting.booleanSwitchable->GetDefault());
            g_settings->SetBool(cpuSetting.identifier, cpuSetting.setting.booleanSwitchable->GetValue());
            break;
        case SettingType::BooleanSetting:
            g_settings->SetDefaultBool(cpuSetting.identifier, cpuSetting.setting.booleanSetting->GetDefault());
            g_settings->SetBool(cpuSetting.identifier, cpuSetting.setting.booleanSetting->GetValue());
            break;
        default:
            UNIMPLEMENTED();
        }
        g_settings->RegisterCallback(cpuSetting.identifier, CpuSettingChanged, nullptr);
    }

    g_settings->SetDefaultBool(NXLoaderSetting::Has39BitAddressSpace, false);
    g_settings->SetBool(NXLoaderSetting::Has39BitAddressSpace, false);
    g_settings->RegisterCallback(NXLoaderSetting::Has39BitAddressSpace, NceInputChanged, nullptr);

    g_settings->SetDefaultBool(NXCpuSetting::NceEnabled, false);
    UpdateNceEnabled();
}

void SaveCpuSettings(void)
{
    typedef std::map<std::string, JsonValue> SectionMap;
    SectionMap sections;

    for (const CpuSetting & cpuSetting : settings)
    {
        switch (cpuSetting.settingType)
        {
        case SettingType::CpuBackend:
            if (cpuSetting.setting.cpuBackend->GetValue() != cpuSetting.setting.cpuBackend->GetDefault())
            {
                sections[cpuSetting.json_section][cpuSetting.json_key] =
                    CpuBackendToString(cpuSetting.setting.cpuBackend->GetValue());
            }
            break;
        case SettingType::CpuAccuracy:
            if (cpuSetting.setting.cpuAccuracy->GetValue() != cpuSetting.setting.cpuAccuracy->GetDefault())
            {
                sections[cpuSetting.json_section][cpuSetting.json_key] =
                    CpuAccuracyToString(cpuSetting.setting.cpuAccuracy->GetValue());
            }
            break;
        case SettingType::BooleanSwitchable:
            if (cpuSetting.setting.booleanSwitchable->GetValue() != cpuSetting.setting.booleanSwitchable->GetDefault())
            {
                sections[cpuSetting.json_section][cpuSetting.json_key] = cpuSetting.setting.booleanSwitchable->GetValue();
            }
            break;
        case SettingType::BooleanSetting:
            if (cpuSetting.setting.booleanSetting->GetValue() != cpuSetting.setting.booleanSetting->GetDefault())
            {
                sections[cpuSetting.json_section][cpuSetting.json_key] = cpuSetting.setting.booleanSetting->GetValue();
            }
            break;
        default:
            UNIMPLEMENTED();
        }
    }

    JsonValue root;
    for (const auto & [section, values] : sections)
    {
        if (values.size() > 0)
        {
            root[section] = values;
        }
    }
    g_settings->SetSectionSettings("nxemu-cpu", root.isNull() ? "" : JsonStyledWriter().write(root));
}
