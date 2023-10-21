# Nazara Localization

Nazara Localization is a [Nazara Engine](https://github.com/NazaraEngine/NazaraEngine) module that allows you to add localized text to your project.

You can use it in any kind of commercial and non-commercial applications without any restriction ([MIT license](http://opensource.org/licenses/MIT)).

## Authors

Sid - main developper

## How to Use

```
// Add Localization.hpp to your includes
#include <NazaraLocalization/Localization.hpp>

// main.cpp
{
    // Add Nz::Localization to the modules list
    Nz::Modules<Nz::Graphics, Nz::Localization,...> nazara;

    Nz::Localization::Instance()->LoadLocalizationFile("path_to_localization_file.csv");

    Nz::LocalizedText hello = "LOC_HELLO_WORLD";
    printf("text: %s\n", hello.c_str());
}
```


## Contribute

##### Don't hesitate to contribute to Nazara Engine by:
- Extending the [wiki](https://github.com/NazaraEngine/NazaraEngine/wiki)
- Submitting a patch to GitHub  
- Post suggestions/bugs on the forum or the [GitHub tracker](https://github.com/NazaraEngine/NazaraEngine/issues)    
- [Fork the project](https://github.com/NazaraEngine/NazaraEngine/fork) on GitHub and [push your changes](https://github.com/NazaraEngine/NazaraEngine/pulls)  
- Talking about Nazara Engine to other people, spread the word!  
- Doing anything else that might help us

## Links

[Discord](https://discord.gg/MvwNx73)  