FILES=$(filter-out newpolka/test.c newpolka/test0.c newpolka/test1.c newpolka/test_environment.c newpolka/pk_approximate.c newpolka/pk_extract.c, $(wildcard newpolka/*.c))
SRC_FILES=$(subst newpolka/, ,$(FILES))
 
.PHONY: compile test

all: compile test
compile: 
	cd newpolka ; \
	clang -I /home/klee/klee_src/include -emit-llvm -O0 -c -g $(SRC_FILES) -g -DNUM_LONGLONGRAT ; \
	cd .. ; \
	cd apron ; \
	clang -I /home/klee/klee_src/include -emit-llvm -O0 -c -g -DNUM_LONGDOUBLE *.c ; \
	rm -f test_texpr0.bc ; \
	cd .. ; \
	cd itv ; \
	clang -I /home/klee/klee_src/include -emit-llvm -O0 -c -g -DNUM_MPZ *.c ; \
	rm -f test.bc test2.bc *_debug.bc ; \
	cd ..	       
 
test:
	cd newpolka/tests/klee; \
        rm -rf klee-*; \
	clang -I /home/klee/klee_src/include -I ../../ -I ../../../num -I ../../../apron -emit-llvm -O0 -c -g *.c -g -DNUM_LONGLONGRAT; \
        number=$(start) ; while [ $${number} -le $(number) ] ; do \
		llvm-link ../../*.bc ../../../apron/*.bc ../../../itv/itvRll.bc ../../../itv/itv_linexprRll.bc ../../../itv/itv_linearizeRll.bc test_poly.bc test_poly$${number}.bc -o test$${number}.bc ; \
                startTime=`date +%s` ; \
                klee -load=/usr/local/lib/libgmp.so.10.3.2 -allow-external-sym-calls -max-forks=300 -max-depth=7 -check-overshift=false -solver-backend=stp -sym-malloc-bound=512 test$${number}.bc ; \
                endTime=`date +%s` ; \
                runtime=`expr $$endTime - $$startTime` ; \
                echo "Execution time: $$runtime sec for test_poly$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true
