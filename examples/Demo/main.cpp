
#include <Nazara/Core/Application.hpp>

#include <NazaraLocalization/Localization.hpp>
#include <NazaraLocalization/LocalizedText.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
	NazaraUnused(argc);
	NazaraUnused(argv);

	Nz::Application<Nz::Localization> nazara;

	Nz::LocalizedText helloWorld("LOC_HELLO_WORLD");
	Nz::LocalizedText textWithArguments = Nz::LocalizedText("LOC_TEXT_ARG").Arg("PI").Arg("pi", Nz::Pi<float>);

	// Works with printf as if you were printing a string
	// There is no localization data loaded so it will just print the key
	printf("%s\n%s\n", helloWorld.c_str(), textWithArguments.c_str());

	// We load the CSV containing our data
	Nz::Localization::Instance()->LoadLocalizationFile("localization.csv");

	Nz::Localization::Instance()->SetLocale("fr-FR");
	std::cout << helloWorld << std::endl << textWithArguments << std::endl;
	Nz::Localization::Instance()->SetLocale("en-US");
	std::cout << helloWorld << std::endl << textWithArguments << std::endl;
	Nz::Localization::Instance()->SetLocale("pt-BR");
	std::cout << helloWorld << std::endl << textWithArguments << std::endl;

	return 0;
}