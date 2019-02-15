 #!/bin/bash

# i = input command file
# so = student's shell output
# co = correct output (as bash does) 

shell=sshell
shell_source=$1
wrongCommandMessage="Shell: Incorrect command."
cdWrongArgs="Shell: Incorrect command."
checkyoop="Please compare the expected output file to student output file to see what went wrong."
byeMessage="Shell: Goodbye."
count=1								#Keep Test Case Count. Printed by print_status function and increased\
exitImplemented=0
command_a=bash_test_part_a.txt
totalmarks=0
prefix=bash_output_
if [ -z "$shell_source" ]
then
	echo "Please enter the source code as command line argument."
	exit
else
	gcc $shell_source -o $shell  &>/dev/null	#$shell is name of the executable
	if [ ! -e $shell ]
	then
		echo "Compilation failed"
		exit
	fi
fi

cleanop()
{
	so=$1
	co=$2	
	sed -i -e 's/$ //g' "$so" #deleting spaces after end of lines
	sed -i -e "/^$byeMessage/d" "$so"
	sed -i -e '/^$/d' "$so" #deleting blank lines
}

exitchecker()
{
	i=$1
	so=part_a_student_output_exit.txt
	touch $i $so
	echo exit > $command_a
	echo
	echo "Testcase $count: exit command"
	count=$(($count + 1))
	# ./$shell $command_a > $so &
	exitMessage=`grep -c "$byeMessage" $so`
	echo exitMessage $exitMessage
	sleep 1
	exitWorked=$(ps aux | grep -v "grep" | grep -c "$shell")
	if [[ $exitMessage -eq 1 && "$exitWorked" -eq 0 ]]; then
		echo "Exit command has been implemented correctly."
		rm -f "$so"
		exitImplemented=1
		totalmarks=$(($totalmarks + 1))
	else
		echo "Exit command has NOT been implemented correctly."
	fi
	# echo
	echo "----------------------------------------"
}

