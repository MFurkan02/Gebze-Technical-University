

public class Corporate_Customer extends Customer{
    private String company_name;

    public Corporate_Customer(String name, String surname, String address,String phone,int ID,int opID,String company_name){
        super(name,surname,address,phone,ID,opID);
        this.company_name=company_name;
    }
    public void print_customer(){
        
        System.out.println("Name & Surname : " + get_name() + " " + get_surname());
        System.out.println("Address : " + get_address());
        System.out.println("Phone : " + get_phone());
        System.out.println("ID : "+ get_ID());
        System.out.println("Operator ID : " + get_opID());
        System.out.println("Company name : " + company_name);
    }
    
}