#include <NazaraLocalization/LocalizedText.hpp>
#include <NazaraLocalization/Localization.hpp>

#include <fmt/format.h>
#include <fmt/args.h>

namespace Nz
{

	LocalizedText::LocalizedText()
		: LocalizedText("")
	{ }

	LocalizedText::LocalizedText(std::string_view str)
		: m_str(str)
		, m_locale(nullptr)
	{}

	LocalizedText::LocalizedText(const char* str)
		: LocalizedText(std::string_view{ str })
	{}

	void LocalizedText::Update() const
	{
		if (m_locale != Nz::Localization::Instance()->m_currentLocale)
		{
			m_cachedStr = {};
			m_locale = Nz::Localization::Instance()->m_currentLocale;

			if (!m_index.has_value())
			{
				size_t index;
				if (Nz::Localization::Instance()->FindIndexForKey(m_str, index))
					m_index = index;
			}

			if (m_index.has_value())
			{
				m_cachedStr = BuildFormattedString();
			}
		}
	}

	std::string LocalizedText::BuildFormattedString() const
	{
		if (m_parameters.empty()) // No parameters, no formatting
			return m_locale->localizedStrings[m_index.value()];

		fmt::dynamic_format_arg_store<fmt::format_context> store;
		for (auto& p : m_parameters)
		{
			if (p.second)
			{
				std::visit(Overloaded{
					[&](auto&& value) {
						store.push_back(fmt::arg(p.second->c_str(), value));
					}
				}, p.first);
			}
			else
			{
				std::visit(Overloaded{
					[&](auto&& value) {
						store.push_back(value);
					}
				}, p.first);
			}
		}
		return fmt::vformat(m_locale->localizedStrings[m_index.value()], store);
	}

	const char* LocalizedText::c_str() const
	{
		Update();
		return (m_cachedStr) ? m_cachedStr->c_str() : m_str.c_str();
	}
	const char* LocalizedText::data() const
	{
		Update();
		return (m_cachedStr) ? m_cachedStr->data() : m_str.data();
	}
	size_t LocalizedText::length() const
	{
		Update();
		return (m_cachedStr) ? m_cachedStr->length() : m_str.length();
	}
	size_t LocalizedText::size() const
	{
		Update();
		return (m_cachedStr) ? m_cachedStr->size() : m_str.size();
	}

	const std::string& LocalizedText::ToString() const
	{
		Update();
		return (m_cachedStr) ? m_cachedStr.value() : m_str;
	}
}