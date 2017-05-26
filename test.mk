number ?= 24
all:
	cd octagons/tests; \
	clang -I /home/klee/klee_src/include -I ../ -I ../../num -I ../../apron -emit-llvm -c -g *.c -g -DNUM_LONGINT; \
        number=$(number) ; while [ $${number} -ge 0 ] ; do \
		llvm-link ../*.bc ../../apron/*.bc test_oct.bc test_oct$${number}.bc -o test$${number}.bc ; \
                klee -allow-external-sym-calls -sym-malloc-bound=128 -max-depth=5 test$${number}.bc ; \
		number=`expr $$number - 1` ; \
    	done; \
        true
