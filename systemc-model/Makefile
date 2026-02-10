TARGET := epc_optimizer
CXX := g++
LD := $(CXX)
C_FLAGS := -I/usr/local/systemc-2.3.3/include -O0 -g3 -Wall -c
LD_OPTIONS = -L/usr/local/systemc-2.3.3/lib-linux64 -Wl,-rpath /usr/local/systemc-2.3.3/lib-linux64 -o $(TARGET)
LIBS := -lm -lsystemc
C_SOURCES = $(shell ls *.cpp)
C_OBJS := $(patsubst %.cpp, %.o, $(C_SOURCES))
%.o:%.cpp
	@echo 'Building file: $(@F)'
	@echo '--------------------'
	$(CXX) $(C_FLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '
all:$(TARGET)
$(TARGET): $(C_OBJS)
	@echo 'From $(C_OBJS)'
	@echo 'building binary $(@F)'
	$(LD) $(LD_OPTIONS) $(C_OBJS) $(LIBS)
clean:
	rm -f $(TARGET) *.o *.vcd
.PHONY: $(TARGET)