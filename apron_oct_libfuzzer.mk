APRON=/home/apron/apron/
OCTAGONS=/home/apron/octagons/
ITV=/home/apron/itv/
LIB_PATHS=-L $(ITV) -L $(APRON) -L $(OCTAGONS)

all: compile test
compile: 
	make CC=clang CFLAGS="-fsanitize-coverage=trace-pc-guard -fPIC -O0" CXX=clang++ CXXFLAGS="-fsanitize-coverage=trace-pc-guard -fPIC -O0"
	sudo make install

test:	octagons/tests/libFuzzer/*.c	
	cd octagons/tests/libFuzzer; \
    number=$(start) ; while [ $${number} -le $(number) ] ; do \
	    clang -lstdc++ -fsanitize=address -fsanitize-coverage=trace-pc-guard -I /usr/local/include -I $(OCTAGONS) $(num) $(LIB_PATHS) test_oct.c test_oct$${number}.c \
        /home/libFuzzer.a -o test$${number} -lmpfr -lgmp -lapron -litv $(lib); \
		startTime=`date +%s` ; \
        ./test$${number} -max_len=10000 -detect_leaks=0 -rss_limit_mb=8192 -timeout=1800 -print_final_stats=1 MY_CORPUS/ SEED_CORPUS/; \
		endTime=`date +%s` ; \
        runtime=`expr $$endTime - $$startTime` ; \
        echo "Execution time: $$runtime sec for test_oct$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true
