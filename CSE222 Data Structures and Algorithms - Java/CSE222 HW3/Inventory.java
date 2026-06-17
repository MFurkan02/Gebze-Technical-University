import java.io.FileWriter; // file writing
import java.time.LocalDate; // getting the current date to write on the report to the file.
import java.time.format.DateTimeFormatter; // to format the date to the desired format.
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Scanner;


public class Inventory {
    
    private LinkedList<ArrayList<Device>> ElectronicDevices;

    public Inventory(){
        ElectronicDevices = new LinkedList<>();
    }

    /**
     * In this method, we iterate through the ArrayLists of the linked list with an enhanced for loop then adding the desired Device according to its category.
     * If the category of the device is not found a new ArrayList is added for that category and device is added to that category(ArrayList).If and only if the
     * category is one of the 5 categories(TV,Laptop,SmartPhone,Smart Watch,Headphones).
     * Time complexity of this method is O(n) where 'n' is the length of the linked list which is the number of ArrayLists(categories) in the linked list.
     * It is because there is a for loop iterates through our linked list which has time complexity of O(n) there is also a bunch of if-else statements
     * However since each of them is O(1) it does not effect since there is O(n).
     * Time complexity : O(n).
     */
    public void addDevice(){

        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter category name(TV,Laptop,Smart Phone,Smart Watch,Headphones) : ");
        String category  = scanner.nextLine();

        System.out.print("Enter device name: ");
        String device    = scanner.nextLine();
        
        System.out.print("Enter price: ");
        double price     = scanner.nextDouble();

        System.out.print("Enter quantity: ");
        int quantity     = scanner.nextInt();

       

        for (ArrayList<Device> devices : ElectronicDevices ) {
            if( devices.get(0).Category().equals(category) ){
                if ( devices.get(0).Category().equals("Smart Phone") ) {
                    SmartPhone newSP = new SmartPhone(device,price,quantity);
                    devices.add(newSP);
                    System.out.println(newSP.Category() + ", " + newSP.Name() + ", " + newSP.Price() + "$, " +  newSP.Quantity() + " amount added..." );
                    return;
                }
                else if ( devices.get(0).Category().equals("Smart Watch") ) {
                    SmartWatch newSW = new SmartWatch(device,price,quantity);
                    devices.add(newSW);
                    System.out.println(newSW.Category() + ", " + newSW.Name() + ", " + newSW.Price() + "$, " +  newSW.Quantity() + " amount added..." );
                    return;
                }
                else if ( devices.get(0).Category().equals("TV") ) {
                    TV newTV = new TV(device,price,quantity);
                    devices.add(newTV);
                    System.out.println(newTV.Category() + ", " + newTV.Name() + ", " + newTV.Price() + "$, " +  newTV.Quantity() + " amount added..." );
                    return;
                }
                else if ( devices.get(0).Category().equals("Headphones") ) {
                    Headphones newHP = new Headphones(device,price,quantity);
                    devices.add(newHP);
                    System.out.println(newHP.Category() + ", " + newHP.Name() + ", " + newHP.Price() + "$, " +  newHP.Quantity() + " amount added..." );

                    return;
                }
                else if ( devices.get(0).Category().equals("Laptop") ) {
                    Laptop newLaptop = new Laptop(device,price,quantity);
                    devices.add(newLaptop);
                    System.out.println(newLaptop.Category() + ", " + newLaptop.Name() + ", " + newLaptop.Price() + "$, " +  newLaptop.Quantity() + " amount added..." );
                    return;
                }
            }

        }

        if( category.equals("Smart Phone") ){
            ArrayList<Device> newPhone = new ArrayList<>();
            SmartPhone newDevice = new SmartPhone(device, price, quantity);
            newPhone.add(newDevice);
            ElectronicDevices.add(newPhone);
            System.out.println(newDevice.Category() + ", " + newDevice.Name() + ", " + newDevice.Price() + "$, " +  newDevice.Quantity() + " amount added..." );
        }
        else if( category.equals("TV") ){
            ArrayList<Device> newTV = new ArrayList<>();
            TV newDevice = new TV(device, price, quantity);
            newTV.add(newDevice);
            ElectronicDevices.add(newTV);
            System.out.println(newDevice.Category() + ", " + newDevice.Name() + ", " + newDevice.Price() + "$, " +  newDevice.Quantity() + " amount added..." );
        }
        else if( category.equals("Laptop") ){
            ArrayList<Device> newLaptop = new ArrayList<>();
            Laptop newDevice = new Laptop(device, price, quantity);
            newLaptop.add(newDevice);
            ElectronicDevices.add(newLaptop);
            System.out.println(newDevice.Category() + ", " + newDevice.Name() + ", " + newDevice.Price() + "$, " +  newDevice.Quantity() + " amount added..." );
        }
        else if( category.equals("Headphones") ){
            ArrayList<Device> newHP = new ArrayList<>();
            Headphones newDevice = new Headphones(device, price, quantity);
            newHP.add(newDevice);
            ElectronicDevices.add(newHP);
            System.out.println(newDevice.Category() + ", " + newDevice.Name() + ", " + newDevice.Price() + "$, " +  newDevice.Quantity() + " amount added..." );
        }
        else if( category.equals("Smart Watch") ){
            ArrayList<Device> newSW = new ArrayList<>();
            SmartWatch newDevice = new SmartWatch(device, price, quantity);
            newSW.add(newDevice);
            ElectronicDevices.add(newSW);
            System.out.println(newDevice.Category() + ", " + newDevice.Name() + ", " + newDevice.Price() + "$, " +  newDevice.Quantity() + " amount added..." );
        }
    }

