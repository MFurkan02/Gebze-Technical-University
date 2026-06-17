JC = javac
JFLAGS = -classpath .
JD = javadoc
JDFLAGS = -protected -splitindex -use -author -version -d ./javadoc
RM = rm
JR = java

all: clean compile run

compile:
	javac -d . Person.java SocialNetworkGraph.java Main.java 
run:
	java Main  

clean:
	rm -f *.class

doc : 	
	$(JD) $(JDFLAGS) *.java 

