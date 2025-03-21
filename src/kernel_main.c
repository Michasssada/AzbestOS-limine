void kernel_main()
{
    while (1)
    {
        asm volatile ("cli; hlt");
    }
}