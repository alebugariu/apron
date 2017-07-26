APRON=/home/apron/apron
OCTAGONS=/home/apron/octagons
INCLUDES=-I $(APRON) -I /home/apron/itv -I /home/apron/num -I $(OCTAGONS)
LIBS=-L $(APRON) -L $(OCTAGONS)
OBJS=$(APRON)/ap_manager.o $(APRON)/ap_generic.o $(APRON)/ap_dimension.o $(APRON)/ap_generator0.o $(APRON)/ap_tcons0.c $(APRON)/ap_texpr0.o $(APRON)/ap_lincons0.o $(APRON)/ap_linexpr0.o $(APRON)/ap_linearize.o $(APRON)/ap_coeff.o $(APRON)/ap_scalar.o $(APRON)/ap_interval.o $(OCTAGONS)/tests/libFuzzer/test_oct.o $(OCTAGONS)/oct_representation.o $(OCTAGONS)/oct_predicate.o $(OCTAGONS)/oct_hmat.o $(OCTAGONS)/oct_transfer.o $(OCTAGONS)/oct_closure.o $(OCTAGONS)/oct_nary.o $(OCTAGONS)/oct_print.o $(OCTAGONS)/oct_resize.o 

all: compile test
compile: 
	cd octagons ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard $(INCLUDES) -O0 -c -g *.c -g -DNUM_LONGDOUBLE ; \
	rm oct_test.o ; \
	cd .. ; \
	cd apron ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard $(INCLUDES) -O0 -c -g *.c -DNUM_LONGDOUBLE ; \
	rm test_texpr0.o ; \
	cd ..    

test:	octagons/tests/libFuzzer/*.c	
	cd octagons/tests/libFuzzer; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard $(INCLUDES) -O0 -c -g -DNUM_LONGDOUBLE test_oct.c; \
        number=$(start) ; while [ $${number} -le $(number) ] ; do \
		clang -lstdc++ -fsanitize=address -fsanitize-coverage=trace-pc-guard -DNUM_LONGDOUBLE -I /usr/local/include $(INCLUDES) $(OBJS) $(LIBS) test_oct$${number}.c /home/libFuzzer.a -o test$${number} -lmpfr -lgmp -loctD -lapron; \
		startTime=`date +%s` ; \
                ./test$${number} -max_len=10000 -detect_leaks=0 -rss_limit_mb=8192 -timeout=1800 -print_final_stats=1 MY_CORPUS/ SEED_CORPUS/; \
		endTime=`date +%s` ; \
                runtime=`expr $$endTime - $$startTime` ; \
                echo "Execution time: $$runtime sec for test_oct$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true
