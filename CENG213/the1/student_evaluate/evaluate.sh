#!/bin/bash


TASKS=(
CASE1
CASE2
CASE3
CASE4
CASE5
CASE6
CASE7
CASE8
CASE9
CASE10
CASE11
CASE12
CASE13
CASE14
CASE15
CASE16
CASE17
CASE18
CASE19
CASE20
CASE21
CASE22
CASE23
CASE24
CASE25
CASE26
CASE27
CASE28
CASE29
CASE30
CASE31
CASE32
CASE33
CASE34
CASE35
CASE36
CASE37
CASE38
)

MAIN_FILES=(
"Mainlist_01.cpp"
"Mainlist_02.cpp"
"Mainlist_03.cpp"
"Mainlist_04.cpp"
"Mainlist_05.cpp"
"Mainlist_06.cpp"
"Mainlist_07.cpp"
"Mainlist_08.cpp"
"Mainlist_09.cpp"
"Mainlist_10.cpp"
"Mainlist_11.cpp"
"Mainlist_12.cpp"
"Mainlist_13.cpp"
"Mainlist_14.cpp"
"Mainlist_15.cpp"
"MainVideoShare_01.cpp"
"MainVideoShare_02.cpp"
"MainVideoShare_03.cpp"
"MainVideoShare_04.cpp"
"MainVideoShare_05.cpp"
"MainVideoShare_06.cpp"
"MainVideoShare_07.cpp"
"MainVideoShare_08.cpp"
"MainVideoShare_09.cpp"
"MainVideoShare_10.cpp"
"MainVideoShare_11.cpp"
"MainVideoShare_12.cpp"
"MainVideoShare_13.cpp"
"MainVideoShare_14.cpp"
"MainVideoShare_15.cpp"
"MainVideoShare_16.cpp"
"MainVideoShare_17.cpp"
"MainVideoShare_18.cpp"
"MainVideoShare_19.cpp"
"MainVideoShare_20.cpp"
"MainVideoShare_21.cpp"
"MainVideoShare_22.cpp"
"MainVideoShare_23.cpp"
)

COMPILE_FILES=(
"Mainlist_01.cpp"
"Mainlist_02.cpp"
"Mainlist_03.cpp"
"Mainlist_04.cpp"
"Mainlist_05.cpp"
"Mainlist_06.cpp"
"Mainlist_07.cpp"
"Mainlist_08.cpp"
"Mainlist_09.cpp"
"Mainlist_10.cpp"
"Mainlist_11.cpp"
"Mainlist_12.cpp"
"Mainlist_13.cpp"
"Mainlist_14.cpp"
"Mainlist_15.cpp"
"MainVideoShare_01.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_02.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_03.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_04.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_05.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_06.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_07.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_08.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_09.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_10.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_11.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_12.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_13.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_14.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_15.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_16.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_17.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_18.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_19.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_20.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_21.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_22.cpp VideoShare.cpp Video.cpp User.cpp"
"MainVideoShare_23.cpp VideoShare.cpp Video.cpp User.cpp"
)

PNTS=(
4
1
2
4
2
4
2
4
2
1
4
2
2
5
1
2
2
2
2
1
2
1
3
1
1
2
1
1
2
2
2
2
3
8
6
8
3
3
)

rm -r logs
rm -r grade.txt

GRADE=0

for i in ${!TASKS[@]}; do
	mkdir -p logs/${TASKS[${i}]}
	cp mains/${MAIN_FILES[${i}]} logs/${TASKS[${i}]}/
	cp expected_outputs/${TASKS[${i}]}".out" logs/${TASKS[${i}]}/
	cp inputs/* logs/${TASKS[${i}]}/
	cp submit/* logs/${TASKS[${i}]}/
done


    for i in ${!TASKS[@]}; do

        printf "Testing ${TASKS[${i}]}(${PNTS[${i}]} pts.):" >> grade.txt
        
	cd logs/${TASKS[${i}]}

        g++ ${COMPILE_FILES[${i}]} -o ${TASKS[${i}]} 2> ${TASKS[${i}]}_compile.err
        # if compiled, test
        if [ $? -eq 0 ]; then
                    
            # to capture segmentation fault
            { timeout 3 stdbuf -i0 -o0 -e0 ./${TASKS[${i}]} > ${TASKS[${i}]}.myout 2> ${TASKS[${i}]}.err ; TIMEOUT=$?; }  2> ${TASKS[${i}]}.sys.err

           if [ $TIMEOUT -eq 124 ]; then
                printf "<Time limit is exceeded>" >> grade.txt
            # valid signals between 1-64
            elif [ $TIMEOUT -ge 129 -a $TIMEOUT -le 192 ]; then
                sig=$(($TIMEOUT - 128))
                kill -l $sig | awk '$0=">"$0'
            # any other return code except 0
            elif [ $TIMEOUT -ne 0 ]; then
                if [ ${i} -eq 20 ] || [ ${i} -eq 21 ]; then
                    echo RETURNED:
                    echo $TIMEOUT | awk '$0=">"$0'
                    echo "Memory leak after clear"
                else
                    echo RETURNED:
                    echo $TIMEOUT | awk '$0=">"$0'
                fi
            fi 


            # if segfault show
            if [ -s ${TASKS[${i}]}.sys.err ]; then
                printf "<SEG. FAULT>" >> ../../grade.txt
            fi

            diff ${TASKS[${i}]}.out ${TASKS[${i}]}.myout > ${TASKS[${i}]}.dif
            d=$?

            # if different display outputs
            if [ $d -eq 1 ]; then
                printf "<output is different>" >> ../../grade.txt
            fi
            
            if [ $d -eq 0 ] && [ ! -s ${TASKS[${i}]}.sys.err ]; then
                printf "PASS\n" >> ../../grade.txt
                GRADE=$(awk "BEGIN {print $GRADE+${PNTS[${i}]}; exit}")
            elif [ $d -eq 1 ]; then
                printf "FAIL\n" >> ../../grade.txt
            elif [ -s ${TASKS[${i}]}.sys.err ]; then
                printf "ERROR\n" >> ../../grade.txt
            else
                printf "ERROR\n" >> ../../grade.txt
            fi            
        else
            printf "COMPILE ERROR\n" >> ../../grade.txt
        fi
        echo
    cd ..
    cd ..
    done

echo

printf "Result: ${GRADE}\n" >> grade.txt








