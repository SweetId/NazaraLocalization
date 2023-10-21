#include <NazaraLocalization/Localization.hpp>

#include <Nazara/Core/StringExt.hpp>

#include <fstream>

namespace Nz
{
    Localization* Localization::s_instance = nullptr;

    Localization::Localization(Config /*config*/)
        : ModuleBase("Localization", this)
        , m_currentLocale(nullptr)
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

        m_locales.clear();
        SplitString(line, ";", [this](std::string_view str) {
            if (!str.empty())
                m_locales.push_back({ std::string(str) });
            return true;
        });

        size_t index = 0;
        while (std::getline(file, line))
        {
            std::vector<std::string_view> values;
            SplitString(line, ";", [&values](std::string_view str) {
                values.push_back(str);
                return true;
            });

            m_lookupTable[std::string(values[0])] = index++;
            for (size_t i = 0; i < m_locales.size() && i < values.size() - 1; ++i)
            {
                m_locales[i].localizedStrings.push_back(std::string(values[i + 1]));
            }
        }

        return true;
    }

    bool Localization::SetLocale(const std::string& locale)
    {
        auto it = std::find_if(m_locales.begin(), m_locales.end(), [locale](auto&& loc) { return loc.name == locale; });
        if (it == m_locales.end())
        {
            m_currentLocale = nullptr;
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

}  // end of namespace Nz
