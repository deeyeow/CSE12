#This is the Makefile to use for Homework Assignment #10
# To use, at the prompt, type:
#
#       make Driver             # This will make executable Driver
# or
#       make clean              # This will safely remove old stuff

DIR=$(HOME)/../public/hw10

all:	Driver 

Driver:	Driver.class
	echo 'java Driver $$*' > Driver
	chmod ug+rx Driver

Driver.class:	Base.java Driver.java Heap.java
	javac -g Driver.java

valgrind:
	echo 'not supported'

clean:
	rm -f Driver core *.class

new:
	make clean
	make

backup:
	cp *.[ch] *.java Makefile backup

directories:
	mkdir $(HOME)/hw10
	mkdir $(HOME)/hw10/backup

install:
	cp $(DIR)/Makefile \
		$(DIR)/Base.java \
		$(DIR)/Driver.java.empty \
		$(DIR)/Heap.java.empty \
		$(HOME)/hw10

public:
	make new

	if [ ! -d $(DIR) ]; then mkdir $(DIR); fi
	
	# remove .o files to prepare for java executable creation
	rm -f *.o

	# produce java executables
	#gcj -c *.java
	#gcj -o Driver2 --main=Driver Base.o Driver.o List.o MyLib.o \
		#Stack.o SymTab.o HashTable.o
	#gcj -o Calc2 --main=Main Base.o Calculator.o HashTable.o List.o \
		#Main.o MyLib.o Stack.o SymTab.o
	
	# done with .o files
	rm -f *.o

	# lock public directory
	chmod 700 $(DIR)

	# copy all files to directory
	cp -p Makefile  *.java *.empty Driver $(DIR) 
	#
	# copy html files into public_html/hw6/
	cp -p *html $(HOME)/public_html/hw10/
	 
	# strip executables of debugging information
	strip $(DIR)/Driver

	# remove solution files from public directory
	rm -f $(DIR)/Heap.java
	rm -f $(DIR)/Driver.java

	# give all permissions to other users, take away some permissions later
	chmod o-rwx $(DIR)/*

	# set correct permissions on executables
	chmod go=x $(DIR)/Calc1 $(DIR)/Driver1

	# set correct permissions on source files
	chmod go+r $(DIR)/Makefile \
		$(DIR)/Base.java \
		$(DIR)/Driver.java.empty \
		$(DIR)/Heap.java.empty \
		$(DIR)/Makefile \

        # move html files to public_html directory
	#rm -r $(HOME)/public_html/hw6/
	if [ -d $(HOME)/public_html/hw10/ ]; then rm -r \
	$(HOME)/public_html/hw10/; fi
	mkdir $(HOME)/public_html/hw10
	cp *html $(HOME)/public_html/hw10
	chmod 644 $(HOME)/public_html/hw10/*html
	chmod 755 $(HOME)/public_html/hw10/

	#Unlock public directory
	chmod 755 $(DIR)
	# copy solution to Tracker's account
	#scp -p Makefile *.c *.h *.java cs12xzz@ieng6-201.ucsd.edu:~/hw6
	rm -f *.o *.class core
