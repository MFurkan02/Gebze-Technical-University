JC = javac
JFLAGS = -classpath .
JD = javadoc
JDFLAGS = -protected -splitindex -use -author -version -d ./javadoc
RM = rm
JR = java

all: clean compile run

compile:
	javac -d . SmartPhone.java SmartWatch.java TV.java Laptop.java Headphones.java Device.java Inventory.java TheMenu.java 
run:
	java TheMenu

clean:
	rm -f *.class

doc : 	
	$(JD) $(JDFLAGS) *.java 

