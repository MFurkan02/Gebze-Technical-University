import java.io.File ; 
import java.util.Scanner ;

public class Menu {
    public static void main(String[] args) {
        MenuScreen();
    }
    public static void MenuScreen(){
        int order_index=0;
        int customer_index=0;
        int operator_index=0;
        Order allOrders[] = new Order[100];
        Customer allCustomers[] = new Customer[100];
        Operator allOperators[] = new Operator[100];

        String FileName = "content.txt";
        File fl = new File(FileName);
        try (Scanner scanner = new Scanner(fl) ) {
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                
                // Spliting the line into an array of strings
                try{

                    String[] content = line.split(";",-1);
                    int len = content.length; // to check the length of the array.
                    
                    // Assigning the type to check if it is an order , customer or operator.
                    String type = content[0];
                    
                    if(type.equals("order") && len==6 ){
                        String p_name = content[1];
                        int count = Integer.parseInt(content[2]);
                        int price = Integer.parseInt(content[3]);
                        int status = Integer.parseInt(content[4]);
                        int id = Integer.parseInt(content[5]);
                        int check=1;
                        // checking for string being empty or integers being positive etc.
                        if(  p_name.isBlank() || count<=0 || price<=0 || id<=0 || status<0 || status>3 ) 
                            check=0;
                        if(check==1){ // if there is no problem it will be added to orders array.
                            Order order = new Order(p_name, count, price, status, id);
                            allOrders[order_index++]=order;
                        }
                    }
                    else if(type.equals("retail_customer") && len==7){
                        String name = content[1];
                        String surname = content[2];
                        String address= content[3];
                        String phone= content[4];
                        int ID= Integer.parseInt(content[5]);
                        int opID = Integer.parseInt(content[6]);

                        int check=1;
                        // checking for string being empty or integers being positive etc.
                        if(ID<1 || opID<1 || name.isBlank() || surname.isBlank() || address.isBlank() || phone.isBlank())
                            check=0;

                        for(int i=0;i<customer_index;++i){
                            if(allCustomers[i].get_ID()==ID){ // if 2 customers have the same ID it will not be added to the all customers array.
                                check=0;
                            }
                        }

                        if(check==1){  // if there is no problem it will be added to customers array.
                            Retail_Customer rc = new Retail_Customer(name, surname, address, phone, ID, opID);
                            allCustomers[customer_index++]=rc;
                        }
                    }
                    
                    else if(type.equals("corporate_customer") && len==8){
                        String name = content[1];
                        String surname = content[2];
                        String address= content[3];
                        String phone= content[4];
                        int ID= Integer.parseInt(content[5]);
                        int opID = Integer.parseInt(content[6]);
                        String company_name = content[7];

                        int check=1;
                        // checking for string being empty or integers being positive etc.
                        if(ID<1 || opID<1 || name.isBlank() || surname.isBlank() || address.isBlank() || phone.isBlank() || company_name.isBlank())
                            check=0;

                        for(int i=0;i<customer_index;++i){
                            if(allCustomers[i].get_ID()==ID){ // if 2 customers have the same ID it will not be added to the all customers array.
                                check=0;
                            }
                        }

                        if(check==1){ // if there is no problem it will be added to customers array.
                            Corporate_Customer cc = new Corporate_Customer(name, surname, address, phone, ID, opID, company_name);
                            allCustomers[customer_index++]=cc;
                        }
                        
                    }
                    else if(type.equals("operator") && len==7){

                        String name = content[1];
                        String surname = content[2];
                        String address= content[3];
                        String phone= content[4];
                        int ID= Integer.parseInt(content[5]);
                        int wage= Integer.parseInt(content[6]);
                        
                        int check=1;
                        // checking for string being empty or integers being positive etc.
                        if(ID<0 || wage<0 || name.isBlank() || surname.isBlank() || address.isBlank() || phone.isBlank() )
                            check=0;
                        // checking if an operator or customer has the same id
                        for(int i=0;i<operator_index;i++){
                            if(allOperators[i].get_ID()==ID){
                                check=0;
                            }
                        }
                        for(int i=0;i<customer_index;++i){
                            if(allCustomers[i].get_ID()==ID){ // if 2 customers have the same ID it will not be added to the all customers array.
                                check=0;
                            }
                        }

                        if(check==1){ // if there is no problem it will be added to operators array.
                            Operator op = new Operator(name,surname,address,phone,ID,wage);
                            allOperators[operator_index++]=op;
                        }
                    }
            }
            catch(Exception exc){} // to catch the exceptions when a string is read when an integer was waited to be entered or etc.
            }
        }
        catch(Exception exc2){} // to catch the exception of file not being found.

        // after all the file is read and the objects have been created.
        // first the operators and customers will be defined. 
        for(int i=0;i<operator_index;++i){
            allOperators[i].define_customers(allCustomers);
        }
        for(int i=0;i<customer_index;++i){
            allCustomers[i].define_orders(allOrders);
        }

        try{
            Scanner scanner = new Scanner(System.in); 
            System.out.println("Please enter your ID...");
            int id = scanner.nextInt();
            int flag=0;
            for(int i=0;i<operator_index;i++){
                if(allOperators[i].get_ID()==id){ // if the entered ID is an operator, customer and orders will be printed if there is any. 
                    flag=1;
                    allOperators[i].print_operator();
                    allOperators[i].print_customers();
                    System.out.println("----------------------------");
                }
            }
            for(int i=0;i<customer_index;i++){
                if(allCustomers[i].get_ID()==id){ // if the entered ID is a customer , orders will be printed if there is any. 
                    flag=1;
                    System.out.println("*** Customer Screen *** ");
                    allCustomers[i].print_customer();
                    allCustomers[i].print_orders();
                }
            }
            if(flag==0)
                System.out.println("No operator/customer was found with ID " + id + ". Please try again.");

            scanner.close();
        }
        catch(Exception exc3){
            System.out.println("Oops! Program is terminated! Please enter an integer next time!");
        }
  
    }
}

