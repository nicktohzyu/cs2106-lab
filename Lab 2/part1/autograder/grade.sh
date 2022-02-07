#!/bin/bash

function autograde {
	echo -e "Test date and time: $(date +%A), $(date +"%d %B %Y"), $(date +%T)\n"
	cd ref

	numFiles=$(ls *.in | wc -l)
	# Compile the reference program
	gcc utils.h utils.c sum.c -o $1
	# Generate reference output files
	for i in *.in; do
		./$1 < $i > $i.out
	done
	# Delete temporary files
	rm $1

	cd ../subs
	numStudents=$(ls | wc -l)

	# Now mark submissions
	for i in *; do
		grade=0
		cd $i
		# Compile C code, print compile error message to output file
		gcc utils.h utils.c sum.c 2> results.out
		if [[ $? -eq 1 ]]; then
			echo -e "Directory $i has a compile error.\nDirectory $i score $grade / $numFiles"
			break
		fi
		# Generate output from C code using *.in files in ref
		for j in ../../ref/*.in; do
			./a.out < $j > $j-student.out
			# Compare with reference output files  and award 1 mark if they are identical
			if cmp -s "$j-student.out" "$j.out"; then
				grade=$(($grade+1))
			fi
			rm $j-student.out
		done
		rm a.out utils.h.gch
		# print score for student
		echo "Directory $i score $grade / $numFiles"
		cd ..
	done
	# print total files marked.
	echo -e "\nProcessed $numStudents files."
}

# Check if we have enough arguments
if [[ $# -ne 1 ]]; then
	echo "Usage: ./grade.sh <filename>"
else 
	autograde $1
fi

    