    /**
     * In this remove method, there is an nested enhanced for loop.In the outer loop I iterate through the ArrayLists(categories) and in the inner loop
     * I iterate through the Device objects in that ArrayList(category) and if the device is found I remove that device.
     * If we assume the length of the linked list as 'n' and assume the average device numbers in categories as 'm' the time complexity is O(n*m) since
     * the for loops iterates through the linked list(n) and Devices in the ArrayLists(m) resulting in n*m time complexity. 
     * Also if we assume the total number of devices as 'd' then the time complexity will be O(d) which is the same as O(n*m).
     * However, the remove method of Java has a time complexity of O(n) so the worst time complexity of this method is assuming length as 'd' 
     * d(from nested enhanced for loop) times d(remove method).
     * Time complexity : O((n*m)^2) or O(d^2).
     */
    public void removeDevice(){

        Scanner scanner = new Scanner(System.in);
        
        System.out.println("Enter the name of the device to be removed : ");
        
        String name = scanner.nextLine();


        for (ArrayList<Device> categories : ElectronicDevices ) {
            for(Device devices : categories){
                if(devices.Name().equals(name)){
                    if(categories.size()==1){
                        ElectronicDevices.remove(categories);
                    }
                    else{
                        categories.remove(devices);
                    }
                    return;
                }
            }
        }
        scanner.close();
        
        System.out.println("Device is not found! Please try typing it again!");
    }

    /**
     * In this update method, there is an nested enhanced for loop just like in remove method.In the outer loop I iterate through the ArrayLists(categories) and in the inner loop
     * we iterate through the Device objects in that ArrayList(category) and if the device to be updated is found I update that device with the desired values.
     * If we assume the length of the linked list as 'n' and assume the average device numbers in categories as 'm' the time complexity is O(n*m) since
     * the for loops iterates through the linked list(n) and Devices in the ArrayLists(m) resulting in n*m time complexity.
     * Also if we assume the total number of devices as 'd' then the time complexity will be O(d) which is the same as O(n*m).
     * The constant setting operations has O(1) time complexity therefore does not effect the time complexity much.In conclusion ;
     * Time complexity : O(n*m) or O(d).
     */
    public void updateDevice(){
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter the name of the device to update: ");
        
        String name = scanner.nextLine();
        
        double nprice=0.0;
        int n_quantity=0;
        int same_price=0,same_quantity=0;

        System.out.print("Enter new price (leave blank to keep current price): ");
        String new_price = scanner.nextLine();
        if(new_price.isEmpty())
            same_price=1;
        else
            nprice = Double.parseDouble(new_price);

        System.out.print("Enter new quantity (leave blank to keep current quantity): ");
        String new_quantity = scanner.nextLine();
        if(new_quantity.isEmpty())
            same_quantity=1;
        else
            n_quantity = Integer.parseInt(new_quantity);

        if(nprice<0 || n_quantity<0){
            System.out.println("Price or quantity should be more than 0!");
            return;
        }

        for(ArrayList<Device> categories : ElectronicDevices){
            for(Device devices : categories){
                if(devices.Name().equals(name)){
                    if(same_price==1 && same_quantity==1)
                        return;
                    else if(same_price==1)
                        devices.setQuantity(n_quantity);
                    else if(same_quantity==1)
                        devices.setPrice(nprice);
                    else {
                        devices.setQuantity(n_quantity);
                        devices.setPrice(nprice);
                    }
                    System.out.println(devices.Name() + " details updated : Price - " + devices.Price() + "$, Quantity - " + devices.Quantity());
                    return;
                }
            }
        }
        System.out.println("Device is not found! Please try typing it again!");
    }

