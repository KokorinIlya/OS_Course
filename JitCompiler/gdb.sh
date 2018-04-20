echo "info line 4
disassemble /r 0x400526" > buffer_file

gdb return_value < buffer_file
