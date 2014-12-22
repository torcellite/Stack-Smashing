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
    command="gcc -w -static -ggdb -fno-stack-protector -z execstack -w $i -o bin/$file_name"
    eval $command
    echo "Created bin/$file_name"
done


eval "javac src/getshellcode.java -d bin/"

echo "Created bin/getshellcode.class"


for i in src/*.asm
do
    file_name="$i"
    file_name="${i#src\/}"
    file_name="${file_name%.asm}"
    command="yasm -f elf64 $i -o bin/$file_name.o"
    eval $command
    echo "Created bin/$file_name.o"
done

for i in bin/*.o
do
    file_name="$i"
    file_name="${i%.o}"
    command="ld -o $file_name.out $i"
    eval $command
    echo "Created $file_name.out"
done
