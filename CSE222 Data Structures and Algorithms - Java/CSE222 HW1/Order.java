
public class Order {
    private String product_name;
    private int count;
    private int total_price;
    private int status;
    private int customer_ID;


    public Order(String product_name,int count,int total_price,int status,int customer_ID){
        this.product_name=product_name;
        this.count=count;
        this.total_price=total_price;
        this.status=status;
        this.customer_ID=customer_ID;
    }

    public int get_customer_ID(){
        return customer_ID;
    }

    public void print_order(){
                System.out.print(" => Product name: " + product_name + " - Count: " + count + " - Total price: " + total_price + " - Status: ");
                if(status==0){
                    System.out.println("Initialized.");
                }
                else if(status==1){
                    System.out.println("Processing.");
                }
                else if(status==2){
                    System.out.println("Completed.");
                }
                else if(status==3){
                    System.out.println("Cancelled.");
                }

    }
}
