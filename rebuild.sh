#!/bin/bash

now=`date +%Y%m%d%H%M`

doc=`date +%Y%m%d%H%M -r src/getshellcode.java`
difference=$(( now - doc ))
if (( difference <= 15)); then
	if eval "javac src/getshellcode.java -d bin/"; then
		echo "Created bin/getshellcode.class"
	else
		echo "Failed to create bin/getshellcode.class"
	fi
fi

if ls src | grep -q "\.c"; then
	for i in src/*.c
	do
		doc=`date +%Y%m%d%H%M -r $i`
		difference=$(( now - doc ))
		if (( difference <= 15)); then
		    file_name="$i"
		    file_name="${i#src\/}"
		    file_name="${file_name%.c}"
		    command="gcc -w -static -ggdb -fno-stack-protector -z execstack $i -o bin/$file_name"
			eval "rm -f bin/$file_name"
		    if eval $command; then
		    	echo "Created bin/$file_name"
			else
				echo "Failed to create bin/$file_name"
			fi
		fi
	done
fi


if ls src | grep -q "\.asm"; then
	for i in src/*.asm
	do
		doc=`date +%Y%m%d%H%M -r $i`
		difference=$(( now - doc ))
		if (( difference <= 15)); then
		    file_name="$i"
		    file_name="${i#src\/}"
		    file_name="${file_name%.asm}"
		    command="yasm -f elf64 $i -o bin/$file_name.o"
		    if eval $command; then
			    echo "Created bin/$file_name.o"
			    command="ld -o bin/$file_name.out bin/$file_name.o"
			    if eval $command; then
				    echo "Created bin/$file_name.out"
				else
					echo "Failed to create bin/$file_name.out"
				fi
			else
				echo "Failed to create bin/$file_name.o"
			fi
		fi
	done
fi

for i in `find src/* -maxdepth 1 -type d`
do
	dir_name="$i"
	dir_name="${i#src\/}"
	command="mkdir -p bin/$dir_name"
	if `ls bin | grep \"$dir_name\"`; then
		if eval $command; then
			echo "Created bin/$dir_name folder"
		else
			echo "Failed to create bin/$dir_name folder"
		fi
	fi
done

for i in src/*/*.c
do
	doc=`date +%Y%m%d%H%M -r $i`
	difference=$(( now - doc ))
	if (( difference <= 15)); then
	    file_name="$i"
	    file_name="${i#src\/}"
	    file_name="${file_name%.c}"
	    command="gcc -w -static -ggdb -fno-stack-protector -z execstack $i -o bin/$file_name"
		eval "rm -f bin/$file_name"
	    if eval $command; then
	    	echo "Created bin/$file_name"
		else
			echo "Failed to create bin/$file_name"
		fi
	fi
done

for i in src/*/*.asm
do
	doc=`date +%Y%m%d%H%M -r $i`
	difference=$(( now - doc ))
	if (( difference <= 15)); then
	    file_name="$i"
	    file_name="${i#src\/}"
	    file_name="${file_name%.asm}"
	    command="yasm -f elf64 $i -o bin/$file_name.o"
	    if eval $command; then
		    echo "Created bin/$file_name.o"
		    command="ld -o bin/$file_name.out bin/$file_name.o"
		    if eval $command; then
			    echo "Created bin/$file_name.out"
			else
				echo "Failed to create bin/$file_name.out"
			fi
		else
			echo "Failed to create bin/$file_name.o"
		fi
	fi
done												
