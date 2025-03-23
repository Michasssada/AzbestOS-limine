with open("src/drivers/cpu/idt/ptrs.c", "w") as ptrs_fd:
	ptrs_fd.write("#include \"idt.h\"\n")
	for i in range(0, 255):
		ptrs_fd.write(f"extern void* idt_isr{i}();\n");

	ptrs_fd.write("volatile void* handlers_ptrs[] = {");
	for i in range(0, 255):
		ptrs_fd.write(f"(void*)&idt_isr{i}, ");
	ptrs_fd.write("};\n")

	ptrs_fd.write("volatile struct gate_descriptor_64 idt[256];\n");

	# eh time for ze table function
	ptrs_fd.write("void initialize_idt_table()\n{\n");
	for i in range(0, 255):
		ptrs_fd.write(f"idt[{i}] = (volatile struct gate_descriptor_64)IDT_ENTRY(handlers_ptrs[{i}], 0x8, 0xe, 0, 1, 0);");
	ptrs_fd.write("}\n");
