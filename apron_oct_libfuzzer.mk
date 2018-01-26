APRON=/home/apron/apron/
OCTAGONS=/home/apron/octagons/
ITV=/home/apron/itv/
LIB_PATHS=-L $(ITV) -L $(APRON) -L $(OCTAGONS)

all: compile test
compile: 
	make CC=clang CFLAGS="-fsanitize-coverage=trace-pc-guard -fPIC -O0" CXX=clang++ CXXFLAGS="-fsanitize-coverage=trace-pc-guard -fPIC -O0"
	sudo make install

test:	tests/libFuzzer/*.c	
	cd tests/libFuzzer; \
    number=$(start) ; while [ $${number} -le $(number) ] ; do \
    	rm MY_CORPUS/* ; \
	    clang -lstdc++ -fsanitize-coverage=trace-pc-guard -I /usr/local/include -I $(APRON) -I $(OCTAGONS) $(LIB_PATHS) test.c test$${number}.c \
        /home/libFuzzer.a -o test$${number} -lmpfr -lgmp -lapron -litv $(lib) $(format) -DAP_OCT; \
		startTime=`date +%s%N` ; \
        ./test$${number} -max_len=10000 -detect_leaks=0 -rss_limit_mb=0 -max_total_time=1800 -timeout=3600 -seed=1046527 -print_final_stats=1 MY_CORPUS/ SEED_CORPUS/; \
		endTime=`date +%s%N` ; \
        runtime=$$((($$endTime - $$startTime)/1000000)) ; \
        echo "Execution time: $$runtime millisec for test_oct$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true
