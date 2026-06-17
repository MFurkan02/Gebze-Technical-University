
public class Customer extends Person{
    private Order[] orders;
    private int operator_ID;

    public Customer(String name, String surname, String address,String phone,int ID,int opID){
        super(name,surname,address,phone,ID);
        this.operator_ID=opID;
        this.orders = new Order[100];
    }

    public int get_opID(){
        return operator_ID;
    }

    public void print_customer() {
        // checking if the customer is a retail or corporate then printing according to its type.
        if(this instanceof Corporate_Customer){
            Corporate_Customer temp = (Corporate_Customer) this;
            temp.print_customer();
        }
        else if(this instanceof Retail_Customer){
            Retail_Customer temp = (Retail_Customer) this;
            temp.print_customer();
        }
    }
    public void print_orders() {
        define_orders(orders); // first defining the orders then printing it using the order class method.
        for(int i=0;i<orders.length ; ++i){
            if(orders[i]!=null){
                System.out.print("Order #" + (i+1) );
                orders[i].print_order();
            }
        }
    }
    public void define_orders(Order[] ord) {
        int index=0;
        for(int i=0;i<ord.length ; ++i){ // if the customer ID of the order matches it will be added to the orders array.
            if(ord[i]!=null && ord[i].get_customer_ID() == get_ID()){
                orders[index++]=ord[i];
            }
        }
    }
}