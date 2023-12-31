#pragma once

#include <Nazara/Core/Core.hpp>
#include <NazaraLocalization/Config.hpp>

#include <map>

namespace Nz
{
    struct NAZARA_LOCALIZATION_API Locale
    {
        std::string name;
        std::vector<std::string> localizedStrings;
    };

    class NAZARA_LOCALIZATION_API Localization : public Nz::ModuleBase<Localization>
    {
        friend ModuleBase;

    public:
        using Dependencies = TypeList<Core>;
        struct Config {};

        NazaraStaticSignal(OnLocaleInstalled, std::string_view);

        Localization(Config config);
        ~Localization();

        // Loads a CSV containing the localization strings
        bool LoadLocalizationFile(const std::filesystem::path& filepath);

        // Changes the locale
        bool SetLocale(const std::string& locale);
        std::vector<std::string> GetInstalledLocales() const;

        bool FindIndexForKey(std::string_view key, size_t& index) const;
        const std::string& GetStringAtIndex(size_t index) const;

    private:
        size_t GetOrCreateLocale(std::string_view name);
        size_t GetOrCreateLookupIndex(std::string_view name);

        std::vector<Locale> m_locales;
        std::unordered_map<std::string, size_t> m_lookupTable;
        
        Locale* m_currentLocale;
        size_t m_currentLocaleIndex;

        static Localization* s_instance;

        friend class LocalizedText;
    };
}
