#pragma once

#include <Nazara/Core/Core.hpp>
#include <NazaraLocalization/Config.hpp>

#include <map>

namespace Nz
{
    class NAZARA_LOCALIZATION_API Localization : public Nz::ModuleBase<Localization>
    {
        friend ModuleBase;

    public:
        using Dependencies = TypeList<Core>;
        struct Config {};

        Localization(Config config);
        ~Localization();

        // Loads a CSV containing the localization strings
        bool LoadLocalizationFile(const std::filesystem::path& filepath);

        // Changes the locale
        bool SetLocale(const std::string& locale);

        bool FindIndexForKey(std::string_view key, size_t& index) const;
        const std::string& GetStringAtIndex(size_t index) const;

    private:
        struct Locale
        {
            std::string name;
            std::vector<std::string> localizedStrings;
        };

        std::vector<Locale> m_locales;
        std::unordered_map<std::string, size_t> m_lookupTable;
        
        Locale* m_currentLocale;

        static Localization* s_instance;

        friend class LocalizedText;
    };
}
