################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/TI/ccs100/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./DSP2803x_Adc.obj" \
"./DSP2803x_CodeStartBranch.obj" \
"./DSP2803x_CpuTimers.obj" \
"./DSP2803x_DefaultIsr.obj" \
"./DSP2803x_GlobalVariableDefs.obj" \
"./DSP2803x_PieCtrl.obj" \
"./DSP2803x_PieVect.obj" \
"./DSP2803x_SysCtrl.obj" \
"./DSP2803x_TempSensorConv.obj" \
"./DSP2803x_usDelay.obj" \
"./Example_2803xAdc_TempSensorConv.obj" \
"C:/TI/C2000Ware_3_04_00_00_Software/device_support/f2803x/headers/cmd/DSP2803x_Headers_nonBIOS.cmd" \
"../csl_28035_RAM_lnk.cmd" \
"../csl/csl2803x_ml.lib" \
$(GEN_CMDS__FLAG) \
-lrts2800_ml.lib \
-lIQmath.lib \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include csl/subdir_vars.mk
-include subdir_rules.mk
-include csl/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Example_2803xAdc_TempSensorConv.out \

EXE_OUTPUTS__QUOTED += \
"Example_2803xAdc_TempSensorConv.out" \

BIN_OUTPUTS += \
Example_2803xAdc_TempSensorConv.hex \

BIN_OUTPUTS__QUOTED += \
"Example_2803xAdc_TempSensorConv.hex" \


# All Target
all: Example_2803xAdc_TempSensorConv.out

# Tool invocations
Example_2803xAdc_TempSensorConv.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/TI/ccs100/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --define=_DEBUG --define=LARGE_MODEL -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi -z -m"Example_2803xAdc_TempSensorConv.map" --stack_size=0x200 --warn_sections -i"D:/TI/ccs100/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS/lib" -i"D:/TI/ccs100/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS/include" -i"D:/projeler/sf_projeler/Arti_electronic/Buck_converter/Code_Composer_Studio/buck_converter" -i"C:/TI/C2000Ware_3_04_00_00_Software/device_support/f2803x/common/lib" -i"C:/TI/C2000Ware_3_04_00_00_Software/libraries/math/IQmath/c28/lib" --reread_libs --issue_remarks --verbose_diagnostics --xml_link_info="Example_2803xAdc_TempSensorConv_linkInfo.xml" --entry_point=code_start --rom_model -o "Example_2803xAdc_TempSensorConv.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Example_2803xAdc_TempSensorConv.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: C2000 Hex Utility'
	"D:/TI/ccs100/ccs/tools/compiler/ti-cgt-c2000_20.2.4.LTS/bin/hex2000" --diag_wrap=off -o "Example_2803xAdc_TempSensorConv.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "DSP2803x_Adc.obj" "DSP2803x_CodeStartBranch.obj" "DSP2803x_CpuTimers.obj" "DSP2803x_DefaultIsr.obj" "DSP2803x_GlobalVariableDefs.obj" "DSP2803x_PieCtrl.obj" "DSP2803x_PieVect.obj" "DSP2803x_SysCtrl.obj" "DSP2803x_TempSensorConv.obj" "DSP2803x_usDelay.obj" "Example_2803xAdc_TempSensorConv.obj" 
	-$(RM) "DSP2803x_Adc.d" "DSP2803x_CpuTimers.d" "DSP2803x_DefaultIsr.d" "DSP2803x_GlobalVariableDefs.d" "DSP2803x_PieCtrl.d" "DSP2803x_PieVect.d" "DSP2803x_SysCtrl.d" "DSP2803x_TempSensorConv.d" "Example_2803xAdc_TempSensorConv.d" 
	-$(RM) "DSP2803x_CodeStartBranch.d" "DSP2803x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

