#include <NazaraLocalization/Localization.hpp>

#include <Nazara/Core/StringExt.hpp>

#include <fstream>

namespace Nz
{
    Localization* Localization::s_instance = nullptr;

    Localization::Localization(Config /*config*/)
        : ModuleBase("Localization", this)
        , m_currentLocale(nullptr)
        , m_currentLocaleIndex(0)
    { }

    Localization::~Localization()
    {}

    bool Localization::LoadLocalizationFile(const std::filesystem::path& filepath)
    {
        std::ifstream file(filepath);
        if (!file)
            return false;

        // read header
        std::string line;
        std::getline(file, line);

        std::vector<size_t> locales;
        SplitString(line, ";", [&](std::string_view str) {
            if (!str.empty())
                locales.push_back(GetOrCreateLocale(str));
            return true;
        });

        while (std::getline(file, line))
        {
            std::vector<std::string_view> values;
            SplitString(line, ";", [&values](std::string_view str) {
                values.push_back(str);
                return true;
            });

            size_t lookup = GetOrCreateLookupIndex(values[0]);

            for (size_t i = 0; i < values.size() - 1; ++i)
            {
                if (m_locales[locales[i]].localizedStrings.size() <= lookup)
                    m_locales[locales[i]].localizedStrings.resize(lookup + 1);

                m_locales[locales[i]].localizedStrings[lookup] = std::string(values[i + 1]);
            }
        }

        // ensure all loaded locales have values (even if empty) for all lookup keys
        for (auto&& locale : m_locales)
            locale.localizedStrings.resize(m_lookupTable.size());
        return true;
    }

    bool Localization::SetLocale(const std::string& locale)
    {
        for (size_t i = 0; i < m_locales.size(); ++i)
        {
            if (m_locales[i].name == locale)
            {
                m_currentLocale = &m_locales[i];
                m_currentLocaleIndex = i;
                return true;
            }
        }
       
        return false;
    }

        m_currentLocale = &(*it);
        return true;
    }

    bool Localization::FindIndexForKey(std::string_view key, size_t& index) const
    {
        auto it = m_lookupTable.find(std::string(key));
        if (it == m_lookupTable.end())
            return false;

        index = it->second;
        return true;
    }

    const std::string& Localization::GetStringAtIndex(size_t index) const
    {
        static std::string empty = "";
        if (m_currentLocale == nullptr)
            return empty;

        return m_currentLocale->localizedStrings.at(index);
    }

    size_t Localization::GetOrCreateLocale(std::string_view locale)
    {
        for (size_t i = 0; i < m_locales.size(); ++i)
            if (m_locales[i].name == locale)
                return i;

        m_locales.emplace_back(std::string(locale));

        if (nullptr != m_currentLocale)
        {
            // make sure pointer is still valid (this will invalidate all LocalizedText)
            m_currentLocale = &m_locales[m_currentLocaleIndex];
        }

        return m_locales.size() - 1;
    }

    size_t Localization::GetOrCreateLookupIndex(std::string_view name)
    {
        auto it = m_lookupTable.find(std::string(name));
        if (it != m_lookupTable.end())
            return it->second;

        size_t index = m_lookupTable.size();
        m_lookupTable[std::string(name)] = index;
        return index;
    }

}  // end of namespace Nz
