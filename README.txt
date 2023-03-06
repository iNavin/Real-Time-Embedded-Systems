1. Download CMSIS_5 repository from https://github.com/ARM-software/CMSIS_5

2. Unzip somewhere. This contains many useful packages, we will extract
   rtx kernel files.

3. Copy the following files:

# RTX source code and internal header files
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Source/*.c .
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Source/*.h .
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Include/* .

# RTX main header file
cp CMSIS_5-develop/CMSIS/RTOS2/Include/cmsis_os2.h .

# We use SysTick for timer tick
cp CMSIS_5-develop/CMSIS/RTOS2/Include/os_tick.h .
cp CMSIS_5-develop/CMSIS/RTOS2/Source/os_systick.c .

# OS configuration file and stubs
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Config/RTX_Config.h .
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Config/RTX_Config.c .
# (In RTX_Config.h, change OS_ROBIN_ENABLE from 1 to 0.)

# architecture specific header files
mkdir arch

# compiler and architecture specific header files
cp CMSIS_5-develop/CMSIS/Core/Include/cmsis_version.h arch
cp CMSIS_5-develop/CMSIS/Core/Include/cmsis_compiler.h arch
cp CMSIS_5-develop/CMSIS/Core/Include/cmsis_gcc.h arch
cp CMSIS_5-develop/CMSIS/Core/Include/core_cm4.h arch
cp CMSIS_5-develop/CMSIS/Core/Include/mpu_armv7.h arch

# Cortex-M4 device specific header files
cp CMSIS_5-develop/Device/ARM/ARMCM4/Include/ARMCM4.h arch
cp CMSIS_5-develop/Device/ARM/ARMCM4/Include/ARMCM4_FP.h arch
cp CMSIS_5-develop/Device/ARM/ARMCM4/Include/system_ARMCM4.h arch

# assembly files for exception handlers
cp CMSIS_5-develop/CMSIS/RTOS2/RTX/Source/GCC/irq_armv7m.S .

# empty RTE_Components.h
touch RTE_Components.h

4. Edit the following files:
        os_systick.c rtx_core_c.h rtx_core_cm.h rtx_lib.c rtx_lib.c
   Change:
        #include CMSIS_device_header
   to:
        #include "ARMCM4_FP.h"

5. Create empty RTE_Components.h

6. Copy startup.c file from our earlier assignments and rename the following 
   exception handlers:
   SVCallHandler -> SVC_Handler
   PendSVHandler -> PendSV_Handler
   SysTickHandler -> SysTick_Handler

   Remove startup.o from libbsp.a.
   Remove timer.o from libbsp.a because SysTick is now used by the kernel.

7. Copy linker command file from an earlier assignment:
cp bare.ld rtos.ld

8. Use the main.c file provided with this assignment.

9. Modify build_lib.sh and build.sh from your catch game:

# build_lib.sh:
#   Compile all the c files in the current folder and build librtx.a.

# build.sh:
#   Compile startup.c and main.c link with libbsp.a and librtx.a.

10. You will get a linker error about ARM.extab sections. To resolve this,
remove the lines containing .fnstart, .fnend and .cantunwind in irq_armv7m.S
(total 9 lines).

Once you resolve, you should be able to build the executable and run it
on the target board.

Submit a photo/video of the display after successfully bringing up the system.

Answer the following:
1. How many tasks are running in the system? Which ones?
2. What does osTaskYield do? What is its effect on the display?
3. Edit RTX_Config.h. Change OS_ROBIN_ENABLE to 1. Rebuild the library as
   well as rebuild the program. Do you see any changes in the display?
4. Change priorities of task1 and task2. What effect do you see on the display?
5. Familiarize yourself with os_cmsis2.h file. This header file contains
   all the APIs for using RTX - or any other CMSIS RTOS2 compatible kernel.