lschecker() 
{
	i=$1
	so=part_a_student_output_ls.txt
	co=$prefix"ls.txt"
	touch $i $so $co
	echo "Testcase $count: regular ls command"
	count=$(($count + 1))
	echo ls > $command_a
	echo ls -l >> $command_a
	echo exit >> $command_a
	./$shell $command_a > $so &
	./$command_a > $co 2> /dev/null
	sleep 1
	cleanop $so $co
	#ls output differs sl3 machine, when it displays a count of total files
	sed -i -e '/^total/d' $so $co
	#deleting file names that can cause conflict in outputs of ls
	sed -i -e '/^part_a_student_output/d' "$so" "$co"
	sed -i -e '/^bash_test/d' "$so" "$co" 
	sed -i -e '/^bash_output/d' "$so" "$co" 
	sed -i -e '/'$so'$/d' "$so" "$co"
	sed -i '/'$co'$/d' "$so" "$co"
	lso=`diff -U 0 $so $co | grep ^@ | wc -l`
	if [[ $lso -eq 0 ]]; then 
	echo "ls worked. " 
	totalmarks=$(($totalmarks + 1))
	rm -f "$so" "$co"
	else
	echo "ls failed. $checkyoop"
	fi
	echo "----------------------------------------"
}
issueschecker()
{
	i=$1
	so=part_a_student_output_ls_issues.txt
	co=$prefix"lsissues.txt"
	touch $i $so $co
	echo "Testcase $count: ls command <with issues>"
	count=$(($count + 1))
	echo ls -l > $command_a
	echo ls >> $command_a
	echo ls -e >> $command_a
	echo ls -x >> $command_a
	echo garbage >> $command_a
	echo exit >> $command_a
	echo "___________________________________________"
	echo "Student code handling incorrect options:"
	./$shell $command_a > $so &
	./$command_a > $co 2> /dev/null
	sleep 1
	echo "___________________________________________"
	cleanop $so $co

	#ls output differs sl3 machine, when it displays a count of total files
	sed -i -e '/^total/d' "$so" "$co"
	#deleting file names that can cause conflict in outputs of ls
	sed -i -e '/^part_a_student_output/d' "$so" "$co"
	sed -i -e '/^bash_test/d' "$so" "$co" 
	sed -i -e '/^bash_output/d' "$so" "$co" 
	sed -i -e '/'$so'$/d' "$so" "$co"
	sed -i '/'$co'$/d' "$so" "$co"
	countOfFailed=`grep -c "$wrongCommandMessage" $so`
	# sed -i -e "/^$wrongCommandMessage/d" "$so"
	if [[ $countOfFailed -eq 1 ]]; then
		echo "Incorrect command case handled correctly."
		totalmarks=$(($totalmarks + 1))
	else
		echo "Incorrect command case NOT handled correctly."
	fi
	lso=`diff -U 0 $so $co | grep ^@ | wc -l`
	if [[ $lso -eq 0 ]]; then 
	echo "ls <with issues> worked. " #regular ls, ls with correct options, ls with incorrect options, exit
	# rm part_a_student_output* bash_output*
	rm -f "$so" "$co"
	totalmarks=$(($totalmarks + 1))
	else
	echo "ls <with issues> failed. $checkyoop" #examine output and then decide. Basic ls works: 2 marks.
	fi
	echo "----------------------------------------"
}
pwdchecker() #testing cd with pwd
{
	so=part_a_student_output_pwd.txt
	co=$prefix"pwd.txt"
	touch $i $so $co
	echo "Testcase $count: pwd/cd command"
	count=$(($count + 1))
	`echo pwd > $command_a`
	`echo cd .. >> $command_a`
	`echo pwd >> $command_a`
	`echo exit >> $command_a`
	`echo pwd > $command_a`
	`echo exit >> $command_a`
	./$shell $command_a > $so &
	./$command_a > $co
	sleep 1
	cleanop $so $co
	pwdo=`diff -U 0 $so $co | grep ^@ | wc -l`
	if [[ $pwdo -eq 0 ]]; then
	echo "pwd worked."
	rm -f "$so" "$co"
	totalmarks=$(($totalmarks + 1))
	else
	echo "pwd failed. $checkyoop"
	fi
	echo "----------------------------------------"
}
cdchecker() #testing cd with pwd
{
	so=part_a_student_output_cd.txt
	co=$prefix"cd.txt"
	touch $i $so $co
	echo "Testcase $count: cd command"
	count=$(($count + 1))
	mkdir testing 2> /dev/null
	`echo cd testing > $command_a`
	`echo pwd >> $command_a`
	`echo cd testing >> $command_a`
	`echo cd t1 t2 > $command_a`
	`echo exit >> $command_a`
	./$command_a > $co 2> /dev/null
	./$shell $command_a > $so &
	sleep 1
	cleanop $so $co
	countOfFailed=`grep -c "$wrongCommandMessage" $so`
	cdo=`diff -U 0 $so $co | grep ^@ | wc -l`
	if [[ $cdo -eq 1 && $countOfFailed -eq 1 ]]; then
	echo "cd worked."
	rm -f "$so" "$co"
	totalmarks=$(($totalmarks + 1))
	else
	echo "cd failed. $checkyoop"
	fi
	rmdir testing
	echo "----------------------------------------"
}
echochecker() #testing cd with pwd
{
	so=part_a_student_output_echo.txt
	co=$prefix"echo.txt"
	echo "Testcase $count: Checking echo command"
	count=$(($count + 1))
	`echo "echo hello" > $command_a`
	`echo "echo pwd" >> $command_a`
	# `echo "echo echo" >> $command_a`
	`echo exit >> $command_a`
	./$shell $command_a > $so &
	./$command_a > $co
	sleep 1
	cleanop $so $co
	pwdo=`diff -U 0 $so $co | grep ^@ | wc -l`
	if [[ $pwdo -eq 0 ]]; then
	  echo "echo worked."
	  rm -f "$so" "$co"
	  totalmarks=$(($totalmarks + 1))
	else
	    echo "echo failed. $checkyoop"
	fi
	echo "----------------------------------------"
}
datechecker() #testing cd with pwd
{
	touch $so $co
	so=part_a_student_output_date.txt
	co=$prefix"date.txt"
	echo "Testcase $count Checking date command"
	count=$(($count + 1))
	`echo "date +%D" > $command_a`
	`echo exit >> $command_a`
	./$shell $command_a > $so &
	./$command_a > $co
	sleep 1
	cleanop $so $co
	dateo=`diff -U 0 $so $co | grep ^@ | wc -l`
	if [[ $dateo -eq 0 ]]; then
	echo "date worked."
	rm -f "$so" "$co"
	totalmarks=$(($totalmarks + 1))
	else
	echo "date failed. $checkyoop"
	fi
	echo "----------------------------------------"
}
echo --------
echo PART A 8 marks
echo --------
touch $command_a
chmod 777 $command_a
exitchecker $command_a 
lschecker $command_a
issueschecker $command_a
cdchecker $command_a
pwdchecker $command_a
datechecker $command_a
sleep 1
echochecker $command_a
echo
echo "Totalmarks: $totalmarks"
rm -f "$command_a"


