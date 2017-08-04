APRON=/home/apron/apron
POLY=/home/apron/newpolka
NUM=/home/apron/num
INCLUDES=-I $(APRON) -I /home/apron/itv -I $(NUM) 
POLY_INCLUDES=$(INCLUDES) -I $(POLY)
LIBS=-L $(APRON) -L $(POLY)
FILES=$(filter-out newpolka/test.c newpolka/test0.c newpolka/test1.c newpolka/test_environment.c, $(wildcard newpolka/*.c))
SRC_FILES=$(subst newpolka/, ,$(FILES))
APRON_OBJS=$(APRON)/ap_manager.o $(APRON)/ap_generic.o $(APRON)/ap_dimension.o $(APRON)/ap_generator0.o $(APRON)/ap_tcons0.c $(APRON)/ap_texpr0.o $(APRON)/ap_lincons0.o $(APRON)/ap_linexpr0.o $(APRON)/ap_linearize.o $(APRON)/ap_coeff.o $(APRON)/ap_scalar.o $(APRON)/ap_interval.o 
OBJS=$(APRON_OBJS) $(POLY)/tests/libFuzzer/test_poly.o $(POLY)/*.o /home/apron/itv/*.o

.PHONY: compile test

all: compile test
compile: 
	cd newpolka ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -c -g $(INCLUDES)  $(SRC_FILES) -DNUM_LONGLONGRAT ; \
	cd .. ; \
	cd apron ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -c -g $(INCLUDES) -DNUM_LONGDOUBLE *.c ; \
	rm -f test_texpr0.o ; \
	cd .. ; \
	cd itv ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -c -g -I $(NUM) -I $(APRON) -DNUM_MPZ *.c ; \
	rm -f test.o test2.o *_debug.o ; \
	cd ..

test:
	cd newpolka/tests/libFuzzer; \
	clang -fsanitize-coverage=trace-pc-guard $(POLY_INCLUDES) -O0 -c -g test_poly.c -DNUM_LONGLONGRAT; \
        number=$(start) ; while [ $${number} -le $(number) ] ; do \
		clang -lstdc++ -fsanitize=address -fsanitize-coverage=trace-pc-guard -DNUM_DOUBLE -I /usr/local/include $(POLY_INCLUDES) $(OBJS) $(LIBS) test_poly$${number}.c /home/libFuzzer.a -o test$${number} -lmpfr -lgmp; \
                startTime=`date +%s` ; \
		./test$${number} -max_len=10000 -detect_leaks=0 -rss_limit_mb=8192 -timeout=3600 -print_final_stats=1 MY_CORPUS/ SEED_CORPUS/; \
                endTime=`date +%s` ; \
                runtime=`expr $$endTime - $$startTime` ; \
                echo "Execution time: $$runtime sec for test_poly$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true

