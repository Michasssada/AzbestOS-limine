cmake_minimum_required(VERSION 3.10)

project(AzbestOS VERSION 1.0 LANGUAGES C CXX ASM)

enable_language(ASM_NASM)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_ASM_NASM_COMPILER nasm)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "-Wall -O0 -fno-stack-protector -fno-omit-frame-pointer -Wextra -ffreestanding -fno-stack-check -fno-lto -ffunction-sections -fdata-sections -m64 -mno-sse -mno-mmx -mno-sse2 -fno-pic -mno-sse3 -mno-ssse3 -mno-avx -mno-avx2 -mno-avx512f -mno-red-zone -march=x86-64 -static -Wformat-security -Wformat -g -mcmodel=kernel")
set(CMAKE_CXX_FLAGS "-fpermissive -fno-rtti -fno-omit-frame-pointer -Wall -O0 -fno-stack-protector -Wextra -ffreestanding -fno-stack-check -fno-lto -ffunction-sections -fdata-sections -fno-exceptions -fno-pic -m64 -mno-sse -mno-mmx -mno-sse2 -mno-sse3 -mno-ssse3 -mno-avx -mno-avx2 -mno-avx512f -mno-red-zone -march=x86-64 -fno-threadsafe-statics -std=c++20 -Wformat-security -Wformat -g -mcmodel=kernel")
set(CMAKE_EXE_LINKER_FLAGS "-lgcc -nostdlib -static -z text -T ../linker.ld -Wl,--export-dynamic -z max-page-size=0x1000")
set(CMAKE_ASM_NASM_FLAGS "-f elf64 -I src/")

add_executable(AzbestOS
        src/init/init.asm
        src/kernel_main.c
        src/init/gdt.asm
        src/drivers/uart/uart.c
        src/drivers/display/display.c
)

include_directories(AzbestOS PRIVATE src)

add_custom_target(PostBuild ALL
    COMMAND ${CMAKE_COMMAND} -E rename AzbestOS kernel.elf
    COMMAND chmod +x ../iso.sh
    COMMAND ../iso.sh
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Post-build: Renaming AzbestOS to kernel.elf and invoking iso.sh"
    DEPENDS AzbestOS
)

add_custom_target(PerformAll ALL
    COMMAND qemu-system-x86_64.exe -cpu qemu64,+fsgsbase,+syscall -M smm=off -smp 6 -M q35 -m 2G -debugcon stdio -cdrom image.iso -drive if=pflash,unit=0,format=raw,file=../ovmf/ovmf-code-x86_64.fd,readonly=on -drive if=pflash,unit=1,format=raw,file=../ovmf/ovmf-vars-x86_64.fd
    DEPENDS PostBuild
)

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE SOURCE_FILES
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/src/*.cpp
            ${CMAKE_SOURCE_DIR}/src/*.h
    )
    add_custom_target(Format
            COMMAND ${CLANG_FORMAT} -i ${SOURCE_FILES}
            COMMENT "Running clang-format on source files"
    )
endif()