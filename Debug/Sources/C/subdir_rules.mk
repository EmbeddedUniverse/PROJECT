################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Sources/C/main.obj: ../Sources/C/main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/c6000_7.4.20/bin/cl6x" --abi=coffabi -g --include_path="D:/S5/PROJET/PROJECT/Includes" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --include_path="C:/ti/c6000_7.4.20/include" --include_path="C:/ti/dsk6713/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="Sources/C/main.d" --obj_directory="Sources/C" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/C/mathematic.obj: ../Sources/C/mathematic.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/c6000_7.4.20/bin/cl6x" --abi=coffabi -g --include_path="D:/S5/PROJET/PROJECT/Includes" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --include_path="C:/ti/c6000_7.4.20/include" --include_path="C:/ti/dsk6713/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="Sources/C/mathematic.d" --obj_directory="Sources/C" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/C/test.obj: ../Sources/C/test.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/c6000_7.4.20/bin/cl6x" --abi=coffabi -g --include_path="D:/S5/PROJET/PROJECT/Includes" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --include_path="C:/ti/c6000_7.4.20/include" --include_path="C:/ti/dsk6713/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="Sources/C/test.d" --obj_directory="Sources/C" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


