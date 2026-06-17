% Facts
place(1). % admin_office
place(2). % cafeteria
place(3). % library
place(4). % engineering_building
place(5). % lecture_hall_A
place(6). % social_science_building
place(7). % institute_x
place(8). % institute_y

% Routes between places and time to travel (undirected)
path(1, 2, 4).
path(2, 1, 4).
path(1, 4, 3).
path(4, 1, 3).
path(1, 3, 1).
path(3, 1, 1).
path(4, 3, 5).
path(3, 4, 5).
path(4, 5, 2).
path(5, 4, 2).
path(3, 2, 5).
path(2, 3, 5).
path(3, 6, 2).
path(6, 3, 2).
path(3, 8, 3).
path(8, 3, 3).
path(6, 2, 2).
path(2, 6, 2).
path(6, 7, 8).
path(7, 6, 8).
path(8, 5, 3).
path(5, 8, 3).

% personnel(id,kg,hours,delivery,place)

delivery_person(1, 80, [8, 12, 16, 20, 24],none,3). % Person 1 with ID, capacity, work hours, no current job, and current location.
delivery_person(2, 73, [9, 13, 17, 21], 3, 6).
delivery_person(3, 64, [10, 14, 18, 22],none,8).


% object(id,kg,pick,drop,urgency,personnel)

object(1, 53, 1, 3, low, none). % Object with ID, weight, pickup place, drop-off place, urgency, and current delivery person if in transit.
object(2, 32, 4, 6, medium, none).
object(3, 17, 1, 7, high, 2).
object(4, 5, 8, 2, low, none).
object(5, 19, 6, 8, high, none).

% Rule to find the route and total time needed
route(Pickup, Drop, TotalTime) :-
    path(Pickup, Drop, TotalTime).

route(Pickup, Drop, TotalTime) :-
    path(Pickup, Intermediate, Time1),
    route(Intermediate, Drop, Time2),
    TotalTime is Time1 + Time2.

% for person1
check_object(ObjID) :-
    delivery_person(PID, PersonKG, _, Transit , Place),
    (Transit = none -> % if delivery person not in transit
            object(ObjID, ObjKG, Pickup, Drop, _, InTransit),
            InTransit = none, % if object is not in transit.
            PersonKG >= ObjKG, % ip person can carry the object.
            route(Place,Pickup,Time1),
            %write("Time1 is "),write(Time1), 
            route(Pickup,Drop,Time2),
            %write("Time2 is "),write(Time2), 
            Total is Time1 + Time2, % getting the total time for personnel to go pickup and drop the object.
            write("Total time for personnel ID "),
            write(PID),
            write(" to deliver object ID "),
            write(ObjID),
            write(" is "),
            write(Total) , write("t");
        %Transit = ObjID , % if in transit printing the person delivering it.
        write('Object '), write(ObjID),
        write(' is already in transit with delivery person '), write(PID)).

% for person3
check_object2(ObjID) :-
    delivery_person(PID, PersonKG, _, Transit , Place),
    PID=3,
    (Transit = none -> % if delivery person not in transit
            object(ObjID, ObjKG, Pickup, Drop, _, InTransit),
            InTransit = none, % if object is not in transit.
            PersonKG >= ObjKG, % ip person can carry the object.
            route(Place,Pickup,Time1),
            %write("Time1 is "),write(Time1), 
            route(Pickup,Drop,Time2),
            %write("Time2 is "),write(Time2), 
            Total is Time1 + Time2, % getting the total time for personnel to go pickup and drop the object.
            write("Total time for personnel ID "),
            write(PID),
            write(" to deliver object ID "),
            write(ObjID),
            write(" is "),
            write(Total) , write("t");
        %Transit = ObjID , % if in transit printing the person delivering it.
        write('Object '), write(ObjID),
        write(' is already in transit with delivery person '), write(PID)).

% 3 delivery person one in transit as wanted from us(person2).
% for person1
?- check_object(1),nl.
?- check_object(2),nl.
?- check_object(3),nl.
?- check_object(4),nl.
?- check_object(5),nl,nl.

% for person3 (person2 is in transit so he will not be available).
?- check_object2(1),nl.
?- check_object2(2),nl.
?- check_object2(4),nl.
?- check_object2(5),nl.