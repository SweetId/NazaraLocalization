#pragma once

#include <NazaraUtils/Prerequisites.hpp>

#if defined(NAZARA_LOCALIZATION_STATIC)
	#define NAZARA_LOCALIZATION_API
#else
	#ifdef NAZARA_LOCALIZATION_BUILD
	    #define NAZARA_LOCALIZATION_API NAZARA_EXPORT
	#else
        #define NAZARA_LOCALIZATION_API NAZARA_IMPORT
	#endif
#endif