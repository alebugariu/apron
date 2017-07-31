APRON=/home/apron/apron
POLY=/home/apron/newpolka
INCLUDES=-I $(APRON) -I /home/apron/itv -I /home/apron/num -I $(POLY)
LIBS=-L $(APRON) -L $(POLY)
FILES=$(filter-out newpolka/test.c newpolka/test0.c newpolka/test1.c newpolka/test_environment.c newpolka/pk_approximate.c newpolka/pk_extract.c, $(wildcard newpolka/*.c))
SRC_FILES=$(subst newpolka/, ,$(FILES))
 
.PHONY: compile test

all: compile test
compile: 
	cd newpolka ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -c -g $(SRC_FILES) -DNUM_LONGLONGRAT ; \
	cd .. ; \
	cd apron ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -c -g -DNUM_LONGDOUBLE *.c ; \
	rm -f test_texpr0.bc ; \
	cd .. ; \
	cd itv ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -c -g -DNUM_MPZ *.c ; \
	rm -f test.bc test2.bc *_debug.bc ; \
	cd ..
 
test:
	cd newpolka/tests/libFuzzer; \
	clang -fsanitize-coverage=trace-pc-guard $(INCLUDES) -O0 -c -g test_poly.c -DNUM_LONGLONGRAT; \
        number=$(start) ; while [ $${number} -le $(number) ] ; do \
		#clang -lstdc++ -fsanitize=address -fsanitize-coverage=trace-pc-guard -DTHRESHOLD=0.75 -DNUM_DOUBLE -I /usr/local/include $(INCLUDES) $(OBJS) test_poly$${number}.c /home/libFuzzer.a -o test$${number} -lmpfr -lgmp; \
                startTime=`date +%s` ; \
		#./test$${number} -max_len=10000 -detect_leaks=0 -rss_limit_mb=8192 -timeout=3600 -print_final_stats=1 MY_CORPUS/ SEED_CORPUS/; \
                endTime=`date +%s` ; \
                runtime=`expr $$endTime - $$startTime` ; \
                echo "Execution time: $$runtime sec for test_poly$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true
