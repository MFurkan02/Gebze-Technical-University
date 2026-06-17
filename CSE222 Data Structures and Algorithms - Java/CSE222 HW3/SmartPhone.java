
public class SmartPhone implements Device {
    
    private final String category = "SmartPhone";

    private String name;

    private double price;

    private int quantity;

    public SmartPhone(String name,double price,int quantity){
        this.name=name;
        this.price=price;
        this.quantity=quantity;
    }

    /**
     * O(1)
     */
    @Override
    public String Category(){
        return category;
    }

    /**
     * O(1)
     */
    @Override
    public String Name(){
        return name;
    }

    /**
     * O(1)
     */
    @Override
    public void setName(String name){
        this.name=name;
    }

    /**
     * O(1)
     */
    @Override
    public double Price(){
        return price;
    }

    /**
     * O(1)
     */
    @Override
    public void setPrice(double price){
        this.price=price;
    }

    /**
     * O(1)
     */
    @Override
    public int Quantity(){
        return quantity;
    }

    /**
     * O(1)
     */
    @Override
    public void setQuantity(int quantity){
        this.quantity = quantity;
    }
}
