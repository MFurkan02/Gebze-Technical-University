

public class Retail_Customer extends Customer{
    public Retail_Customer(String name, String surname, String address,String phone,int ID,int opID){
        super(name,surname,address,phone,ID,opID);
    }

    public void print_customer(){
        System.out.println("Name & Surname : " + get_name() + " " + get_surname());
        System.out.println("Address : " + get_address());
        System.out.println("Phone : " + get_phone());
        System.out.println("ID : "+ get_ID());
        System.out.println("Operator ID : " + get_opID());
    }
}
