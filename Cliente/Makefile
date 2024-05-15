LIBS= -lpthread -lm 	#Libraries used if needed

SRCS= main.cpp Accelerometer.cpp Colorimeter.cpp i2c.cpp clientReceive.cpp clientSend.cpp

BIN=app
CFLAGS+= -g -O0
OBJS=$(subst .cpp,.o,$(SRCS))

all : $(BIN)
$(BIN): $(OBJS)
	@echo [link] $@
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	@echo [Compile] $<
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	@rm -f $(OBJS) $(BIN)
