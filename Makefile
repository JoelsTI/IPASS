#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := 

# header files in this project
HEADERS :=

# other places to look for files for this project
SEARCH  := 

TARGET := arduino_due 
SERIAL_PORT := COM3 
CONSOLE_BAUDRATE := 9600 

# set RELATIVE to the next higher directory 
# and defer to the appropriate Makefile.* there
RELATIVE := ..
include $(RELATIVE)/Makefile.due