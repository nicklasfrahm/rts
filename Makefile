.PHONY: inversion
inversion:
	g++ inversion.cpp -l pthread -o inversion.elf
	sudo taskset -c 1 ./inversion.elf

.PHONY: inherit
inherit:
	g++ inherit.cpp -l pthread -o inherit.elf
	sudo taskset -c 1 ./inherit.elf

.PHONY: ceiling
ceiling:
	g++ ceiling.cpp -l pthread -o ceiling.elf
	sudo taskset -c 1 ./ceiling.elf