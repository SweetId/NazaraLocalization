
target("NzLocalization-demo")
	set_group("Examples")
	add_files("main.cpp")
	add_packages("nazara")
	add_deps("NazaraLocalization")
	set_rundir(".")