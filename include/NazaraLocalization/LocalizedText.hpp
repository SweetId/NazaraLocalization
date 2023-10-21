#pragma once

#include <NazaraLocalization/Config.hpp>
#include <NazaraLocalization/Localization.hpp>

#include <variant>
#include <optional>
#include <ostream>
#include <string_view>

namespace Nz
{
	class NAZARA_LOCALIZATION_API LocalizedText
	{
	public:
		LocalizedText();
		LocalizedText(std::string_view str);

		template <typename T> LocalizedText& Arg(T&& v)
		{
			NamedParameter p;
			p.first = v;
			m_parameters.push_back(std::move(p));
			return *this;
		}
		template <> LocalizedText& Arg(const char*&& v) { return Arg(std::string(v)); }

		template <typename T> LocalizedText& Arg(std::string_view name, T&& v)
		{
			NamedParameter p;
			p.first = v;
			p.second = name;
			m_parameters.push_back(std::move(p));
			return *this;
		}
		template <> LocalizedText& Arg(std::string_view name, const char*&& v) { return Arg(name, std::string(v)); }


		const char* c_str() const;
		const char* data() const;
		size_t length() const;
		size_t size() const;

		const std::string& ToString() const;
		const std::string& GetBaseString() const { return m_str; }

		// == operator compares the key only
		friend auto operator==(const LocalizedText& A, const LocalizedText& B) { return A.m_str == B.m_str; }
		// <=> operator compares localized string
		friend auto operator<=>(const LocalizedText& A, const LocalizedText& B) { return A.ToString() <=> B.ToString(); }

		friend std::ostream& operator<<(std::ostream& out, const Nz::LocalizedText& dt)
		{
			return out << dt.ToString();
		}

	private:
		void Update() const;
		std::string BuildFormattedString() const;

		// Cache for localization
		mutable std::optional<size_t> m_index; // index in the lookup array
		mutable Localization::Locale* m_locale; // current locale to avoid fetching pointer every time
		mutable std::optional<std::string> m_cachedStr; // computed string (loca + parameters formatting)

		std::string m_str;

		using Parameter = std::variant<
			Int8, Int16, Int32, Int64,
			UInt8, UInt16, UInt32, UInt64,
			float, double,
			std::string
			>;
		using NamedParameter = std::pair<Parameter, std::optional<std::string>>;
		std::vector<NamedParameter> m_parameters;
	};
}
