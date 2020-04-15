.PHONY: run
run:
	g++ shed_test.cpp -l pthread -o shed_test.elf
	sudo taskset -c 1 ./shed_test.elf
