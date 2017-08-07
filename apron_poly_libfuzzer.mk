APRON=/home/apron/apron
POLY=/home/apron/newpolka
NUM=/home/apron/num
ITV=/home/apron/itv
INCLUDES=-I $(APRON) -I $(ITV) -I $(NUM) 
POLY_INCLUDES=$(INCLUDES) -I $(POLY)
LIB_PATHS=-L $(ITV)
LIBS=-lmpfr -lgmp -litvIl -litvIll -litvRl -litvRll -litvMPZ -litvMPQ -litvDl -litvMPFR
FILES=$(filter-out newpolka/test.c newpolka/test0.c newpolka/test1.c newpolka/test_environment.c, $(wildcard newpolka/*.c))
SRC_FILES=$(subst newpolka/, ,$(FILES))
ITV_FILES=itv.c itv_linearize.c itv_linexpr.c
APRON_OBJS=$(APRON)/ap_manager.o $(APRON)/ap_generic.o $(APRON)/ap_dimension.o $(APRON)/ap_generator0.o $(APRON)/ap_tcons0.c $(APRON)/ap_texpr0.o $(APRON)/ap_lincons0.o $(APRON)/ap_linexpr0.o $(APRON)/ap_linearize.o $(APRON)/ap_coeff.o $(APRON)/ap_scalar.o $(APRON)/ap_interval.o 
OBJS=$(APRON_OBJS) $(POLY)/tests/libFuzzer/test_poly.o $(POLY)/*.o

.PHONY: compile test

all: compile test
compile: 
	cd newpolka ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -c -g $(INCLUDES)  $(SRC_FILES) -DNUM_LONGLONGRAT ; \
	cd .. ; \
	cd itv ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -I $(NUM) -I $(APRON) -DNUM_LONGINT -shared -fPIC -o libitvIl.so $(ITV_FILES) ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -I $(NUM) -I $(APRON) -DNUM_LONGLONGINT -shared -fPIC -o libitvIll.so $(ITV_FILES) ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -I $(NUM) -I $(APRON) -DNUM_LONGRAT -shared -fPIC -o libitvRl.so $(ITV_FILES) ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -I $(NUM) -I $(APRON) -DNUM_LONGLONGRAT -shared -fPIC -o libitvRll.so $(ITV_FILES) ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -I $(NUM) -I $(APRON) -DNUM_MPZ  -shared -fPIC -o libitvMPZ.so $(ITV_FILES) ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -I $(NUM) -I $(APRON) -DNUM_MPQ -shared -fPIC -o libitvMPQ.so $(ITV_FILES) ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -I $(NUM) -I $(APRON) -DNUM_DOUBLE -shared -fPIC -o libitvD.so $(ITV_FILES) ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -I $(NUM) -I $(APRON) -DNUM_LONGDOUBLE -shared -fPIC -o libitvDl.so $(ITV_FILES) ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -I $(NUM) -I $(APRON) -DNUM_MPFR -shared -fPIC -o libitvMPFR.so $(ITV_FILES) ; \
	cd .. ; \
	cd apron ; \
	clang -fsanitize=address -fsanitize-coverage=trace-pc-guard -O0 -c -g $(INCLUDES) -DNUM_LONGDOUBLE *.c; \
	rm -f test_texpr0.o ; \
	cd .. ; \
	

test:
	cd newpolka/tests/libFuzzer; \
	clang -fsanitize-coverage=trace-pc-guard $(POLY_INCLUDES) -O0 -c -g test_poly.c -DNUM_LONGLONGRAT; \
        number=$(start) ; while [ $${number} -le $(number) ] ; do \
		clang -lstdc++ -fsanitize=address -fsanitize-coverage=trace-pc-guard -DNUM_DOUBLE -I /usr/local/include $(POLY_INCLUDES) $(OBJS) $(LIB_PATHS) test_poly$${number}.c /home/libFuzzer.a -o test$${number} $(LIBS) ; \
        startTime=`date +%s` ; \
		./test$${number} -max_len=10000 -detect_leaks=0 -rss_limit_mb=8192 -timeout=3600 -print_final_stats=1 MY_CORPUS/ SEED_CORPUS/; \
        endTime=`date +%s` ; \
        runtime=`expr $$endTime - $$startTime` ; \
        echo "Execution time: $$runtime sec for test_poly$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true