    /**
     * In this ListAll method, there is an nested enhanced for loop just like in previous methods.In the outer loop we iterate through the ArrayLists(categories) and in the inner loop
     * we iterate through the Device objects in that ArrayList(category) and I display all the devices.
     * If we assume the length of the linked list as 'n' and assume the average device numbers in categories as 'm' the time complexity is O(n*m) since
     * the for loops iterates through the linked list(n) and Devices in the ArrayLists(m) resulting in n*m time complexity.
     * Also if we assume the total number of devices as 'd' then the time complexity will be O(d) which is the same as O(n*m).
     * The constant getting operations(devices.Category() etc. ) has O(1) time complexity therefore does not effect the time complexity much.In conclusion ;
     * Time complexity : O(n*m) or O(d).
     */
    public void ListAll(){
        int index=0;
        System.out.println("Device List: ");
        for(ArrayList<Device> categories : ElectronicDevices){
            for(Device devices : categories){
                index++;
                System.out.println(index + ". Category: " + devices.Category() + ", Name : " + devices.Name() + ", Price: " + devices.Price() + "$ ,Quantity: " + devices.Quantity());
            }
        }
    }

    /**
     * In this FindCheapest method, there is an nested enhanced for loop just like in previous methods.In the outer loop we iterate through the ArrayLists(categories) and in the inner loop
     * we iterate through the Device objects in that ArrayList(category).Before the loop I created a Device object to keep the CheapestDevice object.The first
     * object is a dummy temporary object and it is changed with the first Device object in the loop in '|| CheapestDevice.Price()==-1' since the price of the temp object is -1.
     * After that if any of the other devices is chepaer the CheapestDevice object is replaced with that object and finally that object is returned.
     * If we assume the length of the linked list as 'n' and assume the average device numbers in categories as 'm' the time complexity is O(n*m) since
     * the for loops iterates through the linked list(n) and Devices in the ArrayLists(m) resulting in n*m time complexity.
     * Also if we assume the total number of devices as 'd' then the time complexity will be O(d) which is the same as O(n*m).
     * The constant if statement has O(1) time complexity therefore does not effect the time complexity much.In conclusion ;
     * Time complexity : O(n*m) or O(d).
     * @return CheapestDevice(Device object)
     */
    public Device FindCheapest(){
            Device CheapestDevice = new Laptop(null, -1, 0);
            for(ArrayList<Device> categories : ElectronicDevices){
                for(Device devices : categories){
                    if(devices.Price() < CheapestDevice.Price() || CheapestDevice.Price()==-1)
                        CheapestDevice = devices;
                }
            }
            return CheapestDevice;  
    }

