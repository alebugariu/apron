APRON=/home/apron/apron/
POLY=/home/apron/newpolka/
ITV=/home/apron/itv/
LIB_PATHS=-L $(ITV) -L $(APRON) -L $(POLY)

.PHONY: compile test

all: compile test
compile: 
	make CC=clang CFLAGS="-fsanitize-coverage=trace-pc-guard -fPIC -O0" CXX=clang++ CXXFLAGS="-fsanitize-coverage=trace-pc-guard -fPIC -O0"
	sudo make install
	
test:
	cd newpolka/tests/libFuzzer; \
    number=$(start) ; while [ $${number} -le $(number) ] ; do \
		clang -lstdc++ -fsanitize-coverage=trace-pc-guard -I /usr/local/include $(LIB_PATHS) test_poly.c test_poly$${number}.c \
			  /home/libFuzzer.a -o test$${number} -lmpfr -lgmp -lapron -litv $(lib); \
        startTime=`date +%s` ; \
		./test$${number} -max_len=10000 -detect_leaks=0 -rss_limit_mb=0 -max_total_time=3600 -print_final_stats=1 MY_CORPUS/ SEED_CORPUS/; \
        endTime=`date +%s` ; \
        runtime=`expr $$endTime - $$startTime` ; \
        echo "Execution time: $$runtime sec for test_poly$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true

