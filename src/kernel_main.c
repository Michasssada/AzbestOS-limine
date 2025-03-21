extern void load_gdt();
void kernel_main()
{
    load_gdt();
    while (1)
    {
        asm volatile ("cli; hlt");
    }
}