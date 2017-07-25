INCLUDES=-I /home/apron/apron -I /home/apron/itv -I /home/apron/num -I /home/apron/octagons
OBJS=/home/apron/octagons/tests/libFuzzer/test_oct.o

all: compile test
compile: 
	cd octagons ; \
	clang $(INCLUDES) -O0 -c -g *.c -g -DNUM_LONGDOUBLE ; \
	rm oct_test.o ; \
	cd .. ; \
	cd apron ; \
	clang $(INCLUDES) -O0 -c -g *.c -DNUM_LONGDOUBLE ; \
	rm test_texpr0.o ; \
	cd ..    

test:	octagons/tests/libFuzzer/*.c	
	cd octagons/tests/libFuzzer; \
	clang $(INCLUDES) -O0 -c -g -DNUM_LONGDOUBLE test_oct.c; \
        number=$(start) ; while [ $${number} -le $(number) ] ; do \
		clang -lstdc++ -fsanitize=address -fsanitize-coverage=trace-pc-guard -DNUM_LONGDOUBLE -I /usr/local/include $(INCLUDES) $(OBJS) test_oct$${number}.c /home/libFuzzer.a -o test$${number} -lmpfr -lgmp; \ ; \
		startTime=`date +%s` ; \
                ./test$${number} -max_len=10000 -detect_leaks=0 -rss_limit_mb=8192 -timeout=1800 -print_final_stats=1 MY_CORPUS/ SEED_CORPUS/; \
		endTime=`date +%s` ; \
                runtime=`expr $$endTime - $$startTime` ; \
                echo "Execution time: $$runtime sec for test_oct$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true
