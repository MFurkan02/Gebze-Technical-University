
classify(_,_,PetalLength,_) :-
    PetalLength =< 2.45,
    write('yes -> Iris-setosa'),
    nl.
    
classify(_, _, PetalLength, PetalWidth) :-

    (PetalLength > 2.45, PetalLength =< 4.95, PetalWidth =< 1.65) ,

    write('yes -> Iris-versicolor'), nl.

classify(SepalLength, _, PetalLength, PetalWidth) :-

    (PetalWidth =< 1.75, PetalLength > 4.95, PetalWidth > 1.55, SepalLength =< 6.95) ,

    write('yes -> Iris-versicolor'), nl.

classify(_, SepalWidth, PetalLength, PetalWidth) :-

    (PetalLength > 2.45, PetalWidth > 1.75, PetalLength =< 4.85, SepalWidth > 3.1) ,

    write('yes -> Iris-versicolor'), nl.


classify(_, _, PetalLength, PetalWidth) :-
    
    PetalLength > 4.85,
    PetalWidth > 1.75 ,
    write('yes -> Iris-virginica') , nl.

classify(_, SepalWidth, PetalLength, PetalWidth) :-

    PetalLength > 2.45,
    PetalLength =< 4.85,
    PetalWidth > 1.75,
    SepalWidth =< 3.1 ,
    write('yes -> Iris-virginica') , nl.

classify(_, _, PetalLength, PetalWidth) :-

    PetalLength > 4.95,
    PetalWidth =< 1.55,
    write('yes -> Iris-virginica') , nl.

classify(SepalLength, _, PetalLength, PetalWidth) :-

    PetalLength > 4.95,
    PetalWidth > 1.55,
    PetalWidth =< 1.75,
    SepalLength > 6.95,
    write('yes -> Iris-virginica') , nl.

classify(_ , _ , PetalLength, PetalWidth) :-

    PetalLength > 2.45,
    PetalLength =< 4.95,
    PetalWidth =< 1.75,
    PetalWidth > 1.65,
    write('yes -> Iris-virginica') , nl.



% Query of classification
?- classify(4.9,2.4,3.3,1.0).
?- classify(5.4,3.4,1.5,0.4).
?- classify(7.2,3.6,6.1,2.5).
?- classify(5.8,2.7,3.9,1.2).
?- classify(5.9,3.2,4.8,1.8).
?- classify(7.6,3.0,6.6,2.1).
?- classify(5.4,3.4,1.5,0.4).
?- classify(6.1,2.8,4.7,1.2).