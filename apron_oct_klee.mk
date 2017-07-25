all: compile test
compile: 
	cd octagons ; \
	clang -I /home/klee/klee_src/include -emit-llvm -O0 -c -g *.c -g -DNUM_LONGDOUBLE ; \
	rm oct_test.bc ; \
	cd .. ; \
	cd apron ; \
	clang -I /home/klee/klee_src/include -emit-llvm -O0 -c -g *.c -DNUM_LONGDOUBLE ; \
	rm test_texpr0.bc ; \
	cd ..    

test:	octagons/tests/klee/*.c	
	cd octagons/tests/klee; \
	rm -rf klee-out-* ; \
	clang -I /home/klee/klee_src/include -I ../../ -I ../../../num -I ../../../apron -emit-llvm -O0 -c -g *.c -g -DNUM_LONGDOUBLE; \
        number=$(start) ; while [ $${number} -le $(number) ] ; do \
		llvm-link ../../*.bc ../../../apron/*.bc test_oct.bc test_oct$${number}.bc -o test$${number}.bc ; \
		startTime=`date +%s` ; \
                klee -allow-external-sym-calls -sym-malloc-bound=8192 -max-forks=100 -solver-backend=stp test$${number}.bc ; \
		endTime=`date +%s` ; \
                runtime=`expr $$endTime - $$startTime` ; \
                echo "Execution time: $$runtime sec for test_oct$$number\n" ; \
		number=`expr $$number + 1` ; \
    	done; \
        true
