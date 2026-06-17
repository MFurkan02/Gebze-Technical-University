JC = javac
JFLAGS = -classpath .
JD = javadoc
JDFLAGS = -protected -splitindex -use -author -version -d ./javadoc
RM = rm
JR = java

all: clean compile run

compile:
	javac -d . AVLTree.java Stock.java StockDataManager.java GUIVisualization.java
run:
	java -Xint StockDataManager  

clean:
	rm -f *.class

doc : 	
	$(JD) $(JDFLAGS) *.java 

