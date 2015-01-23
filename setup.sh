#!/bin/bash
echo "0" | sudo tee /proc/sys/kernel/randomize_va_space
echo "Disabled ASLR"

eval "rm -rf bin/"

eval "mkdir bin/"


for i in src/*.c
do
    file_name="$i"
    file_name="${i#src\/}"
    file_name="${file_name%.c}"
    command="gcc -w -static -ggdb -fno-stack-protector -z execstack $i -o bin/$file_name"
    if eval $command; then
	    echo "Created bin/$file_name"
	else
		echo "Failed to create bin/$file_name"
	fi
done


if eval "javac src/getshellcode.java -d bin/"; then
	echo "Created bin/getshellcode.class"
else
	echo "Failed to create bin/getshellcode.class"
fi

for i in src/*.asm
do
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
done
