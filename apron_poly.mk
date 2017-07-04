FILES=$(filter-out newpolka/test.c newpolka/test0.c newpolka/test1.c newpolka/test_environment.c newpolka/pk_test.c newpolka/pk_approximate.c newpolka/pk_user.c newpolka/pk_extract.c, $(wildcard newpolka/*.c))
SRC_FILES=$(subst newpolka/, ,$(FILES))
 
.PHONY: compile test

all: compile test
compile: 
	cd newpolka ; \
	clang -I /home/klee/klee_src/include -emit-llvm -O0 -c -g $(SRC_FILES) -g -DNUM_LONGLONGRAT; \
	cd .. ; \
	cd apron ; \
	clang -I /home/klee/klee_src/include -emit-llvm -O0 -c -g -DNUM_LONGDOUBLE *.c; \
	rm test_texpr0.bc ; \
	cd ..	       
 
test:
	cd newpolka/tests; \
        #rm -rf klee-*; \
	clang -I /home/klee/klee_src/include -I ../ -I ../../num -I ../../apron -emit-llvm -O0 -c -g *.c -g -DNUM_LONGLONGRAT; \
        number=$(start) ; while [ $${number} -le $(number) ] ; do \
		llvm-link ../*.bc ../../apron/*.bc test_poly.bc test_poly$${number}.bc -o test$${number}.bc ; \
                startTime=`date +%s` ; \
                klee -allow-external-sym-calls -max-forks=100 -solver-backend=stp -sym-malloc-bound=128 test$${number}.bc ; \
                endTime=`date +%s` ; \
                runtime=`expr $$endTime - $$startTime` ; \
                echo "Execution time: $$runtime sec for test_poly$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true