    /**
     * In this SortDevices method, there is two nested for loops.In first for loops, in the outer loop we iterate through the ArrayLists(categories) and in the inner loop
     * we iterate through the Device objects in that ArrayList(category).In the first nested for loop 
     * I stored all the Devices by adding them to a ArrayList to sort them using this ArrayList.In the second nested for loop I used two loops first for iterating through every element
     * and second to compare every element after that element for sorting.In that second loop if the next element is smaller than the current element(price of the Device object)
     * smaller object is kept in another object before the removal then it is removed and added to the place where the current element is placed.This sorting mechanism
     * occurs for every element in the ArrayList<Device>.
     * The time complexity of the first nested for loop which is an enhanced for loop used in previous methods is ;
     * If we assume the length of the linked list as 'n' and assume the average device numbers in categories as 'm' the time complexity is O(n*m) since
     * the for loops iterates through the linked list(n) and Devices in the ArrayLists(m) resulting in n*m time complexity.
     * The time complexity of the second  for loop which is the loop used for sorting the Device objects is ;
     * The length of the SortedList<Device> which includes all the devices is actually n*m.
     * It comes from the first nested for loop since it iterates through every device in the linked list (n categories * m average device in a category = n*m devices ).
     * Therefore in the second nested for loop we iterate through the devices and the next element of the devices in the inner loop it results in ;
     * ( (n*m)^2 ) time complexity in overall since it both loops iterates through the ArrayList.The inner loop iterates through the ArrayList less since 
     * it starts from current index + 1.However it still results in time complexity similar to (n*m) in that loop.Therefore the time complexity is O( (n*m)^2 )
     * In addition, removing method in the second nested loop also has a time complexity of O(n) which is O(n*m) therefore the overall worst time complexity is
     * O( (n*m)^2 )[from 2nd nested for loop] times O(n*m)[from remove method] which is  O( (n*m)^3 ) if we assume n*m as total device number as 'd' shortly
     * the time complexity is O(d^3).
     * The constant adding,getters and setters has O(1) time complexity therefore does not effect the time complexity much.In conclusion ;
     * Time complexity : O( (n*m)^3 ) or O(d^3). 
     */
    public void SortDevices(){
    // 1 3 5 4 2
       ArrayList<Device> SortedList = new ArrayList<>();
        for(ArrayList<Device> categories : ElectronicDevices){
            for(Device devices : categories){
                SortedList.add(devices);
            }
        }
        ArrayList<Device> MainSortedList = SortedList;
        for(int i=0;i<SortedList.size();++i){
            for(int j=i+1;j<SortedList.size();++j){
                if(SortedList.get(i).Price() > SortedList.get(j).Price()){
                    Device smaller = SortedList.get(j);
                    MainSortedList.remove(SortedList.get(j));
                    MainSortedList.add(i, smaller);
                    
                }
            }
        }

        for(int i=0;i<MainSortedList.size();++i){
            System.out.println(i+1 + ". Category: " + MainSortedList.get(i).Category() + ", Name : " + MainSortedList.get(i).Name() + ", Price: " + MainSortedList.get(i).Price() + "$ ,Quantity: " + MainSortedList.get(i).Quantity());

        }
        
    }

    /**
     * In this CalculateTotal method, there is an nested enhanced for loop just like in previous methods.In the outer loop we iterate through the ArrayLists(categories) and in the inner loop
     * we iterate through the Device objects in that ArrayList(category) then for each Device object we calculate its value by multiplying its price with its quantity
     * and keep this value in total_value variable.After calculating every devices' value in total_value I returned total_value variable.
     * If we assume the length of the linked list as 'n' and assume the average device numbers in categories as 'm' the time complexity is O(n*m) since
     * the for loops iterates through the linked list(n) and Devices in the ArrayLists(m) resulting in n*m time complexity.
     * The constant operations(calculations and getters) has O(1) time complexity therefore does not effect the time complexity much.In conclusion ;
     * Also if we assume the total number of devices as 'd' then the time complexity will be O(d) which is the same as O(n*m).
     * Time complexity : O(n*m) or O(d).
     * @return total_value
     */
    public double CalculateTotal(){
        double total_value = 0.0;
        for(ArrayList<Device> categories : ElectronicDevices){
            for(Device devices : categories){
                total_value+= devices.Price() * devices.Quantity();
            }
        }
        return total_value;
    }

