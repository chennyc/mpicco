# All of the sources participating in the build are defined here
-include smc-compute/ops/ops.mk
-include smc-compute/smc-compute.mk
-include objects.mk

#Add test program to the build variables
CPP_SRCS += \
#test-code.cpp 

OBJS += \
#test-code.o 

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif


# Add inputs and outputs from these tool invocations to the build variables 
# All Target
all: mult add_mal add bitop bitop_mal eqz_mal eqz ltz ltz_mal
mult: $(OBJS) $(USER_OBJS) 
	g++ -g -c -w mult30.cpp 
	g++ -g -o mult mult30.o $(OBJS) $(USER_OBJS) $(LIBS) 
add_mal: $(OBJS) $(USER_OBJS) 
	g++ -g -c -w multadd_Batch_trans_M.cpp 
	g++ -g -o add_mal multadd_Batch_trans_M.o $(OBJS) $(USER_OBJS) $(LIBS) 

add: $(OBJS) $(USER_OBJS) 
	g++ -g -c -w multadd_Batch_trans.cpp 
	g++ -g -o add multadd_Batch_trans.o $(OBJS) $(USER_OBJS) $(LIBS) 

eqz_mal: $(OBJS) $(USER_OBJS) 
	g++ -g -c -w eqz_trans_mal.cpp 
	g++ -g -o eqz_mal eqz_trans_mal.o $(OBJS) $(USER_OBJS) $(LIBS) 

eqz: $(OBJS) $(USER_OBJS) 
	g++ -g -c -w eqz_trans.cpp 
	g++ -g -o eqz eqz_trans.o $(OBJS) $(USER_OBJS) $(LIBS) 

ltz: $(OBJS) $(USER_OBJS) 
	g++ -g -c -w ltz.cpp 
	g++ -g -o ltz ltz.o $(OBJS) $(USER_OBJS) $(LIBS) 

ltz_mal: $(OBJS) $(USER_OBJS) 
	g++ -g -c -w ltz_mal.cpp 
	g++ -g -o ltz_mal ltz_mal.o $(OBJS) $(USER_OBJS) $(LIBS) 

bitop: $(OBJS) $(USER_OBJS) 
	g++ -g -c -w bitwise_op.cpp 
	g++ -g -o bitop bitwise_op.o $(OBJS) $(USER_OBJS) $(LIBS) 

bitop_mal: $(OBJS) $(USER_OBJS) 
	g++ -g -c -w bitwise_op_mal.cpp 
	g++ -g -o bitop_mal bitwise_op_mal.o $(OBJS) $(USER_OBJS) $(LIBS) 
# replace the line above with this one for multi-threaded programs
#	g++ -o test-code test-code.o -I ./ompi/include/ompi $(OBJS) $(USER_OBJS) $(LIBS) -L ./ompi/lib/ompi -L ./ompi/lib/ompi/default -lort -lrt -lboost_thread 

clean:
	-$(RM) *.o test-code

#Clean-Everything
clean-all:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) *.o test-code 

