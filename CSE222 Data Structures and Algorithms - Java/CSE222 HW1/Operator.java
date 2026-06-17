

public class Operator extends Person {
    private int wage;
    private Customer[] customers;

    public Operator(String name, String surname, String address,String phone,int ID,int wage){
        super(name,surname,address,phone,ID);
        this.wage=wage;
        this.customers = new Customer[100];
    }

    
    public void print_operator() {
                System.out.println("*** Operator Screen ***");
                System.out.println("----------------------------");
                System.out.println("Name & Surname : " + get_name() + " " + get_surname());
                System.out.println("Address : " + get_address());
                System.out.println("Phone : " + get_phone());
                System.out.println("ID : "+ get_ID());
                System.out.println("Wage : " + wage);
    }
    public void print_customers() {
        define_customers(customers);
        if(customers[0]==null){
            System.out.println("----------------------------");
            System.out.println("This operator doesn't have any customer.");
            return;
        }
        for(int i=0;i<customers.length ; ++i){
            if (customers[i] != null) {
                System.out.println("----------------------------");
                System.out.print("Customer #" + (i+1) );
                if(customers[i] instanceof Corporate_Customer)
                    System.out.println(" (a corporate customer) : ");
                else if(customers[i] instanceof Retail_Customer)
                    System.out.println(" (a retail customer) : ");
                customers[i].print_customer();
                customers[i].print_orders();
            }
        }
    }
    
    public void define_customers(Customer[] cst) { // sallamasyon
        int index=0;
        for(int i=0; i<cst.length ; ++i){ // if the customer opID matches with the operator's ID then it will be added to the operator's customer array.
            if(cst[i] != null && cst[i].get_opID() == get_ID()){
                customers[index++]=cst[i];
            }
        }
    }
}
