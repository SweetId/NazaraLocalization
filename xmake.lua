set_project("NazaraLocalization")

add_rules("mode.asan", "mode.tsan", "mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

add_repositories("nazara-engine-repo https://github.com/NazaraEngine/xmake-repo")
add_requires("nazarautils", "nzsl")
add_requires("nazaraengine", { alias = "nazara", debug = is_mode("debug") })
add_requires("fmt")

add_includedirs("include", "src")
set_languages("c89", "c++20")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")

if has_config("erronwarn") then
	set_warnings("allextra", "error")
else
	set_warnings("allextra")
end

if is_plat("mingw", "linux") then
	add_cxflags("-Wno-subobject-linkage")
end

if is_plat("windows") then
	add_defines("_CRT_SECURE_NO_WARNINGS")
	add_cxxflags("/bigobj", "/permissive-", "/Zc:__cplusplus", "/Zc:externConstexpr", "/Zc:inline", "/Zc:lambda", "/Zc:preprocessor", "/Zc:referenceBinding", "/Zc:strictStrings", "/Zc:throwingNew")
	add_cxflags("/w44062") -- Enable warning: switch case not handled
	add_cxflags("/wd4251") -- Disable warning: class needs to have dll-interface to be used by clients of class blah blah blah
	add_cxflags("/wd4275") -- Disable warning: DLL-interface class 'class_1' used as base for DLL-interface blah
elseif is_plat("mingw") then
	add_cxflags("-Og", "-Wa,-mbig-obj")
	add_ldflags("-Wa,-mbig-obj")
end

set_runtimes(is_mode("debug") and "MDd" or "MD")

if is_mode("asan", "tsan", "ubsan") then
	set_optimize("none") -- by default xmake will optimize asan builds
elseif is_mode("coverage") then
	if not is_plat("windows") then
		add_links("gcov")
	end
elseif is_mode("releasedbg") then
	set_fpmodels("fast")
	add_vectorexts("sse", "sse2", "sse3", "ssse3")
end

target("NazaraLocalization")
	add_packages("nazara", {public = true})
	add_packages("fmt", {public = false})
	set_kind("$(kind)")
	set_group("Libraries")
	add_headerfiles("include/(NazaraLocalization/**.hpp)")
	add_headerfiles("include/(NazaraLocalization/**.inl)")
	add_headerfiles("src/NazaraLocalization/**.h", { prefixdir = "private", install = false })
	add_headerfiles("src/NazaraLocalization/**.hpp", { prefixdir = "private", install = false })
	add_headerfiles("src/NazaraLocalization/**.inl", { prefixdir = "private", install = false })
	add_files("src/NazaraLocalization/**.cpp")

	-- for now only shared compilation is supported (except on platforms like wasm)
	if not is_plat("wasm") then
		set_kind("shared")
	else
		set_kind("static")
		add_defines("NAZARA_LOCALIZATION_STATIC", { public = true })
	end
	
	add_defines("NAZARA_LOCALIZATION_BUILD")
	if is_mode("debug") then
		add_defines("NAZARA_LOCALIZATION_DEBUG")
	end

includes("examples/xmake.lua")