    /**
     * In this Restock method, there is an nested enhanced for loop just like in previous methods.In the outer loop we iterate through the ArrayLists(categories) and in the inner loop
     * we iterate through the Device objects in that ArrayList(category).Before that the device name is wanted from the user then if the stock will be added or removed
     * is asked from the user then the values.According to that values after the device is found in the for loop its stock value is changed.
     * If we assume the length of the linked list as 'n' and assume the average device numbers in categories as 'm' the time complexity is O(n*m) since
     * the for loops iterates through the linked list(n) and Devices in the ArrayLists(m) resulting in n*m time complexity.
     * Also if we assume the total number of devices as 'd' then the time complexity will be O(d) which is the same as O(n*m).
     * The constant operations(if statements,calculations,setters and getters) has O(1) time complexity therefore does not effect the time complexity much.In conclusion ;
     * Time complexity : O(n*m) or O(d).
     */
    public void Restock(){
        Scanner scanner = new Scanner(System.in);
    
        System.out.print("Enter the name of the device to restock: ");
        String name = scanner.nextLine();
        
        System.out.println("Do you want to add or remove stock? (Add/Remove): ");
        String choice = scanner.next();

        int status=0;

        if(choice.equals("Add"))
            status=1;
        
        
        else if(choice.equals("Remove"))
            status=-1;
        
        else{
            System.out.println("Please enter either 'Add' or 'Remove' case sensitively!");
            return;
        }
        
        int quantity=0;

        if(status==1){
            System.out.print("Enter the quantity to add: ");
            quantity = scanner.nextInt();
        }

        else if(status==-1){
            System.out.print("Enter the quantity to remove: ");
            quantity = scanner.nextInt();
        }

        if(quantity<0){
            System.out.println("Quantity must be more than 0!");
            return;
        }

        quantity = status * quantity;

        for(ArrayList<Device> categories : ElectronicDevices){
            for(Device devices : categories){
                if(devices.Name().equals(name)){
                    if(status==1){
                        devices.setQuantity( devices.Quantity() + quantity);
                        System.out.println(devices.Name() + " restocked. New quantity: " + devices.Quantity());
                        return;
                    }
                    if(status==-1){
                        if( (-1*quantity) <= devices.Quantity()){
                            devices.setQuantity( devices.Quantity() + quantity);
                            System.out.println(devices.Name() + " stock reduced. New quantity: " + devices.Quantity());
                            return; 
                        }
                    }
                    
                }
            }
        }
        System.out.println("Device is either not found or quantity is too low! Please try typing it again!");
    }

    /**
     * In this Export method, there is an nested enhanced for loop just like in previous methods.In the outer loop we iterate through the ArrayLists(categories) and in the inner loop
     * we iterate through the Device objects in that ArrayList(category) and display all the device objects with their categories,names,prices and quantities.
     * If we assume the length of the linked list as 'n' and assume the average device numbers in categories as 'm' the time complexity is O(n*m) since
     * the for loops iterates through the linked list(n) and Devices in the ArrayLists(m) resulting in n*m time complexity.
     * Also if we assume the total number of devices as 'd' then the time complexity will be O(d) which is the same as O(n*m).
     * The constant getting operations(devices.Category() etc. ) has O(1) time complexity therefore does not effect the time complexity much.In conclusion ;
     * Time complexity : O(n*m) or O(d).
     */
    public void Export(){
        String path = "InventoryReport.txt";

        try {
            FileWriter fileWriter = new FileWriter(path);

            LocalDate currentDate = LocalDate.now();
            DateTimeFormatter day = DateTimeFormatter.ofPattern("dd");
            DateTimeFormatter month_year = DateTimeFormatter.ofPattern("MMMM yyyy");
            String current_day = currentDate.format(day) + "th " ;
            String CurrentDate = current_day + currentDate.format(month_year);

            // Write the text to the file
            fileWriter.write("Electronic Shops Inventory Report\n");
            fileWriter.write("Generated on : " + CurrentDate + "\n");

            fileWriter.write("----------------------------------------------------------\n");
            fileWriter.write("| No. | Category       |      Name              |   Price  | Quantity |\n");
            fileWriter.write("----------------------------------------------------------\n");
            
            int num=0;

            for(ArrayList<Device> categories : ElectronicDevices){
                for(Device devices : categories){
                    String price = Double.toString(devices.Price());
                    String quan = Integer.toString(devices.Quantity());
                    num++;
                    String filew = String.format("|  %d  |  %-12s  |  %-12s  | $%-4s | %-4s |\n",
                    num, devices.Category(), devices.Name(), price, quan);
                    fileWriter.write(filew);
                }
            }

            fileWriter.write("----------------------------------------------------------\n");
            fileWriter.write("Summary:\n");
            fileWriter.write("- Total Number of Devies :          " + num  + "\n");
            fileWriter.write("- Total Inventory Value  :         $" + CalculateTotal() + "\n");
            fileWriter.write("End of Report");

            // Close the BufferedWriter
            fileWriter.close();

        } catch (Exception e) {
            // Handle any IO exceptions
            System.err.println("Error writing to the file: " + e.getMessage());
        }
    }


}
